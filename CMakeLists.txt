cmake_minimum_required (VERSION 3.8)
project (gapi)

option(USE_OPENGL "Use reference OpenGL implementation" OFF)

# get rid of annoying MSVC warnings.
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
set (CMAKE_CXX_STANDARD 17)

include_directories(".")
if(WIN32)
  link_directories(${ADDITIONAL_LIBRARY_DIRS})
endif()

#uncomment this to detect broken memory problems via gcc sanitizers
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize-address-use-after-scope -fno-omit-frame-pointer -fsanitize=leak -fsanitize=undefined -fsanitize=bounds-strict")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ")

add_executable(app main.cpp 
                       bitmap/Bitmap.cpp
                       bitmap/Bitmap.h
                       lgapi.cpp
                       scene/scene.cpp
                       scene/scene.h
                        "window/window.h" "window/window.cpp" "structs/structs.h" "structs/structs.cpp" "window/AppWindow.cpp" "window/AppWindow.h")

include_directories(./ external/LiteMath)

set_target_properties(app PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

# target_link_libraries(testapp LINK_PUBLIC OpenMP::OpenMP_CXX)                 # for your implementation
