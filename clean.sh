rm -r build
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -GNinja
cd ..
