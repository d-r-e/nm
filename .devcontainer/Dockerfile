FROM ubuntu:focal
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -yq \
    gcc \
    make \
    vim \
    tree \
    watch \
    valgrind \
    binutils \
    binwalk \
    build-essential \
    git
RUN apt-get install -yq man
RUN yes | unminimize