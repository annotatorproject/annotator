FROM ubuntu:18.04
LABEL maintainer="chriamue@gmail.com"
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y build-essential clang git cmake \
	qt5-default qttools5-dev qttools5-dev-tools libboost-system-dev libboost-filesystem-dev libboost-python-dev python-numpy \
	libopencv-dev libpoco-dev libdlib-dev

RUN mkdir -p /workspace && cd /workspace && git clone https://github.com/annotatorproject/annotatorlib
RUN cd /workspace/annotatorlib && mkdir build && cd build && cmake -DCMAKE_CXX_COMPILER=clang++ -DBUILD_SHARED_LIBS=OFF -DCMAKE_BUILD_TYPE=Release .. \
	&& make -j3 && make install

RUN cd /workspace && git clone https://github.com/annotatorproject/annotator
RUN cd /workspace/annotator && mkdir build && cd build && cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Release .. \
	&& make -j3

CMD /workspace/annotator/build/annotator
