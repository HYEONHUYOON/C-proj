#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include<stdlib.h>

#define READLEN_MAX 1024

typedef __int8 bool;
#define TRUE 1
#define FALSE 0

//������
typedef struct {
	//flag Condition
	//0x01 => ���������� ����

	char flag;
	char buffer[READLEN_MAX];
}tokenBuffer;

//Ű���� ���� ����ü
struct arrayCount {
	//���ڿ� �Է� ����
	int s;
	//���� �Է� ����
	int d;
	//���� �Է� ����
	int c;
	//��ü �Է� ����
	int allCount;
};

struct elementsStruct {
	//count
	int numCnt;
	int chCnt;
	int strCnt;

	//���������ڷ� ���� ������ ����
	int* num;
	char* ch;
	char** str;

	//���� ����
	bool isFaile;
};

//" "���� ���ڿ��� �޴� �Լ�
char *ReadFileRine(FILE* file)
{
	char readBuffer[READLEN_MAX];

	int readCnt = 0;

	bool comment = FALSE;

	// " �� ���ö� ���� �ޱ�
	do {
		// �ּ��� ��
		if (readBuffer[readCnt] == '/')
		{
			readBuffer[readCnt] = fgetc(file);
			if (readBuffer[readCnt] == '/') {
				readBuffer[readCnt] = '\0';
				comment = TRUE;
			}
		}

		//�ּ��� ������
		if (comment == TRUE && readBuffer[readCnt] == '\n')
		{
			comment = FALSE;
		}

		//���� " �� �޾Ҵٸ�
		if (readBuffer[readCnt] == '"' && comment == FALSE) {
			readBuffer[readCnt] = fgetc(file);

			// �ٽ� " �� ���� �� ���� ���ڿ� �ޱ�
			while (readBuffer[readCnt] != '"')
			{
				readBuffer[++readCnt] = fgetc(file);
			}
			readBuffer[readCnt] = '\0';
			return readBuffer;
		}

	}while ((readBuffer[readCnt] = fgetc(file)) != EOF);

	// " "���� ���ڿ��� ���� �� 
	readBuffer[0] = '\0';
	return readBuffer;
}

//%�˻� �Ͽ� �޸� ���� �Ҵ�
struct arrayCount ForMalloc(char* str)
{
	//�ʱ�ȭ
	struct arrayCount cnt = { 0,0,0 };

	int i = 0;
	
	while (str[i] != '\0')
	{
		//���������� ������
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
				//���� ó�� �ϱ�
				printf("���������ڰ� �����Դϴ�.");
			}
		}
		i++;
	}	

	return cnt;
}

//��ūȭ
tokenBuffer* tokenizing(char* str)
{
	int i = 0;
	int cnt = 0;

	//���й��� ���� Ȯ��
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

	// ������ ��Ҹ� NULL�� ����
	ring[cnt + 1].buffer[0] = '\0';

	return ring;
}

//���ڿ����� ���ڿ� ����
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

