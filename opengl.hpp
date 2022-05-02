#ifndef LEARNOPENGLSTARTER__OPENGL_HPP_
#define LEARNOPENGLSTARTER__OPENGL_HPP_

#include "program.hpp"

namespace opengl {

class Error : public std::exception {
 public:
  using std::exception::exception;
};

void drawPoints(GLint first, GLsizei count) {
    glDrawArrays(GL_POINTS, first, count);
}

}

#endif //LEARNOPENGLSTARTER__OPENGL_HPP_
