#ifndef LEARNOPENGL__UTILS_H_
#define LEARNOPENGL__UTILS_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <optional>

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
GLuint createShaderProgram(const std::string& vertShaderFile,
                           const std::string& fragShaderFile,
                           std::optional<std::string> vertShaderUtilsFile) {
  const std::string& vshaderSource = readShaderSource(vertShaderFile);
  std::string vUtilsShaderSource;
  if (vertShaderUtilsFile) {
    vUtilsShaderSource = readShaderSource(vertShaderUtilsFile.value());
  }
  const std::string& fshaderSource = readShaderSource(fragShaderFile);
  const char* vshaderSourcePtr = vshaderSource.c_str();
  const char* vUtilsShaderSourcePtr = vUtilsShaderSource.c_str();
  const char* fshaderSourcePtr = fshaderSource.c_str();
  GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
  GLuint vUtilsShader;
  if (vertShaderUtilsFile) {
    vUtilsShader = glCreateShader(GL_VERTEX_SHADER);
  }
  GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(vShader, 1, &vshaderSourcePtr, NULL);
  if (vertShaderUtilsFile) {
    glShaderSource(vUtilsShader, 1, &vUtilsShaderSourcePtr, NULL);
  }
  glShaderSource(fShader, 1, &fshaderSourcePtr, NULL);
  glCompileShader(vShader);
  if (vertShaderUtilsFile) {
    glCompileShader(vUtilsShader);
  }
  glCompileShader(fShader);
  GLuint vfProgram = glCreateProgram();
  glAttachShader(vfProgram, vShader);
  if (vertShaderUtilsFile) {
    glAttachShader(vfProgram, vUtilsShader);
  }
  glAttachShader(vfProgram, fShader);
  glLinkProgram(vfProgram);
  return vfProgram;
}

}

#endif //LEARNOPENGL__UTILS_H_
