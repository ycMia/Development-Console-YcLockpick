#pragma once

#include "pch.h"
#include "YcBitBase.h"

inline YcBiter::~YcBiter()
{
	delete[] _bData; _bData = NULL;//为了不内存泄露还是写一下吧
#ifdef DEBUG
	outMsg(0, "Yciter of ID:%d killed", _classID);
#endif // DEBUG
}

inline YcBiter::YcBiter(int length,int id) :  _length(length), _classID(id)
{
		delete[] _bData;
		_bData = new bool[_length];
		memset(_bData, 0, _length * sizeof(bool));
#ifdef DEBUG
		outMsg(0, "YcBiter %d created (Way : Normal), length %d ,_bData's Address: 0x%x",_classID,Debug_GetLen(),_bData);
#endif // DEBUG
}

inline YcBiter::YcBiter(int length,bool * inData , int id) : _length(length), _classID(id)
{
	//Way of Clone
	delete[] _bData; _bData = NULL;
	_bData = new bool[_length];
	memset(_bData, 0, _length * sizeof(bool));//使用之前还是清理一下吧
	for (int i = 0; i < _length; i++)
	{
		_bData[i] = inData[i];
	}
#ifdef DEBUG
	outMsg(0, "Biter %d created (Way: Clone), length: %d, _bData's Address: 0x%x", _classID, _length, _bData);
	Debug_OutMsgShow();
#endif // DEBUG
}

inline bool YcBiter::Widen_EmptySide(int n,bool mode)
{
	if (n <= 0)	return false;

	bool* ptemp = new bool[_length + n];
	for (int i = 0; i < n; i++)
		ptemp[i] = false;
	if(mode == true)
		for (int i = 0; i < _length; i++)
			ptemp[i+n] = _bData[i];
	else
		for (int i = 0; i < _length; i++)
			ptemp[i] = _bData[i];
	delete [] _bData;
	_bData = ptemp;
	ptemp = NULL;
	_length+=n;
	_current = 0;
#ifdef DEBUG
	outMsg(0, "_bData WidenLed %d. New _bData address at 0x%x", n, _bData);
#endif //DEBUG
	return true;
}

inline bool YcBiter::Flush_bData(int n)
{
	if (n <= 0)
	{
#ifdef DEBUG
		outMsg(2, "input wrong: \"size n<=0\" while running YcBiter::Flush(int n)");
#endif //DEBUG
		return false;
	}
	else
	{
		_current = 0;
		_length = n;//给予_lenghth , _current新值
		delete[] _bData;
		_bData = new bool[n];
		for (int i = 0; i <	_length; i++)
			_bData[i] = false;
#ifdef DEBUG
		outMsg(0, "YcBiter with class ID \"%d\"'s_bData flushed. New _bData created, address at 0x%x, length of %d",_classID, _bData, _length);
#endif //DEBUG
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
