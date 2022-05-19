#ifndef LEARNOPENGL__UTILS_H_
#define LEARNOPENGL__UTILS_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>

namespace utils {

inline
std::string readShaderSource(const std::string& filePath) {
  std::string content;
  std::ifstream fileStream(filePath, std::ios::in);
  std::string line = "";
  while (!fileStream.eof()) {
    std::getline(fileStream, line);
    content.append(line + "\n");
  }
  fileStream.close();
  return content;
}

inline
GLuint createShaderProgram(const std::string& vertShaderFile, const std::string& fragShaderFile) {
  const std::string& vshaderSource = readShaderSource(vertShaderFile);
  const std::string& fshaderSource = readShaderSource(fragShaderFile);
  const char* vshaderSourcePtr = vshaderSource.c_str();
  const char* fshaderSourcePtr = fshaderSource.c_str();
  GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
  GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(vShader, 1, &vshaderSourcePtr, NULL);
  glShaderSource(fShader, 1, &fshaderSourcePtr, NULL);
  glCompileShader(vShader);
  glCompileShader(fShader);
  GLuint vfProgram = glCreateProgram();
  glAttachShader(vfProgram, vShader);
  glAttachShader(vfProgram, fShader);
  glLinkProgram(vfProgram);
  return vfProgram;
}

}

#endif //LEARNOPENGL__UTILS_H_
