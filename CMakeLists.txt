cmake_minimum_required(VERSION 2.8)
PROJECT(585)

add_definitions(-W -Wall -ansi -pedantic -g)

# QT
find_package(Qt4 COMPONENTS QtCore QtGui QtOpenGL REQUIRED)

FILE(GLOB RENDERER_SOURCES Renderer/*.cpp)
FILE(GLOB RENDERER_HEADERS Renderer/*.h)
FILE(GLOB PHYSICS_SOURCES Physics/*.cpp)
FILE(GLOB PHYSICS_HEADERS Physics/*.h)
FILE(GLOB COMMON_SOURCES Common/*.cpp)
FILE(GLOB COMMON_HEADERS Common/*.h)
set(SOURCES ${RENDERER_SOURCES} ${PHYSICS_SOURCES} ${COMMON_SOURCES} main.cpp)
set(HEADERS ${RENDERER_HEADERS} ${PHYSICS_HEADERS} ${COMMON_HEADERS})


QT4_WRAP_CPP(HEADERS_MOC ${HEADERS})
include(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

#include_directories(build)  # for the moc
#SET(QT_USE_QTOPENGL TRUE)
#GL
find_package(OpenGL)
include_directories(${OPENGL_INCLUDE_DIR})


#bullet
set(BULLET_LIBRARIES libBulletDynamics.a libBulletCollision.a libLinearMath.a )
include_directories(/usr/local/include/bullet/)
link_directories(/usr/local/lib)

ADD_EXECUTABLE(game ${SOURCES} ${HEADERS_MOC})
TARGET_LINK_LIBRARIES(game ${QT_LIBRARIES} ${OPENGL_LIBRARIES} ${BULLET_LIBRARIES})
