#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include<stdlib.h>

#define READLEN_MAX 1024

typedef __int8 bool;
#define TRUE 1
#define FALSE 0

//링버퍼
typedef struct {
	//flag Condition
	//0x01 => 형식지정자 있음

	char flag;
	char buffer[READLEN_MAX];
}tokenBuffer;

//키워드 갯수 구조체
struct arrayCount {
	//문자열 입력 갯수
	int s;
	//정수 입력 갯수
	int d;
	//문자 입력 갯수
	int c;
	//전체 입력 갯수
	int allCount;
};

struct elementsStruct {
	//count
	int numCnt;
	int chCnt;
	int strCnt;

	//형식지정자로 받을 포인터 변수
	int* num;
	char* ch;
	char** str;

	//성공 여부
	bool isFaile;
};

//" "안의 문자열을 받는 함수
char *ReadFileRine(FILE* file)
{
	char readBuffer[READLEN_MAX];

	int readCnt = 0;

	bool comment = FALSE;

	// " 가 나올때 까지 받기
	do {
		// 주석일 때
		if (readBuffer[readCnt] == '/')
		{
			readBuffer[readCnt] = fgetc(file);
			if (readBuffer[readCnt] == '/') {
				readBuffer[readCnt] = '\0';
				comment = TRUE;
			}
		}

		//주석이 끝나면
		if (comment == TRUE && readBuffer[readCnt] == '\n')
		{
			comment = FALSE;
		}

		//만약 " 를 받았다면
		if (readBuffer[readCnt] == '"' && comment == FALSE) {
			readBuffer[readCnt] = fgetc(file);

			// 다시 " 가 나올 떄 까지 문자열 받기
			while (readBuffer[readCnt] != '"')
			{
				readBuffer[++readCnt] = fgetc(file);
			}
			readBuffer[readCnt] = '\0';
			return readBuffer;
		}

	}while ((readBuffer[readCnt] = fgetc(file)) != EOF);

	// " "안의 문자열이 없을 때 
	readBuffer[0] = '\0';
	return readBuffer;
}

//%검사 하여 메모리 동적 할당
struct arrayCount ForMalloc(char* str)
{
	//초기화
	struct arrayCount cnt = { 0,0,0 };

	int i = 0;
	
	while (str[i] != '\0')
	{
		//형식지정자 나오면
		if (str[i] == '%')
		{
			switch (str[i+1])
			{
			case 'd':
				cnt.d++;
				cnt.allCount++;
				break;
			case 's':
				cnt.s++;
				cnt.allCount++;
				break;
			case 'c':
				cnt.c++;
				cnt.allCount++;
				break;
			default:
				break;
			}

			if (str[i + 2] == "%")
			{
				//예외 처리 하기
				printf("형식지정자가 연속입니다.");
			}
		}
		i++;
	}	

	return cnt;
}

//토큰화
tokenBuffer* tokenizing(char* str)
{
	int i = 0;
	int cnt = 0;

	//구분문자 갯수 확인
	while (str[i] != '\0')
	{
		if (str[i] == ' ')
		{
			if (str[i - 1] != ' ')
			{
				cnt++;
			}
		}
		i++;
	}

	tokenBuffer* ring = malloc(sizeof(tokenBuffer) * (cnt + 2));

	strcpy(ring[0].buffer, strtok(str, " "));

	for (int i = 0; i < cnt; i++)
	{
		strcpy(ring[i + 1].buffer, strtok(NULL, " "));
	}

	// 마지막 요소를 NULL로 설정
	ring[cnt + 1].buffer[0] = '\0';

	return ring;
}

//문자열에서 문자열 제거
void RemoveString(char* origin, char* delimiter)
{
	int i, j, k;
	int len1 = strlen(origin);
	int len2 = strlen(delimiter);
	int found;

	for (i = 0; i <= len1 - len2; i++) {
		found = 1;
		for (j = i, k = 0; delimiter[k] != '\0'; j++, k++) {
			if (origin[j] != delimiter[k]) {
				found = 0;
				break;
			}
		}
		if (found == 1) {
			for (j = i, k = 0; k < len2; j++, k++) {
				origin[j] = origin[j + len2];
			}
			i--;
			len1 = strlen(origin);
		}
	}

}

