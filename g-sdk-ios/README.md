## GiGA Genie Inside Device SDK for iOS release

### Contents
* InsideSDK.framework/ : GiGA Genie Inside Device SDK iOS Library
* InsideSDK.framework/models : KWS-S 호출어 모델 파일

### Supported Platforms
* iOS 9.0 or later supported
* Xcode 13 버전 권장

### How to use GiGA Genie Inside Device SDK for iOS
```
1. Xcode iOS 프로젝트 생성

2. 프로젝트의 Build Phases - Link Binary With libraries의 +버튼을 눌러 InsideSDK.framework를 추가

3. Build Phases 내의 + 버튼을 눌러 New Copy Files Phase 를 추가한 후 이름을 변경 (Copy Files > Embedded Framework)

  해당 항목이 존재한다면 추가하지 않아도 됩니다.

4. 3번에서 추가한 Embedded Framework - Destination 을 Frameworks 로 변경하고 + 버튼을 눌러서 프레임워크를 추가

5. 프로젝트의 Build Settings 에서 Framework Search Paths 로 검색하여 해당 경로를 개발환경에 맞게 수정

6. 프로젝트의 Build Settings 에서 bitcode 로 검색하여 'NO'로 설정

7. iOS 마이크 권한 추가 및 http 통신을 위한 ATS 설정 추가

  * Info.plist에 'Privacy - Microphone Usage Description'을 추가
  * Info.plist에 'App Transport Security' 설정 추가

8. InsideSDK 를 import 하고 InsideSDKDelegate 를 implement 한 후, agent_onCommand, agent_onEvent 메소드를 재정의 한 다음 개발을 진행
```




