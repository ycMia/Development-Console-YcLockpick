#pragma once

#include "pch.h"
#include "YcBitBase.h"

inline YcBiter::~YcBiter()
{
	delete[] _bData; _bData = NULL;
}

inline YcBiter::YcBiter(int length, int id) : _length(length), _classID(id)
{
	delete[] _bData;
	_bData = new bool[_length];
	memset(_bData, 0, _length * sizeof(bool));
}

inline YcBiter::YcBiter(int length, bool * inData, int id) : _length(length), _classID(id)
{
	//Way of Clone
	delete[] _bData; _bData = NULL;
	_bData = new bool[_length];
	memset(_bData, 0, _length * sizeof(bool));
	for (int i = 0; i < _length; i++)
	{
		_bData[i] = inData[i];
	}
}

inline bool YcBiter::Widen_EmptySide(int n, bool true2left_OR_false2right)
{
	if (n <= 0)	return false;

	bool* ptemp = new bool[_length + n];
	for (int i = 0; i < n; i++)
		ptemp[i] = false;
	if (true2left_OR_false2right == true)
		for (int i = 0; i < _length; i++)
			ptemp[i + n] = _bData[i];
	else
		for (int i = 0; i < _length; i++)
			ptemp[i] = _bData[i];
	delete[] _bData;
	_bData = ptemp;
	ptemp = NULL;
	_length += n;

	return true;
}

inline bool YcBiter::Flush_bData(int n)
{
	if (n <= 0)
	{
		return false;
	}
	else
	{
		_length = n;//给予_lenghth新值
		delete[] _bData;
		_bData = new bool[n];
		for (int i = 0; i < _length; i++)
			_bData[i] = false;
		return true;
	}
}

inline void YcBiter::CoverWrite(bool data[], int inLength)
{
	int difference = 0;
	if (inLength <= _length)
		difference = _length - inLength;//difference被赋值为差距值
	else
		Flush_bData(inLength);
	for (int i = 0; i < _length; i++)//_length 已经被Flush()赋予新值
		_bData[i + difference] = data[i];//data较短时,不处理_bData的高位
	delete[] data;
}

inline void YcBiter::CoverWrite_RoughString(string str)
{
	string str2 = "";
	for (int i = 0; i < (int)str.length(); i++)
	{
		if (str[i] == ' ' || str[i] == '	')//eat spaces and tabs
			continue;
		str2 += str[i];
	}

	CoverWrite(strToBool_RS(str2), str2.length());
}

inline string YcBiter::Debug_Get_bData_Hex(bool format)
{
	string str;
	if (!format)
		str += "0x";
	int tlen;
	for (tlen = _length; tlen % 8 != 0; tlen++) {} //补充到8位
	int l_distance = tlen - _length;

	int rev = 0;
	for (int i = 0; i <= tlen; i++)//见 if (i == tlen) 的限制
	{
		if (i < l_distance)
		{
			continue;
		}
		if (i % 4 == 0 && i != 0)
		{
			if (rev < 10)
			{
				str += (char)('0' + rev);
			}
			else
			{
				str += (char)('A' + rev - 10);
			}
			if (i == tlen)	int b = 0;
			if (format && i % 8 == 0)
				str += ' ';
			rev = 0;
		}

		if (i == tlen)
		{
			break;
			//i需要等于tlen(也就是长度为tlen+1)用于结算最后一位
			//而为了防止爆数组这里需要做break
		}

		if (_bData[i - l_distance])
		{
			rev += (int)pow(2, 3 - (i % 4));
		}
	}
	return str;
}

inline string YcBiter::Debug_GetRoughDataString()
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
