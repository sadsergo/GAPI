cmake_minimum_required (VERSION 3.8)
project (gapi_test)

option(USE_OPENGL "Use reference OpenGL implementation" OFF)

# get rid of annoying MSVC warnings.
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
set (CMAKE_CXX_STANDARD 11)

if(USE_OPENGL)
  add_definitions(-DUSE_OPENGL)
endif()

include_directories(".")
if(WIN32)
  link_directories(${ADDITIONAL_LIBRARY_DIRS})
endif()

find_package(OpenMP)

#uncomment this to detect broken memory problems via gcc sanitizers
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize-address-use-after-scope -fno-omit-frame-pointer -fsanitize=leak -fsanitize=undefined -fsanitize=bounds-strict")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(USE_OPENGL)
add_executable(testapp main.cpp 
                       ./load_img/Bitmap.cpp
                       ./openGL_ref/lgapi_ref.cpp
                       ./scene/scene.cpp)
else()
add_executable(testapp main.cpp 
                       ./load_img/Bitmap.cpp
                       ./pipeline/lgapi.cpp
                       ./scene/scene.cpp
                       ./structs/VBHB.cpp
                       ./structs/Vec.cpp
                       ./structs/Mat.cpp
                       SubPixel_buf.cpp
                        "color/color.cpp" "color/color.h")
endif()
include_directories(./ external/LiteMath)

set_target_properties(testapp PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

if(USE_OPENGL)
  target_link_libraries(testapp LINK_PUBLIC OpenMP::OpenMP_CXX X11 GL GLU glut) # for reference implementation
else()
  target_link_libraries(testapp LINK_PUBLIC OpenMP::OpenMP_CXX ) # No X11                 # for your implementation
endif()
