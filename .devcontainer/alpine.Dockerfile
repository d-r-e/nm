FROM alpine:3.18
RUN apk --update --no-cache add gcc binutils vim valgrind make git curl file musl-dev bash gdb diffutils
SHELL ["/bin/bash", "-c"]