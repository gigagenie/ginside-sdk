# G-SDK for Linux

## Contents
* conf/ : KWS model data
* include/ : C++ header files
* lib/ : shared library(libginside.so, libKwsRnet.so)
  - ubuntu-x86_64/*.so : Ubuntu-x86_64 supported
  - ubuntu-aarch64/*.so : Ubuntu-aarch64 supported
  - centos-x86_64/*.so : CentOS-x86_64 supported
  - raspbian-armv7l/*.so : Raspberry Pi supported
  - poky-aud-systemd-aarch64/*.so : G-INSIDE Reference H/W supported(Yocto Linux aarch64)
  - arm-cortex-a53_a13x/*.so : ARM Cortex-A53 supported
* test-src/ : SDK API 테스트 앱 Open Source
* [sample-src](https://github.com/gigagenie/sample-client-linux-grpc) : Sample Client Template Open Source

## Supported Platforms
* Ubuntu Linux
  * Ubuntu x86_64 : build with gRPC v1.45.2, protobuf v3.19.4, cJSON v1.7.x, OpenSSL 1.1
  * Ubuntu aarch64 : build with gRPC v1.45.2, protobuf v3.19.4, cJSON v1.7.x, OpenSSL 1.1
* CentOS Linux
  * CentOS x86_64 : build with gRPC v1.45.2, protobuf v3.19.4, cJSON v1.7.x, OpenSSL 1.0
* Raspbian for Raspberry Pi
  * Raspbian armv7l : build with gRPC v1.33.1, protobuf v3.13.0, cJSON v1.7.x, OpenSSL 1.1
* Embedded Linux
  * G-INSIDE Reference H/W(Yocto Linux aarch64)
    - lib dependency: libprotoc 3.6.1, grpc 1.14.1, cJSON 1.7.12, OpenSSL 1.0.2g
  * ARM Cortex-A53

## SDK 이용을 위해 아래 라이브러리가 필요합니다.
* [OpenSSL](#openssl)
* [cJSON](#cjson)
* [gRPC and Protocol Buffers](#grpc)

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

### OpenSSL

```
[Ubuntu]
$ sudo apt install libssl-dev

[CentOS]
$ sudo yum install openssl-devel
```

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

### [gRPC](https://github.com/grpc/grpc)

```
$ git clone --recurse-submodules -b v1.45.2 https://github.com/grpc/grpc
$ cd grpc
$ mkdir -p cmake/build
$ cd cmake/build
$ cmake \
  -DgRPC_INSTALL=ON \
  -DgRPC_BUILD_TESTS=OFF \
  -DgRPC_SSL_PROVIDER=package \
  -DBUILD_SHARED_LIBS=ON \
  -DCMAKE_INSTALL_PREFIX=$MY_INSTALL_DIR \
  -DgRPC_BUILD_CSHARP_EXT=OFF \
  -DgRPC_BUILD_GRPC_CSHARP_PLUGIN=OFF \
  -DgRPC_BUILD_GRPC_NODE_PLUGIN=OFF \
  -DgRPC_BUILD_GRPC_OBJECTIVE_C_PLUGIN=OFF \
  -DgRPC_BUILD_GRPC_PHP_PLUGIN=OFF \
  -DgRPC_BUILD_GRPC_PYTHON_PLUGIN=OFF \
  -DgRPC_BUILD_GRPC_RUBY_PLUGIN=OFF \
  ../..
$ make
$ make install
```
SEE ALSO: [Quick Start gRPC](https://grpc.io/docs/languages/cpp/quickstart/)

### Protocol Buffer

```
[gRPC 설치시 자동 설치되며, 필요한 경우 gRPC 최상위 폴더에서 진행]
$ cd third_party/protobuf
$ ./autogen.sh
$ ./configure
$ make
$ sudo make install
$ sudo ldconfig
```

