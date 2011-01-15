cmake_minimum_required(VERSION 2.8)
PROJECT(585)

add_definitions(-W -Wall -ansi -pedantic -g)

#aux_source_directory(Renderer RENDERER_SOURCES)

SET(SOURCES ${RENDERER_SOURCES})



# QT
find_package(Qt4 COMPONENTS QtCore QtGui QtOpenGL REQUIRED)

set(SOURCES Renderer/Renderer.cpp Renderer/Main.cpp Renderer/Point.cpp Renderer/Trackball.cpp Renderer/Vector3.cpp)
#set(SOURCES Renderer/*.cpp)
#execute_process(COMMAND find /Users/oricohen/Documents/585/Renderer -name '*.cpp' | xargs echo OUTPUT_VARIABLE RENDERER_SOURCES)
#execute_process(COMMAND echo -n Renderer/Renderer.cpp Renderer/Main.cpp Renderer/Point.cpp Renderer/Trackball.cpp Renderer/Vector3.cpp
#    OUTPUT_VARIABLE RENDERER_SOURCES)
#set(SOURCES ${RENDERER_SOURCES})
set(HEADERS Renderer/Renderer.h Renderer/Point.h Renderer/Trackball.h Renderer/Vector3.h)


QT4_WRAP_CPP(HEADERS_MOC ${HEADERS})
include(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

#include_directories(build)  # for the moc
#SET(QT_USE_QTOPENGL TRUE)
#GL
find_package(OpenGL)
include_directories(${OPENGL_INCLUDE_DIR})

ADD_EXECUTABLE(game ${SOURCES} ${HEADERS_MOC})
TARGET_LINK_LIBRARIES(game ${QT_LIBRARIES} ${OPENGL_LIBRARIES})
