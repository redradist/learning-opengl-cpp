#ifndef OPENGL_WRAPPERS
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include "opengl/opengl.hpp"
#include "opengl/log.hpp"

using namespace std;

#define numVAOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];

std::string readShaderSource(const char *filePath) {
  std::string content;
  std::ifstream fileStream(filePath, ios::in);
  std::string line = "";
  while (!fileStream.eof()) {
    std::getline(fileStream, line);
    content.append(line + "\n");
  }
  fileStream.close();
  return content;
}

GLuint createShaderProgram() {
  GLint vertCompiled;
  GLint fragCompiled;
  GLint linked;

  GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
  GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
  string vertShaderStr = readShaderSource("../../shaders/vertex.glsl");
  string fragShaderStr = readShaderSource("../../shaders/fragment.glsl");
  const char *vshaderSource = vertShaderStr.c_str();
  const char *fshaderSource = fragShaderStr.c_str();
  glShaderSource(vShader, 1, &vshaderSource, NULL);
  glShaderSource(fShader, 1, &fshaderSource, NULL);
  glCompileShader(vShader);
  opengl::checkOpenGLError();
  glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
  if (vertCompiled != 1) {
    cout << "vertex compilation failed" << endl;
    opengl::log::printShaderLog(vShader);
  }
  glCompileShader(fShader);
  opengl::checkOpenGLError();
  glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
  if (fragCompiled != 1) {
    cout << "fragment compilation failed" << endl;
    opengl::log::printShaderLog(fShader);
  }
  GLuint vfProgram = glCreateProgram();
  glAttachShader(vfProgram, vShader);
  glAttachShader(vfProgram, fShader);
  glLinkProgram(vfProgram);
  opengl::checkOpenGLError();
  glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
  if (linked != 1) {
    cout << "linking failed" << endl;
    opengl::log::printProgramLog(vfProgram);
  }
  return vfProgram;
}

void init(GLFWwindow* window) {
  renderingProgram = createShaderProgram();
  glGenVertexArrays(numVAOs, vao);
  glBindVertexArray(vao[0]);
}

float x = 0.0f; // location of triangle on x axis
float inc = 0.01f; // offset for moving the triangle

void display(GLFWwindow* window, double currentTime) {
  glClear(GL_DEPTH_BUFFER_BIT);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT); // clear the background to black, each time

  glUseProgram(renderingProgram);

  x += inc; // move the triangle along x axis
  if (x > 1.0f) inc = -0.01f; // switch to moving the triangle to the left
  if (x < -1.0f) inc = 0.01f; // switch to moving the triangle to the right
  GLuint offsetLoc = glGetUniformLocation(renderingProgram, "offset"); // get ptr to "offset"
  glProgramUniform1f(renderingProgram, offsetLoc, x); // send value in "x" to "offset"

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
  opengl::Shader fShader(GL_FRAGMENT_SHADER, opengl::Path::from("../../shaders/fragment.glsl"));
  opengl::Program program;
  program.attach(std::move(vShader));
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
