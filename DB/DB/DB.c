#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include<stdlib.h>

#define READLEN_MAX 1024

typedef __int8 bool;
#define TRUE 1
#define FALSE 0

//버퍼
typedef struct {
	//flag Condition
	//0x01 => 형식지정자 연속됨
	//0x02 => 형식지정자 단독

	char flag;
	char buffer[READLEN_MAX];
}tokenBuffer;

typedef struct {
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
}elementsStruct;

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

//" "안의 문자열을 받는 함수
char *ReadFileRine(FILE* file)
{
	char readBuffer[READLEN_MAX];
	int readCnt = 0;
	bool comment = FALSE;

	// " 가 나올때 까지 받기
	do {
		// 주석일 때
		if (readBuffer[readCnt] == '/'){
			readBuffer[readCnt] = fgetc(file);
			if (readBuffer[readCnt] == '/') {
				readBuffer[readCnt] = '\0';
				comment = TRUE;
			}
		}

		//주석이 끝나면
		if (comment == TRUE && readBuffer[readCnt] == '\n'){
			comment = FALSE;
		}

		//만약 " 를 받았다면
		if (readBuffer[readCnt] == '"' && comment == FALSE) {
			readBuffer[readCnt] = fgetc(file);

			// 다시 " 가 나올 떄 까지 문자열 받기
			while (readBuffer[readCnt] != '"'){
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
		if (str[i] == '%'){
			switch (str[i+1]){
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

//예외처리
bool TokenCheck(tokenBuffer token) {
	int i =0;
	while (token.buffer[i] != '\0')
	{
		//연속된 형식지정자
		if (token.buffer[i] == '%' && token.buffer[i + 2] == '%')
		{
			return FALSE;
		}
		//단독된 형식지정자
		if(token.buffer[i] == '%' && token.buffer[i + 2] == '\0')
		{
			return FALSE;
		}
		i++;
	}
	return TRUE;
}

//토큰화
tokenBuffer* tokenizing(char* str)
{
	int i = 0;
	int cnt = 0;

	//나눌 구분문자 갯수 확인
	while (str[i] != '\0'){
		if ((str[i] == ' ')&&(str[i - 1] != ' ')){
			cnt++;
		}
		i++;
	}

	//토큰 동적 할당
	tokenBuffer* token = malloc(sizeof(tokenBuffer) * (cnt + 2));

	//차례대로 토큰화
	strcpy(token[0].buffer, strtok(str, " "));

	for (int i = 0; i < cnt; i++){
		strcpy(token[i + 1].buffer, strtok(NULL, " "));
	}

	// 마지막 요소에 NULL문자
	token[cnt + 1].buffer[0] = '\0';

	return token;
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
Scan(char* origin, char* compareStr, elementsStruct* element)
{
	printf("origin : %s \ncompareStr : %s\n", origin, compareStr);
	int originCnt = 0;
	int compCnt = 0;

	char str[READLEN_MAX];
	str[0] = '\0';

	while (origin[originCnt] != '\0' && compareStr[compCnt] != '\0'){
		int originRocation;
		//같으면 거름
		if (origin[originCnt] == compareStr[compCnt]){
			originCnt++;
			compCnt++;
		}else {
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
			while (origin[originCnt] != '%' && origin[originCnt] != '\0'){
				str2[insertCnt] = origin[originCnt];
				originCnt++;
				insertCnt++;
			}
			str2[insertCnt] = '\0';

			//나누어야 하면
			if (str2[0] != '\0'){
				//구분문자가 있는 가장 첫 위치
				char* pch;
				pch = strstr(str, str2);

				//받은 문자 위치 자름
				RemoveString(str, pch);
			}
			originCnt = originRocation;

			//형식지정자 나오면
			if (origin[originCnt] == '%'){
				switch (origin[originCnt + 1]){
				case 'd':
					element->num[element->numCnt] = atoi(str);
					element->numCnt++;
					originCnt += 2;
					compCnt += strlen(str);
					break;
				case 's':
					//count 수정 하여 올바르게 넣기 수정 
					strcpy(element->str[element->strCnt], str);
					element->strCnt++;
					originCnt += 2;
					compCnt += strlen(str);
					break;
				case 'c':
					//printf("입력 -  \n");
					element->ch[element->chCnt] = str[0];
					element->chCnt++;
					originCnt += 2;
					compCnt += strlen(str);
					break;
				default:
					break;
				}
			}
		}
	}
}

//문자열 비교 함수 %형식지정자를 제외한 문자가 모두 포함되어 있는지 검사
bool checkStrCmp(char* origin, char* compareStr)
{
	struct arrayCount cnt = ForMalloc(origin); 
	char **delimiter = (char**)malloc(sizeof(char*) * cnt.allCount+1);
	for (int i = 0; i < cnt.allCount+1; i++) {
		delimiter[i] = (char*)malloc(sizeof(char) * READLEN_MAX);
		strcpy(delimiter[i], "\0");
	}

	int i = 0;
	int j = 0;
	int delimiterCnt = 0;

	while (origin[i] != '\0'){
		if (origin[i] != '%'){
			delimiter[delimiterCnt][j] = origin[i];
			i++;
			j++;
		}
		else{
			i += 2;
			delimiter[delimiterCnt][j] = '\0';
			j = 0;
			delimiterCnt++;
		}
	}
	
	delimiter[delimiterCnt][j] = '\0';

	for (int i = 0; i < cnt.allCount + 1; i++){
		char* pch;
		pch = strstr(compareStr, delimiter[i]);
		if (pch == NULL){
			return FALSE;
			break;
		}
	}
	return TRUE;
}

//나누어 각 주솟값에 반환
elementsStruct Comapre(char* keyWordStr, char* resultStr)
{
	//요소 구조체
	elementsStruct elements;
	elements.numCnt = 0;
	elements.chCnt = 0;
	elements.strCnt = 0;

	//키워드 갯수 받기
	struct arrayCount cnt = ForMalloc(keyWordStr);

	////아무것도 안받았을 때 예외처리
	if (cnt.d == 0 && cnt.c == 0 && cnt.s == 0)
	{
		elements.isFaile = FALSE;
		return elements;
	}

	//토큰
	tokenBuffer* tokens1 = tokenizing(keyWordStr);
	tokenBuffer* tokens2 = tokenizing(resultStr);

	int i = 0;
	//문자열 규칙오류 검사
	while (tokens1[i].buffer[0] != '\0')
	{
		if (!TokenCheck(tokens1[i]))
		{
			elements.isFaile = FALSE;
			return elements;
		}
		i++;
	}
	//동적 할당
	elements.num = (int*)malloc(cnt.d * 4);
	elements.ch = (char*)malloc(cnt.c);
	elements.str = (char**)malloc(sizeof(char*) * cnt.s);
	for (int i = 0; i < cnt.s; i++) {
		elements.str[i] = (char*)malloc(sizeof(char) * READLEN_MAX);
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
	while ((strcmp(tokens1[tok1Cnt].buffer,"\0") != 0)){
		//같으면 PASS 
		if (strcmp(tokens1[tok1Cnt].buffer, tokens2[tok2Cnt].buffer) == 0){
			tok1Cnt++;
			tok2Cnt++;
		}
		else {
			//현재 배열 INDEX
			int tok2Rocate = tok2Cnt;

			//다시 같은 문자열이 나올때 까지 동적 할당된 문자열에 붙임
			strcat(exchange[exchageCnt], tokens2[tok2Cnt].buffer);
			tok2Cnt++;
			while (checkStrCmp(tokens1[tok1Cnt + 1].buffer, tokens2[tok2Cnt].buffer) != TRUE){
				strcat(exchange[exchageCnt], " ");
				strcat(exchange[exchageCnt], tokens2[tok2Cnt].buffer);
				tok2Cnt++;
			}
			//그 뒤에 바로 같은 문자열 이라면
			if (tok2Rocate == tok2Cnt){
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
	elementsStruct elements = Comapre(keyWordStr, resultStr);

	if (elements.isFaile == FALSE)
	{
		printf("문자열이 규칙에 맞지 않습니다.");
		return 0;
	}

	//출력
	printf("int %d개 => ",elements.numCnt);
	for (int i = 0; i < elements.numCnt; i++){
		printf("%d 번째 : %d | ", i+1, elements.num[i]);
	}
	printf("\n");

	printf("char %d개 => ", elements.chCnt);
	for (int i = 0; i < elements.chCnt; i++){
		printf("%d 번째 : %c | ", i + 1, elements.ch[i]);
	}
	printf("\n");

	printf("String %d개 => ", elements.strCnt);
	for (int i = 0; i < elements.strCnt; i++){
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