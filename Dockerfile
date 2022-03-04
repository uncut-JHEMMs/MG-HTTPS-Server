FROM fedora:latest

RUN dnf update -y
RUN dnf -y install gdb
RUN dnf -y install vim
RUN dnf -y install zip
RUN dnf -y install curl
RUN dnf -y install clang
RUN dnf -y install htop
RUN dnf -y install iftop
RUN dnf -y install iotop
RUN dnf -y install cscope
RUN dnf -y install dos2unix
RUN dnf -y install tree
RUN dnf -y group install "Development Tools"
RUN dnf -y install gcc
RUN dnf -y install libcurl
RUN dnf -y install texinfo
RUN dnf -y install gnutls
RUN dnf -y install doxygen
RUN dnf -y install automake
RUN dnf -y install m4
RUN dnf -y install libtool
RUN dnf -y install libmicrohttpd
RUN dnf -y install libmicrohttpd-devel

#RUN git clone https://git.gnunet.org/libmicrohttpd.git
#WORKDIR libmicrohttpd
#RUN ./bootstrap
#RUN mkdir build
#WORKDIR build
#RUN ../configure
#RUN make
#RUN make install
#RUN ldconfig
#WORKDIR /

RUN git clone https://github.com/etr/libhttpserver.git
WORKDIR libhttpserver
RUN ./bootstrap
RUN mkdir build
WORKDIR build
RUN ../configure
RUN make
RUN make install
RUN ldconfig
WORKDIR /

#RUN git clone https://github.com/protocolbuffers/protobuf.git
#WORKDIR protobuf
#RUN git submodule update --init --recursive
#RUN ./autogen.sh
#RUN ./configure
#RUN make
#RUN make check
#RUN make install
#RUN ldconfig

RUN dnf -y install openssl-devel
RUN yum -y install gnutls-devel
RUN dnf -y install openssl
RUN dnf -y install sysstat
RUN dnf -y install perf
RUN dnf -y install ioping
RUN dnf -y install fio
ENV PKG_CONFIG_PATH=/usr/local/lib/pkgconfig
ENV LD_LIBRARY_PATH=/usr/local/lib

WORKDIR /MG-HTTPS-Server/
