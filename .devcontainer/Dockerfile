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

RUN apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/*
COPY test/bin/binutils_nm /usr/bin/nm
ENV DEBIAN_FRONTEND=dialog

