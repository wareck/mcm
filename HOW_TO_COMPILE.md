## How to compile :

**For Linux :**

    sudo apt install cmake g++ 
    cmake -S . -B build -DCMAKE_BUILD_TYPE=Release 
    cmake --build build -j

**For Windows:**

    sudo apt install g++-mingw-w64-x86-64-posix 
    cmake cmake -S . -B build-win -DCMAKE_TOOLCHAIN_FILE=cmake/mingw-w64-x86_64.cmake -DCMAKE_BUILD_TYPE=Release 
    cmake --build build-win -j

