# GAPI
gapi for my purposes<br />

Надо удалить папку build и написать в консоль <br />

mkdir build && cd build<br />
cmake -DCMAKE_BUILD_TYPE=Release (или Debug) -DUSE_OPENGL=OFF ..<br />
make -j 8<br />

Если будут рендерится черные картинки, то надо сделать: <br />
cd ..<br />
cd ./build && make -j 8 && cd .. && ./build/testapp<br />
