//Langton's ant 
#include<conio.h>
#include<bits/stdc++.h>
#include<iostream>
#include<stdio.h>
#include<windows.h>

using namespace std;

class LangtonsAnt
{
public:
	
	LangtonsAnt(int n) : _squaresize(n),_forward(1),_ant(new int[2]),_mcount(0)
	{
		_square = new bool *[_squaresize];
		_presquare = new bool *[_squaresize];
		for(int i=0;i<_squaresize;i++)
		{
			_square[i] = new bool [_squaresize];
			_presquare[i] = new bool [_squaresize];
		}		
		InitSquare(false);
	}
	

	void InitSquare(bool b)
	{	
		for(int i=0;i<_squaresize;i++)
			for(int j=0;j<_squaresize;j++)
				_presquare[i][j]=_square[i][j]=b;
	}

	~LangtonsAnt()
	{
		for(int i=0;i<_squaresize;i++)
		{
			delete[] _square[i];
			delete[] _presquare[i];
		}
		delete[] _square;
		delete[] _presquare;
	}

	void Run(bool clog)
	{
		while(Next(clog)){}
	}
	
	void Run_back(bool clog)
	{
		// Setant(_squaresize/2,_squaresize/2);
		while(Previous(clog)){}
		RetreatBit();
	}

	void RetreatBit()//just for run_back's tail treatment;
	{
		switch(_forward)
		{
			case 1:
				_ant[1]+=1;
				break;
			case 3:
				_ant[1]-=1;
				break;
			case 4:
				_ant[0]-=1;	
				break;
			case 2:
				_ant[0]+=1;
				break;
		}
	}

	void Run(int c)
	{
		Setant(_squaresize/2,_squaresize/2);
		while(Next(false)&&_mcount<=c){}
		gotoxy(0,_squaresize);
		printf("end");
		_mcount = 0;
	}
		
	void gotoxy(short x, short y) 
	{
	    COORD coord = {x, y}; 
		//	----->x
		//  |
		//  |
		//  V
		//  y
	    //COORD��Windows API�ж����һ�ֽṹ�����ͣ���ʾ����̨��Ļ�ϵ����ꡣ
	    //��������Ƕ�����COORD���͵ı���coord�������β�x��y���г�ʼ����
	    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
	    //GetStdHandle(STD_OUTPUT_HANDLE); ��ȡ����̨������
	    //Ȼ����SetConsoleCursorPosition���ÿ���̨(cmd)���λ��
	}
	
	
	void show()
	{
		for(int i=0;i<_squaresize;i++)
		{
			for(int j=0;j<_squaresize;j++)
			if(_presquare[i][j] != _square[i][j])
			{
				gotoxy(i,j);
				printf("%c",(_square[i][j] ? '#':'.')); 
			}
		}
		
		
		gotoxy(_ant[0],_ant[1]);

		if(_forward == 1)	printf("^"); 
		if(_forward == 2)	printf(">"); 
		if(_forward == 3)	printf("V"); 
		if(_forward == 4)	printf("<");

		for(int i=0;i<_squaresize;i++)
			for(int j=0;j<_squaresize;j++)
				_presquare[i][j] = _square[i][j];
	}
	
	void Turn()
	{
		if(_square[_ant[0]][_ant[1]]==true)
		{
			if(_forward+1>4)
			_forward=0;
			else	_forward+=1;
		}
		else
		{
			if(_forward-1<0)	_forward=4;
			else _forward-=1;
		}
	}

	void Turn_back()
	{
		if(_square[_ant[0]][_ant[1]]==false)
		{
			if(_forward+1>4)
			_forward=0;
			else	_forward+=1;
		}
		else
		{
			if(_forward-1<0)	_forward=4;
			else _forward-=1;
		}
	}

	bool Check_move()
	{
		switch(_forward)
		{
			case 1:
				_square[_ant[0]][_ant[1]] = !_square[_ant[0]][_ant[1]];
				_ant[1]+=1;
				if((_ant[1])>_squaresize-1)return false;
				break;
			case 3:
				_square[_ant[0]][_ant[1]] = !_square[_ant[0]][_ant[1]];
				_ant[1]-=1;
				if((_ant[1])<0)return false;
				break;
			case 4:
				_square[_ant[0]][_ant[1]] = !_square[_ant[0]][_ant[1]];
				_ant[0]-=1;	
				if((_ant[0])<0)return false;
				break;
			case 2:
				_square[_ant[0]][_ant[1]] = !_square[_ant[0]][_ant[1]];
				_ant[0]+=1;
				if((_ant[0])>_squaresize-1)return false;
				break;
		}
		return true; //allow
	}
	
	bool Check_move_back()
	{
		
		gotoxy(_ant[0],_ant[1]);
		if(_ant[0]>=0&&_ant[0]<64&&_ant[1]>=0&&_ant[1]<64)
		printf("%c",(_square[_ant[0]][_ant[1]] ? '#':'.'));
		
		switch(_forward)
		{
			case 3:
				_ant[1]+=1;
				if((_ant[1])>_squaresize-1)return false;
				_square[_ant[0]][_ant[1]] = !_square[_ant[0]][_ant[1]];
				break;
			case 1:
				_ant[1]-=1;
				if((_ant[1])<0)return false;
				_square[_ant[0]][_ant[1]] = !_square[_ant[0]][_ant[1]];
				break;
			case 2:
				_ant[0]-=1;	
				if((_ant[0])<0)return false;
				_square[_ant[0]][_ant[1]] = !_square[_ant[0]][_ant[1]];
				break;
			case 4:
				_ant[0]+=1;
				if((_ant[0])>_squaresize-1)return false;
				_square[_ant[0]][_ant[1]] = !_square[_ant[0]][_ant[1]];
				break;
		}
		return true; //allow
	}

	bool Previous(bool clog)
	{
		gotoxy(0,0);
		printf("x:%dy:%d",_ant[0],_ant[1]);

		_mcount-=1;

		if(!Check_move_back()) {
			gotoxy(0,_squaresize);
			cout<<"out of range _pre"<<endl;
			getch();
			return false;
		}
		Turn_back();
		if(clog)	getch();
		show();
		return true;

		getch();
	}

	bool Next(bool clog)
	{
		gotoxy(0,0);
		printf("x:%dy:%d",_ant[0],_ant[1]);
		_mcount+=1;
		Turn();
		if(!Check_move()) {
			gotoxy(0,_squaresize);
			cout<<"out of range"<<endl;
	//		system("pause");
			getch();
			return false;
		}
		if(clog)	getch();
	//	system("pause");
		if(_mcount%1==0)	show();
		return true;
	}
	
	void InitPrint()
	{	
		for(int i=0;i<_squaresize;i++)
		{ 
			for(int j=0;j<_squaresize;j++)
			{
				gotoxy(j,i);
				printf("."); 
			}
		}
	}
	
	void Setant(int x,int y) 
	{
		_ant[0] = x;
		_ant[1] = y;
	}
private:
	const int _squaresize;
	bool **_presquare;
	bool **_square;
	//false==white
	//true==black
	
	int _mcount;
	int *_ant;
	int _forward;
	//1==up
	//2==right
	//3==down
	//4==left
	//	y
	//	^
	//	|
	//	----->x
};

int main()
{
	int squaresize = 64;
	system("mode con cols=64");
	LangtonsAnt payload(squaresize);

	
	payload.Setant(squaresize/2,squaresize/2);
	payload.Run(false);
	payload.Run_back(false);
	payload.Run(false);
	payload.Run_back(false);
	printf("end");
	getche();
}
