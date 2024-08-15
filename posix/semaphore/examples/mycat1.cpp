#include <stdio.h>
#include <stdlib.h>
#include "posix_api.hpp"

#define	BUFFSIZE		8192

int
main(int argc, char **argv)
{
	int		fd, n;
	char	buff[BUFFSIZE];

	if (argc != 2) {
        printf("usage: mycat1 <pathname>\n");
        exit(1);
    }

	fd = Open(argv[1], O_RDONLY);

	while ( (n = Read(fd, buff, BUFFSIZE)) > 0)
		Write(STDOUT_FILENO, buff, n);

    Close(fd);

	exit(0);
}
