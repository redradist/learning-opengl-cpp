#ifndef LEARNOPENGLSTARTER__GLHANDLE_HPP_
#define LEARNOPENGLSTARTER__GLHANDLE_HPP_

#include <algorithm>
extern "C" {

#include <GL/glew.h>

}

class GLHandle {
 public:
    GLHandle() = default;
    GLHandle(GLHandle&& other) noexcept {
        operator=(std::move(other));
    }
    GLHandle& operator=(GLHandle&& other) noexcept {
        handle_ = other.handle_;
        other.handle_ = {};
        return *this;
    }

    GLuint handle() const {
        return handle_;
    }

 protected:
    GLuint handleFrom(const GLHandle& handle) const {
        return handle.handle();
    }

    GLuint handle_{};
};

#endif //LEARNOPENGLSTARTER__GLHANDLE_HPP_
