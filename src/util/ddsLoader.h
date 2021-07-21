#ifndef DDS_LOADER_H_
#define DDS_LOADER_H_

#include "src/renderer/opengl/openGLExtensions.h"
#include <stdlib.h>
#include <string.h>

namespace util {
// Load a .DDS file using GLFW's own loader
GLuint loadDDS(const char * imagepath);

}

#endif
