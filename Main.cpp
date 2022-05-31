#pragma comment (lib,"msimg32.lib")

#include <Windows.h>
#include <tchar.h>
#include <random>

#include "resource.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

#define MAX_MONSTER 100

using std::default_random_engine;
using std::random_device;
using std::uniform_int_distribution;

struct Player{
	RECT rect;
	int aIndex; // Animation Index
};

struct Monster {
	RECT rect;
	int aIndex;	// Animation Index
};

struct Item {
	RECT rect;
};

Player Mario;
Player Luigi;

Monster Gumba[MAX_MONSTER];
Monster Troopa[MAX_MONSTER];

RECT gateSpot;	// EndLineBox

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window class Name";
LPCTSTR lpszWindowName = L"Window Programming Lab";

void DrawBackGround(HDC, HDC, HBITMAP, HBITMAP);
void DrawMario(HDC, HDC, HBITMAP,HBITMAP);
void DrawLuigi(HDC, HDC, HBITMAP, HBITMAP);
void DrawGumba(HDC, HDC, HBITMAP, HBITMAP);
void DrawTroopa(HDC, HDC, HBITMAP, HBITMAP);
void DrawGate(HDC, HDC, HBITMAP, HBITMAP);

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC, backMemDC, memDC;
	static HBITMAP backBitmap, oldBackBitmap;
	static HBITMAP backGround, oldBackGround;
	static HBITMAP hMario, oldMario;
	static HBITMAP hLuigi, oldLuigi;
	static HBITMAP hGumba, oldGumba;
	static HBITMAP gate, oldGate;
	switch (uMsg)
	{
	case WM_CREATE:
		backGround = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		//hGumba = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
		gate = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));

		gateSpot.left = 1050;
		gateSpot.top = 0;
		gateSpot.right = 1200;
		gateSpot.bottom = 150;

		Mario.rect.left = 100;
		Mario.rect.top = 100;
		Mario.rect.right = 100;
		Mario.rect.bottom = 100;

		Gumba[0].rect.left = 100;
		Gumba[0].rect.top = 100;
		Gumba[0].rect.right  = 200;
		Gumba[0].rect.bottom = 200;
		break;
	case WM_CHAR:
		// W A S D -> Mario Move, I J K L -> Luigi Move
		switch (tolower(wParam))
		{
		case 'w':
			break;
		case 's':
			break;
		case 'a':
			break;
		case 'd':
			break;
		case 'i':
			break;
		case 'k':
			break;
		case 'j':
			break;
		case 'l':
			break;
		case 'g':	// Into Gate
			break;
		default:
			break;
		}
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		backMemDC = CreateCompatibleDC(hDC);
		memDC = CreateCompatibleDC(hDC);

		backBitmap = CreateCompatibleBitmap(hDC, WINDOW_WIDTH, WINDOW_HEIGHT);
		oldBackBitmap = static_cast<HBITMAP>(SelectObject(backMemDC, backBitmap));
		PatBlt(backMemDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, WHITENESS);

		DrawBackGround(backMemDC, memDC, backGround,oldBackGround);
		DrawGate(backMemDC, memDC, gate, oldGate);
		//DrawMario(backMemDC, memDC, hMario);
		//DrawLuigi(backMemDC, memDC, hLuigi);

		//DrawGumba(backMemDC, memDC, hGumba);
		BitBlt(hDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, backMemDC, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


void DrawBackGround(HDC backMemDC, HDC memDC, HBITMAP backGround, HBITMAP oldBackGround)
{
	oldBackGround = static_cast<HBITMAP>(SelectObject(memDC, backGround));
	TransparentBlt(backMemDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, memDC, 2, 11, 510, 510, RGB(255, 0, 0));
	SelectObject(memDC, oldBackGround);
}

void DrawGate(HDC backMemDC, HDC memDC, HBITMAP gate, HBITMAP oldGate)
{
	oldGate = static_cast<HBITMAP>(SelectObject(memDC, gate));
	TransparentBlt(backMemDC, gateSpot.left-10, gateSpot.top, 130, 130, memDC, 355, 30, 150, 150, RGB(255, 0, 255));
	SelectObject(memDC, oldGate);
}
void DrawMario(HDC backMemDC, HDC memDC, HBITMAP hMario, HBITMAP oldMario)
{
	SelectObject(memDC, hMario);
	TransparentBlt(backMemDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, memDC, 2, 11, 510, 510, RGB(255, 0, 0));
}

void DrawLuigi(HDC backMemDC, HDC memDC, HBITMAP hLuigi, HBITMAP oldLuigi)
{

}

void DrawGumba(HDC backMemDC, HDC memDC, HBITMAP hGumba, HBITMAP oldGumba)
{
	SelectObject(memDC, hGumba);
	TransparentBlt(backMemDC, Gumba[0].rect.left, Gumba[0].rect.top, 40, 40, memDC, 0, 145, 27, 23, RGB(0, 162, 232));
}