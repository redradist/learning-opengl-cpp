#ifndef OPENGL_WRAPPERS
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "opengl/utils.hpp"

using namespace std;

#define numVAOs 1
#define numVBOs 2

float cameraX, cameraY, cameraZ;
float cubeLocX, cubeLocY, cubeLocZ;

GLuint renderingProgram;

GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// allocate variables used in display() function, so that they won’t need to be allocated during rendering
GLuint mvLoc, projLoc;
int width, height;
float aspect;

glm::mat4 pMat, vMat, mMat, mvMat;

void setupVertices() { // 36 vertices, 12 triangles, makes 2x2x2 cube placed at origin
  float vertexPositions[108] = {
      -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
      1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
      1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
      1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
      1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
      -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
      -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
      1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
      -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
      1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f
  };
  glGenVertexArrays(1, vao);
  glBindVertexArray(vao[0]);
  glGenBuffers(numVBOs, vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
}

void init(GLFWwindow* window) {
  renderingProgram = utils::createShaderProgram("../../shaders/vertex.glsl",
                                                "../../shaders/fragment.glsl");
  cameraX = 0.0f; cameraY = 0.0f; cameraZ = 8.0f;
  cubeLocX = 0.0f; cubeLocY = -2.0f; cubeLocZ = 0.0f; // shift down Y to reveal perspective
  setupVertices();
}

void display(GLFWwindow* window, double currentTime) {
  glClear(GL_DEPTH_BUFFER_BIT);
  glUseProgram(renderingProgram);
  // get the uniform variables for the MV and projection matrices
  mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
  projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
  // build perspective matrix
  glfwGetFramebufferSize(window, &width, &height);
  aspect = static_cast<float>(width) / static_cast<float>(height);
  pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees
  // build view matrix, model matrix, and model-view matrix
  vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
  mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX, cubeLocY, cubeLocZ));
  mvMat = vMat * mMat;
  // copy perspective and MV matrices to corresponding uniform variables
  glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
  // associate VBO with the corresponding vertex attribute in the vertex shader
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);
  // adjust OpenGL settings and draw model
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glDrawArrays(GL_TRIANGLES, 0, 36);
}

int main() { // main() is unchanged from before
  if (!glfwInit()) { exit(EXIT_FAILURE); }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  GLFWwindow* window = glfwCreateWindow(600, 600, "Chapter 4 - program 1", NULL, NULL);
  glfwMakeContextCurrent(window);
  if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
  glfwSwapInterval(1);
  init(window);
  while (!glfwWindowShouldClose(window)) {
    display(window, glfwGetTime());
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
#else
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <optional>
#include <array>

#include "program.hpp"
#include "vertex_array.hpp"

std::optional<opengl::Program> renderingProgram;
constexpr size_t kNumVAOs = 1;
std::optional<opengl::VertexArray<kNumVAOs>> vao;

opengl::Program createShaderProgram() {
  opengl::Shader vShader(GL_VERTEX_SHADER, opengl::Path::from("../../shaders/vertex.glsl"));
  opengl::Shader vTranShader(GL_VERTEX_SHADER, opengl::Path::from("../../shaders/transform.glsl"));
  opengl::Shader fShader(GL_FRAGMENT_SHADER, opengl::Path::from("../../shaders/fragment.glsl"));
  opengl::Program program;
  program.attach(std::move(vShader));
  program.attach(std::move(vTranShader));
  program.attach(std::move(fShader));
  program.link();
  return std::move(program);
}

void init(GLFWwindow* window) {
  renderingProgram = createShaderProgram();
  vao = opengl::VertexArray<kNumVAOs>{};
}

float x = 0.0f; // location of triangle on x axis
float inc = 0.01f; // offset for moving the triangle

void display(GLFWwindow* window, double currentTime) {
  glClear(GL_DEPTH_BUFFER_BIT);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT); // clear the background to black, each time

  auto& program = renderingProgram.value();
  program.use();

  x += inc; // move the triangle along x axis
  if (x > 1.0f) inc = -0.01f; // switch to moving the triangle to the left
  if (x < -1.0f) inc = 0.01f; // switch to moving the triangle to the right
  program.sendUniform1f("offset", x); // send value in "x" to "offset"

  glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main() {
  if (!glfwInit()) { exit(EXIT_FAILURE); }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  GLFWwindow* window = glfwCreateWindow(600, 600, "Chapter2 - program1", NULL, NULL);
  glfwMakeContextCurrent(window);

  if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }

  glfwSwapInterval(1);
  init(window);

  while (!glfwWindowShouldClose(window)) {
    display(window, glfwGetTime());
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  exit(EXIT_SUCCESS);
}
#endif
