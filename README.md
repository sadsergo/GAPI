# GAPI
gapi for my purposes

Надо удалить папку build и написать в консоль 
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release (или Debug) -DUSE_OPENGL=OFF ..
make -j 8

Если будут рендерится черные картинки, то надо сделать: 
cd ..
cd ./build && make -j 8 && cd .. && ./build/testapp
