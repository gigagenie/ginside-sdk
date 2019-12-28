# G-SDK for Linux

## Contents
* conf/ : KWS model data
* include/ : C++ header files
* lib/ : shared library(libginside.so, libKwsRnet.so)
  - ubuntu-x86_64/*.so : Ubuntu-x86_64 supported
  - centos-x86_64/*.so : CentOS-x86_64 supported
  - raspbian-armv7l/*.so : Raspberry Pi supported
  - poky-aud-systemd-aarch64/*.so : G-INSIDE Reference H/W supported(Yocto Linux aarch64)
* sample-src/ : Sample Client Template Open Source
* test-src/ : SDK API 테스트 앱 Open Source

## Supported Platforms
* Ubuntu Linux
  * Ubuntu x86_64 : OpenSSL 1.1 required
* CentOS Linux
  * CentOS x86_64 : OpenSSL 1.0 required
* Raspbian for Raspberry Pi
  * Raspbian armv7l : OpenSSL 1.1 required
* Embedded Linux
  * G-INSIDE Reference H/W(Yocto Linux aarch64)
    - lib dependency: libprotoc 3.6.1, grpc 1.14.1, cJSON 1.7.12, OpenSSL 1.0.2g

## SDK 이용을 위해 아래 라이브러리가 필요합니다.
* [gRPC](#grpc-installation)
* [cJSON](#cjson-installation)
* OpenSSL

## gRPC installation

https://github.com/grpc/grpc/blob/v1.22.0/src/cpp/README.md

### Pre-requistes
```
[Ubuntu]
$ sudo apt-get install build-essential autoconf automake libtool pkg-config curl make cmake g++ unzip git

[CentOS]
$ sudo yum groupinstall 'Development Tools'
$ sudo yum install cmake

[Raspbian]
$ sudo apt-get install autoconf automake libtool cmake
```

### Protobuf
```
$ git clone https://github.com/protocolbuffers/protobuf.git
$ cd protobuf
$ git submodule update --init --recursive
$ ./autogen.sh

$ ./configure
$ make
$ make check
$ sudo make install
$ sudo ldconfig
```
or
```
Visit to https://github.com/protocolbuffers/protobuf/releases/latest
Download protobuf-cpp-[VERSION].tar.gz
build & install
```

### gRPC C++
```
$ git clone -b $(curl -L https://grpc.io/release) https://github.com/grpc/grpc
$ cd grpc
$ git submodule update --init
$ make
$ sudo make install
```

## cJSON installation

https://github.com/DaveGamble/cJSON/blob/master/README.md

```
$ git clone https://github.com/DaveGamble/cJSON
$ cd cJSON
$ mkdir build
$ cd build
$ cmake ..
$ make
$ sudo make install
$ sudo ldconfig
```