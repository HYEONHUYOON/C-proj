#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define _CRT_SECURE_NO_WARNINGS

void Scanf(char* ch, void* a,void* b,void *c, void* d)
{
	void* binary[4];
	binary[0] = a;
	binary[1] = b;
	binary[2] = c;
	binary[3] = d;

	int cnt = 0;

	char str[1024];
	gets(str);

	char* pch = strtok(str, " ");

	for (int i = 0; i < sizeof(ch) / sizeof(ch[0]); i++)
	{
		if (ch[i] == '%')
		{
			switch (ch[i + 1])
			{
			case 'd':
				printf("%s /", pch);
				binary[cnt] = atoi(pch);
				pch = strtok(NULL, " ");
				cnt++;
				break;
			case 's':
				printf("%s /", pch);
				binary[cnt] = pch;
				cnt++;
				pch = strtok(NULL, " ");
				break;
			case 'f':
				printf("%s /", pch);
				//binary[cnt] = atof(pch);
				cnt++;
				pch = strtok(NULL, " ");
				break;
			case 'c':
				printf("%s /", pch);
				binary[cnt] = pch;
				pch = strtok(NULL, " ");
				break;
			}
		}
		
	}
}

int main()
{
	int a;
	int b;
	int c;
	int d;

	char str[100];
	float f;
	//char c;

	Scanf("%d %d %d %d",&a,&b,&c,&d);
	//scanf("%d %f",&a,&f);
	printf("%d %d %d %d", a, b, c, d);
	return 0;
}

