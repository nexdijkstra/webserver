#include"lotto.h"


void lottomachine::initializeBuf(int size) 
{
				/*cgi에서 값을 받을 변수 초기화*/
				numberBuf = (char*)malloc(sizeof(char) * (size + 1));
				memset(numberBuf, '\0', size + 1);
}

void lottomachine::getNumber(char* buf) 
{
				/*서버에서 넘겨준 body를 파싱을 위해 저장한다.*/
				strcpy(numberBuf + size, buf);
				size += strlen(buf);
}

void lottomachine::splitNumber() 
{

				/*서버에서 넘겨준 값을 구분자, 단위로 파싱하여 숫자 배열에 저장한다.*/
				char* p;
				char* temp = (char*)malloc(sizeof(char) * (size + 1));
				memset(temp, '\0', size + 1);

				strcpy(temp, numberBuf);
				p = strstr(temp, "Content-Type:");
				strcpy(temp, p);
				p = NULL;
				p = strtok(temp, " ");
				p = strtok(NULL, "application/vnd.ms-excel");
				strcpy(temp, p);

				strcpy(numberBuf, temp);

				p = NULL;
				p = strtok(numberBuf, " ");


				while((p = strtok(NULL, ",")) != NULL)
				{
								AtoI(atoi(p));	
				}


}

void lottomachine::pickNumber() 
{//번호를 뽑는 함수
				for (int i = 0; i < 46; i++) {
								lottonumber[i] = 0;
								lottoprobability[i] = 0;
				}
				/*다음의 순서로 번호를 선택해야 선택이 된다.*/
				ratingNumber();
				findProbability(); 
				allRandom(); 

				float tempnumber[46] = { 0.0 };
				for (int i = 0; i < 46; i++) {
								tempnumber[i] = lottoprobability[i];//원본 보존
				}

				int i = 6;
				int index = 1;
				float max = tempnumber[index];
				while (i--) {
								for (int j = 2; j < 46; j++) {
												if (max < tempnumber[j]) {//빈도수의 최대값 구하기
																index = j;
																max = tempnumber[j];
												} else if (max == tempnumber[j]) {//만약 빈도수가 같으면 랜덤하게 수 선택
																int coin = rand() % 2;
																if (coin == 0) {
																				index = j;
																				max = tempnumber[j];
																} else {
																				continue;
																}
												}
								}

								cout << index << " ";
								tempnumber[index] = -1;
								index = 1;
								max = tempnumber[index];
				}
}

void lottomachine::ratingNumber() 
{
				for (int i = 1; i < 46; i++) {
								lottoprobability[i] = ((float) lottonumber[i] / 624);//나온 숫자의 빈도수 구하기 나온 횟수/전체 횟수
				}

}

void lottomachine::findBigNumber() 
{

				int tempnumber[46] = { 0 };
				for (int i = 1; i < 46; i++) {
								tempnumber[i] = lottonumber[i];
				}

				int i = 45;
				int index = 1;
				int max = tempnumber[1];
				while (i--) {
								for (int j = 2; j < 46; j++) {
												if (max < tempnumber[j]) {
																index = j;
																max = tempnumber[j];
												}
								}
								lottoprobability[index] = (float) (tempnumber[index]) / 624;
								tempnumber[index] = 0;
								index = 1;
								max = tempnumber[index];
				}
}

void lottomachine::AtoI(int str) 
{
				lottonumber[str]++;
}

void lottomachine::findProbability() 
{
				int i = 7;
				int odd = 0;
				int even = 0;
				int temp[8] = { 0 };

				while (1) {
								while (i--) {
												/* 1~10 , 11~ 20,...31~40 까지는 20프로의 확률로
												 * 41~45 까지는 10%의 확률로 구한다.*/
												int number = rand() % 9;
												if (number == 0 || number == 1) {
																int number = (rand() % 10) + 1;
																if (number % 2 != 0)
																				odd++;
																else
																				even++;
																temp[i] = number;

												} else if (number == 2 || number == 3) {

																int number = (rand() % 10) + 11;
																if (number % 2 != 0)
																				odd++;
																else
																				even++;
																temp[i] = number;
												} else if (number == 4 || number == 5) {
																int number = (rand() % 10) + 21;
																if (number % 2 != 0)
																				odd++;
																else
																				even++;
																temp[i] = number;

												} else if (number == 6 || number == 7) {
																int number = (rand() % 10) + 31;
																if (number % 2 != 0)
																				odd++;
																else
																				even++;
																temp[i] = number;
												} else if (number == 8) {
																int number = (rand() % 5) + 41;
																if (number % 2 != 0)
																				odd++;
																else
																				even++;
																temp[i] = number;
												}
								}

								if (odd < even) {//홀수가 더 커야지 숫자를 승인
												i = 7;
												odd = 0;
												even = 0;
												continue;

								} else {
												break;
								}

				}

				/*위에서 뽑은 번호에 확률을 더해준다.*/
				for (int k = 1; k < 8; k++) {
								if (temp[k] >= 1 && temp[k] <= 40) {
												if (lottoprobability[temp[k]] >= 0.0) {
																lottoprobability[temp[k]] += 0.04;
												} else {
																lottoprobability[temp[k]] = 0;
																lottoprobability[temp[k]] += 0.04;
												}
								} else {
												if (lottoprobability[temp[k]] >= 0.0) {
																lottoprobability[temp[k]] += 0.02;
												} else {
																lottoprobability[temp[k]] = 0;
																lottoprobability[temp[k]] += 0.02;
												}
								}
				}
}

void lottomachine::allRandom() 
{
				/*모든 번호를 랜덤으로 뽑아봄*/
				int num = (rand() % 45) + 1;
				lottoprobability[num] += (1 / 45);

}
