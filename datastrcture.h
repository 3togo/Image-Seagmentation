#ifndef DATASTRCTURE_H
#define DATASTRCTURE_H
typedef struct 
{
	float w;
	int a;
	int b;
} edge;
typedef struct
{
	int rank;
	int size;
	int p;
}RegUnt;
class region
{
public:
	region(int elnum);
	~region();
	void joint(int a, int b);
	int getFlag(int a);
	int getSize (int a) const;
	int getGegNum() const;
private:
	RegUnt *Unt;
	int regNum;
};
#endif