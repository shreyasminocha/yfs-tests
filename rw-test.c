#include <comp421/yalnix.h>
#include <comp421/iolib.h>
#include <comp421/filesystem.h>

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define SIZE (512 * 64)

char expected_buf[SIZE] = { 0 };

static int backed_write(int fd, char *buf, int size) {
	int orig_pos = Seek(fd, 0, SEEK_CUR);

	char *end = buf + size;
	if (end > buf + SIZE) {
		end = buf + SIZE;
	}
	if (size > 0) {
		memcpy(expected_buf + orig_pos, buf, end - buf);
	}

	int result = Write(fd, buf, size);

	Sync();

	int pos = Seek(fd, 0, SEEK_CUR);

	assert(pos >= 0);

	assert(Seek(fd, 0, SEEK_SET) == 0);

	static char temp_buf[SIZE] = { 0 };
	int read_res = Read(fd, temp_buf, SIZE);
	assert(read_res >= 0);
	assert(read_res >= size);
	assert(memcmp(expected_buf, temp_buf, read_res) == 0);

	assert(Seek(fd, pos, SEEK_SET) == pos);

	return result;
}

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;

	int fd;

	fd = Create("/foo");
	assert(fd >= 0);

	printf("hmm\n");

	static char buf[SIZE];

	int i;
	for (i = 0; i < SIZE; i += 4) {
		*(unsigned short*)&buf[i] = i / 512;
		*(unsigned short*)&buf[i + 2] = i % 512;
	}

	printf("1\n");
	assert(backed_write(fd, buf + 100, 1024 - 8) == 1024 - 8);
	printf("2\n");
	assert(backed_write(fd, buf + 200, 16) == 16);
	assert(Seek(fd, 5, SEEK_SET) == 5);
	printf("3\n");
	assert(backed_write(fd, buf + 30, 16) == 16);
	printf("4\n");
	assert(Seek(fd, 0, SEEK_SET) == 0);
	assert(backed_write(fd, buf, SIZE) == SIZE);


	assert(Close(fd) == 0);
	assert(Shutdown() == 0);
	return 0;
}
