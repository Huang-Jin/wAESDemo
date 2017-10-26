#ifndef _W_AES_H_
#define _W_AES_H_

class WAES
{
public:
	WAES();
	~WAES();
	
	char * encrypt(char *data, int dataLen, char *key);
	char * decrypt(char *key);


private:
	void getIntArray(char *data, int pArray[4][4]);
	void fromIntArray(int pArray[4][4], char *data);
	bool checkKeyLen(int len);
	void extendKey(char *key);
	void addRoundKey(int pArray[4][4], int roundIndex);
	void subBytes(int pArray[4][4]);
	void shiftRows(int pArray[4][4]);
	void mixColumns(int pArray[4][4]);

private:
	int m_mode = 128;
};





#endif // !_W_AES_H_

