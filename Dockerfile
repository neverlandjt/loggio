FROM boost-grpc-base:latest
ARG target

ENV TARGET $target

EXPOSE 8080

WORKDIR /opt/logger/build
COPY . /opt/logger

RUN cmake .. && \
    cmake --build . && \
    CTEST_OUTPUT_ON_FAILURE=TRUE cmake --build . --target  ${TARGET}


ENTRYPOINT ./${TARGET}