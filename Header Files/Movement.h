#pragma once
class Movement
{
public:
	Movement();
	Movement(int fromx,int fromy,int tox,int toy);
	~Movement();
	int fromx;
	int fromy;
	int tox;
	int toy;
};

