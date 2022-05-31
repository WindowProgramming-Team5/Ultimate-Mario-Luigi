#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib, "msimg32.lib") 

struct Rect {
	int x, y;
	int endx;
	BOOL isDie;
	int right, bottom;
	RECT r;
	int hp;
	int type;
	int loc;
	int sceneState;
};
struct Rect hero;
struct Rect* enemy;

void CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
HINSTANCE hinstance;
TCHAR lpszClass[] = TEXT("l");
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

HBRUSH hbrush, oldbrush;
HDC hdc, MemDC, tmpDC;
HDC memdc;
HBITMAP BackBit, oldBackBit, Back;
HBITMAP Hero;
RECT bufferRT;
PAINTSTRUCT ps;
BITMAP bmp;
BITMAP bmpb1;
BOOL isJump = FALSE;
BOOL isAni = FALSE;
int bbw1, bbh1;
RECT r;
int scene1,tempY;
float JumpTime = 0.f, JumpHeight = 0, JumpPower = 65.f;
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	hinstance = hInstance;

	if (!hPrevInstance) {
		WndClass.cbClsExtra = 0;
		WndClass.cbWndExtra = 0;
		WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);		//--- 윈도우 배경색 변경 가능
		WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		WndClass.hInstance = hInstance;
		WndClass.lpfnWndProc = (WNDPROC)WndProc;
		WndClass.lpszClassName = lpszClass;
		WndClass.lpszMenuName = NULL;
		WndClass.style = CS_HREDRAW | CS_VREDRAW;
		RegisterClass(&WndClass);
	}
	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, 0, 0, 1024, 768, NULL, (HMENU)NULL, hInstance, NULL);		//--- 윈도우 크기 변경 가능
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage) {
	case WM_CREATE:
		srand(time(NULL));
		Hero = (HBITMAP)LoadImage(hinstance, TEXT("Root1.bmp"), IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(Hero, sizeof(BITMAP), &bmpb1);
		bbw1 = bmpb1.bmWidth;
		bbh1 = bmpb1.bmHeight;


		hero.right = bbw1 / 6;
		hero.bottom = bbh1 / 4;
		SetTimer(hWnd, 8, 16, TimerProc);
		GetClientRect(hWnd, &r);
		hero.x = r.right - 200;
		hero.y = r.bottom - 200;
		tempY = hero.bottom;
		break;
	case WM_PAINT:
		MemDC = BeginPaint(hWnd, &ps); // 더블 버퍼링 처리 준비
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &bufferRT);
		hdc = CreateCompatibleDC(MemDC);
		BackBit = CreateCompatibleBitmap(MemDC, bufferRT.right, bufferRT.bottom);
		oldBackBit = (HBITMAP)SelectObject(hdc, BackBit);
		PatBlt(hdc, 0, 0, bufferRT.right, bufferRT.bottom, WHITENESS); // 여기까지 더블 버퍼링 준비
		memdc = CreateCompatibleDC(hdc);


		//이 아래부터 실제 그리기
		SelectObject(memdc, Hero);
		TransparentBlt(hdc, hero.x, hero.y + JumpHeight, hero.right, hero.bottom,
			memdc, hero.right * scene1, bbh1 / 4, hero.right, bbh1 / 4, RGB(0, 0, 255));


		//더블 버퍼링 끝처리, 오브젝트 삭제
		GetClientRect(hWnd, &bufferRT);
		BitBlt(MemDC, 0, 0, bufferRT.right, bufferRT.bottom, hdc, 0, 0, SRCCOPY);
		SelectObject(hdc, oldBackBit);
		SelectObject(hdc, oldBackBit);
		DeleteObject(BackBit);
		DeleteDC(hdc);
		DeleteDC(memdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime) { 
	//타이머에서는 객체들의 이동 처리
	if (GetAsyncKeyState(VK_LEFT)) {
		if (hero.x + 10 > 0)
			hero.x -= 10;
	}
	if (GetAsyncKeyState(VK_RIGHT)) {
		if (hero.right + hero.x < r.right)
			hero.x += 10;
	}
	if (GetAsyncKeyState(VK_SPACE)) {
		if (!isJump) {
			isJump = TRUE;
		}
	}
	if (isJump) {
		JumpHeight = (JumpTime * JumpTime - JumpPower * JumpTime) / 4.f;
		JumpTime += 1.6f;
		if (JumpTime > JumpPower) {
			JumpTime = 0; JumpHeight = 0; isJump = FALSE;// KillTimer(hWnd, 1);
		}
	}
	InvalidateRect(hWnd, NULL, FALSE);
}

