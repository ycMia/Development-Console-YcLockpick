//--YcLockpick.cpp--
#include "pch.h"

#define DEBUG

#include "YcBitBase.cpp"

using namespace std;

//--程序入口点main()--
int main()
{
	YcBiter a("10101110101000111");
	cout << a.Debug_Get_bData_Hex() << endl;
	cout << a.Debug_GetRoughDataString() << endl;
}

