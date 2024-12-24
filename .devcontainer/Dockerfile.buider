ARG UBUNTU_VERSION=jammy

FROM ubuntu:$UBUNTU_VERSION

ENV DEBIAN_FRONTEND=noninteractive
ADD docker/install_dependencies.sh /usr/local/bin/
RUN /usr/local/bin/install_dependencies.sh