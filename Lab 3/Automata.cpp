#include "Model.h"
#include <ctime>
#include <fstream>
using namespace std;
ifstream fin("Input.txt");

void Automata::delFromOrganisms(string name_, Point p)
{
	for (size_t i = 0; i < organisms[name_].size(); i++)
	{
		string name1 = organisms[name_][i]->getName();
		Point p1 = organisms[name_][i]->getPosition();
		if (name1 == name_ && p.x == p1.x && p.y == p1.y)
		{
			organisms[name_].erase(organisms[name_].begin() + i);
			return;
		}
	}
}


void Automata::live(HDC hdc)
{
	for (int i = 0; i < time; i++)
	{
		map <string, vector<Organism*>>::iterator cur;
		for (cur = organisms.begin(); cur != organisms.end(); cur++)
		{
			for (int j = 0; j < organisms[(*cur).first].size(); j++)
			{
				organisms[(*cur).first][j]->breed();
			}
		}
		addKidsToOrganisms();
		field->redrawField(hdc);
		Sleep(1000);
	}
}

void Automata::life(vector<Organism*>org)
{
	for (int i = 0; i < org.size(); i++)
	{
		if (!org[i]->used)
			org[i]->breed();
	}
}

Automata::Automata(HDC hdc, double length, short radDisp, short radBreed, short fertility, short density, bool sexBreed,
	int time, int size, int noOfSpp)
{
	this->radDisp = radDisp;
	this->radBreed = radBreed;
	this->fertility = fertility;
	this->density = density;
	this->sexBreed = sexBreed;
	this->time = time;
	this->size = size;
	this->noOfSpp = noOfSpp;
	field = new Field(hdc, length, size);
	readData();
	Sleep(1000);
}

void Automata::readData()
{
	vector<Point> temp;
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
		{
			Point p;
			p.x = i;
			p.y = j;
			temp.push_back(p);
		}
	while (noOfSpp)
	{
		string name;
		Color rgb;
		fin >> name
			>> rgb.r
			>> rgb.g
			>> rgb.b;
		for (int i = 0; i < density; i++)
		{
			int randNum = rand() % temp.size();
			Point p = temp[randNum];
			temp.erase(temp.begin() + randNum);
			Organism *org = new Organism(rgb, name, p, this, field);
			field->field[p.x][p.y] = org;
			organisms[name].push_back(org);
		}

		noOfSpp--;
	}
	temp.clear();
}

void Automata::addKidsToOrganisms()
{
	map <string, vector<Organism*>>::iterator cur;
	for (cur = newBorn.begin(); cur != newBorn.end(); cur++)
	{
		for (int j = 0; j < newBorn[(*cur).first].size(); j++)
		{
			Point p = newBorn[(*cur).first][j]->getPosition();
			if (field->field[p.x][p.y])
			{
				int randNum = rand() % 2;
				if (randNum)
				{
					delFromOrganisms(field->field[p.x][p.y]->getName(), p);
					field->field[p.x][p.y] = newBorn[(*cur).first][j];
					organisms[newBorn[(*cur).first][j]->getName()].push_back(newBorn[(*cur).first][j]);
				}
			}
			else
			{
				field->field[p.x][p.y] = newBorn[(*cur).first][j];
				organisms[newBorn[(*cur).first][j]->getName()].push_back(newBorn[(*cur).first][j]);
			}
		}
	}
	newBorn.clear();
}

void read(short &radDisp, short &radBreed, short &fertility, short &density, bool &sexBreed, int &time, int &size,
	int &noOfSpp)
{

	fin >> size
		>> radDisp
		>> radBreed
		>> fertility
		>> density
		>> time
		>> sexBreed
		>> noOfSpp;
};

vector<Organism*> Automata::getGroup(int i)
{
	map <string, vector<Organism*>>::iterator cur;
	int k = 0;
	for (cur = organisms.begin(); cur != organisms.end(); cur++)
	{
		if (k == i)
			return organisms[(*cur).first];
		k++;
	}
}