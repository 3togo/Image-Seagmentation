#include"datastrcture.h"
region::region(int elnum)
{
	regNum = elnum;
	Unt = new RegUnt[elnum];
	for (int i = 0; i < elnum; i++)
	{
		Unt[i].rank = 0;
		Unt[i].p = i;               //记录元素的父节点
		Unt[i].size = 1;
		
	}
}
region::~region()
{
	delete[]Unt;
}
void region::joint(int a, int b)
{
	if (Unt[a].rank > Unt[b].rank)
	{
		Unt[a].size += Unt[b].size;
		Unt[b].p = a;
	}
	else
	{
		Unt[b].size += Unt[a].size;
		Unt[a].p = b;
		if (Unt[a].rank == Unt[b].rank)
		{
			Unt[b].rank++;
		}
	}
	regNum--;
}
int region::getFlag(int a)
{
	int temp = a;
	while (temp!=Unt[temp].p)
	{
		temp = Unt[temp].p;
	}
	Unt[a].p = temp;
	return temp;
	

}
int region::getGegNum() const
{
	return regNum;
}
int region::getSize(int a) const
{
	return Unt[a].size;
}
