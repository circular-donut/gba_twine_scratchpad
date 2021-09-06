FROM debian:stretch

MAINTAINER Aaron Hansen <aaron.hansen@gatech.edu>
# Copied/adapted from https://github.com/devkitPro/docker/blob/master/devkitarm/Dockerfile

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y apt-utils vim && \
    apt-get install -y --no-install-recommends sudo ca-certificates pkg-config curl wget bzip2 xz-utils make git bsdtar doxygen gnupg && \
    apt-get clean

RUN wget https://github.com/devkitPro/pacman/releases/download/v1.0.2/devkitpro-pacman.amd64.deb && \
    dpkg -i devkitpro-pacman.amd64.deb && \
    rm devkitpro-pacman.amd64.deb && \
    dkp-pacman -Scc --noconfirm


ENV DEVKITPRO=/opt/devkitpro
ENV PATH=${DEVKITPRO}/tools/bin:$PATH

RUN sudo ln -s /proc/self/mounts /etc/mtab

RUN sudo dkp-pacman -Syyu --noconfirm gba-dev && \
    dkp-pacman -Scc --noconfirm

RUN mkdir /butano
RUN mkdir /common
RUN mkdir /butanoPackage

ENV DEVKITARM=${DEVKITPRO}/devkitARM

# Python dependencies
RUN sudo apt-get install -y python

WORKDIR /butanoPackage
ADD https://github.com/GValiente/butano/archive/refs/tags/7.2.0.tar.gz /butanoPackage
RUN tar -xzf 7.2.0.tar.gz
RUN rm 7.2.0.tar.gz

WORKDIR /
RUN mkdir /gba
RUN mkdir /gba/src

RUN cp -r butanoPackage/butano-7.2.0/butano /

RUN cp -r butanoPackage/butano-7.2.0/common /


WORKDIR /
COPY . /gba/src
CMD cd /gba/src && make clean && make -j8
VOLUME ["/gba/src"]