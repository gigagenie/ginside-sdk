## G-Plugin 2.0 for iOS

### Contents
* GPlugin.framework/ : G-Plugin 2.0 iOS Library
* G-Plugin_2.0_Developer_Guide_iOS.pdf : 개발자 가이드 문서

### How to use G-Plugin 2.0 for iOS

1. Xcode iOS 프로젝트 생성 후, GPlugin.framework을 Linked framework으로 추가
2. iOS 마이크 권한 추가 필요
  * 프로젝트 앱 소스에 아래 header 추가

```objective-c
#import <AVFoundation/AVFoundation.h>
```

  * Info.plist에 'Privacy - Microphone Usage Description'을 추가

(!) 자세한 내용은 개발자 가이드 참고
