## G-Plugin 2.0 for Android release

### Files
* g-plugin-android_2.x.x.aar : G-Plugin 2.0 Android Library
* g-plugin2-android_guide.pdf : G-Plugin 2.0 Developer Guide

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
    implementation 'io.grpc:grpc-okhttp:1.11.0'
    implementation 'io.grpc:grpc-protobuf-lite:1.11.0'
    implementation 'io.grpc:grpc-stub:1.11.0'

    implementation 'javax.annotation:javax.annotation-api:1.2'
}
```

### Manifest 권한 추가
```
<uses-permission android:name="android.permission.INTERNET" />
<uses-permission android:name="android.permission.RECORD_AUDIO" />
<uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />
<uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />
<uses-permission android:name="android.permission.READ_EXTERNAL_STORAGE" />
```