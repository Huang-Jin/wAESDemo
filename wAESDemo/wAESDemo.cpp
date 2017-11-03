#include "wAES.h"

int main(int argc, char** argv)
{
	WAES aesDemo;

	//char data[] = "abcdefghijklmnop";
	//char key[] = "ponmlkjihgfedcba";

	//aesDemo.encrypt(data, 16, key);
	//aesDemo.decrypt(data, 16, key);

	char filename[] = "code.txt";
	char codedfilename[] = "code.wAESe";
	char key[] = "ponmlkjihgfedcba";
	aesDemo.encryptFile(filename, key);
	aesDemo.decryptFile(codedfilename, key);

	return 0;
}