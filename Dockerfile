FROM centos:latest
RUN sed -i 's/mirrorlist/#mirrorlist/g' /etc/yum.repos.d/CentOS-Linux-*
RUN sed -i 's|#baseurl=http://mirror.centos.org|baseurl=http://vault.centos.org|g' /etc/yum.repos.d/CentOS-Linux-*

RUN yum update -y
RUN yum -y install gdb
RUN yum -y install vim
RUN yum -y install zip
RUN yum -y install curl
RUN yum -y install clang
RUN yum -y install epel-release
RUN yum -y install htop
RUN yum -y install libmicrohttpd
RUN yum -y install iftop
RUN yum -y install iotop
RUN yum -y install cscope
RUN yum -y install dos2unix
RUN yum -y install tree
RUN dnf -y group install "Development Tools"
RUN dnf -y install gcc-toolset-11-gcc
RUN dnf -y install gcc-toolset-11-gcc-c++
RUN dnf -y --enablerepo=powertools install texinfo

RUN git clone https://git.gnunet.org/libmicrohttpd.git
WORKDIR libmicrohttpd
RUN ./bootstrap
RUN mkdir build
WORKDIR build
RUN ../configure
RUN make
RUN make install
WORKDIR /

RUN git clone https://github.com/etr/libhttpserver.git
WORKDIR libhttpserver
RUN ./bootstrap
RUN mkdir build
WORKDIR build
RUN ../configure
RUN make
RUN make install
WORKDIR /

RUN git clone https://github.com/protocolbuffers/protobuf.git
WORKDIR protobuf
RUN git submodule update --init --recursive
RUN ./autogen.sh
RUN ./configure
RUN make
RUN make check
RUN make install
RUN ldconfig
ENV PKG_CONFIG_PATH=/usr/local/lib/pkgconfig
ENV LD_LIBRARY_PATH=/usr/local/lib

WORKDIR /MG-HTTPS-Server/
