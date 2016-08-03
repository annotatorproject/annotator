# Annotator

### Quick start

First install CMake, Qt5, OpenCV and a c++11 compiler (gcc 5).

```
sudo apt-get install libboost-system-dev libboost-filesystem-dev
sudo apt-get install qtbase5-dev
sudo apt-get install libopencv-dev
```

Clone and install [dlib](https://dlib.net).

Then clone and build [annotatorlib](github.com/lasmue/annotatorlib).

```sh
git clone https://github.com/lasmue/annotatorlib
git submodule update --init --recursive
cd annotatorlib
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

Now you can clone and build annotator.

```sh
git clone https://github.com/lasmue/annotator
git submodule update --init --recursive
cd annotator
mkdir build
cd build
cmake -Dannotatorlib_DIR=../annotatorlib -DCMAKE_BUILD_TYPE=Debug ..
make
```
