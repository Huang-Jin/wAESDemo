#ifndef _W_AES_H_
#define _W_AES_H_
#include <string>

class WAES
{
public:
	WAES();
	~WAES();
	
	void encrypt(char *data, int dataLen, char *key);
	void decrypt(char *coded, int cLen, char *key);

	void encryptFile(char *filename, char *key);
	void decryptFile(char *filename, char *key);

private:
	void getIntArray(char *data, int pArray[4][4]);
	void fromIntArray(int pArray[4][4], char *str);
	bool checkKeyLen(int len);
	void extendKey(char *key);
	void leftLoopBytes(int array[4], int step);
	void addRoundKey(int pArray[4][4], int round);
	void subBytes(int pArray[4][4]);
	void shiftRows(int pArray[4][4]);
	void mixColumns(int pArray[4][4]);

	int T(int number, int round);
	int getNumFromSBox(int index);
	int getLeftBits(int data);
	int getRightBits(int data);
	int getIntFromChar(char c);
	int getWordFromStr(char *str);
	int mergeArrayToInt(int array[4]);
	void splitIntToArray(int num, int array[4]);
	void printArray(int pArray[4][4]);
	void printASCII(char *str, int len);

	int GFMul2(int s);
	int GFMul3(int s);
	int GFMul4(int s);
	int GFMul8(int s);
	int GFMul9(int s);
	int GFMul11(int s);
	int GFMul12(int s);
	int GFMul13(int s);
	int GFMul14(int s);
	int GFMul(int n, int s);

	int getNumFromdeSBox(int index);
	void deSubBytes(int pArray[4][4]);
	void rightLoopBytes(int array[4], int step);
	void deShiftRows(int pArray[4][4]);
	void deMixColumns(int pArray[4][4]);
	// xor two array
	void addRoundToWArray(int aArray[4][4], int bArray[4][4]);
	void getArrayFromKey(int i, int pArray[4][4]);

	void splitFileName(char *filename, std::string & fileBody, std::string & fileExt);

private:
	int m_extendedKeys[44];
	int m_mode = 128;
};

#endif // !_W_AES_H_

