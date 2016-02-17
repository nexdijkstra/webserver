#include<stdio.h>
#include<iostream>
#include<string.h>
#include<unistd.h>
#include<string>
#include<time.h>
#include<fstream>
#include<stdlib.h>

using namespace std;

class lottomachine {

				public:
				char buf[BUFSIZ];
				char* numberBuf;
				float lottoprobability[46];//로또의 확률을 저장하는 변수
				int lottonumber[46];//로또 번호의 빈도수를 저장하는 변수
				int size;
				fstream Lottofile;
				public:

				lottomachine() {
								size = 0;
								memset(buf, '\0', BUFSIZ);
								for (int i = 0; i < 46; i++) {
												lottonumber[i] = 0;
												lottoprobability[i] = 0;
								}
				}
				void initializeBuf(int size);//cgi에서 넘어온 데이터저장 변수 초기화

				void getNumber(char* buf);//cgi에서 넘어온 데이터를 저장하는 함수

				void splitNumber();//데이터 파싱

				void pickNumber();//번호를 출력하는 함수

				private:
				void ratingNumber();//빈도수구하는 함수

				void findBigNumber();

				void AtoI(int str);//번호를 누적하는 함수

				void findProbability();//번호가 나올 확률 계산

				void allRandom();//랜덤으로 무작위 수 추첨
};
