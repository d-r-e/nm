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
    build-essential \
    git
RUN apt-get autoremove -y && apt-get clean -y