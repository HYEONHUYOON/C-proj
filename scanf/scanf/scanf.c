#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define _CRT_SECURE_NO_WARNINGS

void Scanf(const char* str, void* a, void* b, void* c, void* d)
{
	//�Է� ���ڿ�
	char strinput[1024];
	gets(strinput);

	//��ūȭ
	char* tok = strtok(strinput, " ");

	//���ڿ� ��ȸ ī��Ʈ
	int chekcnt = 0;

	//�Ű����� void ������
	void* param[4] = { a,b,c,d };

	//�Ű����� ī��Ʈ
	int paramcnt = 0;

	while (1)
	{
		//%�϶� ���� ���� Ȯ��
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

