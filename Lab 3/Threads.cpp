#include"Model.h"

void threading(HDC hdc, Automata *automata, int number)
{
	automata->field->redrawField(hdc);
	Sleep(1000);
	vector<vector<Organism*>>org;
	for (int i = 0; i < automata->getTime(); i++){
		for (int j = 0; j < number; j++)
			org.push_back(automata->getGroup(j));
		split_on_threads(automata, org);
		automata->addKidsToOrganisms();
		for (int j = 0; j < number; j++)
			del_parents(automata, org[j]);
		automata->field->redrawField(hdc);
		org.clear();
		Sleep(1000);
	}
}
void split_on_threads(Automata *automata, vector<vector<Organism*>> org)
{
	vector<thread> th;
	for (int i = 0; i < org.size(); i++)
	{
		th.push_back(thread(shell, automata, org[i]));
	}
	for (auto& t : th) t.join();
}
void shell(Automata *automata, vector<Organism*>org)
{
	automata->life(org);
}
void del_parents(Automata *automata, vector<Organism*>org)
{
	for (int i = 0; i < org.size(); i++)
	{
		Point p = org[i]->getPosition();
		string name_ = org[i]->getName();
		if (org[i]->added == false)
		{
			if (org[i]->used && name_ == automata->field->field[p.x][p.y]->getName()){
				automata->delFromOrganisms(name_, p);
				automata->field->field[p.x][p.y] = NULL;
			}
		}
		else(org[i]->added = false);
	}
}