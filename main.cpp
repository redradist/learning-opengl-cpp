#ifndef OPENGL_WRAPPERS
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "opengl/opengl.hpp"
#include "opengl/log.hpp"

using namespace std;

#define numVAOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];

GLuint createShaderProgram() {
  GLint vertCompiled;
  GLint fragCompiled;
  GLint linked;
  const char *vshaderSource =
      "#version 430 \n"
      "void main(void) \n"
      "{ gl_Position = vec4(0.0, 0.0, 0.0, 1.0); }";
  const char *fshaderSource =
      "#version 430 \n"
      "out vec4 color; \n"
      "void main(void) \n"
      "{ color = vec4(0.0, 0.0, 1.0, 1.0); }";
  GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
  GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
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

void display(GLFWwindow* window, double currentTime) {
  glUseProgram(renderingProgram);
  glPointSize(30.0f);
  glDrawArrays(GL_POINTS, 0, 1);
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

void display(GLFWwindow* window, double currentTime) {
  auto& program = renderingProgram.value();
  program.use();
  glDrawArrays(GL_POINTS, 0, 1);
}

int main() {
  if (!glfwInit()) { exit(EXIT_FAILURE); }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  GLFWwindow* window = glfwCreateWindow(600, 600, "Chapter2 - program1", NULL, NULL);
  glfwMakeContextCurrent(window);

  if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }

  glfwSwapInterval(10);
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
