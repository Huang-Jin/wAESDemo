#include "wAES.h"
#include <string>

WAES::WAES()
{
}

WAES::~WAES()
{
}

char * WAES::encrypt(char *src, int srcLen, char *key)
{
	int keylen = strlen(key);
	//if (srcLen == 0 || srcLen % 16 != 0) {
	//	printf("The length of should be some times of 16!\n");
	//	return NULL;
	//}

	if (!checkKeyLen(keylen)) {
		printf("The length of key is not fit. The length should be 16, 24 or 32.\nThe length of current key is %d\n", keylen);
		return NULL;
	}

	extendKey(key); // extend the key

	char *data;
	int encryptedDataLen;
	if (srcLen % 16 != 0)
	{
		encryptedDataLen = (srcLen / 16 + 1) * 16;
		data = new char[encryptedDataLen];
		for (int i = 0; i < srcLen; i++)
		{
			data[i] = src[i];
		}

		for (int i = 0; i < encryptedDataLen - srcLen; i++)
		{
			data[srcLen + i] = 0;
		}
	}
	else
	{
		encryptedDataLen = srcLen;
		data = new char[encryptedDataLen];
		for (int i = 0; i < encryptedDataLen; i++)
		{
			data[i] = src[i];
		}
	}
	char * encryptedData = new char[encryptedDataLen];
	int pArray[4][4];

	for (int k = 0; k < encryptedDataLen; k += 16) {
		getIntArray(data + k, pArray);
		addRoundKey(pArray, 0);

		// first 9 rounds
		for (int i = 1; i < 10; i++){
			subBytes(pArray);
			shiftRows(pArray);
			mixColumns(pArray);
			addRoundKey(pArray, i);
		}

		// the 10th round
		subBytes(pArray);
		shiftRows(pArray);
		addRoundKey(pArray, 10);

		fromIntArray(pArray, encryptedData + k);
	}

	return encryptedData;
}

bool WAES::checkKeyLen(int len)
{
	if (m_mode == 128 && len != 16)
	{
		return false;
	}

	return true;
}

void WAES::extendKey(char *key)
{

}

void WAES::getIntArray(char *data, int pArray[4][4])
{

}