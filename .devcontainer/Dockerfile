FROM fedora:34 as binsource
FROM rockylinux:9.2 as rocky
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
RUN echo "deb http://debug.mirrors.debian.org/debian-debug/ bullseye-debug main" >> /etc/apt/sources.list.d/debug.list

COPY --from=binsource /usr/bin /bin /fedora/bin/
COPY --from=rocky /usr/bin /bin /rocky/bin/

# Update and install the debug symbols for binutils

RUN apt-get install -yq  binutils-x86-64-linux-gnu-dbg


# Cleanup to reduce image size
RUN apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/*
# Reset the frontend variable
ENV DEBIAN_FRONTEND=dialog

