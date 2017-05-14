#include <vector>
#include <string>
#include <map>
#include <windows.h>
#include <ctime>
#include<iostream>
#include <mutex>
#include<thread>
using namespace std;


class Organism;
class Field;

struct Point
{
	int x, y;
};
struct Color
{
	double r;
	double g;
	double b;
};

class Automata
{
	short radDisp;
	short radBreed;
	short fertility;
	short density;
	bool sexBreed;
	int time;
	int size;
	int noOfSpp;
	map<string, vector<Organism*>> organisms;
	map<string, vector<Organism*>> newBorn;
	friend class Organism;
public:
	Automata(HDC hdc, double length, short radDisp, short radBreed, short fertility, short density, bool sexBreed,
		int time, int size, int noOfSpp);
	void delFromOrganisms(string name_, Point p);
	void live(HDC hdc);
	void life(vector<Organism*>);
	void readData();
	void addKidsToOrganisms();
	int getTime(){ return time; };
	vector<Organism*> getGroup(int i);
	Field *field;
	mutex mtx;
};

class Organism
{
	Color rgb;
	string name;
	Point position;
	Automata *automata;
	Field *field;
public:
	Organism(Color _rgb, string _name, Point _position, Automata *_automata, Field *_field);
	bool getStatus();
	bool breed();
	void makeKid(Organism* mother, Organism *father);
	void makeKidAlone(Organism* father);
	Point getMiddleBetweenPoints(Point p1, Point p2);
	Point getPosition();
	string getName();
	Color getColor();
	bool used = false;
	bool added = true;
};

class Field
{
	double length;
	int size;
	double coef;
	friend class Organism;
	friend class Automata;
public:
	Field(HDC hdc, double _length, int _size);
	BOOL Line(HDC hdc, int x1, int y1, int x2, int y2);
	void redrawField(HDC hdc);
	void drawField(HDC hdc);
	void drawRectangle(HDC hdc, Color rgb, int i, int j);
	vector<vector<Organism*>> field;
};

void read(short &radDisp, short &radBreed, short &fertility, short &density, bool &sexBreed, int &time, int &size,
	int &noOfSpp);
void Thread1(void* pParams);
void Thread2(void* pParams);
void Thread3(void* pParams);
void Thread4(void* pParams);
static Automata *autamata;
static HDC myHDC;
void threading(HDC, Automata*, int);
void split_on_threads(Automata*, vector<vector<Organism*>>);
void shell(Automata*, vector<Organism*>);
void del_parents(Automata*, vector<Organism*>);