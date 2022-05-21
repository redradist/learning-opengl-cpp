#ifndef LEARNOPENGLSTARTER__OPENGL_HPP_
#define LEARNOPENGLSTARTER__OPENGL_HPP_

#include <iostream>

namespace opengl {

class Error : public std::exception {
 public:
  using std::exception::exception;
};

inline
bool checkOpenGLError() {
  bool foundError = false;
  int glErr = glGetError();
  while (glErr != GL_NO_ERROR) {
    std::cout << "glError: " << glErr << std::endl;
    foundError = true;
    glErr = glGetError();
  }
  return foundError;
}

inline
void vertexAttrib(GLuint index,
                  GLint size,
                  GLenum type,
                  GLboolean normalized,
                  GLsizei stride,
                  const void* pointer) {
  glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

inline
void enableVertexAttrib(size_t idx) {
  glEnableVertexAttribArray(idx);
}

inline
void enable(GLint flag) {
  glEnable(flag);
}

inline
void depthFunc(GLint flag) {
  glDepthFunc(flag);
}

inline
void drawPoints(GLint first, GLsizei count) {
    glDrawArrays(GL_POINTS, first, count);
}

inline
void drawTriangles(GLint first, GLsizei count) {
  glDrawArrays(GL_TRIANGLES, first, count);
}

}

#endif //LEARNOPENGLSTARTER__OPENGL_HPP_
