
#include "wAES.h"

int main(int argc, char** argv)
{
	WAES aesDemo;

	char data[] = "abcdefghijklmnop";
	char key[] = "ponmlkjihgfedcba";

	char *result = aesDemo.encrypt(data, 16, key);

	return 0;
}