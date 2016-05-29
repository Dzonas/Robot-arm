// Ramie_robota.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Ramie_robota.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

#define MAX_LOADSTRING 100
#define TMR_1 1

const REAL circle_size = 7;
const REAL arm1_length = 150;
const REAL arm2_length = 150;
const REAL arm_speed = 2;
const int N_TRIANGLE = 6;

HWND hwndButton;
RECT drawArea1 = { 0, 0, 310, 200 };

REAL arc1 = 30;
REAL arc2 = 30;
PointF arm1_start(0.0, (REAL)drawArea1.bottom);
PointF arm1_end(arm1_length, (REAL)drawArea1.bottom);
PointF arm2_start(arm1_end.X, arm1_end.Y);
PointF arm2_end((REAL)(arm1_length + arm2_length), (REAL)drawArea1.bottom);
PointF triangles[N_TRIANGLE][3];
bool grabbed[N_TRIANGLE] = { false, false, false, false, false, false };
bool update_pos = false;
int iter = 0;

std::vector<REAL> arm_pos[2][2];
std::vector<REAL> triangle_pos[N_TRIANGLE][2];

void initialize(void);
void paint(HDC);
void repaintWindow(HWND, HDC &, PAINTSTRUCT &, RECT *);
void arm1_moveUp(void);
void arm1_moveDown(void);
void arm2_moveUp(void);
void arm2_moveDown(void);
void grab(void);
void drop(void);
void update(void);
void drop_update(void);
void get_pos(void);

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
		250, 300, 80, 50,
		hWnd,
		(HMENU)ID_BUTTON1,
		hInstance,
		nullptr);

	hwndButton = CreateWindow(
		TEXT("button"),
		TEXT("Arm 1 up"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		340, 300, 80, 50,
		hWnd,
		(HMENU)ID_BUTTON2,
		hInstance,
		nullptr);

	hwndButton = CreateWindow(
		TEXT("button"),
		TEXT("Arm 1 down"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		420, 300, 80, 50,
		hWnd,
		(HMENU)ID_BUTTON3,
		hInstance,
		nullptr);

	hwndButton = CreateWindow(
		TEXT("button"),
		TEXT("Arm 2 up"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		510, 300, 80, 50,
		hWnd,
		(HMENU)ID_BUTTON4,
		hInstance,
		nullptr);

	hwndButton = CreateWindow(
		TEXT("button"),
		TEXT("Arm 2 down"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		590, 300, 80, 50,
		hWnd,
		(HMENU)ID_BUTTON5,
		hInstance,
		nullptr);

	hwndButton = CreateWindow(
		TEXT("button"),
		TEXT("Grab"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		250, 360, 80, 50,
		hWnd,
		(HMENU)ID_BUTTON6,
		hInstance,
		nullptr);

	hwndButton = CreateWindow(
		TEXT("button"),
		TEXT("Drop"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		330, 360, 80, 50,
		hWnd,
		(HMENU)ID_BUTTON7,
		hInstance,
		nullptr);

	hwndButton = CreateWindow(
		TEXT("button"),
		TEXT("Start rec"),
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		420, 360, 80, 50,
		hWnd,
		(HMENU)ID_BUTTON8,
		hInstance,
		nullptr);

	hwndButton = CreateWindow(
		TEXT("button"),
		TEXT("Stop rec"),
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		500, 360, 80, 50,
		hWnd,
		(HMENU)ID_BUTTON9,
		hInstance,
		nullptr);

	hwndButton = CreateWindow(
		TEXT("button"),
		TEXT("Replay"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		590, 360, 80, 50,
		hWnd,
		(HMENU)ID_BUTTON10,
		hInstance,
		nullptr);

	hwndButton = CreateWindow(
		TEXT("button"),
		TEXT("Reset"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		670, 360, 80, 50,
		hWnd,
		(HMENU)ID_BUTTON11,
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
		case ID_BUTTON8: // Start rec
			update_pos = true;
			update();
			break;
		case ID_BUTTON9: // Stop rec
			update();
			update_pos = false;
			break;
		case ID_BUTTON10: // Replay
			SetTimer(hWnd, TMR_1, 50, 0);
			break;
		case ID_BUTTON11: // Reset
			drop_update();
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
	case WM_TIMER:
		switch (wParam)
		{
		case TMR_1:
			if (arm_pos[0][0].size() > 0 && !update_pos) // If vector isn't empty and Start rec in unticked
			{
				if (iter == arm_pos[0][0].size() - 1) // If reached end of replay vector
				{
					KillTimer(hWnd, TMR_1);
					iter = 0; // Move to start of replay
					break;
				}
				get_pos();
				repaintWindow(hWnd, hdc, ps, &drawArea1);
			}
			else
				KillTimer(hWnd, TMR_1);
			break;
		}
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

void initialize()
{
	// Initialize arms coordinates
	arm1_end.Y = (REAL)(drawArea1.bottom - arm1_length * sin(arc1 * (M_PI / 180)));
	arm1_end.X = (REAL)(arm1_length * abs(cos(arc1 * (M_PI / 180))));

	arm2_start = arm1_end;

	arm2_end.Y = (REAL)(arm2_start.Y - arm2_length * sin(arc2 * (M_PI / 180)));
	arm2_end.X = (REAL)(arm2_length * cos(arc2 * (M_PI / 180)) + arm2_start.X);

	//Initialize triangles coordinates
	triangles[0][0] = PointF((REAL)200, (REAL)(drawArea1.bottom));
	triangles[0][1] = PointF((REAL)220, (REAL)(drawArea1.bottom));
	triangles[0][2] = PointF((REAL)210, (REAL)(drawArea1.bottom - 20));

	triangles[1][0] = PointF((REAL)221, (REAL)(drawArea1.bottom));
	triangles[1][1] = PointF((REAL)241, (REAL)(drawArea1.bottom));
	triangles[1][2] = PointF((REAL)231, (REAL)(drawArea1.bottom - 20));

	triangles[2][0] = PointF((REAL)242, (REAL)(drawArea1.bottom));
	triangles[2][1] = PointF((REAL)262, (REAL)(drawArea1.bottom));
	triangles[2][2] = PointF((REAL)252, (REAL)(drawArea1.bottom - 20));

	triangles[3][0] = PointF((REAL)200, (REAL)(drawArea1.bottom - 21));
	triangles[3][1] = PointF((REAL)220, (REAL)(drawArea1.bottom - 21));
	triangles[3][2] = PointF((REAL)210, (REAL)(drawArea1.bottom - 41));

	triangles[4][0] = PointF((REAL)221, (REAL)(drawArea1.bottom - 21));
	triangles[4][1] = PointF((REAL)241, (REAL)(drawArea1.bottom - 21));
	triangles[4][2] = PointF((REAL)231, (REAL)(drawArea1.bottom - 41));

	triangles[5][0] = PointF((REAL)242, (REAL)(drawArea1.bottom - 21));
	triangles[5][1] = PointF((REAL)262, (REAL)(drawArea1.bottom - 21));
	triangles[5][2] = PointF((REAL)252, (REAL)(drawArea1.bottom - 41));
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

	// Draw arms
	graphics.DrawLine(&pen1, arm1_start, arm1_end);
	graphics.DrawLine(&pen1, arm2_start, arm2_end);
	graphics.DrawEllipse(&pen2, (REAL)(arm1_start.X - arm1_length), (REAL)(arm1_start.Y - arm1_length), (REAL)arm1_length * 2, (REAL)arm1_length * 2);
	graphics.DrawEllipse(&pen3, (REAL)(arm2_start.X - arm2_length), (REAL)(arm2_start.Y - arm2_length), (REAL)arm2_length * 2, (REAL)arm2_length * 2);
	graphics.FillEllipse(&brushRed, arm1_end.X - circle_size / 2, arm1_end.Y - circle_size / 2, circle_size, circle_size);
	graphics.FillEllipse(&brushRed, arm2_end.X - circle_size / 2, arm2_end.Y - circle_size / 2, circle_size, circle_size);

	// Draw triangles
	for(int i = 0; i < N_TRIANGLE; i++)
		graphics.FillPolygon(&brushBlack, triangles[i], 3);
}

void arm1_moveUp()
{
	if (arc1 < 90 && arm2_end.X > drawArea1.left && arm1_end.Y > drawArea1.top && arm1_end.X < drawArea1.right && arm2_end.Y > drawArea1.top && arm2_end.X < drawArea1.right)
	{
		arc1 += arm_speed;

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
		arc1 -= arm_speed;

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
		arc2 += arm_speed;

		arm2_end.Y = (REAL)(arm2_start.Y - arm2_length * sin(arc2 * (M_PI / 180)));
		arm2_end.X = (REAL)(arm2_length * cos(arc2 * (M_PI / 180)) + arm2_start.X);

		for (int i = 0; i < N_TRIANGLE; i++)
			if (grabbed[i])
			{
				triangles[i][0].X = arm2_end.X - 10;
				triangles[i][0].Y = arm2_end.Y + 10;

				triangles[i][1].X = arm2_end.X + 10;
				triangles[i][1].Y = arm2_end.Y + 10;

				triangles[i][2].X = arm2_end.X;
				triangles[i][2].Y = arm2_end.Y - 10;

				break;
			}
	}

	if (update_pos)
		update();
}

void arm2_moveDown()
{
	if ((arm2_end.Y < drawArea1.bottom || (arm2_end.Y == drawArea1.bottom && arc2 == 180)) && arm2_end.X < drawArea1.right)
	{
		arc2 -= arm_speed;

		arm2_end.Y = (REAL)(arm2_start.Y - arm2_length * sin(arc2 * (M_PI / 180)));
		arm2_end.X = (REAL)(arm2_length * cos(arc2 * (M_PI / 180)) + arm2_start.X);

		for (int i = 0; i < N_TRIANGLE; i++)
			if (grabbed[i])
			{
				triangles[i][0].X = arm2_end.X - 10;
				triangles[i][0].Y = arm2_end.Y + 10;

				triangles[i][1].X = arm2_end.X + 10;
				triangles[i][1].Y = arm2_end.Y + 10;

				triangles[i][2].X = arm2_end.X;
				triangles[i][2].Y = arm2_end.Y - 10;

				break;
			}
	}

	if (update_pos)
		update();
}

void grab()
{
	for (int i = 0; i < N_TRIANGLE; i++) // If any of the triangles is already grabbed
		if (grabbed[i])
			return;

	for (int i = 0; i < N_TRIANGLE; i++)
	{
		REAL distance = sqrt((arm2_end.X - triangles[i][2].X) * (arm2_end.X - triangles[i][2].X) + (arm2_end.Y - (triangles[i][2].Y + 10)) * (arm2_end.Y - (triangles[i][2].Y + 10)));

		if (distance < 5)
		{
			grabbed[i] = true;

			triangles[i][0].X = arm2_end.X - 10;
			triangles[i][0].Y = arm2_end.Y + 10;

			triangles[i][1].X = arm2_end.X + 10;
			triangles[i][1].Y = arm2_end.Y + 10;

			triangles[i][2].X = arm2_end.X;
			triangles[i][2].Y = arm2_end.Y - 10;

			break;
		}
	}

	if (update_pos)
		update();
}

void drop()
{
	for (int i = 0; i < N_TRIANGLE; i++)
		if (grabbed[i])
		{
			grabbed[i] = false;
			break;
		}
}

void update()
{
	arm_pos[0][0].push_back(arm1_end.X);
	arm_pos[0][1].push_back(arm1_end.Y);
	arm_pos[1][0].push_back(arm2_end.X);
	arm_pos[1][1].push_back(arm2_end.Y);

	for (int i = 0; i < N_TRIANGLE; i++)
	{
		triangle_pos[i][0].push_back(triangles[i][0].X + 10);
		triangle_pos[i][1].push_back(triangles[i][0].Y - 10);
	}
}

void drop_update()
{

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			while (arm_pos[i][j].size() > 0)
				arm_pos[i][j].pop_back();

	for (int i = 0; i < N_TRIANGLE; i++)
	{
		while (triangle_pos[i][0].size() > 0)
			triangle_pos[i][0].pop_back();

		while (triangle_pos[i][1].size() > 0)
			triangle_pos[i][1].pop_back();
	}
}

void get_pos()
{
	arm1_end.Y = arm_pos[0][1][iter];
	arm1_end.X = arm_pos[0][0][iter];
	arm2_start = arm1_end;
	arm2_end.Y = arm_pos[1][1][iter];
	arm2_end.X = arm_pos[1][0][iter];

	for (int i = 0; i < N_TRIANGLE; i++)
	{
		triangles[i][0].X = triangle_pos[i][0][iter] - 10;
		triangles[i][0].Y = triangle_pos[i][1][iter] + 10;

		triangles[i][1].X = triangle_pos[i][0][iter] + 10;
		triangles[i][1].Y = triangle_pos[i][1][iter] + 10;

		triangles[i][2].X = triangle_pos[i][0][iter];
		triangles[i][2].Y = triangle_pos[i][1][iter] - 10;
	}

	iter++; // Move to the next new position in replay vector
}