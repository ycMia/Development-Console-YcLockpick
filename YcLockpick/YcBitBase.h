#pragma once

#include<iostream>
#include<string>

using namespace std;

class YcBiter
{
public:
	~YcBiter();
	YcBiter() : _classID(-1/*_Const_DefaultClassID_YcBiter*/) {}
	YcBiter(int length, int id = _Const_DefaultClassID_YcBiter);
	YcBiter(int length, bool * inData, int id = _Const_DefaultClassID_YcBiter);//用以克隆

	//----ConstValueDefinitionArea----
	static const int _Const_DefaultClassID_YcBiter = -1;

	//----UsefulArea
	bool Widen_EmptySide(int n, bool true2left_OR_false2right);//mode == true时拓宽左边(值不变),mode == false时拓宽右边(低位向高位移动),成功时返回tre
	bool Flush_bData(int n);	//成功时返回true
	void CoverWrite(bool data[], int inLength);
	void CoverWrite_RoughString(string str);
//	void Debug_OutMsgShow_Bit(void) 于2020年7月8日弃用
	int Length() { return _length; }
	
	//----DEBUG&ShowArea
	bool dumpTo8BitArray(int nBytes,unsigned char address[]);//成功时返回true
	int Debug_GetLen() { return _length; }
	string Debug_GetRoughDataString();//返回形如"10001010"的字符串,而不是raw
	string Debug_Get_bData_Hex(bool format = true);
	
private:
	const int _classID;
	int _length;
	bool * _bData = NULL;

	bool * strToBool_RS(string str2)
	{
		bool * pOut = new bool[str2.length()];
		for (int i = 0; i < (int)str2.length(); i++)
		{
			if (str2[i] == '0')
				pOut[i] = false;
			else
				pOut[i] = true;
		}
		return pOut;
	}//辅助函数,将形如"1000100100"这类的string转为bool的数组 \n CWS指CoverWrite_S,此函数在非特殊情况下应该只与YcBiter_CoverWrite_RoughString或其他类似函数配合使用,所以我把它放在private中
};

class YcComputableBiter : YcBiter
{
public:
	YcComputableBiter(int length, int id = _Const_DefaultClassID_YcBiter) : YcBiter(length,id)
	{

	}
private:
};