FROM debian:bullseye-slim
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
    git \
    man
RUN apt-get autoremove -y && apt-get clean -y