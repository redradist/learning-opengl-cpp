#ifndef LEARNOPENGLSTARTER__VertexBufferObjects_HPP_
#define LEARNOPENGLSTARTER__VertexBufferObjects_HPP_

#include <GL/glew.h>

#include <string>
#include <array>

namespace opengl {

enum class BufferDraw {
  Stream = GL_STREAM_DRAW,
  Static = GL_STATIC_DRAW,
  Dynamic = GL_DYNAMIC_DRAW,
};

template <size_t NumVBOs>
class VertexBufferObjects {
 public:
  VertexBufferObjects() {
    glGenBuffers(vertices_.size(), vertices_.data());
  }

  VertexBufferObjects(VertexBufferObjects&& other) noexcept {
    operator=(std::move(other));
  }

  VertexBufferObjects& operator=(VertexBufferObjects&& other) noexcept {
      vertices_ = other.vertices_;
      other.vertices_ = {};
      return *this;
  }

  ~VertexBufferObjects() {
      if (vertices_ != std::array<GLuint, NumVBOs>{}) {
          glDeleteBuffers(vertices_.size(), vertices_.data());
      }
  }

  void bindVertexBuffer(size_t idx) const {
    glBindBuffer(GL_ARRAY_BUFFER, vertices_.at(idx));
  }

  void copyData(const float* vertexPositions, size_t size, const BufferDraw draw) const {
    glBufferData(GL_ARRAY_BUFFER, size, vertexPositions, static_cast<int>(draw));
  }

 private:
    std::array<GLuint, NumVBOs> vertices_;
};

}

#endif //LEARNOPENGLSTARTER__VertexBufferObjects_HPP_
