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

	//동적 할당
	elements.num = (int*)malloc(cnt.d * 4);
	elements.ch = (char*)malloc(cnt.s);
	elements.str = (char**)malloc(sizeof(char*) * cnt.s);
	for (int i = 0; i < cnt.s; i++) {
		elements.str[i] = (char*)malloc(sizeof(char) * READLEN_MAX);
	}
	
	//토큰
	tokenBuffer* tokens1 = tokenizing(keyWordStr);
	tokenBuffer* tokens2 = tokenizing(resultStr);

	//debug
	/*int Count = 0;
	while (tokens1[Count].buffer[0] != '\0')
	{
		printf("%s ", tokens1[Count++].buffer);
	}
	printf("\n");
	Count = 0;
	while (tokens2[Count].buffer[0] != '\0')
	{
		printf("%s ", tokens2[Count++].buffer);
	}
	printf("\n");*/
	
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
			//printf("같음 1: %s ,2: %s \n", tokens1[tok1Cnt].buffer, tokens2[tok2Cnt].buffer);
			tok1Cnt++;
			tok2Cnt++;
		}
		//다르면
		else {
			//printf("다름 1: %s ,2: %s \n", tokens1[tok1Cnt].buffer, tokens2[tok2Cnt].buffer);

			//현재 배열 INDEX
			int tok2Rocate = tok2Cnt;

			//다시 같은 문자열이 나올때 까지 동적 할당된 문자열에 붙임
			strcat(exchange[exchageCnt], tokens2[tok2Cnt].buffer);
			//printf("INSERT : %s\n", exchange[exchageCnt]);
			while(strcmp(tokens1[tok1Cnt+1].buffer, tokens2[++tok2Cnt].buffer) != 0)
			{	
				strcat(exchange[exchageCnt], " ");
				strcat(exchange[exchageCnt], tokens2[tok2Cnt].buffer);
				//printf("INSERT : %s\n", exchange[exchageCnt]);
			}

			//그 뒤에 바로 같은 문자열 이라면
			if (tok2Rocate == tok2Cnt)
			{
				tok2Cnt++;
			}

			//키워드 확인하여 할당
			Scan(tokens1[tok1Cnt].buffer, exchange[exchageCnt], &elements);
			//printf("read : %s",exchange[exchageCnt]);
			tok1Cnt++;
			exchageCnt++;
		}
	}
	
	printf("1 : %s 2 : %s 3 : %s \n", exchange[0], exchange[1], exchange[2]);

	free(exchange);
	free(tokens1);
	free(tokens2);
	return elements;
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
Scan(char* origin,char* compareStr, struct elementsStruct* element)
{
	printf("\n-문자열 검사-\n");
	int originCnt = 0;
	int compCnt = 0;

	char str[READLEN_MAX];
	strcpy(str, "\0");

	//char str[READLEN_MAX];

	while (origin[originCnt] != '\0'&& compareStr[compCnt] != '\0')
	{
		int originRocation;
		//같으면 거름
		if (origin[originCnt] == compareStr[compCnt])
		{
			originCnt++;
			compCnt++;
			printf("넘김\n");
		}
		else {
			int originRocation = originCnt;
			//같은 영역 제거
			strcpy(str, compareStr + compCnt);

			//토큰 분리
			char str2[READLEN_MAX];
			str2[0] = '\0';
			//형식 지정자 다음으로 이동
			originCnt += 2;
			int insertCnt = 0;
			// %
			while (origin[originCnt] != '%' && origin[originCnt] != '\0')
			{
				str2[insertCnt] = origin[originCnt];
				originCnt++;
				insertCnt++;
			}
			str2[insertCnt] = '\0';
			printf("스트링 : %s \n나눌 문자열 : %s \n",str, str2);
			
			char* pch;
			pch = strstr(str,str2);
			printf("기준 : %s\n", pch);
			
			RemoveString(str, pch);
			printf("read str : %s \n", str);

			//형식지정자 나오면
			if (origin[originCnt] == '%')
			{
				switch (origin[originCnt + 1])
				{
				case 'd':
					printf("입력 -  \n");
					element->num[element->numCnt] = atoi(str);
					printf("완료 : %s \n\n", element->num[element->numCnt]);
					element->strCnt++;
					compCnt += strlen(str);
					originCnt = originRocation;
					break;
				case 's':
					printf("입력 -  \n");
					strcpy(element->str[element->strCnt], str);
					printf("완료 : %s \n\n", element->str[element->strCnt]);
					element->strCnt++;
					compCnt += strlen(str);
					originCnt = originRocation;
					break;
				case 'c':

					break;
				default:
					break;
				}
			}
		}
	}
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

	//파일 사이즈
	fseek(ReadTxt, 0, SEEK_END);
	int fileSize = ftell(ReadTxt);
	rewind(ReadTxt);
	
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

	printf("파일을 닫습니다. \n");
	free(elements.num);
	free(elements.ch);
	free(elements.str);
	fclose(ReadTxt);
	return 0;
} 