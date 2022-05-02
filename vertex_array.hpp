#ifndef LEARNOPENGLSTARTER__VERTEXARRAYOBJECT_HPP_
#define LEARNOPENGLSTARTER__VERTEXARRAYOBJECT_HPP_

#include <GL/glew.h>

#include <string>
#include <array>

namespace opengl {

template <size_t NumVAOs>
class VertexArray {
 public:
    VertexArray() {
        glGenVertexArrays(vertices_.size(), vertices_.data());
        glBindVertexArray(vertices_[0]);
    }

    VertexArray(VertexArray&& other) noexcept {
        operator=(std::move(other));
    }
    VertexArray& operator=(VertexArray&& other) noexcept {
        vertices_ = other.vertices_;
        other.vertices_ = {};
        return *this;
    }

    ~VertexArray() {
        if (vertices_ != std::array<GLuint, NumVAOs>{}) {
            glDeleteVertexArrays(vertices_.size(), vertices_.data());
        }
    }

 private:
    std::array<GLuint, NumVAOs> vertices_;
};

}

#endif //LEARNOPENGLSTARTER__VERTEXARRAYOBJECT_HPP_
