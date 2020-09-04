# G-SDK for Linux

## Contents
* conf/ : KWS model data
* include/ : C++ header files
* lib/ : shared library(libginside.so, libKwsRnet.so)
  - ubuntu-x86_64/*.so : Ubuntu-x86_64 supported
  - centos-x86_64/*.so : CentOS-x86_64 supported
  - raspbian-armv7l/*.so : Raspberry Pi supported
  - poky-aud-systemd-aarch64/*.so : G-INSIDE Reference H/W supported(Yocto Linux aarch64)
  - arm-cortex-a53_a13x/*.so : ARM Cortex-A53 supported
* sample-src/ : Sample Client Template Open Source
* test-src/ : SDK API 테스트 앱 Open Source

## Supported Platforms
* Ubuntu Linux
  * Ubuntu x86_64 : build with gRPC v1.31.0, cJSON v1.7.x, OpenSSL 1.1
* CentOS Linux
  * CentOS x86_64 : build with gRPC v1.31.0, cJSON v1.7.x, OpenSSL 1.0
* Raspbian for Raspberry Pi
  * Raspbian armv7l : OpenSSL 1.1
* Embedded Linux
  * G-INSIDE Reference H/W(Yocto Linux aarch64)
    - lib dependency: libprotoc 3.6.1, grpc 1.14.1, cJSON 1.7.12, OpenSSL 1.0.2g
  * ARM Cortex-A53

## SDK 이용을 위해 아래 라이브러리가 필요합니다.
* [gRPC and Protocol Buffers](#grpc)
* [cJSON](#cjson)
* [OpenSSL](#openssl)

### Build tools

```
[Ubuntu]
$ sudo apt-get install build-essential autoconf automake libtool pkg-config curl make cmake g++ unzip git

[CentOS]
$ sudo yum groupinstall 'Development Tools'
$ sudo yum install cmake

[Raspbian]
$ sudo apt-get install autoconf automake libtool cmake
```
   
### [gRPC](https://github.com/grpc/grpc) and Protocol Buffers

```
$ git clone --recurse-submodules -b $(curl -L https://grpc.io/release) https://github.com/grpc/grpc
$ cd grpc
$ mkdir -p cmake/build
$ cd cmake/build
$ cmake -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF -DBUILD_SHARED_LIBS=ON \
  -DCMAKE_INSTALL=PREFIX=$MY_INSTALL_DIR \
  ../..
$ make
$ make install
```
SEE ALSO: [Quick Start gRPC](https://grpc.io/docs/languages/cpp/quickstart/)

### [cJSON](https://github.com/DaveGamble/cJSON)

```
$ git clone https://github.com/DaveGamble/cJSON
$ cd cJSON
$ mkdir build
$ cd build
$ cmake -DCMAKE_INSTALL_PREFIX=$MY_INSTALL_DIR ..
$ make
$ make install
```
### OpenSSL

```
[Ubuntu]
$ sudo apt install libssl-dev

[CentOS]
$ sudo yum install openssl-devel
```
