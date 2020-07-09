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
  * Ubuntu x86_64 : build with Protobuf v3.12.3, gRPC v1.30.0, cJSON v1.7.x, OpenSSL 1.1
* CentOS Linux
  * CentOS x86_64 : build with Protobuf v3.12.3, gRPC v1.30.0, cJSON v1.7.x, OpenSSL 1.0
* Raspbian for Raspberry Pi
  * Raspbian armv7l : OpenSSL 1.1
* Embedded Linux
  * G-INSIDE Reference H/W(Yocto Linux aarch64)
    - lib dependency: libprotoc 3.6.1, grpc 1.14.1, cJSON 1.7.12, OpenSSL 1.0.2g
  * ARM Cortex-A53

## SDK 이용을 위해 아래 라이브러리가 필요합니다.
* [Protocol Buffers](#protocol-buffers)
* [gRPC](#grpc)
* [cJSON](#cjson)
* OpenSSL

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

### [Protocol Buffers](https://github.com/protocolbuffers/protobuf)
1. Visit [Protobuf latest release](https://github.com/protocolbuffers/protobuf/releases/latest)
2. Download [protobuf-cpp-[VERSION].tar.gz](https://github.com/protocolbuffers/protobuf/releases/latest)
3. Build & Install
   ```
   $ ./configure
   $ make
   $ make check
   $ sudo make install
   $ sudo ldconfig
   ```
   
### [gRPC](https://github.com/grpc/grpc)

    ```
    $ git clone -b $(curl -L https://grpc.io/release) https://github.com/grpc/grpc
    $ cd grpc
    $ git submodule update --init
    $ make
    $ sudo make install
    ```

### [cJSON](https://github.com/DaveGamble/cJSON)

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
