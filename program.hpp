#ifndef LEARNOPENGLSTARTER__PROGRAM_HPP_
#define LEARNOPENGLSTARTER__PROGRAM_HPP_

extern "C" {

#include <GL/glew.h>

}

#include <string>
#include <vector>

#include "glhandle.hpp"
#include "shader.hpp"
#include "opengl/opengl.hpp"
#include "opengl/log.hpp"

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
        opengl::checkOpenGLError();
        GLint linked;
        glGetProgramiv(handle_, GL_LINK_STATUS, &linked);
        if (linked != 1) {
          std::cout << "linking failed" << std::endl;
          opengl::log::printProgramLog(handle_);
        }
    }

    void use() {
        glUseProgram(handle_);
    }

    GLuint getUniformLocation(const std::string& symbol) {
      return glGetUniformLocation(handle_, symbol.c_str());
    }

    void sendUniform1f(const std::string& symbol, float value) {
      GLuint offsetLoc = glGetUniformLocation(handle_, symbol.c_str());
      glProgramUniform1f(handle_, offsetLoc, value);
    }

 private:
    std::vector<Shader> shaders_;
};

}

#endif //LEARNOPENGLSTARTER__PROGRAM_HPP_
