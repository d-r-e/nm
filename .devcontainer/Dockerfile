FROM debian:bookworm-slim
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -yq \
    gcc-multilib \
    make \
    vim \
    tree \
    watch \
    valgrind \
    binutils \
    build-essential \
    git \
    curl \
    file
RUN cd /tmp && wget https://ftp.gnu.org/gnu/binutils/binutils-2.41.tar.gz && \
tar -xzf binutils-2.41.tar.gz 

RUN cd binutils-2.41 && ./configure --prefix=/usr/local && \
make && make install && \
rm -rf /tmp/binutils-2.41.tar.gz /tmp/binutils-2.41
RUN apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/*
ENV DEBIAN_FRONTEND=dialog

