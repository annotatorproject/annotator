FROM debian:jessie
MAINTAINER chriamue@gmail.com

RUN apt-get update && apt-get install -y build-essential git cmake wget libopencv-dev
RUN apt-get install -y qt5-default qttools5-dev qttools5-dev-tools libboost-system-dev libboost-filesystem-dev
RUN apt-get install -y libpoco-dev libmysqlclient-dev openssl libssl-dev

RUN cd /tmp && wget http://www.cmake.org/files/v3.6/cmake-3.6.0.tar.gz \
	&& tar xf cmake-3.6.0.tar.gz && cd cmake-3.6.0 && ./configure \
	&& make -j3 && make install && rm -rf /tmp/cmake-3.6.0

RUN cd /tmp && git clone https://github.com/davisking/dlib.git
RUN cd /tmp/dlib && mkdir -p build && cd build && cmake -DCMAKE_BUILD_TYPE=Release .. \
	&& make -j3 && make install && ldconfig && rm -rf /tmp/dlib

RUN cd /tmp && git clone https://github.com/pocoproject/poco.git
RUN cd /tmp/poco && mkdir -p build && cd build && cmake -DCMAKE_BUILD_TYPE=Release .. \
	&& make -j3 && make install && ldconfig && rm -rf /tmp/poco


RUN mkdir -p /workspace && cd /workspace && git clone https://github.com/lasmue/annotatorlib
RUN cd /workspace/annotatorlib && mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Release .. \
	&& make -j3 && make install

RUN cd /workspace && git clone https://github.com/lasmue/annotator
RUN cd /workspace/annotator && mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Release .. \
	&& make -j3

CMD /workspace/annotator/build/annotator
