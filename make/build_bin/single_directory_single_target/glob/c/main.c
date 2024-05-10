#include <stdio.h>
void swap(int *a, int *b);

int main( void )
{
	int i = 3;
	int j = 5;

	printf("i: %d, j: %d\n", i, j);
	swap(&i, &j);
	printf("i: %d, j: %d\n", i, j);

	return 0;
}
