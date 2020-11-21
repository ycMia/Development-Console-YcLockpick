//--YcBitBase.h--
#pragma once

#include<iostream>
#include<string>

using namespace std;

namespace yctd
{
	class CDataArea//连续的数据存放点
	{
	public:
		CDataArea(void)
		{
			vSetArea(0);
		}

		void vSetArea(int nLength)
		{
			m_nLength = nLength;
			m_pchData = new char[m_nLength];
		}

		int nGetLength(void)
		{
			return m_nLength;
		}
	protected:
		int m_nLength;
		char * m_pchData;
	};

	class CQueue : public CDataArea
	{
	public:
		CQueue(void)
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
