//--YcBitBase.h--
#pragma once

#include<iostream>
#include<string>

using namespace std;

namespace yctd
{
	class DataArea//连续的数据存放点
	{
	public:
		DataArea(void)
		{
			SetArea(0);
		}

		void SetArea(int nLength)
		{
			_nLength = nLength;
			_pchData = new char[_nLength];
		}

		int GetLength(void)
		{
			return _nLength;
		}
	protected:
		int _nLength;
		char * _pchData;
	};

	class Queue : public DataArea
	{
	public:
		Queue(void)
		{
			
		}
	private:
		int m_nCurrent;
	};



	class IYcBase
	{
	public:
		virtual int ngetValue() = 0;
	private:
		
	};

}
