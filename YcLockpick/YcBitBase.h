#pragma once

#include<iostream>
#include<string>

using namespace std;

class YcBiter
{
public:

	~YcBiter()
	{
		cout << "Exiting!!" << endl;
		deleteBuffer_bData();
	}

	//----ConstValueDefinitionArea----
	static const int _Const_DefaultClassID_YcBiter = -1;

	//----Construction----
	
	YcBiter(void) : _classID(_Const_DefaultClassID_YcBiter) 
	{
		_length = 1;
		_bData = new bool[1];
	}
	YcBiter(int length, int id = _Const_DefaultClassID_YcBiter);
	YcBiter(int length, bool * inData, int id = _Const_DefaultClassID_YcBiter);//用以克隆


	//----UseageArea
	bool Widen_EmptySide(int n, bool true2left_OR_false2right);//mode == true时拓宽左边(值不变),mode == false时拓宽右边(低位向高位移动),成功时返回tre
	bool Flush_bData(int n);	//成功时返回true
	void CoverWrite(bool * data, int inLength);
	void CoverWrite_RoughString(string str);
//	void Debug_OutMsgShow_Bit(void) 于2020年7月8日弃用
	int Length() { return _length; }
	
	//----DEBUG&ShowArea
	bool dump8BitArray(int nBytes,unsigned char address[]);//成功时返回true, address的delete需要手动操作
	bool * GetBoolArray(int nbitslength);
	int Debug_GetLen() { return _length; }
	string Debug_GetRoughDataString();//返回形如"10001010"的字符串,而不是raw
	string Debug_Get_bData_Hex(bool format = true);
	
private:
	
	bool * deleteBuffer_bData(void)
	{
		delete[] _bData;	 _bData = NULL;
		_length = 0;
		return _bData;
	}
	//返回新的_bData所指向的地址(正常应该是NULL)

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
protected:

	bool * _bData = NULL;
	int _length = 0;
	const int _classID;

private:
	//friend class YcBiterComputable;
};

class YcBiterComputable : public YcBiter
{
public:
	~YcBiterComputable()
	{
		
	}
	//----Construction----
	YcBiterComputable(void): YcBiter()
	{
		
	}
	YcBiterComputable(int length, int id = _Const_DefaultClassID_YcBiter) : YcBiter(length,id){}

	//bool XOR_sameWidth(YcBiter & a, YcBiter & b);
	bool XOR_sameWidth(bool * a, bool * b);
private:
	
};