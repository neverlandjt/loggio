FROM ubuntu:latest

ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update
RUN  apt-get install -y gcc make cmake git build-essential g++ python-dev
RUN  apt-get install -y autotools-dev libicu-dev libbz2-dev libboost-all-dev libssl-dev
RUN  apt-get install -y autoconf libtool pkg-config

RUN git clone --recurse-submodules -b v1.41.0 https://github.com/grpc/grpc
RUN mkdir -p grpc/cmake/build
WORKDIR /grpc/cmake/build

RUN cmake -DgRPC_INSTALL=ON \
          -DgRPC_BUILD_TESTS=OFF \
          ../..
RUN make -j 2
RUN make install

