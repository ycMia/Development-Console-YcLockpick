//--YcLockpick.cpp--
#include "pch.h"

#define DEBUG

#include "YcBitBase.cpp"

using namespace std;

//--程序入口点main()--
int main()
{
	YcBiter *a = new YcBiter();
	YcBiter *b = new YcBiter();
	YcBiterComputable *c = new YcBiterComputable(4);
	while (true)
	{
		a->CoverWrite_RoughString("1100");
		b->CoverWrite_RoughString("1010");
		//c.XOR_sameWidth(a, b);
		cout << c->Debug_GetRoughDataString() << endl;
		c->XOR_sameWidth(a->GetBoolArray(a->Length()), b->GetBoolArray(b->Length()));
		cout << c->Debug_GetRoughDataString() << endl;
		a->~YcBiter();
		b->~YcBiter();
		c->~YcBiterComputable();
		system("pause");
	}
}

