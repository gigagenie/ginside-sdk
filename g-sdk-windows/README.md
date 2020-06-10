# GiGA Genie Inside Device SDK for Windows release

## Contents
* include/ : header files
* lib/ : library(cJSON, G-INSIDE)
* dll/ : 앱 실행시 필요한 DLL 
* data/ : KWS 모델 데이터 및 roots.pem 등

## Application 빌드를 위해 필요한 헤더 파일과 라이브러리
* cJSON header & library file
  * 64bit
    - include/cJSON/include/cJSON.h
    - lib/cjson/cjson.lib
  * 32bit
    - include/cJSON-x86/include/bcJSON.h, cJSON.h
    - lib/cjson-x86/cjson.lib, cJSON.c
* G-INSIDE header & library file
  * 공통
    - include/ginside.h
    - include/ginsidedef.h
  * 64bit
    - lib/ginsidewin/ginsidewin.lib
  * 32bit
    - lib/ginsidewin-x86/ginsidewin.lib

## Application 빌드 후 실행을 위해 필요한 DLL 및 데이터 파일
* DLLs
  * 64bit
    - cJSON : dll/dllx64/cjson.dll
    - libcurl : dll/dllx64/libcurl-x64.dll
    - zlib : dll/dllx64/zlib.dll
    - OpenSSL : dll/dllx64/libcrypto-1_1-x64.dll, libssl-1_1-x64.dll
    - **KWS library** : dll/dllx64/KwsRnet.dll
    - **G-INSIDE library** : dll/dllx64/ginsidewin.dll
  * 32bit
    - cJSON : dll/dllx86/cjson.dll
    - libcurl : dll/dllx86/libcurl.dll
    - zlib : dll/dllx86/zlib.dll
    - OpenSSL :  dll/dllx86/libcrypto-1_1.dll, libssl-1_1.dll
    - **KWS library** : dll/dllx86/KwsRnet.dll
    - **G-INSIDE library** : dll/dllx86/ginsidewin.dll
* roots.pem : G-INSIDE 서버 gRPC 연결을 위해 필요한 CA 인증서
  * data/root.pem
* conf/ : KWS 호출어 모델 데이터
  * data/conf/*.cnsf

## Visual Studio에 외부 라이브러리 추가 방법

Visual Studio를 이용하여 개발하는 3rd Party Application에서 Window SDK를 사용하기 위해서는 외부 라이브러리를 링크해야 한다. Visual Studio에서 외부 라이브러리를 링크하는 방법에 대해서 아래에 설명한다.

- xx.sln 파일(개발하고 있는 솔루션 파일)을 오픈하고 오른쪽 상단의 솔루션 탐색기에서 프로젝트 명을 오른쪽 마우스 클릭하여 속성 페이지에 들어간다.
- 속성창 왼쪽 메뉴에서 **C/C++ > 일반**탭을 선택하고 **추가 포함 디렉터리리**의 편집 화면에 진입한다.
- 이 때 Window SDK와 같이 배포되는 include/ 내 헤더 파일의 경로를 추가한다. 이때 빌드하는 플랫폼(x64/x86)과 동일한 플랫폼으로 제공되는 헤더 파일의 경로를 추가해야 한다.
- 그 후 속성창 왼쪽 메뉴에서 **링커 > 일반**탭을 선택하고 **추가 라이브러리 디렉터리**의 편집 화면에 진입한다.
- 이번에는 배포되는 lib/ 내 라이브러리의 경로를 추가한다. 앞선 설명에서와 같이 빌드하는 플랫폼과 동일한 플렛폼으로 제공되는 라이브러리 경로를 추가한다.
- 마지막으로 **링커 > 입력**탭을 선택하고 **추가 종속성**의 편집 화면에 진입한다.
- 빌드에 필요한 라이브러리(cjson.lib, ginsidewin.lib)를 추가하고 저장한다.


## Licenses

GiGA Genie Inside Device SDK for Windows와 함께 배포되는 라이브러리들은 아래 라이선스 적용을 받습니다.

* cJSON : MIT license(https://github.com/DaveGamble/cJSON/blob/master/LICENSE)
* libcurl : https://curl.haxx.se/docs/copyright.html
* zlib : http://zlib.net/zlib_license.html
* OpenSSL Apache 2.0 license(https://github.com/openssl/openssl/blob/master/LICENSE)