#pragma once

#include<iostream>
#include<string>

using namespace std;

//Tip
//VS提示 "找到一个或多个多重定义的符号" 解决办法:
//此时将非calss内函数定义为static即可(保证它们作为工具仅仅执行一次)
//例如下列函数

//static void outMsg(int mode, _In_z_ _Printf_format_string_ char const* const _Format, ... )
//{
//	switch (mode)
//	{
//		case 0:
//			cout << "-Message: ";
//			break;
//		case 1:
//			cout << "-Warn: ";
//			break;
//		case 2:
//			cout << "-Error: ";
//			break;
//		default:
//			return;
//			break;
//	}
//	
//	va_list _ArgList;
//	__crt_va_start(_ArgList, _Format);
//	_vfprintf_l(stdout, _Format, NULL, _ArgList);
//	__crt_va_end(_ArgList);
//
//	cout << endl;
//}
//可变参数,详见printf函数源码-此函数在结束后自动添加回车
//这里有bug,某些参数传入后显示会出问题,未排查
//于2020年7月8日弃用

class YcBiter
{
public:
	~YcBiter();
	YcBiter(int id = _Const_DefaultClassID_YcBiter) : _classID(id) {}
	YcBiter(int length, int id = _Const_DefaultClassID_YcBiter);
	YcBiter(int length, bool * inData, int id = _Const_DefaultClassID_YcBiter);//用以克隆

	bool Widen_EmptySide(int n, bool true2left_OR_false2right);//mode == true时拓宽左边(值不变),mode == false时拓宽右边(低位向高位移动),成功时返回tre
	bool Flush_bData(int n);	//成功时返回true
	void CoverWrite(bool data[], int inLength);
	void CoverWrite_RoughString(string str);
	//	void Debug_OutMsgShow_Bit(void) 于2020年7月8日弃用
	string Debug_Get_bData_Hex(bool format = true);
	int Debug_GetLen() { return _length; }
	int Length() { return _length; }
	static const int _Const_DefaultClassID_YcBiter = -1;
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
	}
	//辅助函数,将形如"1000100100"这类的string转为bool的数组
	//-->CWS<-- 指CoverWrite_S,此函数在非特殊情况下应该只与
	//YcBiter_CoverWrite_RoughString或其他类似函数配合使用,所以我把它放在private中
	string GetRoughDataString()
	{
		string dstr;
		for (int i = 0; i < _length; i++)
		{
			if (_bData[i])
				dstr += "1";
			else
				dstr += "0";
		}
		return dstr;
	}
	//返回形如"10001010"的字符串,而不是raw
};
