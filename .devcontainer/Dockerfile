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
    git
RUN echo "deb http://debug.mirrors.debian.org/debian-debug/ bullseye-debug main" >> /etc/apt/sources.list.d/debug.list

# Update and install the debug symbols for binutils


# Cleanup to reduce image size
RUN apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/*

# Reset the frontend variable
ENV DEBIAN_FRONTEND=dialog