cmake_minimum_required (VERSION 3.8)
project (gapi_test)

option(USE_OPENGL "Use reference OpenGL implementation" OFF)

# get rid of annoying MSVC warnings.
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
set (CMAKE_CXX_STANDARD 17)

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

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ")

add_executable(testapp main.cpp 
                       Bitmap.cpp
                       lgapi.cpp
                       scene.cpp
                        "window/window.h" "window/window.cpp" "structs/structs.h" "structs/structs.cpp" "window/AppWindow.cpp" "window/AppWindow.h")

include_directories(./ external/LiteMath)

set_target_properties(testapp PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

target_link_libraries(testapp LINK_PUBLIC OpenMP::OpenMP_CXX)                 # for your implementation
