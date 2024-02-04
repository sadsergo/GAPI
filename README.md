

## How to build project

* mkdir build && cd build
* cmake -DCMAKE_BUILD_TYPE=Release (or Debug) -DUSE_OPENGL=OFF ..
* make -j 8

#### To view not black pictures of scenes need to write:
* cd ./build && make -j 8 && cd .. && ./build/testapp
