# g-sdk-linux-ws

## Directory 구성

* README.md : this file
* lib/{distro-os}-{cpu} (i.e. ubuntu-x86_64): shared library 파일 
1. libginsidews.so : library for release
2. libginsidews_dbg.so: library for debug
3. libKwsRnet.so: library for kws
* inc: header 파일
* conf: kws 모델파일 


# License

g-sdk-linux-ws SDK는 아래의 오픈소스 라이브러리를 사용합니다.
* [cpp-rest : MIT](https://github.com/microsoft/cpprestsdk/blob/master/license.txt)

g-sdk-linux-ws 샘플앱은 위 g-sdk-linux-ws SDK library와 아래의 오픈소스 라이브러리를 사용합니다.
* [alsa : GPL & LGPL License](https://www.alsa-project.org/wiki/Introduction)

Copyright 2020 KT corp.

Licensed to the Apache Software Foundation (ASF) under one or more contributor license agreements. See the NOTICE file distributed with this work for additional information regarding copyright ownership. The ASF licenses this file to you under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
