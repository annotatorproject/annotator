FROM chriamue/annotatorlib:gpu
MAINTAINER chriamue@gmail.com

RUN cd / && git clone https://github.com/annotatorproject/annotator
RUN cd /annotator && mkdir build && cd build \
    && cmake -DCMAKE_BUILD_TYPE=Release .. \
	&& make -j3

RUN cd /annotator/source/annotatorplugins && git clone https://github.com/annotatorproject/annotatorplugin_camshift \
    && cd ../../build && cmake .. && make -j3

RUN cd /annotator/source/annotatorplugins && git clone https://github.com/annotatorproject/annotatorplugin_dnn_mmod.git \
    && cd ../../build && cmake .. && make -j3
	
RUN cd /annotator/source/annotatorplugins && git clone https://github.com/annotatorproject/annotatorplugin_fhog.git \
    && cd ../../build && cmake .. && make -j3

CMD /annotator/build/annotator
