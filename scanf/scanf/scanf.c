#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define _CRT_SECURE_NO_WARNINGS

void Scanf(const char* str, void* a, void* b, void* c, void* d)
{
	//입력 문자열
	char strinput[1024];
	gets(strinput);

	//토큰화
	char* tok = strtok(strinput, " ");

	//문자열 순회 카운트
	int chekcnt = 0;

	//매개변수 void 포인터
	void* param[4] = { a,b,c,d };

	//매개변수 카운트
	int paramcnt = 0;

	while (1)
	{
		//%일때 다음 문자 확인
		if (str[chekcnt] == '%')
		{
			switch (str[chekcnt + 1])
			{
			case 'd':
				*((int*)param[paramcnt]) = atoi(tok);
				tok = strtok(NULL, " ");
				paramcnt++;
				break;

			case 'c':
				*(char*)param[paramcnt] = *tok;
				tok = strtok(NULL, " ");
				paramcnt++;
				break;

			case 'f':
				*((float*)param[paramcnt]) = atof(tok);
				tok = strtok(NULL, " ");
				paramcnt++;
				break;

			case 's':
				strcpy((char*)param[paramcnt], tok);
				tok = strtok(NULL, " ");
				paramcnt++;
				break;
			default:
				break;
			}
		}
		chekcnt++;
		if (str[chekcnt] == '\0')
		{
			break;
		}
	}
}

int main()
{
	int a;
	char b;
	float c;
	char d[1024];

	Scanf("%d %c %f %s", &a, &b, &c, d);
	printf("Values entered: %d %c %f %s\n", a, b, c, d);
	return 0;
}

