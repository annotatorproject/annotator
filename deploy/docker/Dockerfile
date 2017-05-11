FROM ubuntu:16.10
MAINTAINER chriamue@gmail.com

RUN apt-get update && apt-get install -y build-essential clang git cmake \
	qt5-default qttools5-dev qttools5-dev-tools libboost-system-dev libboost-filesystem-dev \
	libopencv-dev libpoco-dev libpocomysql* libpocosqlite* libpoconet* libpocoxml* libmysqlclient-dev openssl libssl-dev

RUN cd /tmp && git clone https://github.com/davisking/dlib.git \
	&& cd /tmp/dlib && mkdir -p build && cd build && cmake -DCMAKE_BUILD_TYPE=Release .. \
	&& make -j3 && make install && ldconfig && rm -rf /tmp/dlib

RUN cd /tmp && git clone  https://github.com/pocoproject/poco \
	&& sed -i 's#mysqlclient_r#mysqlclient mysqlclient_r#g' poco/cmake/FindMySQL.cmake \
	&& cd /tmp/poco && mkdir -p build && cd build && cmake -DCMAKE_BUILD_TYPE=Release .. \
	&& make -j3 && make install && ldconfig && rm -rf /tmp/poco

RUN mkdir -p /workspace && cd /workspace && git clone https://github.com/annotatorproject/annotatorlib
RUN cd /workspace/annotatorlib && mkdir build && cd build && cmake -DCMAKE_CXX_COMPILER=clang++ -DBUILD_SHARED_LIBS=OFF -DCMAKE_BUILD_TYPE=Release .. \
	&& make -j3 && make install

RUN cd /workspace && git clone https://github.com/annotatorproject/annotator
RUN cd /workspace/annotator && mkdir build && cd build && cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Release .. \
	&& make -j3

CMD /workspace/annotator/build/annotator
