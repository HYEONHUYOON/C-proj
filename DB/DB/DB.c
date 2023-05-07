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

	//���� �Ҵ�
	elements.num = (int*)malloc(cnt.d * 4);
	elements.ch = (char*)malloc(cnt.s);
	elements.str = (char**)malloc(sizeof(char*) * cnt.s);
	for (int i = 0; i < cnt.s; i++) {
		elements.str[i] = (char*)malloc(sizeof(char) * READLEN_MAX);
	}
	
	//��ū
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
			//printf("���� 1: %s ,2: %s \n", tokens1[tok1Cnt].buffer, tokens2[tok2Cnt].buffer);
			tok1Cnt++;
			tok2Cnt++;
		}
		//�ٸ���
		else {
			//printf("�ٸ� 1: %s ,2: %s \n", tokens1[tok1Cnt].buffer, tokens2[tok2Cnt].buffer);

			//���� �迭 INDEX
			int tok2Rocate = tok2Cnt;

			//�ٽ� ���� ���ڿ��� ���ö� ���� ���� �Ҵ�� ���ڿ��� ����
			strcat(exchange[exchageCnt], tokens2[tok2Cnt].buffer);
			//printf("INSERT : %s\n", exchange[exchageCnt]);
			while(strcmp(tokens1[tok1Cnt+1].buffer, tokens2[++tok2Cnt].buffer) != 0)
			{	
				strcat(exchange[exchageCnt], " ");
				strcat(exchange[exchageCnt], tokens2[tok2Cnt].buffer);
				//printf("INSERT : %s\n", exchange[exchageCnt]);
			}

			//�� �ڿ� �ٷ� ���� ���ڿ� �̶��
			if (tok2Rocate == tok2Cnt)
			{
				tok2Cnt++;
			}

			//Ű���� Ȯ���Ͽ� �Ҵ�
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
Scan(char* origin,char* compareStr, struct elementsStruct* element)
{
	printf("\n-���ڿ� �˻�-\n");
	int originCnt = 0;
	int compCnt = 0;

	char str[READLEN_MAX];
	strcpy(str, "\0");

	//char str[READLEN_MAX];

	while (origin[originCnt] != '\0'&& compareStr[compCnt] != '\0')
	{
		int originRocation;
		//������ �Ÿ�
		if (origin[originCnt] == compareStr[compCnt])
		{
			originCnt++;
			compCnt++;
			printf("�ѱ�\n");
		}
		else {
			int originRocation = originCnt;
			//���� ���� ����
			strcpy(str, compareStr + compCnt);

			//��ū �и�
			char str2[READLEN_MAX];
			str2[0] = '\0';
			//���� ������ �������� �̵�
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
			printf("��Ʈ�� : %s \n���� ���ڿ� : %s \n",str, str2);
			
			char* pch;
			pch = strstr(str,str2);
			printf("���� : %s\n", pch);
			
			RemoveString(str, pch);
			printf("read str : %s \n", str);

			//���������� ������
			if (origin[originCnt] == '%')
			{
				switch (origin[originCnt + 1])
				{
				case 'd':
					printf("�Է� -  \n");
					element->num[element->numCnt] = atoi(str);
					printf("�Ϸ� : %s \n\n", element->num[element->numCnt]);
					element->strCnt++;
					compCnt += strlen(str);
					originCnt = originRocation;
					break;
				case 's':
					printf("�Է� -  \n");
					strcpy(element->str[element->strCnt], str);
					printf("�Ϸ� : %s \n\n", element->str[element->strCnt]);
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
		printf("������ ���� ���߽��ϴ�. \n");
		return 0;
	}
	else {
		printf("������ �������ϴ�. \n");
	}

	//���� ������
	fseek(ReadTxt, 0, SEEK_END);
	int fileSize = ftell(ReadTxt);
	rewind(ReadTxt);
	
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

	printf("������ �ݽ��ϴ�. \n");
	free(elements.num);
	free(elements.ch);
	free(elements.str);
	fclose(ReadTxt);
	return 0;
} 