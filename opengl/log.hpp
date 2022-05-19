#ifndef LEARNOPENGL__OPENGL_LOG_HPP_
#define LEARNOPENGL__OPENGL_LOG_HPP_

#include <GL/glew.h>
#include <cstdlib>
#include <iostream>

namespace opengl::log {

inline
void printShaderLog(GLuint shader) {
  int len = 0;
  int chWritten = 0;
  char *log;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
  if (len > 0) {
    log = (char *)malloc(len);
    glGetShaderInfoLog(shader, len, &chWritten, log);
    std::cout << "Shader Info Log: " << log << std::endl;
    free(log);
  }
}

inline
void printProgramLog(int prog) {
  int len = 0;
  int chWrittn = 0;
  char *log;
  glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
  if (len > 0) {
    log = (char *)malloc(len);
    glGetProgramInfoLog(prog, len, &chWrittn, log);
    std::cout << "Program Info Log: " << log << std::endl;
    free(log);
  }
}

}

#endif //LEARNOPENGL__OPENGL_LOG_HPP_
