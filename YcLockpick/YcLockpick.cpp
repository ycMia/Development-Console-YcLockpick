//--YcLockpick.cpp--
#include "pch.h"

#define DEBUG

#include "YcBitBase.h"

using namespace std;

int main()
{
	yctd::Queue queueT;
	yctd::Queue queue2T;
	queue2T.SetArea(30);
	cout << queueT.GetLength() << endl;
	cout << queue2T.GetLength() << endl;

	cout << "end" <<endl;
}

