#pragma once

#include <string>

using namespace std;

namespace GLGame {
    struct Texture {
        unsigned int id;
        string type;
        string path;

        Texture(unsigned int id, string type, string path) : id(id), type(type), path(path) {}
    };
} 