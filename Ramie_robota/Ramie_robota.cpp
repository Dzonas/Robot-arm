// Ramie_robota.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Ramie_robota.h"
#define _USE_MATH_DEFINES
#include <cmath>

#define MAX_LOADSTRING 100

const REAL circle_size = 7;
const REAL arm1_length = 150;
const REAL arm2_length = 150;

HWND hwndButton;
RECT drawArea1 = { 0, 0, 310, 200 };

PointF arm1_start(0.0, (REAL)drawArea1.bottom);
PointF arm1_end(arm1_length, (REAL)drawArea1.bottom);
PointF arm2_start(arm1_end.X, arm1_end.Y);
PointF arm2_end((REAL)(arm1_length + arm2_length), (REAL)drawArea1.bottom);
PointF triangle1_1((REAL)200, (REAL)(drawArea1.bottom));
PointF triangle1_2((REAL)220, (REAL)(drawArea1.bottom));
PointF triangle1_3((REAL)210, (REAL)(drawArea1.bottom - 20));
PointF triangle2_1((REAL)221, (REAL)(drawArea1.bottom));
PointF triangle2_2((REAL)241, (REAL)(drawArea1.bottom));
PointF triangle2_3((REAL)231, (REAL)(drawArea1.bottom - 20));
PointF triangle3_1((REAL)242, (REAL)(drawArea1.bottom));
PointF triangle3_2((REAL)262, (REAL)(drawArea1.bottom));
PointF triangle3_3((REAL)252, (REAL)(drawArea1.bottom - 20));
PointF triangle4_1((REAL)200, (REAL)(drawArea1.bottom - 21));
PointF triangle4_2((REAL)220, (REAL)(drawArea1.bottom - 21));
PointF triangle4_3((REAL)210, (REAL)(drawArea1.bottom - 41));
PointF triangle5_1((REAL)221, (REAL)(drawArea1.bottom - 21));
PointF triangle5_2((REAL)241, (REAL)(drawArea1.bottom - 21));
PointF triangle5_3((REAL)231, (REAL)(drawArea1.bottom - 41));
PointF triangle6_1((REAL)242, (REAL)(drawArea1.bottom - 21));
PointF triangle6_2((REAL)262, (REAL)(drawArea1.bottom - 21));
PointF triangle6_3((REAL)252, (REAL)(drawArea1.bottom - 41));
REAL arc1 = 30;
REAL arc2 = 30;
bool grabbed[6] = { false, false, false, false, false, false };

