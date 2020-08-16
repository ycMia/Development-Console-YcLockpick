#pragma once

#include<iostream>
#include<string>

using namespace std;

class YcBiter
{
public:
	~YcBiter()
	{
		deleteBuffer_bData();
	}

	//----Constructions----
	YcBiter(void)
	{
		_length = 1;
		_bData = new bool[1];
		*_bData = false;
	}
	//Default Length for 1
	YcBiter(int length);
	YcBiter(int length, bool * inData);//用以克隆
	YcBiter(string str) {
		CoverWrite_RoughString(str);
	}

	//----UseageArea
	bool Widen_EmptySide(int n, bool true2left_OR_false2right);//mode == true时拓宽左边(值不变),mode == false时拓宽右边(低位向高位移动),成功时返回tre
	bool Flush_bData(int n);	//成功时返回true
	void CoverWrite(bool * data, int inLength);
	void CoverWrite_RoughString(string str);
	int Length() { return _length; }
	
	//----DEBUG&ShowArea
	bool dump8BitArray(int nBytes,unsigned char address[]);//成功时返回true, address的delete需要手动操作
	bool * GetBoolArray(int nbitslength);
	int Debug_GetLen() { return _length; }
	string Debug_GetRoughDataString();//返回形如"10001010"的字符串,而不是raw
	string Debug_Get_bData_Hex(bool format = true);
	
protected:
	bool * deleteBuffer_bData(void);
	bool * strToBool_RS(string str2);
	
	bool * _bData = NULL;
	int _length = 0;

};

class YcBiterComputable : public YcBiter
{
public:
	~YcBiterComputable() {}

	//----Construction----
	YcBiterComputable(YcBiterComputable & yb) 
		: YcBiter(yb.Length(), yb.GetBoolArray(yb.Length())) 
	{}
	YcBiterComputable(YcBiter & yb) 
		: YcBiter(yb.Length(), yb.GetBoolArray(yb.Length()) ) 
	{}

	YcBiterComputable(void) : YcBiter() {}

	YcBiterComputable(int length) : YcBiter(length){}
	YcBiterComputable(int length,bool * inData) : YcBiter(length,inData) {}

	YcBiterComputable operator^ (const YcBiterComputable & target);
	void operator= (const YcBiterComputable & target);
	//运算符重载参考https://www.cnblogs.com/linuxAndMcu/p/10395383.html
	//重载中const的细则见https://blog.csdn.net/weixin_43734095/article/details/104465307

private:
	
};