//Ű���� Ȯ���Ͽ� �Ҵ�
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
		//������ �Ÿ�
		if (origin[originCnt] == compareStr[compCnt])
		{
			printf("�ѱ�\n");
			originCnt++;
			compCnt++;
		}
		else {
			//printf("�ٸ� %c \n", origin[originCnt]);
			//���� ���� ����
			strcpy(str, compareStr + compCnt);

			//��ū �и�
			char str2[READLEN_MAX];
			str2[0] = '\0';
			int originRocation = originCnt;
			//���� ������ �������� �̵�
			originCnt += 2;

			//���������� �� ��ġ ���
			int insertCnt = 0;
			// ���������� �Ǵ� ������ ���ڿ� �޾� ���й��ڷ� ����
			while (origin[originCnt] != '%' && origin[originCnt] != '\0')
			{
				str2[insertCnt] = origin[originCnt];
				originCnt++;
				insertCnt++;
			}
			str2[insertCnt] = '\0';
			//printf("��Ʈ�� : %s \n���� ���ڿ� : %s \n",str, str2);

			//������� �ϸ�
			if (str2[0] != '\0')
			{
				//���й��ڰ� �ִ� ���� ù ��ġ
				char* pch;
				pch = strstr(str, str2);
				//printf("���� : %s\n", pch);

				//���� ���� ��ġ �ڸ�
				RemoveString(str, pch);
				//printf("read str : %s \n", str);
			}

			originCnt = originRocation;
			//printf("�ƴ� % c \n", origin[originCnt]);

			//���������� ������
			if (origin[originCnt] == '%')
			{
				switch (origin[originCnt + 1])
				{
				case 'd':
					//printf("�Է� -  \n");
					element->num[element->numCnt] = atoi(str);
					printf("�Ϸ� : %d \n\n", element->num[element->numCnt]);
					element->numCnt++;
					originCnt += 2;
					compCnt += strlen(str);
					//printf("������? : %c%c \n ������?2 : %c%c \n", origin[originCnt], origin[originCnt + 1], compareStr[compCnt], compareStr[compCnt + 1]);
					break;
				case 's':
					//count ���� �Ͽ� �ùٸ��� �ֱ� ���� 
					//printf("�Է� -  \n");
					strcpy(element->str[element->strCnt], str);
					printf("�Ϸ� : %s \n\n", element->str[element->strCnt]);
					element->strCnt++;
					originCnt += 2;
					compCnt += strlen(str);
					//printf("������? : %c%c \n ������?2 : %c%c \n", origin[originCnt], origin[originCnt+1], compareStr[compCnt], compareStr[compCnt+1]);
					break;
				case 'c':
					//printf("�Է� -  \n");
					element->ch[element->chCnt] = str[0];
					printf("�Ϸ� : %c \n\n", element->ch[element->chCnt]);
					element->chCnt++;
					originCnt += 2;
					compCnt += strlen(str);
					//printf("������? : %c%c \n ������?2 : %c%c \n", origin[originCnt], origin[originCnt + 1], compareStr[compCnt], compareStr[compCnt + 1]);
					break;
				default:
					break;
				}
			}
		}
	}
}