void initialize(void);
void paint(HDC);
void repaintWindow(HWND, HDC &, PAINTSTRUCT &, RECT *);
void arm1_moveUp(void);
void arm1_moveDown(void);
void arm2_moveUp(void);
void arm2_moveDown(void);
void grab(void);
void drop(void);

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
	initialize();

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_RAMIE_ROBOTA, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RAMIE_ROBOTA));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	GdiplusShutdown(gdiplusToken);

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RAMIE_ROBOTA));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_RAMIE_ROBOTA);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	hwndButton = CreateWindow(
		TEXT("button"),
		TEXT("Draw"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		300,
		300,
		80,
		50,
		hWnd,
		(HMENU)ID_BUTTON1,
		hInstance,
		nullptr);

	hwndButton = CreateWindow(
		TEXT("button"),
		TEXT("Arm 1 up"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		390,
		300,
		80,
		50,
		hWnd,
		(HMENU)ID_BUTTON2,
		hInstance,
		nullptr);

	hwndButton = CreateWindow(
		TEXT("button"),
		TEXT("Arm 1 down"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		470,
		300,
		80,
		50,
		hWnd,
		(HMENU)ID_BUTTON3,
		hInstance,
		nullptr);

	hwndButton = CreateWindow(
		TEXT("button"),
		TEXT("Arm 2 up"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		560,
		300,
		80,
		50,
		hWnd,
		(HMENU)ID_BUTTON4,
		hInstance,
		nullptr);

	hwndButton = CreateWindow(
		TEXT("button"),
		TEXT("Arm 2 down"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		640,
		300,
		80,
		50,
		hWnd,
		(HMENU)ID_BUTTON5,
		hInstance,
		nullptr);

	hwndButton = CreateWindow(
		TEXT("button"),
		TEXT("Grab"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		390,
		360,
		80,
		50,
		hWnd,
		(HMENU)ID_BUTTON6,
		hInstance,
		nullptr);

	hwndButton = CreateWindow(
		TEXT("button"),
		TEXT("Drop"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		470,
		360,
		80,
		50,
		hWnd,
		(HMENU)ID_BUTTON7,
		hInstance,
		nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_BUTTON1:
			repaintWindow(hWnd, hdc, ps, &drawArea1);
			break;
		case ID_BUTTON2: // Arm 1 up
			arm1_moveUp();
			repaintWindow(hWnd, hdc, ps, &drawArea1);
			break;
		case ID_BUTTON3: // Arm 1 down
			arm1_moveDown();
			repaintWindow(hWnd, hdc, ps, &drawArea1);
			break;
		case ID_BUTTON4: // Arm 2 up
			arm2_moveUp();
			repaintWindow(hWnd, hdc, ps, &drawArea1);
			break;
		case ID_BUTTON5: // Arm 2 down
			arm2_moveDown();
			repaintWindow(hWnd, hdc, ps, &drawArea1);
			break;
		case ID_BUTTON6: // Grab button
			grab();
			repaintWindow(hWnd, hdc, ps, &drawArea1);
			break;
		case ID_BUTTON7: // Drop button
			drop();
			repaintWindow(hWnd, hdc, ps, &drawArea1);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

int OnCreate(HWND window)
{
	return 0;
}

void initialize()
{
	arm1_end.Y = (REAL)(drawArea1.bottom - arm1_length * sin(arc1 * (M_PI / 180)));
	arm1_end.X = (REAL)(arm1_length * abs(cos(arc1 * (M_PI / 180))));

	arm2_start = arm1_end;

	arm2_end.Y = (REAL)(arm2_start.Y - arm2_length * sin(arc2 * (M_PI / 180)));
	arm2_end.X = (REAL)(arm2_length * cos(arc2 * (M_PI / 180)) + arm2_start.X);
}

void repaintWindow(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps, RECT *drawArea)
{
	if (drawArea == nullptr)
		InvalidateRect(hWnd, nullptr, TRUE); // repaint all
	else
		InvalidateRect(hWnd, drawArea, TRUE); //repaint drawArea
	hdc = BeginPaint(hWnd, &ps);
	paint(hdc);
	EndPaint(hWnd, &ps);
}

void paint(HDC hdc)
{
	Graphics graphics(hdc);
	Pen pen1(Color(255, 0, 0, 255));
	Pen pen2(Color(255, 255, 0, 0));
	Pen pen3(Color(255, 0, 255, 0));
	SolidBrush brushRed(Color::Red);
	SolidBrush brushBlack(Color::Black);

	graphics.DrawLine(&pen1, arm1_start, arm1_end);
	graphics.DrawLine(&pen1, arm2_start, arm2_end);
	graphics.DrawEllipse(&pen2, (REAL)(arm1_start.X - arm1_length), (REAL)(arm1_start.Y - arm1_length), (REAL)arm1_length * 2, (REAL)arm1_length * 2);
	graphics.DrawEllipse(&pen3, (REAL)(arm2_start.X - arm2_length), (REAL)(arm2_start.Y - arm2_length), (REAL)arm2_length * 2, (REAL)arm2_length * 2);
	graphics.FillEllipse(&brushRed, arm1_end.X - circle_size / 2, arm1_end.Y - circle_size / 2, circle_size, circle_size);
	graphics.FillEllipse(&brushRed, arm2_end.X - circle_size / 2, arm2_end.Y - circle_size / 2, circle_size, circle_size);

	PointF triangle1[3] = { triangle1_1, triangle1_2, triangle1_3 };
	PointF triangle2[3] = { triangle2_1, triangle2_2, triangle2_3 };
	PointF triangle3[3] = { triangle3_1, triangle3_2, triangle3_3 };
	PointF triangle4[3] = { triangle4_1, triangle4_2, triangle4_3 };
	PointF triangle5[3] = { triangle5_1, triangle5_2, triangle5_3 };
	PointF triangle6[3] = { triangle6_1, triangle6_2, triangle6_3 };

	graphics.FillPolygon(&brushBlack, triangle1, 3);
	graphics.FillPolygon(&brushBlack, triangle2, 3);
	graphics.FillPolygon(&brushBlack, triangle3, 3);
	graphics.FillPolygon(&brushBlack, triangle4, 3);
	graphics.FillPolygon(&brushBlack, triangle5, 3);
	graphics.FillPolygon(&brushBlack, triangle6, 3);
}

void arm1_moveUp()
{
	if (arc1 < 90 && arm2_end.X > drawArea1.left && arm1_end.Y > drawArea1.top && arm1_end.X < drawArea1.right && arm2_end.Y > drawArea1.top && arm2_end.X < drawArea1.right)
	{
		arc1++;

		arm1_end.Y = (REAL)(drawArea1.bottom - arm1_length * sin(arc1 * (M_PI / 180)));
		arm1_end.X = (REAL)(arm1_length * abs(cos(arc1 * (M_PI / 180))));

		arm2_start = arm1_end;

		arm2_moveUp();
	}
}

void arm1_moveDown()
{
	if (arc1 > 0 && arm2_end.Y < drawArea1.bottom)
	{
		arc1--;

		arm1_end.Y = (REAL)(drawArea1.bottom - arm1_length * sin(arc1 * (M_PI / 180)));
		arm1_end.X = (REAL)(arm1_length * abs(cos(arc1 * (M_PI / 180))));

		arm2_start = arm1_end;

		arm2_moveDown();
	}
}

void arm2_moveUp()
{
	if (arm2_end.X > drawArea1.left && arm2_end.Y > drawArea1.top && arm2_end.X < drawArea1.right)
	{
		arc2++;

		arm2_end.Y = (REAL)(arm2_start.Y - arm2_length * sin(arc2 * (M_PI / 180)));
		arm2_end.X = (REAL)(arm2_length * cos(arc2 * (M_PI / 180)) + arm2_start.X);

		if (grabbed[0])
		{
			triangle1_1.X = arm2_end.X - 10;
			triangle1_1.Y = arm2_end.Y + 10;

			triangle1_2.X = arm2_end.X + 10;
			triangle1_2.Y = arm2_end.Y + 10;

			triangle1_3.X = arm2_end.X;
			triangle1_3.Y = arm2_end.Y - 10;
		}
	}
}

void arm2_moveDown()
{
	if ((arm2_end.Y < drawArea1.bottom || (arm2_end.Y == drawArea1.bottom && arc2 == 180)) && arm2_end.X < drawArea1.right)
	{
		arc2--;

		arm2_end.Y = (REAL)(arm2_start.Y - arm2_length * sin(arc2 * (M_PI / 180)));
		arm2_end.X = (REAL)(arm2_length * cos(arc2 * (M_PI / 180)) + arm2_start.X);

		if (grabbed[0])
		{
			triangle1_1.X = arm2_end.X - 10;
			triangle1_1.Y = arm2_end.Y + 10;

			triangle1_2.X = arm2_end.X + 10;
			triangle1_2.Y = arm2_end.Y + 10;

			triangle1_3.X = arm2_end.X;
			triangle1_3.Y = arm2_end.Y - 10;
		}
	}
}

void grab()
{
	REAL distance1 = sqrt((arm2_end.X - triangle1_3.X) * (arm2_end.X - triangle1_3.X) + (arm2_end.Y - (triangle1_3.Y + 10)) * (arm2_end.Y - (triangle1_3.Y + 10)));

	if (distance1 < 5)
	{
		grabbed[0] = true;

		triangle1_1.X = arm2_end.X - 10;
		triangle1_1.Y = arm2_end.Y + 10;

		triangle1_2.X = arm2_end.X + 10;
		triangle1_2.Y = arm2_end.Y + 10;

		triangle1_3.X = arm2_end.X;
		triangle1_3.Y = arm2_end.Y - 10;
	}
}

void drop()
{
	grabbed[0] = false;
}