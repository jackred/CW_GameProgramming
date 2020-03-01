cmake_minimum_required(VERSION 2.8.4)
project(CW_GameProgramming)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 OLD)
endif(COMMAND cmake_policy)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  SET( LINUX 1)
  SET( APPLE 0)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  SET( LINUX 0)
  SET( APPLE 1 ) 
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  SET( WINDOWS 1)
endif()

find_package (PkgConfig REQUIRED)
find_package (OpenGL REQUIRED)
find_library(GLFW3_LIBRARY glfw REQUIRED)

if(LINUX)
  find_path(GLEW_INCLUDE_DIR GL/glew.h)
  find_library(GLEW_LIBRARY GLEW glew REQUIRED)
endif(LINUX)

if (APPLE)
  include_directories(/usr/local/include)
  find_library(GLEW_LIBRARY glew REQUIRED)
  find_library(COCOA_LIBRARY Cocoa REQUIRED)
  find_library(IOKIT_LIBRARY IOKit REQUIRED)
  find_library(COREVID_LIBRARY CoreVideo REQUIRED)
endif (APPLE)

set(SOURCE_FILES
  source/ALight.cpp
  source/ALoader.cpp
  source/AObject.cpp
  source/App.cpp
  source/Ball.cpp
  source/Camera.cpp
  source/DepthBuffer.cpp
  source/DirLight.cpp
  source/Main.cpp
  source/Mesh.cpp
  source/Maze.cpp
  source/MazeDisplay.cpp
  source/Misc.cpp
  source/Model.cpp
  source/MTLLoader.cpp
  source/OBJLoader.cpp
  source/Particles.cpp
  source/PointLight.cpp
  source/Scene.cpp
  source/Shader.cpp
  source/ShaderLoader.cpp
  source/Wall.cpp
  source/Window.cpp)
include_directories(${PROJECT_SOURCE_DIR}/include/)
include_directories(${PROJECT_SOURCE_DIR}/stb_image/)

add_executable(CW_GameProgramming ${SOURCE_FILES})

if (APPLE)
  target_link_libraries (CW_GameProgramming
  ${GLFW3_LIBRARY}
  ${OPENGL_LIBRARIES}
  ${GLEW_LIBRARY}
  ${COCOA_LIBRARY} ${COREVID_LIBRARY} ${IOKIT_LIBRARY})
endif (APPLE)

if (UNIX)
  target_link_libraries (CW_GameProgramming
    ${GLEW_LIBRARY}
    ${GLFW3_LIBRARY}
    ${OPENGL_LIBRARIES}
    pthread)
endif (UNIX)
