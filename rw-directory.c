#include <comp421/yalnix.h>
#include <comp421/iolib.h>
#include <comp421/filesystem.h>

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;

	int err, fd, size;

	err = MkDir("/foo");
	assert(err == 0);

	fd = Open("/foo");
	assert(fd >= 0);

	char buf[128];
	size = Read(fd, &buf, sizeof buf);
	assert(size == 64);

	err = Seek(fd, 0, SEEK_SET);
	assert(err == 0);

    memset(buf, 0, sizeof buf);

	err = Write(fd, &buf, sizeof buf);
	assert(err != 0); // writing to a directory is an error

    size = Read(fd, &buf, sizeof buf);
	assert(size == 64);

    assert(strcmp(buf + 2, ".") == 0);
    assert(strcmp(buf + 34, "..") == 0);

	assert(Close(fd) == 0);
	assert(Shutdown() == 0);
	return 0;
}
