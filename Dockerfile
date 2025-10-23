#FROM --platform=linux/amd64 ubuntu:22.04
FROM ubuntu:22.04

WORKDIR /forestmq

COPY CMakeLists.txt .
COPY *.c .
COPY *.h .
COPY Dockerfile .

ENV APPLE=0
ENV UNIX=1

RUN apt-get update
RUN apt-get install -y libjansson-dev
RUN apt-get install -y libevent-dev
RUN apt-get install -y libsystemd-dev
RUN apt-get install -y librabbitmq-dev
RUN apt-get install -y pkg-config
RUN apt-get install -y cmake
RUN apt-get install -y build-essential

#RUN mkdir build
RUN cmake -S . -B build
RUN make -C build VERBOSE=1

EXPOSE 8005

RUN chmod +x build/forest_mq

ENTRYPOINT ["./build/forest_mq"]
