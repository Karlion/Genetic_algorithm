#include "Model.h"

BOOL Field::Line(HDC hdc, int x1, int y1, int x2, int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	return LineTo(hdc, x2, y2);
}

void Field::redrawField(HDC hdc)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (field[i][j])
			{
				Color rgb = field[i][j]->getColor();
				drawRectangle(hdc, rgb, i, j);
			}
			else
			{
				Color rgb;
				rgb.r = 255;
				rgb.g = 255;
				rgb.b = 255;
				drawRectangle(hdc, rgb, i, j);
			}
		}
	}
}

Field::Field(HDC hdc, double _length, int _size)
{
	length = _length;
	size = _size;
	coef = length / size;
	drawField(hdc);
	vector<Organism*> temp;
	temp.assign(size, NULL);
	for (int i = 0; i < size; i++)
		field.push_back(temp);
}

void Field::drawRectangle(HDC hdc, Color rgb, int i, int j)
{
	RECT r;
	r.left = j * coef + 3;
	r.top = i * coef + 3;
	r.right = r.left + coef - 6;
	r.bottom = r.top + coef - 6;
	FillRect(hdc, &r, (HBRUSH)CreateSolidBrush(RGB(rgb.r, rgb.g, rgb.b)));
}

void Field::drawField(HDC hdc)
{
	HPEN hPen;
	hPen = CreatePen(PS_INSIDEFRAME, 2, RGB(0, 0, 0));
	SelectObject(hdc, hPen);
	for (int i = 0; i < size + 1; i++)
	{
		Line(hdc, i * coef, 0, i * coef, length);
		Line(hdc, 0, i * coef, length, i * coef);
	}
}