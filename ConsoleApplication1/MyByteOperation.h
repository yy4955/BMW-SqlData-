#pragma once
#include "pch.h"
//ascii תhex
std::string binToHex(const unsigned char *data, size_t size)
{
	std::ostringstream strHex;
	strHex << std::hex << std::setfill('0');
	for (size_t i = 0; i < size; ++i) {
		strHex << std::setw(2) << static_cast<unsigned int>(data[i]);
	}
	return strHex.str();
}
/*
	//vector תstring
	std::string stBuf;
	stBuf.assign(strHex.begin(), strHex.end());
*/
// hex ת ascii
std::vector<unsigned char> hexToBin(const std::string &hex)
{
	std::vector<unsigned char> dest;
	auto len = hex.size();
	dest.reserve(len / 2);
	for (decltype(len) i = 0; i < len; i += 2)
	{
		unsigned int element;
		std::istringstream strHex(hex.substr(i, 2));
		strHex >> std::hex >> element;
		dest.push_back(static_cast<unsigned char>(element));
	}
	return dest;
}

//ע�⣺�ú���  �ַ����м��С�\0���ᱻ�ض�
//string ת16�����ַ�����  "12345678912345678911"==��uint8_t  uSeed[] = { 0x12,0x34,0x56,0x78,0x91,0x23,0x45,0x67,0x89,0x11 }
char* strTo16usz(std::string str, char *szptr)
{
	//std::stringstream ss;
	std::string strTemp;
	char ptr[260]= "";
	int j = 0;
	int k = 0;
	for (int i = 0; i <= (str.size()) / 2; i++)
	{
		//char szTemp[10] = "";
		std::string str1 = str.substr(j, 2);
		//sprintf(szTemp,"0x%s",str1.c_str());
		k = strtol(str1.c_str(), 0, 16);
		j += 2;
		ptr[i] = k;
	}
	memcpy(szptr,ptr,sizeof(ptr));//�ַ����м��С�\0���ᱻ�ض�
	return szptr;
}

//string ת16�����ַ�����  "12345678912345678911"==��uint8_t  uSeed[] = { 0x12,0x34,0x56,0x78,0x91,0x23,0x45,0x67,0x89,0x11 }
std::string strTo16str(std::string str)
{
	/*
	��ȡ�ַ������еġ�,��
	*/
	char pStr[260] = { 0 };
	char pDest[260] = { 0 };
	strcpy(pStr, str.c_str());
	std::stringstream ss;
	std::string szId;//����id
	int j = 0;
	int str1Len = 0;//��¼��һ����ȡ�� ���ĳ���
	int nSplitLen = 0;//��¼ÿ�νضϵĸ���

	for (int i = 0; i <= strlen(str.c_str()); i++)
	{
		pDest[j] = pStr[i];
		j++;

		if (j == 4)
		{
			int nTemp = strtol(pDest, 0, 16);//0x00
			//cout << " nTemp��" << nTemp << endl;
			j = 0;
			str1Len += strlen(pDest);
			ss << std::hex << nTemp;
		}
	}
	ss >> str;
	return str;
}
//0x01020304 ==> 0x04030201
uint32_t BigToLittle32(uint32_t i)
{
	i = ((((uint32_t)(i) & 0xff000000) >> 24) |
		(((uint32_t)(i) & 0x00ff0000) >> 8) |
		(((uint32_t)(i) & 0x0000ff00) << 8) |
		(((uint32_t)(i) & 0x000000ff) << 24));

	return i;
}