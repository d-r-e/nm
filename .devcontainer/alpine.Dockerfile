FROM alpine:3.18
RUN apk --update --no-cache add \
    bash \
    binutils \
    bison \
    curl \
    diffutils \
    file \
    flex \
    gcc \
    gdb \
    gettext-dev \
    git \
    texinfo \
    libintl \
    libtool \
    make \
    musl-dev \
    valgrind \
    vim \
    wget \
    zlib-dev \
    ;
COPY ./devcontainer/nm-alpine /usr/local/bin/nm
SHELL ["/bin/bash", "-c"]