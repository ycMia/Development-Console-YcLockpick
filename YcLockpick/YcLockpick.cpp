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
	YcBiterComputable *c = new YcBiterComputable();
	while (true)
	{
		a = new YcBiter();
		b = new YcBiter();
		a->CoverWrite_RoughString("11 0110 0101 0011 1100 0000 0001 1101 0101 0101");
		b->CoverWrite_RoughString("11 0110 0000 0011 1100 0000 0001 1101 0101 0101");
		c = new YcBiterComputable(a->Length());

		if (c->XOR_sameWidth(a->GetBoolArray(a->Length()), b->GetBoolArray(b->Length())))
			cout << c->Debug_GetRoughDataString() << endl;
		a->~YcBiter(); a = NULL;
		b->~YcBiter(); b = NULL;
		c->~YcBiterComputable(); c = NULL;
		//system("pause");
	}
}

