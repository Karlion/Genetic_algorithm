#include "Model.h"

Organism::Organism(Color _rgb, string _name, Point _position, Automata *_automata, Field *_field)
{
	rgb.b = _rgb.b;
	rgb.g = _rgb.g;
	rgb.r = _rgb.r;
	name = _name;
	position.x = _position.x;
	position.y = _position.y;
	automata = _automata;
	field = _field;
}

bool Organism::breed()
{
	if (automata->sexBreed)
	{
		vector<Organism*> partners;
		for (int i = position.x - automata->radBreed; i < position.x + automata->radBreed; i++)
		{
			for (int j = position.y - automata->radBreed; j < position.y + automata->radBreed; j++)
			{
				if (i >= 0 && j >= 0 && i < automata->size && j < automata->size)
				{
					if (field->field[i][j] && name == field->field[i][j]->getName() && !field->field[i][j]->used)
					{
						Point p;
						p = field->field[i][j]->getPosition();
						if ((p.x != position.x) || (p.y != position.y))
							partners.push_back(field->field[i][j]);
					}
				}
			}
		}
		if (partners.size() > 0)
		{
			Organism *father;
			if (partners.size() == 1)
				father = partners[0];
			else
				father = partners[rand() % (partners.size())];
			makeKid(this, father);
			return true;
		}
		return false;
	}
	else
	{
		makeKidAlone(this);
		return true;
	}
}

Point Organism::getPosition()
{
	return position;
}

void Organism::makeKid(Organism* mother, Organism *father)
{
	Point mP = mother->getPosition();
	Point fP = father->getPosition();
	Point middle = getMiddleBetweenPoints(mP, fP);

	vector<Point> empty;
	vector<Point> full;
	for (int i = middle.x - automata->radDisp; i < middle.x + automata->radDisp; i++)
	{
		for (int j = middle.y - automata->radDisp; j < middle.y + automata->radDisp; j++)
		{
			if (i >= 0 && j >= 0 && i < automata->size && j < automata->size)
			{
				Point p;
				p.x = i;
				p.y = j;
				if (((p.x != mP.x) || (p.y != mP.y)) && ((p.x != fP.x) || (p.y != fP.y)))
				{
					if (field->field[i][j] && field->field[i][j]->getName() != mother->getName())
						full.push_back(p);
					else if(!field->field[i][j])
						empty.push_back(p);
				}
			}
		}
	}
	int childs = automata->fertility;
	while (childs)
	{
		if (empty.size() == 0 && full.size() > 0)
		{
			int randNum;
			int k = full.size();
			randNum = rand() % (full.size());
			Point p = full[randNum];
			full.erase(full.begin() + randNum);
			string name_ = mother->getName();
			Color col = mother->getColor();
			Organism* newOrg = new Organism(col, name_, p, automata, field);
			automata->mtx.lock();
			automata->newBorn[name_].push_back(newOrg);
			automata->mtx.unlock();
		}
		else if (empty.size() > 0)
		{
			int k = empty.size();
			int randNum = rand() % (empty.size());
			Point p = empty[randNum];
			empty.erase(empty.begin() + randNum);
			string name_ = mother->getName();
			Color col = mother->getColor();
			Organism* newOrg = new Organism(col, name_, p, automata, field);
			automata->mtx.lock();
			automata->newBorn[name_].push_back(newOrg);
			automata->mtx.unlock();
		}
		childs--;
	}
	
	mother->used = true;
	mother->added = false;
	father->used = true;
	father->added = false;
}

string Organism::getName()
{
	return name;
}

Color Organism::getColor()
{
	return rgb;
}

Point Organism::getMiddleBetweenPoints(Point p1, Point p2)
{
	Point res;
	int minX, minY;
	if (p1.x > p2.x)
		minX = p2.x;
	else
		minX = p1.x;
	if (p1.y > p2.y)
		minY = p2.y;
	else
		minY = p1.y;
	res.x = abs(p1.x - p2.x) / 2 + minX;
	res.y = abs(p1.y - p2.y) / 2 + minY;
	return res;
}

void Organism::makeKidAlone(Organism* mother)
{
	Point middle = mother->getPosition();

	vector<Point> empty;
	vector<Point> full;
	for (int i = middle.x - automata->radDisp; i < middle.x + automata->radDisp; i++)
	{
		for (int j = middle.y - automata->radDisp; j < middle.y + automata->radDisp; j++)
		{
			if (i >= 0 && j >= 0 && i < automata->size && j < automata->size)
			{
				Point p;
				p.x = i;
				p.y = j;
				if ((p.x != middle.x) ||  (p.y != middle.y))
				{
					if (field->field[i][j] && field->field[i][j]->getName() != mother->getName())
						full.push_back(p);
					else if (!field->field[i][j])
						empty.push_back(p);
				}
			}
		}
	}
	int childs = automata->fertility;
	while (childs)
	{
		if (empty.size() == 0 && full.size() > 0)
		{
			int randNum;
			int k = full.size();
			randNum = rand() % (full.size());
			Point p = full[randNum];
			full.erase(full.begin() + randNum);
			string name_ = mother->getName();
			Color col = mother->getColor();
			Organism* newOrg = new Organism(col, name_, p, automata, field);
			automata->mtx.lock();
			automata->newBorn[name_].push_back(newOrg);
			automata->mtx.unlock();
		}
		else if (empty.size() > 0)
		{
			int k = empty.size();
			int randNum = rand() % (empty.size());
			Point p = empty[randNum];
			empty.erase(empty.begin() + randNum);
			string name_ = mother->getName();
			Color col = mother->getColor();
			Organism* newOrg = new Organism(col, name_, p, automata, field);
			automata->mtx.lock();
			automata->newBorn[name_].push_back(newOrg);
			automata->mtx.unlock();
		}
		childs--;
	}
	
	mother->used = true;
	mother->added = false;
}