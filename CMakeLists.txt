cmake_minimum_required (VERSION 3.10)
project (gapi)

option(USE_OPENGL "Use reference OpenGL implementation" OFF)
option(WIN_APP_VER "Use version for working with gui" OFF)

# get rid of annoying MSVC warnings.
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
# set (CMAKE_CXX_STANDARD 17)

include_directories(".")
if(WIN32)
  link_directories(${ADDITIONAL_LIBRARY_DIRS})
endif()

#uncomment this to detect broken memory problems via gcc sanitizers
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize-address-use-after-scope -fno-omit-frame-pointer -fsanitize=leak -fsanitize=undefined -fsanitize=bounds-strict")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

if (NOT WIN_APP_VER)
  add_executable(
              gapi main.cpp 
              bitmap/Bitmap.cpp
              bitmap/Bitmap.h
              lgapi.cpp
              scene/scene.cpp
              scene/scene.h
              "structs/structs.h" "structs/structs.cpp" "window/AppWindow.cpp" "window/AppWindow.h"
              )
else()
  add_executable(gapi win_app.cpp "window/AppWindow.cpp" "window/AppWindow.h")
endif()

if (NOT PNG AND WIN32 AND NOT MSVC)
    set(PNG_LIBRARY "C:/Program Files(x86)/libpng")
    set(PNG_PNG_INCLUDE_DIR "C:/Program Files(x86)/libpng/include")
endif()


find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

target_include_directories(gapi 
  PUBLIC ${SDL3_INCLUDE_DIRS}
  PUBLIC ${SDL3IMAGE_INCLUDE_DIRS}
)

if (WIN32 AND NOT MSVC)
    target_link_libraries(gapi PUBLIC ${SDL3_LIBRARIES} SDL3_image::SDL3_image mingw32)
else()
    target_link_libraries(gapi PUBLIC ${SDL3_LIBRARIES} SDL3_image::SDL3_image)
endif()

include_directories(./ external/LiteMath)

set_target_properties(gapi PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

# target_link_libraries(testapp LINK_PUBLIC OpenMP::OpenMP_CXX)                 # for your implementation
