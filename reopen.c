#include <comp421/yalnix.h>
#include <comp421/iolib.h>
#include <comp421/filesystem.h>

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;

	int size;

	int fd1 = Create("/foo");
	assert(fd1 >= 0);

	char stuff[] = "hello";
	int n = strlen(stuff);

	size = Write(fd1, stuff, n);

	int fd2 = Open("/foo");
	assert(fd2 >= 0);

	char contents[n];
	size = Read(fd2, &contents, n);
	assert(size == n);

	assert(memcmp(stuff, contents, n) == 0);

	int fd3 = Open("/foo");
	assert(fd3 >= 0);

	memset(contents, 0, sizeof contents);
	size = Read(fd3, &contents, n);
	assert(size == n);

	assert(memcmp(stuff, contents, n) == 0);

	assert(Close(fd1) == 0);
	assert(Close(fd2) == 0);
	assert(Close(fd3) == 0);

	assert(Shutdown() == 0);

	return 0;
}
