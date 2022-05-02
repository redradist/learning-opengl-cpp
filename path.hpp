#ifndef LEARNOPENGLSTARTER__PATH_HPP_
#define LEARNOPENGLSTARTER__PATH_HPP_

#include <string>
#include <utility>

namespace opengl {

struct Path {
    std::string value;

    static Path from(std::string value) {
        return Path{std::move(value)};
    }
};

}

#endif //LEARNOPENGLSTARTER__PATH_HPP_
