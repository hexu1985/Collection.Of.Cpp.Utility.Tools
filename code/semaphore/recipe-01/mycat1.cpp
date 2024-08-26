#include <stdio.h>
#include <stdlib.h>

#define	BUFFSIZE		8192

int
main(int argc, char **argv)
{
	int		n;
	char	buff[BUFFSIZE];

	if (argc != 2) {
        printf("usage: mycat1 <pathname>\n");
        exit(1);
    }

	FILE* fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("open error for %s\n", argv[1]);
        exit(2);
    }

    setbuf(fp, NULL);

	while ( (n = fread(buff, 1, BUFFSIZE, fp)) > 0)
		fwrite(buff, 1, n, stdout);

    fclose(fp);

	exit(0);
}