//키워드 확인하여 할당
Scan(char* origin, char* compareStr, struct elementsStruct* element)
{
	printf("origin : %s \n comp : %s \n", origin, compareStr);
	int originCnt = 0;
	int compCnt = 0;

	char str[READLEN_MAX];
	strcpy(str, "\0");

	//char str[READLEN_MAX];

	while (origin[originCnt] != '\0' && compareStr[compCnt] != '\0')
	{
		int originRocation;
		//같으면 거름
		if (origin[originCnt] == compareStr[compCnt])
		{
			printf("넘김\n");
			originCnt++;
			compCnt++;
		}
		else {
			//printf("다름 %c \n", origin[originCnt]);
			//같은 영역 제거
			strcpy(str, compareStr + compCnt);

			//토큰 분리
			char str2[READLEN_MAX];
			str2[0] = '\0';
			int originRocation = originCnt;
			//형식 지정자 다음으로 이동
			originCnt += 2;

			//형식지정자 후 위치 기억
			int insertCnt = 0;
			// 형식지정자 또는 끝까지 문자열 받아 구분문자로 받음
			while (origin[originCnt] != '%' && origin[originCnt] != '\0')
			{
				str2[insertCnt] = origin[originCnt];
				originCnt++;
				insertCnt++;
			}
			str2[insertCnt] = '\0';
			//printf("스트링 : %s \n나눌 문자열 : %s \n",str, str2);

			//나누어야 하면
			if (str2[0] != '\0')
			{
				//구분문자가 있는 가장 첫 위치
				char* pch;
				pch = strstr(str, str2);
				//printf("기준 : %s\n", pch);

				//받은 문자 위치 자름
				RemoveString(str, pch);
				//printf("read str : %s \n", str);
			}

			originCnt = originRocation;
			//printf("아니 % c \n", origin[originCnt]);

			//형식지정자 나오면
			if (origin[originCnt] == '%')
			{
				switch (origin[originCnt + 1])
				{
				case 'd':
					//printf("입력 -  \n");
					element->num[element->numCnt] = atoi(str);
					printf("완료 : %d \n\n", element->num[element->numCnt]);
					element->numCnt++;
					originCnt += 2;
					compCnt += strlen(str);
					//printf("같은가? : %c%c \n 같은가?2 : %c%c \n", origin[originCnt], origin[originCnt + 1], compareStr[compCnt], compareStr[compCnt + 1]);
					break;
				case 's':
					//count 수정 하여 올바르게 넣기 수정 
					//printf("입력 -  \n");
					strcpy(element->str[element->strCnt], str);
					printf("완료 : %s \n\n", element->str[element->strCnt]);
					element->strCnt++;
					originCnt += 2;
					compCnt += strlen(str);
					//printf("같은가? : %c%c \n 같은가?2 : %c%c \n", origin[originCnt], origin[originCnt+1], compareStr[compCnt], compareStr[compCnt+1]);
					break;
				case 'c':
					//printf("입력 -  \n");
					element->ch[element->chCnt] = str[0];
					printf("완료 : %c \n\n", element->ch[element->chCnt]);
					element->chCnt++;
					originCnt += 2;
					compCnt += strlen(str);
					//printf("같은가? : %c%c \n 같은가?2 : %c%c \n", origin[originCnt], origin[originCnt + 1], compareStr[compCnt], compareStr[compCnt + 1]);
					break;
				default:
					break;
				}
			}
		}
	}
}

//문자열 비교 함수 %형식지정자를 제외한 문자가 모두 있는지 검사
bool checkStrCmp(char* origin, char* compareStr)
{
	printf("검사 : %s \n", origin);
	struct arrayCount cnt = ForMalloc(origin); 

	//기준 
	char **delimiter = (char**)malloc(sizeof(char*) * cnt.allCount+1);
	for (int i = 0; i < cnt.allCount+1; i++) {
		delimiter[i] = (char*)malloc(sizeof(char) * READLEN_MAX);
		strcpy(delimiter[i], "\0");
	}

	int i = 0;
	int j = 0;
	int delimiterCnt = 0;

	while (origin[i] != '\0')
	{
		if (origin[i] != '%')
		{
			//printf("read 1: %s \n", delimiter[delimiterCnt]);
			delimiter[delimiterCnt][j] = origin[i];
			i++;
			j++;
		}
		else if(origin[i] == '%'){
			delimiter[delimiterCnt][j+1] = '\0';
			//printf("read 2: %s \n", delimiter[delimiterCnt]);
			i += 2;
			j = 0;
			delimiterCnt++;
		}
	}

	delimiter[delimiterCnt][j] = '\0';

	for (int i = 0; i < cnt.allCount + 1; i++)
	{
		char* pch;
		pch = strstr(compareStr, delimiter[i]);
		printf("%s | comp whith | %s \n", compareStr, delimiter[i]);
		printf("read result: %s \n", pch);
		if (pch == NULL)
		{
			break;
			return FALSE;
		}
	}

	return TRUE;
}

//나누어 각 주솟값에 반환
struct elementsStruct Comapre(char* keyWordStr, char* resultStr)
{
	//요소 구조체
	struct elementsStruct elements;
	elements.numCnt = 0;
	elements.chCnt = 0;
	elements.strCnt = 0;

	//키워드 갯수 받기
	struct arrayCount cnt = ForMalloc(keyWordStr);

	////아무것도 안받았을 때 예외처리
	//if (cnt.d == 0 && cnt.c == 0 && cnt.s == 0)
	//{
	//	return elements;
	//}

	//동적 할당
	elements.num = (int*)malloc(cnt.d * 4);
	elements.ch = (char*)malloc(cnt.c);
	elements.str = (char**)malloc(sizeof(char*) * cnt.s);
	for (int i = 0; i < cnt.s; i++) {
		elements.str[i] = (char*)malloc(sizeof(char) * READLEN_MAX);
	}
	
