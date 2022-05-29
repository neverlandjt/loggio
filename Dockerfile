FROM ubuntu:latest as boost-grpc-base

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
          ../.. \
          && make -j 2 \
          && make install


FROM boost-grpc-base as installation

WORKDIR /opt/logger/build
COPY ../ /opt/logger
RUN ls
RUN cmake .. && \
    cmake --build . && \
    CTEST_OUTPUT_ON_FAILURE=TRUE cmake --build . --target  client server


FROM ubuntu:latest as runner
ARG target

ENV TARGET $target
WORKDIR /opt/logger/build
COPY --from=installation /opt/logger/build/${TARGET} ./
RUN chmod +x ./${TARGET}
EXPOSE 8080

ENTRYPOINT ./${TARGET}