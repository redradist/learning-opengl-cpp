#ifndef OPENGL_WRAPPERS
#include <cmath>
#include <stack>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "opengl/utils.hpp"

using namespace std;

#define numVAOs 1
#define numVBOs 2

float cameraX, cameraY, cameraZ;
float cubeLocX, cubeLocY, cubeLocZ;
float pyrLocX, pyrLocY, pyrLocZ;

GLuint renderingProgram;

GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// allocate variables used in display() function, so that they won’t need to be allocated during rendering
GLuint mvLoc, projLoc;
int width, height;
float aspect;

glm::mat4 pMat, vMat, mMat, mvMat;
glm::mat4 tMat, rMat;

std::stack<glm::mat4> mvStack;

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

  // pyramid with 18 vertices, comprising 6 triangles (four sides, and two on the bottom)
  float pyramidPositions[54] =
  {
      -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // front face
      1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // right face
      1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // back face
      -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // left face
      -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, // base – left front
      1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f // base – right back
  };

  glGenVertexArrays(1, vao);
  glBindVertexArray(vao[0]);
  glGenBuffers(numVBOs, vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPositions), pyramidPositions, GL_STATIC_DRAW);
}

void init(GLFWwindow* window) {
  renderingProgram = utils::createShaderProgram("../../shaders/vertex.glsl",
                                                "../../shaders/fragment.glsl");
  cameraX = 0.0f; cameraY = 0.0f; cameraZ = 16.0f;
  cubeLocX = 0.0f; cubeLocY = -2.0f; cubeLocZ = 0.0f; // shift down Y to reveal perspective
  pyrLocX = 2.0f; pyrLocY = 2.0f; pyrLocZ = 0.0f; // shift down Y to reveal perspective
  setupVertices();
}

void display(GLFWwindow* window, double currentTime) {
  glClear(GL_DEPTH_BUFFER_BIT);
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(renderingProgram);

  // get the uniform variables for the MV and projection matrices
  mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
  projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

  // build perspective matrix
  glfwGetFramebufferSize(window, &width, &height);
  aspect = static_cast<float>(width) / static_cast<float>(height);
  pMat = glm::perspective(1.0472f, aspect, 0.1f, 10000.0f); // 1.0472 radians = 60 degrees
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

  // push view matrix onto the stack
  vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
  mvStack.push(vMat);
  
  // ---------------------- pyramid == sun --------------------------------------------
  mvStack.push(mvStack.top());
  // sun position
  mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
  mvStack.push(mvStack.top());
  // sun rotation
  mvStack.top() *= glm::rotate(glm::mat4(1.0f), static_cast<float>(currentTime), glm::vec3(1.0f, 0.0f, 0.0f));

  glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LEQUAL);
  // draw the sun
  glDrawArrays(GL_TRIANGLES, 0, 18);
  // remove the sun’s axial rotation from the stack
  mvStack.pop();

  //----------------------- cube == planet ---------------------------------------------
  mvStack.push(mvStack.top());
  mvStack.top() *=
      glm::translate(glm::mat4(1.0f),
                     glm::vec3(sin(static_cast<float>(currentTime)) * 4.0,
                               0.0f,
                               cos(static_cast<float>(currentTime)) * 4.0));
  mvStack.push(mvStack.top());
  // planet rotation
  mvStack.top() *= glm::rotate(glm::mat4(1.0f),
                               static_cast<float>(currentTime),
                               glm::vec3(0.0, 1.0, 0.0));

  glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);
  // draw the planet
  glDrawArrays(GL_TRIANGLES, 0, 36);
  // remove the planet’s axial rotation from the stack
  mvStack.pop();

  //----------------------- smaller cube == moon -----------------------------------
  mvStack.push(mvStack.top());
  mvStack.top() *=
      glm::translate(glm::mat4(1.0f),
                     glm::vec3(0.0f,
                               sin(static_cast<float>(currentTime)) * 2.0,
                               cos(static_cast<float>(currentTime)) * 2.0));
  mvStack.top() *= glm::rotate(glm::mat4(1.0f),
                               static_cast<float>(currentTime),
                               glm::vec3(0.0, 0.0, 1.0));

  // moon rotation
  // make the moon smaller
  mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 0.25f));
  glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);
  // draw the moon
  glDrawArrays(GL_TRIANGLES, 0, 36);

  // remove moon scale/rotation/position,
  // planet position, sun position, and view matrices from stack
  mvStack.pop();
  mvStack.pop();
  mvStack.pop();
  mvStack.pop();
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
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <optional>
#include <array>
#include <stack>

#include "program.hpp"
#include "vertex_array.hpp"
#include "vertex_buffer.hpp"

std::optional<opengl::Program> renderingProgram;
constexpr size_t kNumVAOs = 1;
constexpr size_t kNumVBOs = 2;
std::optional<opengl::VertexArrayObjects<kNumVAOs>> vao;
std::optional<opengl::VertexBufferObjects<kNumVBOs>> vbo;

