# Annotator [ ![Build Status] [travis-image] ] [travis] [ ![Documentation] [codedocs-image] ] [codedocs] [ ![Gitter] [gitter-image] ] [gitter]

[travis-image]: https://travis-ci.org/lasmue/annotator.png?branch=master
[travis]: http://travis-ci.org/lasmue/annotator

[codedocs-image]: https://codedocs.xyz/lasmue/annotator.svg
[codedocs]: https://codedocs.xyz/lasmue/annotator/

[gitter-image]: https://badges.gitter.im/Join%20Chat.svg
[gitter]: https://gitter.im/lasmue/annotator

### Quick start

First install CMake, Qt5, OpenCV and a c++11 compiler (gcc 5).

```
sudo apt-get install libboost-system-dev libboost-filesystem-dev
sudo apt-get install qtbase5-dev
sudo apt-get install libopencv-dev
```

Clone and install [dlib](http://dlib.net).

Then clone and build [annotatorlib](https://github.com/lasmue/annotatorlib).

```sh
git clone https://github.com/lasmue/annotatorlib
cd annotatorlib
git submodule update --init --recursive
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

Now you can clone and build annotator.

```sh
git clone https://github.com/lasmue/annotator
cd annotator
git submodule update --init --recursive
mkdir build
cd build
cmake -Dannotatorlib_DIR=../annotatorlib -DCMAKE_BUILD_TYPE=Debug ..
make
```

### Docker
```sh
xhost +
docker run -e DISPLAY=$DISPLAY -e QT_X11_NOMITSHM=1 -v /tmp/.X11-unix:/tmp/.X11-unix -v ~/:/home/user/ chriamue/annotator
```
