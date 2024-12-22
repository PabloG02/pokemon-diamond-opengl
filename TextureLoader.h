#pragma once

#include "freeglut.h"
#include <string>

namespace TextureLoader {
GLuint loadTexture(const std::string &texturePath, const bool flipVertically = true);
};
