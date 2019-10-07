# GiGA Genie Inside Device SDK for Windows release

## Contents
* include/ : header files
* lib/ : library(cJSON, G-INSIDE)
* dll/ : 앱 실행시 필요한 DLL 
* data/ : KWS 모델 데이터 및 roots.pem 등

## Application 빌드를 위해 필요한 헤더 파일과 라이브러리(64-bit)
* cJSON header & library file
  * include/cJSON/include/cJSON.h
  * lib/cjson/cjson.lib
* G-INSIDE header & library file
  * include/ginside.h
  * include/ginsidedef.h
  * lib/ginsidewin.lib

## Application 빌드 후 실행을 위해 필요한 DLL(64-bit) 및 데이터 파일
* DLLs
  * cJSON : cjson.dll
  * libcurl : libcurl-x64.dll
  * zlib : zlib.dll
  * OpenSSL : libcrypto-1_1-x64.dll, libssl-1_1-x64.dll
  * **KWS library** : KwsRnet.dll
  * **G-INSIDE library** : ginsidewin.dll
* roots.pem : G-INSIDE 서버 gRPC 연결을 위해 필요한 CA 인증서
  * data/root.pem
* conf/ : KWS 호출어 모델 데이터
  * data/conf/*.cnsf

## Licenses

GiGA Genie Inside Device SDK for Windows와 함께 배포되는 라이브러리들은 아래 라이선스 적용을 받습니다.

* cJSON : MIT license(https://github.com/DaveGamble/cJSON/blob/master/LICENSE)
* libcurl : https://curl.haxx.se/docs/copyright.html
* zlib : http://zlib.net/zlib_license.html
* OpenSSL Apache 2.0 license(https://github.com/openssl/openssl/blob/master/LICENSE)