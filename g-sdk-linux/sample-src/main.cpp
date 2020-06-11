/*
Copyright (c) 2019 KT corp.
 
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
 
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
 
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include  "stdio.h"
#include  "string.h"
#include  "strings.h"
#include  "unistd.h"
#include  "sys/time.h"
#include  "time.h"

#include  "stdlib.h"
#include  "ginside.h"
#include  "ginsidedef.h"
#include  "agent_callback.h"

#define  KWS_DET_DETECTED			4

pthread_t kwsThread;

REGISTER_CODE rc;

// KWS 녹음 FLAG
bool B_KWS = false;
// 음성인식 녹음 FLAG
bool B_RECORD = false;

bool B_EXIT_APP = false;

AgentCallback agent_callback;

// 테스트를 위한 변수
bool TEST = false;
int TEST_CNT = 0;

void* kwsDetect(void* param)
{
    int ret;
    int size = 256;
    short testData[size];

    // 이 메소드는 항상 루프를 돌면서 B_KWS 의 값에 따라, 음성을 입력받을지 안받을지를 체크한다.
    while(true)
    {
        if(B_KWS)
        {
            // TODO: ALSA 라이브러리등을 이용하여 음성데이터를 입력받는다. testData 음성데이터가 있다고 가정한다.
            // 입력받은 음성데이터를 kws_detect 로 넘겨준다.
            ret = kws_detect((short *) testData, size);

            // TEST 변수는 임의로 DETECT 처리를 하기 위해 만든 변수입니다.
            // 개발 시 삭제해주세요.
            if(ret == KWS_DET_DETECTED || TEST)
            {
                // 호출어 인식 시
                // flag 처리
                B_KWS = false;

                printf("kwsDetect KWS_DET_DETECTED!!\n");
                agent_startVoice();
            }
            // 실제 개발시엔 입력받은 음 데이터만 보내면 되니, 아래 usleep 은 제거해주세요.
            usleep(1000*10);
        }
        else
        {
            // kws 활성화 중이 아닐 때
            usleep(1000*10);
        }
    }
}

int main()
{
    // TODO: SDK 이용을 위해 발급 받은 client-id, client-key, client-secret을 입력.
    std::string ID = "YOUR-CLIENT-ID";
    std::string KEY = "YOUR-CLIENT-KEY";
    std::string SECRET = "YOUR-CLIENT-SECRET";
    std::string USERID = "";

    // 0.
    // agent_setServerInfo : 개발용 서버 정보를 설정해준다.
    // debugmode 활성
    printf("agent_setServerInfo : inside-dev.gigagenie.ai, 50109, 30109\n");
    agent_setServerInfo("inside-dev.gigagenie.ai", "50109", "30109");
    agent_debugmode(true);

    // 1. agent_register : ID, KEY, SCERET 은 발급 필요. 성공 시 결과값으로 UUID 가 리턴된다.
    // TODO: USERID는 클라이언트 마다 다른 값이 설정되어야 함(예: 단말의 MAC 주소로 설정)
    rc = agent_register(ID, KEY, SECRET, USERID);
    if (rc.rc != 200)
    {
        // 실패 케이스에 대한 에러 처리 필요
        printf("agent_register fail, rc : %d, rcmsg : %s\n", rc.rc, rc.rcmsg.c_str());
        return 0;
    }
    else
    {
        // 리턴된 UUID 값은 저장 후, 재사용하면 된다(agent_register를 다시 호출하지 않아도 됨).
        printf("agent_register success. uuid : %s, rcmsg : %s\n", rc.uuid.c_str(), rc.rcmsg.c_str());
    }

    // 2. agent_init : UUID 유효성을 체크한다.
    rc = agent_init(ID, KEY, SECRET, rc.uuid.c_str());
    if (rc.rc == 200)
    {
        // init 성공
        // agent_callback 등록
        printf("agent_init success.\n");
        agent_setCommandEventCallback(onCommand, onEvent);
    }
    else
    {
        // 실패 케이스에 대한 에러 처리 필요
        printf("agent_init fail, rc : %d, rcmsg : %s\n", rc.rc, rc.rcmsg.c_str());
        return 0;
    }

    // 3. kws_init : KWS 를 init 시킨다. 성공 시엔 0 을 리턴한다.
    int ret = kws_init();
    if(ret != 0)
    {
        printf("kws_init fail, ret : %d\n", ret);
        return 0;
    }
    else
    {
        printf("kws_init success.\n");
    }

    // 4. agent_callback call init
    // 음성인식을 위한 agent_callback init. thread 시작.
    agent_callback.init();

    // kws 인식을 시작하기 위해 thread 를 시작한다.
    pthread_create(&kwsThread, NULL, &kwsDetect, NULL);
    // kws 인식 flag
    B_KWS = true;
    // 음성 인식 flag
    B_RECORD = false;


    while(!B_EXIT_APP)
    {
        sleep(1);
        // for TEST
        // kws 강제 검출 테스트를 위한 TEST 변수
        if(TEST_CNT < 5)
        {
            TEST_CNT++;
        }
        if(TEST_CNT == 5)
        {
            TEST = true;
        }
    }

    // TODO : 메모리 해제 등 종료될때의 처리를 해준다.
    // thread 해제 등..
}
