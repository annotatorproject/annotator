# Annotator [![Stories in Ready][waffle-image]][waffle][![Build Status][travis-image]][travis][![Documentation][codedocs-image]][codedocs][![Gitter][gitter-image]][gitter]

[travis-image]: https://travis-ci.org/annotatorproject/annotator.png?branch=master
[travis]: http://travis-ci.org/annotatorproject/annotator

[codedocs-image]: https://codedocs.xyz/annotatorproject/annotator.svg
[codedocs]: https://codedocs.xyz/annotatorproject/annotator/

[gitter-image]: https://badges.gitter.im/Join%20Chat.svg
[gitter]: https://gitter.im/annotatorproject/annotator

[waffle-image]: https://badge.waffle.io/annotatorproject/annotator.svg?label=ready&title=Ready
[waffle]: http://waffle.io/annotatorproject/annotator

## Quick start

First install CMake, Qt5, OpenCV and a c++11 compiler (gcc 5).

```
sudo apt-get install libboost-system-dev libboost-filesystem-dev
sudo apt-get install qtbase5-dev
sudo apt-get install libopencv-dev
```

Clone and install [dlib](http://dlib.net).

Then clone and build [annotatorlib](https://github.com/annotatorproject/annotatorlib).

Build LibPoco
```sh
git clone https://github.com/pocoproject/poco.git
cd poco
git checkout poco-1.8.0
sed -i 's#mysqlclient_r#mysqlclient mysqlclient_r#g' cmake/FindMySQL.cmake
cd build
cmake ..
make -j2
```

Then build.

```sh
git clone https://github.com/annotatorproject/annotatorlib
git submodule update --init --recursive
cd annotatorlib
mkdir build
cd build
cmake -DPoco_DIR= ../../poco/build/Poco ..
make
```

Now you can clone and build annotator.

```sh
git clone https://github.com/annotatorproject/annotator
cd annotator
git submodule update --init --recursive
mkdir build
cd build
cmake -Dannotatorlib_DIR=../annotatorlib -DCMAKE_BUILD_TYPE=Debug ..
make
```

## Docker

### CPU

```sh
xhost +
docker run --rm -e DISPLAY=$DISPLAY -e QT_X11_NOMITSHM=1 -v /tmp/.X11-unix:/tmp/.X11-unix -v ~/:/home/user/ chriamue/annotator:cpu
```

### GPU (Cuda)

```sh
xhost +
nvidia-docker run --rm -e DISPLAY=$DISPLAY -e QT_X11_NOMITSHM=1 -v /tmp/.X11-unix:/tmp/.X11-unix -v ~/:/home/user/ chriamue/annotator:gpu
```
