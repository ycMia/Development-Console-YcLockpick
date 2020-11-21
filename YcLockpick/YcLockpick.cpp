//--YcLockpick.cpp--
#include "pch.h"

#define DEBUG

#include "YcBitBase.h"

using namespace std;

int main()
{
	yctd::CQueue queueT;
	yctd::CQueue queue2T;
	queue2T.vSetArea(30);
	cout << queueT.nGetLength() << endl;
	cout << queue2T.nGetLength() << endl;
}

