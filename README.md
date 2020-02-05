# GiGA Genie Inside SDK

## What's New?

* Update deviceSDK Developer Guide Document (2020-02-05)
* Mobile SDK for iOS v2.0.1beta released (2020-01-21)
* Raspberry Pi 지원 Device SDK v1.0.1 released (2019-12-28)
* Mobile SDK for Android v2.0.6 released (2019-12-05)
* Device SDK for Linux/Android/Windows v1.0.1 released (2019-11-19)

## GiGA Genie Inside 란?

GiGA Genie Inside(이하, G-INSIDE)는 3rd party 개발자가 자신들의 제품(단말 장치, 서비스, 앱 등)에 KT의 AI Platform인 '기가지니'를 올려서 음성인식과 자연어로 제어하고 기가지니가 제공하는 서비스(생활비서, 뮤직, 라디오 등)를 사용할 수 있도록 해줍니다. G-INSIDE는 기가지니가 탑재된 제품을 개발자들이 쉽게 만들 수 있도록 개발 도구와 문서, 샘플 소스 등 개발에 필요한 리소스를 제공합니다.

## GiGA Genie Inside SDK 개요

GiGA Genie Inside SDK는 다양한 개발 환경 지원을 위해 Device SDK, Mobile SDK, Device API 등을 제공합니다.

* GiGA Genie Inside Device SDK(G-SDK)

  Linux, Android(AOSP), Windows, Web 등 단말 플랫폼별로 제공되며, 마이크(mic)에 대한 제어는 3rd party에서 처리

* GiGA Genie Inside Mobile SDK(G-Plugin)

  Android, iOS 등 스마트폰 앱에 기가지니를 탑재하기 위한 용도로 사용 권장, 마이크(mic)에 대한 제어는 G-Plugin 내에서 처리

* GiGA Genie Inside Device API(G-API)

  G-SDK와 G-Plugin에서 사용되는 기가지니 Cloud AI Platform에서 제공하는 API로 HTTP/2 기반 gRPC와 HTTP/1.1 기반 WebSocket 프로토콜 지원
  > G-API 스펙은 별도 공간을 통해 오픈 예정입니다.

## Supported Platform

GiGA Genie Inside SDK가 지원하는 개발 환경은 아래와 같습니다.
* Device SDK
  - Android API level 19 or later
  - Linux : Ubuntu x86_64, CentOS x86_64, Raspbian armv7l, Yocto aarch64(G-INSIDE reference H/W)
  - Windows 64-bit
* Mobile SDK
  - Android API level 22 or later, iOS 9 or later
* G-API
  - C++, Java(inc. support for Android), Objective-C(for iOS), Python, Ruby, Go, C#, Node.js
  > G-API에서 이용하는 gRPC(**g**RPC **R**emote **P**rocedure **C**alls)는 위 프로그래밍 언어를 지원합니다. 자세한 사항은 https://grpc.io 사이트를 참고하세요.

## 배포 패키지 구성

* README.md : this file
* LICENSE.md : GiGA Genie Inside SDK 이용 약관
* NOTICE.md : 오픈소스 고지문
* RELEASE.md : 릴리즈 노트
* docs/ : Developer Guide, etc.
* g-sdk-android/ : Device SDK for Android 라이브러리
* g-sdk-linux/ : Device SDK for Linux 라이브러리 및 샘플 클라이언트
* g-sdk-windows/ : Device SDK for Windows 라이브러리
* g-plugin-android/ : Mobile SDK for Android 라이브러리 및 개발자 가이드
* g-plugin-ios/ : Mobile SDK for iOS 라이브러리 및 개발자 가이드

## SDK 이용 절차

1. API Link(https://apilink.kt.co.kr) 에서 회원가입 
2. 사업 제휴 신청 및 디바이스 등록 (Console > GiGA Genie > 인사이드 디바이스 등록)
3. 디바이스 등록 완료 후 My Device에서 등록한 디바이스 정보 및 개발키 발급 확인 (Console > GiGA Genie > My Device)
4. 발급된 개발키로 개발 서버 연동 및 테스트
    * setServerInfo API로 개발서버 연동(개발서버 host/port는 변경될 수 있음)
    > HOST: inside-dev.gigagenie.ai, gRPC Port: 50109, REST Port: 30109
    * 발급된 개발키 (client_id, client_key, client_secret)로 개발 테스트
5. 개발 완료 후 상용 검증 신청 (Console > GiGA Genie > My Device)
6. 검증 통과 후 발급된 상용키로 상용 서버 연동 및 테스트
    * setServerInfo를 하지 않으면 default로 상용서버로 연동
    * 발급된 상용키 (client_id, client_key, client_secret)로 상용 적용

## Release Notes

기가지니 인사이드 SDK의 신규 기능, 개선사항, 변경사항, 이슈 수정 내역은 [RELEASE.md](RELEASE.md)를 참고하세요.

## License

GiGA Genie INSIDE SDK License는 [LICENSE.md](LICENSE.md)를 참고하세요.

GiGA Genie INSIDE SDK는 아래의 오픈소스 라이브러리를 사용합니다.

* protobuf : New BSD License (https://github.com/protocolbuffers/protobuf/blob/master/LICENSE)
* gRPC : Apache License 2.0 (https://github.com/grpc/grpc/blob/master/LICENSE)
* cJSON : MIT License(https://github.com/DaveGamble/cJSON/blob/master/LICENSE)
* OpenSSL : Apache License 2.0 (https://github.com/openssl/openssl/blob/master/LICENSE)
* 음성호출(KWS) 라이브러리에서 사용 : iniparser, MPack, rnnoise
  - iniparser : MIT License (https://github.com/ndevilla/iniparser/blob/master/LICENSE)
  - MPack : MIT License (https://github.com/ludocode/mpack/blob/develop/LICENSE)
  - rnnoise : New BSD License (https://github.com/xiph/rnnoise/blob/master/COPYING)