float cameraX, cameraY, cameraZ;
float cubeLocX, cubeLocY, cubeLocZ;
float pyrLocX, pyrLocY, pyrLocZ;

// allocate variables used in display() function, so that they won’t need to be allocated during rendering
GLuint mvLoc, projLoc;
int width, height;
float aspect;

std::stack<glm::mat4> mvStack;

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

  // pyramid with 18 vertices, comprising 6 triangles (four sides, and two on the bottom)
  float pyramidPositions[54] =
  {
      -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // front face
      1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // right face
      1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // back face
      -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // left face
      -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, // base – left front
      1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f // base – right back
  };

  vao = opengl::VertexArrayObjects<kNumVAOs>{};
  vbo = opengl::VertexBufferObjects<kNumVBOs>{};
  vao.value().bindVertexArray(0);

  vbo.value().bindVertexBuffer(0);
  vbo.value().copyData(vertexPositions, sizeof(vertexPositions), opengl::BufferDraw::Static);

  vbo.value().bindVertexBuffer(1);
  vbo.value().copyData(pyramidPositions, sizeof(pyramidPositions), opengl::BufferDraw::Static);
}

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
  cameraX = 0.0f; cameraY = 0.0f; cameraZ = 16.0f;
  cubeLocX = 0.0f; cubeLocY = -2.0f; cubeLocZ = 0.0f; // shift down Y to reveal perspective
  pyrLocX = 2.0f; pyrLocY = 2.0f; pyrLocZ = 0.0f; // shift down Y to reveal perspective
  setupVertices();
}

float x = 0.0f; // location of triangle on x axis
float inc = 0.01f; // offset for moving the triangle

void display(GLFWwindow* window, double currentTime) {
  glClear(GL_DEPTH_BUFFER_BIT);
  glClear(GL_COLOR_BUFFER_BIT);
  auto& program = renderingProgram.value();
  program.use();

  // get the uniform variables for the MV and projection matrices
  glfwGetFramebufferSize(window, &width, &height);
  aspect = static_cast<float>(width) / static_cast<float>(height);
  pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees
  program.sendUniformMatrix4fv("proj_matrix", pMat);

  // push view matrix onto the stack
  vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
  mvStack.push(vMat);

  // ---------------------- pyramid == sun --------------------------------------------
  mvStack.push(mvStack.top());
  // sun position
  mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
  mvStack.push(mvStack.top());
  // sun rotation
  mvStack.top() *= glm::rotate(glm::mat4(1.0f), static_cast<float>(currentTime), glm::vec3(1.0f, 0.0f, 0.0f));

  program.sendUniformMatrix4fv("mv_matrix", mvStack.top());
  vbo->bindVertexBuffer(1);

  opengl::vertexAttrib(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  opengl::enableVertexAttrib(0);
  opengl::enable(GL_DEPTH_TEST);
  opengl::enable(GL_LEQUAL);
  // draw the sun
  opengl::drawTriangles(0, 18);
  // remove the sun’s axial rotation from the stack
  mvStack.pop();

  //----------------------- cube == planet ---------------------------------------------
  mvStack.push(mvStack.top());
  mvStack.top() *=
      glm::translate(glm::mat4(1.0f),
                     glm::vec3(sin(static_cast<float>(currentTime)) * 4.0,
                               0.0f,
                               cos(static_cast<float>(currentTime)) * 4.0));
  mvStack.push(mvStack.top());
  // planet rotation
  mvStack.top() *= glm::rotate(glm::mat4(1.0f),
                               static_cast<float>(currentTime),
                               glm::vec3(0.0, 1.0, 0.0));

  program.sendUniformMatrix4fv("mv_matrix", mvStack.top());
  vbo->bindVertexBuffer(0);
  opengl::vertexAttrib(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  opengl::enableVertexAttrib(0);
  // draw the planet
  opengl::drawTriangles(0, 36);
  // remove the planet’s axial rotation from the stack
  mvStack.pop();

  //----------------------- smaller cube == moon -----------------------------------
  mvStack.push(mvStack.top());
  mvStack.top() *=
      glm::translate(glm::mat4(1.0f),
                     glm::vec3(0.0f,
                               sin(static_cast<float>(currentTime)) * 2.0,
                               cos(static_cast<float>(currentTime)) * 2.0));
  mvStack.top() *= glm::rotate(glm::mat4(1.0f),
                               static_cast<float>(currentTime),
                               glm::vec3(0.0, 0.0, 1.0));

  // moon rotation
  // make the moon smaller
  mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 0.25f));
  program.sendUniformMatrix4fv("mv_matrix", mvStack.top());
  vbo->bindVertexBuffer(0);
  opengl::vertexAttrib(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  opengl::enableVertexAttrib(0);
  // draw the moon
  opengl::drawTriangles(0, 36);

  // remove moon scale/rotation/position,
  // planet position, sun position, and view matrices from stack
  mvStack.pop();
  mvStack.pop();
  mvStack.pop();
  mvStack.pop();
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
