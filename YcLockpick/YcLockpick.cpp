//--YcLockpick.cpp--
#include "pch.h"

#define DEBUG

#include "YcBitBase.cpp"

using namespace std;

//--程序入口点main()--
int main()
{
	YcBiter a("11 1010 1110 0100 0111");
	YcBiter b("1111 1111 1111 1111 1111");
	cout << "a" << endl;
	cout << a.Debug_Get_bData_Hex() << endl;
	cout << a.Debug_GetRoughDataString() << endl;
	a.Widen_EmptySide(7, true);

	YcBiterComputable ybc(a);
	YcBiterComputable ybc2(b);
	a.~YcBiter();
	b.~YcBiter();
	cout << "ybc" << endl;
	cout << ybc.Debug_Get_bData_Hex() << endl;
	cout << ybc.Debug_GetRoughDataString() << endl;

	ybc = ybc ^ ybc2;
	cout << "ybc2" << endl;
	cout << ybc2.Debug_Get_bData_Hex() << endl;
	cout << ybc2.Debug_GetRoughDataString() << endl;
	cout << "ybc" << endl;
	cout << ybc.Debug_Get_bData_Hex() << endl;
	cout << ybc.Debug_GetRoughDataString() << endl;

	ybc = ybc ^ ybc2;
	cout << "ybc2" << endl;
	cout << ybc2.Debug_Get_bData_Hex() << endl;
	cout << ybc2.Debug_GetRoughDataString() << endl;
	cout << "ybc" << endl;
	cout << ybc.Debug_Get_bData_Hex() << endl;
	cout << ybc.Debug_GetRoughDataString() << endl;
}

