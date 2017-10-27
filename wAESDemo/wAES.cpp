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

char * WAES::decrypt(char *coded, int cLen, char *key)
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

	int cArray[4][4];
	for (int k = 0; k < cLen; k += 16) {
		getIntArray(coded + k, cArray);

		addRoundKey(cArray, 10);

		int wArray[4][4];
		for (int i = 9; i >= 1; i--) {
			deSubBytes(cArray);

			deShiftRows(cArray);

			deMixColumns(cArray);
			getArrayFrom4W(i, wArray);
			deMixColumns(wArray);

			addRoundTowArray(cArray, wArray);
		}

		deSubBytes(cArray);

		deShiftRows(cArray);

		addRoundKey(cArray, 0);

		convertArrayToStr(cArray, c + k);

	}
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
	for (int i = 0; i < 4; i++)
		m_extendedKeys[i] = getWordFromStr(key + i * 4);

	for (int i = 4, j = 0; i < 44; i++) {
		if (i % 4 == 0) {
			m_extendedKeys[i] = m_extendedKeys[i - 4] ^ T(m_extendedKeys[i - 1], j);
			j++;
		}
		else {
			m_extendedKeys[i] = m_extendedKeys[i - 4] ^ m_extendedKeys[i - 1];
		}
	}
}

int WAES::T(int number, int round)
{
	int numArray[4];
	splitIntToArray(num, numArray);
	leftLoopBytes(numArray, 1);

	// substitution of bytes
	for (int i = 0; i < 4; i++)
		numArray[i] = getNumFromSBox(numArray[i]);

	int result = mergeArrayToInt(numArray);
	return result ^ m_Rcon[round];
}

int WAES::getNumFromSBox(int index)
{
	int row = getLeftBits(index);
	int col = getRightBits(index);
	return m_S[row][col];
}

void WAES::leftLoopBytes(int array[4], int step)
{
	int temp[4];
	for (int i = 0; i < 4; i++)
		temp[i] = array[i];

	int index = step % 4 == 0 ? 0 : step % 4;
	for (int i = 0; i < 4; i++){
		array[i] = temp[index];
		index++;
		index = index % 4;
	}
}

void WAES::addRoundKey(int pArray[4][4], int round)
{
	int warray[4];
	for (int i = 0; i < 4; i++) {

		splitIntToArray(m_extendedKeys[round * 4 + i], warray);

		for (int j = 0; j < 4; j++) {
			pArray[j][i] = pArray[j][i] ^ warray[j];
		}
	}
}

void WAES::subBytes(int pArray[4][4])
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			pArray[i][j] = getNumFromSBox(pArray[i][j]);
}

void WAES::shiftRows(int pArray[4][4])
{
	int rowTwo[4], rowThree[4], rowFour[4];

	// copy the 2nd, 3rd, 4th line of state matrix.
	for (int i = 0; i < 4; i++) {
		rowTwo[i] = pArray[1][i];
		rowThree[i] = pArray[2][i];
		rowFour[i] = pArray[3][i];
	}

	// left loop
	leftLoopBytes(rowTwo, 1);
	leftLoopBytes(rowThree, 2);
	leftLoopBytes(rowFour, 3);

	// copy back into the state matrix
	for (int i = 0; i < 4; i++) {
		pArray[1][i] = rowTwo[i];
		pArray[2][i] = rowThree[i];
		pArray[3][i] = rowFour[i];
	}
}

void WAES::mixColumns(int pArray[4][4])
{
	int tempArray[4][4];

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			tempArray[i][j] = pArray[i][j];

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++){
		pArray[i][j] = GFMul(m_colM[i][0], tempArray[0][j]) ^ GFMul(m_colM[i][1], tempArray[1][j])
			^ GFMul(m_colM[i][2], tempArray[2][j]) ^ GFMul(m_colM[i][3], tempArray[3][j]);
		}
}

int WAES::getLeftBits(int data)
{
	int left = data & 0x000000f0;
	return left >> 4;
}

int WAES::getRightBits(int data)
{
	return data & 0x0000000f;
}

int WAES::getIntFromChar(char c)
{
	int result = (int)c;
	return result & 0x000000ff;
}

int WAES::getWordFromStr(char *str)
{
	int one = getIntFromChar(str[0]);
	one = one << 24;
	int two = getIntFromChar(str[1]);
	two = two << 16;
	int three = getIntFromChar(str[2]);
	three = three << 8;
	int four = getIntFromChar(str[3]);
	return one | two | three | four;
}

int WAES::mergeArrayToInt(int array[4])
{
	int one = array[0] << 24;
	int two = array[1] << 16;
	int three = array[2] << 8;
	int four = array[3];
	return one | two | three | four;
}

void WAES::convertArrayToStr(int pArray[4][4], char *str)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			*str++ = (char)pArray[j][i];
}

void WAES::splitIntToArray(int num, int array[4])
{
	int one = num >> 24;
	array[0] = one & 0x000000ff;
	int two = num >> 16;
	array[1] = two & 0x000000ff;
	int three = num >> 8;
	array[2] = three & 0x000000ff;
	array[3] = num & 0x000000ff;
}

void WAES::printArray(int pArray[4][4])
{
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++)
			printf("a[%d][%d] = 0x%x ", i, j, pArray[i][j]);
		printf("\n");
	}
	printf("\n");
}

void WAES::printASCII(char *str, int len)
{
	for (int i = 0; i < len; i++)
		printf("0x%x ", getIntFromChar(str[i]));
	printf("\n");
}

int WAES::GFMul2(int s)
{
	int result = s << 1;
	int a7 = result & 0x00000100;

	if (a7 != 0) {
		result = result & 0x000000ff;
		result = result ^ 0x1b;
	}

	return result;
}

int WAES::GFMul3(int s)
{
	return GFMul2(s) ^ s;
}

int WAES::GFMul4(int s)
{
	return GFMul2(GFMul2(s));
}

int WAES::GFMul8(int s)
{
	return GFMul2(GFMul4(s));
}

int WAES::GFMul9(int s)
{
	return GFMul8(s) ^ s;
}

int WAES::GFMul11(int s)
{
	return GFMul9(s) ^ GFMul2(s);
}

int WAES::GFMul12(int s)
{
	return GFMul8(s) ^ GFMul4(s);
}

int WAES::GFMul13(int s)
{
	return GFMul12(s) ^ s;
}

int WAES::GFMul14(int s)
{
	return GFMul12(s) ^ GFMul2(s);
}

int WAES::GFMul(int n, int s)
{
	int result;

    if(n == 1)
        result = s;
    else if(n == 2)
        result = GFMul2(s);
    else if(n == 3)
        result = GFMul3(s);
    else if(n == 0x9)
        result = GFMul9(s);
    else if(n == 0xb)//11
        result = GFMul11(s);
    else if(n == 0xd)//13
        result = GFMul13(s);
    else if(n == 0xe)//14
        result = GFMul14(s);

    return result;
}

void WAES::getIntArray(char *data, int pArray[4][4])
{
	int k = 0;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++){
		pArray[j][i] = getIntFromChar(data[k]);
		k++;
		}
}