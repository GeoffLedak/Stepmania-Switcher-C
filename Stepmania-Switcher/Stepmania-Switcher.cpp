// Stepmania-Switcher.cpp : Defines the entry point for the application.
//

#include <windows.h>
#include <ShellAPI.h>

#pragma comment(lib, "winmm.lib")


#include "framework.h"
#include "Stepmania-Switcher.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name


HWND stepMania5Button;
HWND ddrExtremeButton;
HWND shutdownButton;

char stepMania5Selected = 0;
char ddrExtremeSelected = 1;
char shutdownSelected = 2;

char currentlySelected = stepMania5Selected;


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void clearSelected(HWND);
void selectSelected(HWND);

void selectStepMania5(HWND);
void selectDDRextreme(HWND);

void launchSelected(HWND);
void launchStepMania5(HWND);
void launchDDRextreme(HWND);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_STEPMANIASWITCHER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_STEPMANIASWITCHER));

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

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_STEPMANIASWITCHER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    // wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.hbrBackground = CreateSolidBrush(0xFF00FF);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_STEPMANIASWITCHER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {


    case WM_CREATE:
    {
        stepMania5Button = CreateWindowExA(
            HTMAXBUTTON,
            "BUTTON",  // Predefined class; Unicode assumed
            "StepMania 5",      // Button text
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
            270,         // x position
            230,         // y position
            340,        // Button width
            340,        // Button height
            hWnd,     // Parent window
            (HMENU) 1,
            NULL,
            NULL);      // Pointer not needed.





        ddrExtremeButton = CreateWindowExA(
            HTMAXBUTTON,
            "BUTTON",  // Predefined class; Unicode assumed
            "DDR Extreme",      // Button text
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
            890,         // x position
            230,         // y position
            340,        // Button width
            340,        // Button height
            hWnd,     // Parent window
            (HMENU)2,
            NULL,
            NULL);      // Pointer not needed.





        shutdownButton = CreateWindowExA(
            HTMAXBUTTON,
            "BUTTON",  // Predefined class; Unicode assumed
            "Shut down",      // Button text
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
            1330,         // x position
            700,         // y position
            150,        // Button width
            120,        // Button height
            hWnd,     // Parent window
            (HMENU)3,
            NULL,
            NULL);      // Pointer not needed.






        if (joySetCapture(hWnd, JOYSTICKID1, NULL, FALSE))
        {
            MessageBeep(MB_ICONEXCLAMATION);
            MessageBox(hWnd, L"Couldn't capture the joystick.", NULL,
                MB_OK | MB_ICONEXCLAMATION);
            PostMessage(hWnd, WM_CLOSE, 0, 0L);
        }





        break;
    }



    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {


            case 1:
            {
                STARTUPINFOW si = { 0 };
                si.cb = sizeof(si);
                PROCESS_INFORMATION pi = { 0 };

                // Create the child process
                BOOL success = CreateProcessW(
                    L"C:\\Windows\\system32\\notepad.exe",  // Path to executable
                    NULL,                                   // Command line arguments
                    NULL,                                   // Process attributes
                    NULL,                                   // Thread attributes
                    FALSE,                                  // Inherit handles
                    0,                                      // Creation flags
                    NULL,                                   // Environment
                    NULL,                                   // Working directory
                    &si,                                    // Startup info
                    &pi);                                   // Process information

                if (success)
                {
                    // Wait for the process to exit
                    WaitForSingleObject(pi.hProcess, INFINITE);

                    // Process has exited - check its exit code
                    DWORD exitCode;
                    GetExitCodeProcess(pi.hProcess, &exitCode);

                    // At this point exitCode is set to the process' exit code

                    // Handles must be closed when they are no longer needed
                    CloseHandle(pi.hThread);
                    CloseHandle(pi.hProcess);
                }


                break;
            }
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






    case MM_JOY1BUTTONDOWN:


        if ((UINT)wParam & JOY_BUTTON1)
        {
            currentlySelected = stepMania5Selected;
            RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
            selectStepMania5(hWnd);
        }
        else if ((UINT)wParam & JOY_BUTTON2)
        {
            currentlySelected = ddrExtremeSelected;
            RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
            selectDDRextreme(hWnd);
        }
        else if ((UINT)wParam & JOY_BUTTON3)
        {


        }
        else if ((UINT)wParam & JOY_BUTTON4)
        {
            launchSelected(hWnd);
        }



        break;






    case WM_PAINT:
        {

            clearSelected(hWnd);
            selectSelected(hWnd);

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



void clearSelected(HWND hWnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);

    HBRUSH brush = CreateSolidBrush(RGB(255, 0, 255));


    // StepMania 5
    RECT rect = { 260, 220, 620, 580 };
    FillRect(hdc, &rect, brush);



    // DDR Extreme
    rect = { 880, 220, 1240, 580 };
    FillRect(hdc, &rect, brush);



    // Shut Down
    rect = { 1320, 690, 1490, 830 };
    FillRect(hdc, &rect, brush);



    EndPaint(hWnd, &ps);
    // RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}





void selectSelected(HWND hWnd) {

    if (currentlySelected == stepMania5Selected) {
        selectStepMania5(hWnd);
    }
    else if (currentlySelected == ddrExtremeSelected) {
        selectDDRextreme(hWnd);
    }


}




void selectStepMania5(HWND hWnd) {

    HDC hdc = GetDC(hWnd);

    RECT rect = { 260, 220, 620, 580 };
    HBRUSH brush = CreateSolidBrush(RGB(0, 255, 255));

    FillRect(hdc, &rect, brush);

    DeleteObject(brush);

    RedrawWindow(stepMania5Button, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

}




void selectDDRextreme(HWND hWnd) {

    HDC hdc = GetDC(hWnd);

    RECT rect = { 880, 220, 1240, 580 };
    HBRUSH brush = CreateSolidBrush(RGB(0, 255, 255));

    FillRect(hdc, &rect, brush);

    DeleteObject(brush);

    RedrawWindow(ddrExtremeButton, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

}



void launchSelected(HWND hWnd) {


    if (currentlySelected == stepMania5Selected) {
        launchStepMania5(hWnd);
    }
    else if (currentlySelected == ddrExtremeSelected) {
        launchDDRextreme(hWnd);
    }


}


void launchStepMania5(HWND hWnd) {

    STARTUPINFOW si = { 0 };
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi = { 0 };

    // Create the child process
    BOOL success = CreateProcessW(
        L"C:\\Games\\StepMania 5\\Program\\StepMania.exe",  // Path to executable
        NULL,                                   // Command line arguments
        NULL,                                   // Process attributes
        NULL,                                   // Thread attributes
        FALSE,                                  // Inherit handles
        0,                                      // Creation flags
        NULL,                                   // Environment
        NULL,                                   // Working directory
        &si,                                    // Startup info
        &pi);                                   // Process information

    if (success)
    {
        // Wait for the process to exit
        WaitForSingleObject(pi.hProcess, INFINITE);

        // Process has exited - check its exit code
        DWORD exitCode;
        GetExitCodeProcess(pi.hProcess, &exitCode);

        // At this point exitCode is set to the process' exit code

        // Handles must be closed when they are no longer needed
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);




        // RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);



    }

}


void launchDDRextreme(HWND hWnd) {

    STARTUPINFOW si = { 0 };
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi = { 0 };

    // Create the child process
    BOOL success = CreateProcessW(
        L"C:\\Program Files (x86)\\StepMania\\Program\\StepMania.exe",  // Path to executable
        NULL,                                   // Command line arguments
        NULL,                                   // Process attributes
        NULL,                                   // Thread attributes
        FALSE,                                  // Inherit handles
        0,                                      // Creation flags
        NULL,                                   // Environment
        NULL,                                   // Working directory
        &si,                                    // Startup info
        &pi);                                   // Process information

    if (success)
    {
        // Wait for the process to exit
        WaitForSingleObject(pi.hProcess, INFINITE);

        // Process has exited - check its exit code
        DWORD exitCode;
        GetExitCodeProcess(pi.hProcess, &exitCode);

        // At this point exitCode is set to the process' exit code

        // Handles must be closed when they are no longer needed
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);




        // RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);



    }





}
