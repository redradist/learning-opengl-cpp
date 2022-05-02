#ifndef LEARNOPENGLSTARTER__PROGRAM_HPP_
#define LEARNOPENGLSTARTER__PROGRAM_HPP_

extern "C" {

#include <GL/glew.h>

}

#include <string>
#include <vector>

#include "glhandle.hpp"
#include "shader.hpp"

namespace opengl {

class Program : public GLHandle {
 public:
    Program() {
        handle_ = glCreateProgram();
    }

    Program(Program &&other) = default;
    Program &operator=(Program &&other) = default;

    ~Program() {
        if (handle_ != GLuint{}) {
            glDeleteProgram(handle_);
        }
    }

    void attach(Shader &&shader) {
        glAttachShader(handle_, handleFrom(shader));
        shaders_.push_back(std::move(shader));
    }

    void link() {
        glLinkProgram(handle_);
    }

    void use() {
        glUseProgram(handle_);
    }

 private:
    std::vector<Shader> shaders_;
};

}

#endif //LEARNOPENGLSTARTER__PROGRAM_HPP_
