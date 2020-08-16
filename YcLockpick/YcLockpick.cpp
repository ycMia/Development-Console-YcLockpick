//--YcLockpick.cpp--
#include "pch.h"

#define DEBUG

#include "YcBitBase.cpp"

using namespace std;

//--程序入口点main()--
int main()
{
	YcBiter a("11 1010 1110 0100 0111");
	cout << a.Debug_Get_bData_Hex() << endl;
	cout << a.Debug_GetRoughDataString() << endl;

	YcBiterComputable ybc(a);
	a.~YcBiter();

	cout << ybc.Debug_Get_bData_Hex() << endl;
	cout << ybc.Debug_GetRoughDataString() << endl;
}

