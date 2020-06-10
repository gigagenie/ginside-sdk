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

#ifndef  __TEST_API_H__
#define  __TEST_API_H__


#include  "ginside.h"
#include  "ginsidedef.h"


//		case 'D':
void test_debugmode();
//		case '1':
REGISTER_CODE test_register(std::string ID, std::string KEY, std::string SECRET);
//		case '2':
void test_init(std::string ID, std::string KEY, std::string SECRET, char *puuid);
//		case '3':
void test_unregister();
//		case '4':
void test_reset();
//		case '5':
void test_sendText();
//		case '6':
void test_sendCommand();
//		case '7':
void test_startVoice();
//		case '8':
void test_stopVoice();
//		case '9':
//void test_getTTS();
std::string test_getTTS();
//		case '0':
void test_serviceLogin();
//		case 'a':
void test_dialog();
//		case 'b':
void test_kws_init();
//		case 'c':
void test_kws_detect();
//		case 'e':
void test_kws_setKeyword();
//		case 'f':
void test_setServerInfo();
//		case 'g':
void test_getVersion();
//		case 'i':
void test_setLocation();
//		case 'j':
void test_setModelPath();
//     case 'k':
void test_serviceLoginStatus();
//     case 'l':
void test_serviceLogout();

void test_all_kws();


#endif		// end of __TEST_API_H__