//���ڿ� �� �Լ� %���������ڸ� ������ ���ڰ� ��� �ִ��� �˻�
bool checkStrCmp(char* origin, char* compareStr)
{
	printf("�˻� : %s \n", origin);
	struct arrayCount cnt = ForMalloc(origin); 

	//���� 
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

//������ �� �ּڰ��� ��ȯ
struct elementsStruct Comapre(char* keyWordStr, char* resultStr)
{
	//��� ����ü
	struct elementsStruct elements;
	elements.numCnt = 0;
	elements.chCnt = 0;
	elements.strCnt = 0;

	//Ű���� ���� �ޱ�
	struct arrayCount cnt = ForMalloc(keyWordStr);

	////�ƹ��͵� �ȹ޾��� �� ����ó��
	//if (cnt.d == 0 && cnt.c == 0 && cnt.s == 0)
	//{
	//	return elements;
	//}

	//���� �Ҵ�
	elements.num = (int*)malloc(cnt.d * 4);
	elements.ch = (char*)malloc(cnt.c);
	elements.str = (char**)malloc(sizeof(char*) * cnt.s);
	for (int i = 0; i < cnt.s; i++) {
		elements.str[i] = (char*)malloc(sizeof(char) * READLEN_MAX);
	}
	
	//��ū
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
	
	//�ٸ� ���ڿ� ���� ����
	char** exchange = (char**)malloc(sizeof(char*)*cnt.allCount);
	for (int i = 0; i < cnt.allCount; i++) {
		exchange[i] = (char*)malloc(sizeof(char) * READLEN_MAX);
		strcpy(exchange[i],"\0");
	}

	//while Count
	int exchageCnt = 0;
	int tok1Cnt = 0;
	int tok2Cnt = 0;

	//������ �ϴ� ���� ��ŭ
	while ((strcmp(tokens1[tok1Cnt].buffer,"\0") != 0))
	{
		//������ PASS 
		if (strcmp(tokens1[tok1Cnt].buffer, tokens2[tok2Cnt].buffer) == 0)
		{
			printf("���� \n");
			tok1Cnt++;
			tok2Cnt++;
		}
		else {
			printf("�ٸ� : %s | %s \n", tokens1[tok1Cnt].buffer, tokens2[tok2Cnt].buffer);
			//���� �迭 INDEX
			int tok2Rocate = tok2Cnt;

			//�ٽ� ���� ���ڿ��� ���ö� ���� ���� �Ҵ�� ���ڿ��� ����
			strcat(exchange[exchageCnt], tokens2[tok2Cnt].buffer);
			tok2Cnt++;

			//if (checkStrCmp(tokens1[tok1Cnt + 1].buffer, tokens2[tok2Cnt].buffer) == FALSE) {
			// * DEBUG *
				//�̹���� �ٲپ�� ��
				while (checkStrCmp(tokens1[tok1Cnt + 1].buffer, tokens2[tok2Cnt].buffer) != TRUE)
				{
					printf(" : %s \n", exchange[exchageCnt]);
					strcat(exchange[exchageCnt], " ");
					strcat(exchange[exchageCnt], tokens2[tok2Cnt].buffer);
					tok2Cnt++;
				}
			
			
			printf("�Ϸ� %s \n",exchange);

			//�� �ڿ� �ٷ� ���� ���ڿ� �̶��
			if (tok2Rocate == tok2Cnt)
			{
				tok2Cnt++;
			}
			
			//Ű���� Ȯ���Ͽ� �Ҵ�
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
		printf("������ ���� ���߽��ϴ�. \n");
		return 0;
	}
	else {
		printf("������ �������ϴ�. \n");
	}

	////���� ������
	//fseek(ReadTxt, 0, SEEK_END);
	//int fileSize = ftell(ReadTxt);
	//rewind(ReadTxt);
	
	//���� ���ڿ�
	char keyWordStr[READLEN_MAX];
	char resultStr[READLEN_MAX];

	strcpy(keyWordStr, ReadFileRine(ReadTxt));
	strcpy(resultStr, ReadFileRine(ReadTxt));

	//���������� �޾��� ��
	if (keyWordStr[0] != '\0' && resultStr[0] != '\0') {
		printf("ù��° �� : %s \n", keyWordStr);
		printf("�ι�° �� : %s \n", resultStr);
	}
	else {
		printf("���ڿ� �ޱ⿡ ���� �߽��ϴ�. \n");
		printf("������ �ݽ��ϴ�. \n");
		fclose(ReadTxt);
		return 0;
	}

	//��� ����ü
	struct elementsStruct elements = Comapre(keyWordStr, resultStr);

	//���
	printf("int %d�� => ",elements.numCnt);
	for (int i = 0; i < elements.numCnt; i++)
	{
		printf("%d ��° : %d | ", i+1, elements.num[i]);
	}
	printf("\n");

	printf("char %d�� => ", elements.chCnt);
	for (int i = 0; i < elements.chCnt; i++)
	{
		printf("%d ��° : %c | ", i + 1, elements.ch[i]);
	}
	printf("\n");

	printf("String %d�� => ", elements.strCnt);
	for (int i = 0; i < elements.strCnt; i++)
	{
		printf("%d ��° : %s | ", i + 1, elements.str[i]);
	}
	printf("\n");

	//�Ҵ��� �޸� ����
	free(elements.num);
	free(elements.ch);
	free(elements.str);

	printf("������ �ݽ��ϴ�. \n");
	fclose(ReadTxt);
	return 0;
} 