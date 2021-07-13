## GiGA Genie Inside Device SDK for Android release

### Files
* g-sdk-android_1.1.x.aar : G-SDK Android Library
* kwsrnet-release.aar : KWS-S Android Library

### gradle 외부 라이브러리 추가 설정
```
android{
    compileOptions{
        sourceCompatibility JavaVersion.VERSION_1_8
        targetCompatibility JavaVersion.VERSION_1_8
    }
}

dependencies{
    // JSON
    api 'com.google.code.gson:gson:2.8.2'
    
    implementation 'io.reactivex.rxjava2:rxjava:2.1.14'
    implementation 'io.reactivex.rxjava2:rxandroid:2.0.2'
    
    // REST
    implementation 'com.squareup.retrofit2:retrofit:2.3.0'
    implementation 'com.squareup.retrofit2:converter-gson:2.3.0'
    implementation('com.squareup.retrofit2:converter-simplexml:2.3.0') {
        exclude group: 'xpp3', module: 'xpp3'
        exclude group: 'stax', module: 'stax-api'
        exclude group: 'stax', module: 'stax'
    }
    
    // gRPC
    implementation 'io.grpc:grpc-okhttp:1.38.0'
    implementation 'io.grpc:grpc-protobuf-lite:1.38.0'
    implementation 'io.grpc:grpc-stub:1.38.0'

    implementation 'javax.annotation:javax.annotation-api:1.2'
}
```

### Manifest 권한 추가
```java
<uses-permission android:name="android.permission.INTERNET" />
<uses-permission android:name="android.permission.RECORD_AUDIO" />
<uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />
<uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />
<uses-permission android:name="android.permission.READ_EXTERNAL_STORAGE" />
```
