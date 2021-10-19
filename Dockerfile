FROM ubuntu:latest
ARG target

ENV TARGET $target

ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update
RUN  apt-get install gcc make cmake git -y
RUN  apt-get install build-essential g++ python-dev autotools-dev libicu-dev libbz2-dev libboost-all-dev libssl-dev -y
RUN  apt-get install -y autoconf libtool pkg-config
RUN git clone --recurse-submodules -b v1.41.0 https://github.com/grpc/grpc
RUN mkdir -p grpc/cmake/build
WORKDIR /grpc/cmake/build

RUN cmake -DgRPC_INSTALL=ON \
          -DgRPC_BUILD_TESTS=OFF \
          ../..
RUN make -j 2
RUN make install

EXPOSE 8080

WORKDIR /opt/logger/build
COPY . /opt/logger

RUN cmake .. && \
    cmake --build . && \
    CTEST_OUTPUT_ON_FAILURE=TRUE cmake --build . --target  ${TARGET}

ENTRYPOINT ./${TARGET} -p 8080 -a localhost:50051
