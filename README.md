

## How to build project

* mkdir build && cd build
* cmake -DCMAKE_BUILD_TYPE=Release (or Debug) (-G Ninja) ..
* make -j 8 (if you use ninja then write flag -G Ninja in cmake and then type ninja)

#### To view not black pictures of scenes need to write:
* cd ./build && make -j 8 && cd .. && ./build/testapp
