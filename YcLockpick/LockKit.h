#pragma once
#include<cstring>
#include<iostream>
class LockKit
{j
public:
	LockKit(int idenfinder)
		:_id(id)
	{

	}
	void GetStr(string str)
	{
		_str = str;
	}
	~LockKit()
	{
		std::cout << "********" << std::endl << "destoried LockKit id " << _id << std::endl<< "********" << std::endl;
	}
private:
	const int _id;
	string _str;
};
