#ifndef LEARNOPENGLSTARTER__VertexArrayObjects_HPP_
#define LEARNOPENGLSTARTER__VertexArrayObjects_HPP_

#include <GL/glew.h>

#include <string>
#include <array>

namespace opengl {

template <size_t NumVAOs>
class VertexArrayObjects {
 public:
  VertexArrayObjects() {
      glGenVertexArrays(vertices_.size(), vertices_.data());
  }

  VertexArrayObjects(VertexArrayObjects&& other) noexcept {
      operator=(std::move(other));
  }

  VertexArrayObjects& operator=(VertexArrayObjects&& other) noexcept {
      vertices_ = other.vertices_;
      other.vertices_ = {};
      return *this;
  }

  ~VertexArrayObjects() {
      if (vertices_ != std::array<GLuint, NumVAOs>{}) {
          glDeleteVertexArrays(vertices_.size(), vertices_.data());
      }
  }

  void bindVertexArray(size_t idx) {
    glBindVertexArray(vertices_.at(idx));
  }

  void generateVertexArrays() {
    glGenVertexArrays(1, vertices_.data());
  }

 private:
    std::array<GLuint, NumVAOs> vertices_;
};

}

#endif //LEARNOPENGLSTARTER__VertexArrayObjects_HPP_
