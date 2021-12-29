#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int n = atoi(argv[1]);
	FILE *files[n];
	for(int i = 1; i <= n; i++)
	{
		char name[12];
		sprintf(name, "Result_%d.txt", i);
		files[i] = fopen(name, "w");
		fprintf(*(files + i), "Hello, World %d\n", i);
		fclose(*(files + i));
	}
	return 0;
}