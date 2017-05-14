#include <windows.h>
#include "Model.h"
#include<thread>

using namespace std;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND hWnd;
	MSG lpMsg;
	WNDCLASS w;
	char szProgName[] = "Modeling";
	w.lpszClassName = szProgName;
	w.hInstance = hInstance;
	w.lpfnWndProc = WndProc;
	w.hCursor = LoadCursor(NULL, IDC_ARROW);
	w.hIcon = 0;
	w.lpszMenuName = 0;
	w.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	w.style = CS_HREDRAW | CS_VREDRAW;
	w.cbClsExtra = 0;
	w.cbWndExtra = 0;

	if (!RegisterClass(&w))
		return 0;
	hWnd = CreateWindow(szProgName, "Modeling", WS_OVERLAPPEDWINDOW, 50, 50, 718, 740,
		(HWND)NULL, (HMENU)NULL, (HINSTANCE)hInstance, (HINSTANCE)NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&lpMsg, NULL, 0, 0))
	{
		TranslateMessage(&lpMsg);
		DispatchMessage(&lpMsg);
	}
	return(lpMsg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HPEN hPen;
	HDC hdc;
	Automata *automata;
	srand(time(0));
	short radDisp;
	short radBreed;
	short fertility;
	short density;
	bool sexBreed;
	int time;
	int size;
	int noOfSpp;

	switch (messg)
	{
	case WM_SIZE:
		break;
	case WM_CREATE:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		read(radDisp, radBreed, fertility, density, sexBreed, time, size, noOfSpp);
		automata = new Automata(hdc, 700, radDisp, radBreed, fertility, density, sexBreed, time, size, noOfSpp);
		threading(hdc, automata, noOfSpp);
		//automata->live(hdc);
		ValidateRect(hWnd, NULL);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return(DefWindowProc(hWnd, messg, wParam, lParam));
	}
	return 0;
}