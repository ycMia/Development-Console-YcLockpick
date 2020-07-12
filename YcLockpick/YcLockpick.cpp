//--YcLockpick.cpp--
#include "pch.h"

#define DEBUG

#include "YcBitBase.cpp"

using namespace std;

//--程序入口点main()--
int main()
{
	YcBiter a;
	YcBiter b;
	YcBiterComputable c(4);
	a.CoverWrite_RoughString("1100");
	b.CoverWrite_RoughString("1010");
	//c.XOR_sameWidth(a, b);
	cout << c.Debug_GetRoughDataString() << endl;
	c.XOR_sameWidth2(a.GetBoolArray(a.Length()), b.GetBoolArray(b.Length()));
	cout << c.Debug_GetRoughDataString() << endl;
}

