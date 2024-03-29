cmake_minimum_required(VERSION 2.8.3...3.19)
project(rawEngine)

set(default_USE_X11 ON)
find_package(X11)
find_package(OpenGL REQUIRED)

if(X11_FOUND)
    link_directories(${X11_LIBRARY_DIR})
    include_directories(${X11_INCLUDE_DIR})
endif()

list(APPEND PLATFORM_SOURCES main.cpp)


set(SOURCES_FILES_MAIN
${PLATFORM_SOURCES}
	"src/core/App.h"
	"src/core/App.cpp"
	"src/core/timer.h"
	"src/core/timer.cpp"
	"src/core/platform.h"


	"src/renderer/opengl/openGLExtensions.h"
	"src/renderer/opengl/openGLExtensions.cpp"
	"src/renderer/shader.h"
	"src/renderer/shader.cpp"
	"src/renderer/data/mesh.h"
	"src/renderer/data/mesh.cpp"
	"src/renderer/texture.h"
	"src/renderer/texture.cpp"
	"src/renderer/model.h"
	"src/renderer/model.cpp"
	"src/renderer/renderer.h"
	"src/renderer/renderer.cpp"



	"src/scene/loadLevel.h"
	"src/scene/loadLevel.cpp"


	"src/util/ddsLoader.h"
	"src/util/ddsLoader.cpp"
	"src/util/stream.h"
	"src/util/stream.cpp"


	"src/game/particle/particle.h"
	"src/game/particle/fountainEmitter.h"
	"src/game/particle/fountainEmitter.cpp"
	"src/game/particle/particleEmitter.h"
	"src/game/particle/particleEmitter.cpp"
	"src/game/particle/fireRing.h"
	"src/game/particle/fireRing.cpp"
        "src/game/particle/fountain.h"
        "src/game/particle/fountain.cpp"
	"src/game/camera.h"
	"src/game/camera.cpp"
	"src/game/player.h"
	"src/game/player.cpp"
	"src/game/entity.h"
	"src/game/entity.cpp"

        "src/libTexture/typeDefs.h"
        "src/libTexture/data.h"
        "src/libTexture/data.cpp"
        "src/libTexture/texture.h"
        "src/libTexture/texture.cpp"
        "src/libTexture/compressedTexture.h"
        "src/libTexture/compressedTexture.cpp"
        "src/libTexture/ddsDefs.h"
        "src/libTexture/ddsLoader.h"
        "src/libTexture/ddsLoader.cpp"
        "src/libTexture/textureLoader.h"
        "src/libTexture/libTexture.h"
        "src/libTexture/libTexture.cpp"

)

include_directories("libs/")
include_directories("libs/glm/")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${PROJECT_NAME} ${SOURCES_FILES_MAIN})
if (MSVC)
	target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} shell32 user32 gdi32 -NODEFAULTLIB:LIBCMT)
else()

    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} -lm)
    set (CMAKE_CXX_FLAGS "-g")
endif()
    

