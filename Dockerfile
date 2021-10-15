FROM ubuntu:latest

RUN apt-get update
RUN sudo apt-get install gcc make cmake git
RUN sudo apt-get install build-essential g++ python-dev autotools-dev libicu-dev libbz2-dev libboost-all-dev libssl-dev
RUN sudo apt install -y l autoconf libtool pkg-config
RUN git clone --recurse-submodules -b v1.41.0 https://github.com/grpc/grpc
RUN cd grpc & mkdir -p cmake/build & pushd cmake/build
RUN cmake -DgRPC_INSTALL=ON \
          -DgRPC_BUILD_TESTS=OFF \
          ..\..
RUN make -j
RUN make install

EXPOSE 8080

WORKDIR /opt/logger/build
COPY . /opt/logger

RUN cmake .. && \
    cmake --build . && \
    CTEST_OUTPUT_ON_FAILURE=TRUE cmake --build . --target client


