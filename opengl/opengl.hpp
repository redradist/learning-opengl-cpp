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
void drawPoints(GLint first, GLsizei count) {
    glDrawArrays(GL_POINTS, first, count);
}

}

#endif //LEARNOPENGLSTARTER__OPENGL_HPP_
