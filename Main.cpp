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

enum class Dir{LEFT=0,RIGHT,UP,DOWN};

struct Player{
	RECT rect;
	int aIndex; // Animation Index
	Dir dir;
};

struct Monster {
	RECT rect;
	int aIndex;	// Animation Index
};

struct Item {
	RECT rect;
};

Player mario;
Player luigi;

Monster gumba[MAX_MONSTER];
Monster troopa[MAX_MONSTER];

RECT gateSpot;	// EndLineBox

RECT temp;	// Just temp
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
		hMario = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP4));
		hLuigi = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP5));
		gate = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));

		gateSpot.left = 1050;
		gateSpot.top = 0;
		gateSpot.right = 1200;
		gateSpot.bottom = 150;

		/* Mario start position */
		mario.rect.left = 100;
		mario.rect.top = 100;
		mario.rect.right = mario.rect.left+80;
		mario.rect.bottom = mario.rect.top + 80;

		/* Luigi start position */
		luigi.rect.left = 200;
		luigi.rect.top = 200;
		luigi.rect.right = luigi.rect.left+75;
		luigi.rect.bottom = luigi.rect.top+80;
		
		SetTimer(hWnd, 1000, 5000, NULL);
		break;
	case WM_CHAR:
		// W A S D -> Mario Move, I J K L -> Luigi Move
		switch (tolower(wParam))
		{
		case 'w':
			mario.rect.top -= 5;
			mario.rect.bottom -= 5;
			mario.dir = Dir::UP;
			break;
		case 's':
			mario.rect.top += 5;
			mario.rect.bottom += 5;
			mario.dir = Dir::DOWN;
			break;
		case 'a':
			mario.rect.left -= 5;
			mario.rect.right -= 5;
			mario.dir = Dir::LEFT;
			break;
		case 'd':
			mario.rect.left += 5;
			mario.rect.right += 5;
			mario.dir = Dir::RIGHT;
			break;
		case 'i':
			luigi.rect.top -= 5;
			luigi.rect.bottom -= 5;
			luigi.dir = Dir::UP;
			break;
		case 'k':
			luigi.rect.top += 5;
			luigi.rect.bottom += 5;
			luigi.dir = Dir::DOWN;
			break;
		case 'j':
			luigi.rect.left -= 5;
			luigi.rect.right -= 5;
			luigi.dir = Dir::LEFT;
			break;
		case 'l':
			luigi.rect.left += 5;
			luigi.rect.right += 5;
			luigi.dir = Dir::DOWN;
			break;

		case 'g':	// Into Gate
			// Mario into gate
			if (IntersectRect(&temp, &gateSpot, &mario.rect)) {

			}
			// Luigi into gate
			else if (IntersectRect(&temp, &gateSpot, &luigi.rect)) {

			}
			break;
		default:
			break;
		}
		InvalidateRect(hWnd, NULL, FALSE);
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
		DrawMario(backMemDC, memDC, hMario,oldMario);
		DrawLuigi(backMemDC, memDC, hLuigi,oldLuigi);


		//DrawGumba(backMemDC, memDC, hGumba);
		BitBlt(hDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, backMemDC, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
		break;
	case WM_TIMER:
		switch (wParam)
		{
			/* Monster Spawning */
		case 1000:
			break;
		default:
			break;
		}
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
	oldMario = static_cast<HBITMAP>(SelectObject(memDC, hMario));
	TransparentBlt(backMemDC, mario.rect.left, mario.rect.top, 80, 80, memDC, 12, 251, 34, 38, RGB(0, 174, 0));
	SelectObject(memDC, oldMario);
}

void DrawLuigi(HDC backMemDC, HDC memDC, HBITMAP hLuigi, HBITMAP oldLuigi)
{
	oldLuigi = static_cast<HBITMAP>(SelectObject(memDC, hLuigi));
	TransparentBlt(backMemDC, luigi.rect.left, luigi.rect.top, 75, 80, memDC, 4, 256, 24, 44, RGB(0, 174, 174));
	SelectObject(memDC, oldLuigi);
}

//void DrawGumba(HDC backMemDC, HDC memDC, HBITMAP hGumba, HBITMAP oldGumba)
//{
//	SelectObject(memDC, hGumba);
//	TransparentBlt(backMemDC, Gumba[0].rect.left, Gumba[0].rect.top, 40, 40, memDC, 0, 145, 27, 23, RGB(0, 162, 232));
//}