	//토큰
	tokenBuffer* tokens1 = tokenizing(keyWordStr);
	tokenBuffer* tokens2 = tokenizing(resultStr);

	int i = 0;
	while (tokens1[i].buffer[0] != '\0')
	{
		printf("token : %s \n", tokens1[i].buffer);
		i++;
	}

	printf("\n");
	i = 0;
	while (tokens2[i].buffer[0] != '\0')
	{
		printf("token : %s \n", tokens2[i].buffer);
		i++;
	}
	
	//다른 문자열 담을 공간
	char** exchange = (char**)malloc(sizeof(char*)*cnt.allCount);
	for (int i = 0; i < cnt.allCount; i++) {
		exchange[i] = (char*)malloc(sizeof(char) * READLEN_MAX);
		strcpy(exchange[i],"\0");
	}

	//while Count
	int exchageCnt = 0;
	int tok1Cnt = 0;
	int tok2Cnt = 0;

	//나눠야 하는 갯수 만큼
	while ((strcmp(tokens1[tok1Cnt].buffer,"\0") != 0))
	{
		//같으면 PASS 
		if (strcmp(tokens1[tok1Cnt].buffer, tokens2[tok2Cnt].buffer) == 0)
		{
			printf("같음 \n");
			tok1Cnt++;
			tok2Cnt++;
		}
		else {
			printf("다름 : %s | %s \n", tokens1[tok1Cnt].buffer, tokens2[tok2Cnt].buffer);
			//현재 배열 INDEX
			int tok2Rocate = tok2Cnt;

			//다시 같은 문자열이 나올때 까지 동적 할당된 문자열에 붙임
			strcat(exchange[exchageCnt], tokens2[tok2Cnt].buffer);
			tok2Cnt++;

			//if (checkStrCmp(tokens1[tok1Cnt + 1].buffer, tokens2[tok2Cnt].buffer) == FALSE) {
			// * DEBUG *
				//이방식을 바꾸어야 함
				while (checkStrCmp(tokens1[tok1Cnt + 1].buffer, tokens2[tok2Cnt].buffer) != TRUE)
				{
					printf(" : %s \n", exchange[exchageCnt]);
					strcat(exchange[exchageCnt], " ");
					strcat(exchange[exchageCnt], tokens2[tok2Cnt].buffer);
					tok2Cnt++;
				}
			
			
			printf("완료 %s \n",exchange);

			//그 뒤에 바로 같은 문자열 이라면
			if (tok2Rocate == tok2Cnt)
			{
				tok2Cnt++;
			}
			
			//키워드 확인하여 할당
			Scan(tokens1[tok1Cnt].buffer, exchange[exchageCnt], &elements);
			tok1Cnt++;
			exchageCnt++;
		}
	}

	free(exchange);
	free(tokens1);
	free(tokens2);
	return elements;
}

int main()
{
	FILE* ReadTxt;  

	if ((ReadTxt = fopen("DB.txt", "r")) == NULL) {
		printf("파일을 열지 못했습니다. \n");
		return 0;
	}
	else {
		printf("파일을 열었습니다. \n");
	}

	////파일 사이즈
	//fseek(ReadTxt, 0, SEEK_END);
	//int fileSize = ftell(ReadTxt);
	//rewind(ReadTxt);
	
	//저장 문자열
	char keyWordStr[READLEN_MAX];
	char resultStr[READLEN_MAX];

	strcpy(keyWordStr, ReadFileRine(ReadTxt));
	strcpy(resultStr, ReadFileRine(ReadTxt));

	//정상적으로 받았을 때
	if (keyWordStr[0] != '\0' && resultStr[0] != '\0') {
		printf("첫번째 줄 : %s \n", keyWordStr);
		printf("두번째 줄 : %s \n", resultStr);
	}
	else {
		printf("문자열 받기에 실패 했습니다. \n");
		printf("파일을 닫습니다. \n");
		fclose(ReadTxt);
		return 0;
	}

	//요소 구조체
	struct elementsStruct elements = Comapre(keyWordStr, resultStr);

	//출력
	printf("int %d개 => ",elements.numCnt);
	for (int i = 0; i < elements.numCnt; i++)
	{
		printf("%d 번째 : %d | ", i+1, elements.num[i]);
	}
	printf("\n");

	printf("char %d개 => ", elements.chCnt);
	for (int i = 0; i < elements.chCnt; i++)
	{
		printf("%d 번째 : %c | ", i + 1, elements.ch[i]);
	}
	printf("\n");

	printf("String %d개 => ", elements.strCnt);
	for (int i = 0; i < elements.strCnt; i++)
	{
		printf("%d 번째 : %s | ", i + 1, elements.str[i]);
	}
	printf("\n");

	//할당한 메모리 해제
	free(elements.num);
	free(elements.ch);
	free(elements.str);

	printf("파일을 닫습니다. \n");
	fclose(ReadTxt);
	return 0;
} 