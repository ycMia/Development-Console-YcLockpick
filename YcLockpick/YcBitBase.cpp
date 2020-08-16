#pragma once

#include "pch.h"
#include "YcBitBase.h"

inline YcBiter::YcBiter(int length)
{
	_length = length;
	_bData = new bool[_length];
	memset(_bData, 0, _length * sizeof(bool));
}

inline YcBiter::YcBiter(int length, bool * inData) 
{
	_length = length;
	//Way of Clone
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
	deleteBuffer_bData();
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

inline void YcBiter::CoverWrite(bool * data, int inLength)
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
	deleteBuffer_bData();
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
			if (i == tlen) {}
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

inline bool YcBiter::dump8BitArray(int nBytes, unsigned char address[])
{
	if (nBytes * 8 < _length)	return  false;
	int tlen;
	int current = 0;
	for (tlen = _length; tlen % 8 != 0; tlen++) {} //补充到8位
	int l_distance = tlen - _length;

	for (int i = 0; i <= tlen; i++)//见 if (i == tlen) 的限制
	{
		if (i < l_distance)		continue;
		if (i % 8 == 0 && i != 0)	current++;
		if (i == tlen)	break;
		//i需要等于tlen(也就是长度为tlen+1)用于结算最后一位
		//而为了防止爆数组这里需要做break
		if (_bData[i - l_distance])
			address[current] += (int)pow(2, 7 - (i % 8));
	}
	return true;
}

inline bool * YcBiter::GetBoolArray(int nbitslength)
{
	if (nbitslength < _length)	return nullptr;
	else
	{
		bool * address = new bool[nbitslength];
		//issue 2020-07-12
		nbitslength--;
		while (nbitslength > 0)
		{
			address[nbitslength] = _bData[nbitslength];
			nbitslength--;
		}
		return address;
	}
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

inline bool YcBiterComputable::XOR_sameWidth(bool * a, bool * b)
{
	if (_bData == nullptr)	return false;
	for (int i = 0; i < _length; i++)//默认_length与a和b指向的相应bool型数组的长度相等
	{
		_bData[i] = a[i] ^ b[i];
	}
	delete[] a;
	delete[] b;
	return true;
}

inline YcBiterComputable YcBiterComputable::operator xor(YcBiter & target)
{
	if (_length < target.Length())
	{
		Widen_EmptySide(target.Length() - _length, true);
	}
	YcBiterComputable ybc;
	if (ybc.XOR_sameWidth(_bData, target.GetBoolArray(target.Length())))
		return ybc;
	else
		return YcBiterComputable();
}

inline bool * YcBiter :: deleteBuffer_bData(void)
{
	if (_bData != nullptr)
	{
		delete[] _bData;
		_bData = NULL;
	}//多次执行delete(或delete[])将会导致问题出现
	else
	{
		_bData = NULL;
	}
	_length = 0;
	return _bData;
}
//返回新的_bData所指向的地址(正常应该是NULL)

inline bool * YcBiter :: strToBool_RS(string str2)
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
