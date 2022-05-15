#ifndef LEARNOPENGLSTARTER__SHADER_HPP_
#define LEARNOPENGLSTARTER__SHADER_HPP_

extern "C" {

#include <GL/glew.h>

}

#include <string>
#include <fstream>

#include "glhandle.hpp"
#include "path.hpp"
#include "opengl/opengl.hpp"

namespace opengl {

class Shader : public GLHandle {
 public:
    Shader(const int shaderId, const Path &filePath) {
        std::string content;
        std::ifstream fileStream(filePath.value, std::ios::in);
        if (!fileStream.is_open()) {
          throw Error("Cannot open file !!");
        }

        std::string line;
        while (!fileStream.eof()) {
            getline(fileStream, line);
            content.append(line + "\n");
        }
        fileStream.close();

        handle_ = glCreateShader(shaderId);
        const auto raw_str = content.c_str();
        glShaderSource(handle_, 1, &raw_str, nullptr);
        glCompileShader(handle_);
    }

    Shader(const int shaderId, const std::string &shaderSource) {
        handle_ = glCreateShader(shaderId);
        const auto raw_str = shaderSource.c_str();
        glShaderSource(handle_, 1, &raw_str, nullptr);
        glCompileShader(handle_);
    }

    Shader(Shader &&other) = default;
    Shader &operator=(Shader &&other) = default;

    ~Shader() {
        if (handle_ != GLuint{}) {
            glDeleteShader(handle_);
        }
    }
};

}

#endif //LEARNOPENGLSTARTER__SHADER_HPP_
