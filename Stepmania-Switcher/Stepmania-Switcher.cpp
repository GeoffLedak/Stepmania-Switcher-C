// Stepmania-Switcher.cpp : Defines the entry point for the application.
//

#include <windows.h>
#include <stdio.h>
#include <ShellAPI.h>

#pragma comment(lib, "winmm.lib")

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "advapi32.lib")


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
HWND exitButton;

HWND configButton;

HWND theMainWindow;

char stepMania5Selected = 0;
char ddrExtremeSelected = 1;
char shutdownSelected = 2;
char exitSelected = 3;

char currentlySelected = stepMania5Selected;

unsigned int joyPosition = 0;
unsigned int buttonState = 0;

unsigned int leftButton = 1;
unsigned int rightButton = 2;
unsigned int selectButton = 8;
unsigned int backButton = 4;

char enableConfigMode = 0;

char hasLoadedConfig = 0;

char* stepManiaPath = new char[256];
char* ddrExtremePath = new char[256];


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void clearSelected(HWND);
void selectSelected(HWND);

void selectStepMania5(HWND);
void selectDDRextreme(HWND);
void selectShutdown(HWND);
void selectExit(HWND);

void launchSelected(HWND);
void launchStepMania5(HWND);
void launchDDRextreme(HWND);
int shutdownDaComputah(HWND);
void exitDaProgram(HWND);

void handleJoystickInput();
void startConfig();
void loadConfig();


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

        if (!hasLoadedConfig)
            loadConfig();

        if (enableConfigMode)
            startConfig();
        else
            handleJoystickInput();





    }

    return (int) msg.wParam;
}




void startConfig() {



    int i = 0;

    unsigned int configButtonState = 0;

    char hasDoneAllButtons = 0;

    char hasDoneLeft = 0;
    char hasDoneRight = 0;
    char hasDoneSelect = 0;
    char hasDoneBack = 0;

    JOYINFO joyinfo;

    ShowWindow(configButton, SW_SHOW);
    RedrawWindow(configButton, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
    SendMessage(configButton, WM_SETTEXT, 0, (LPARAM)_T("Press Left"));


    while (!hasDoneAllButtons) {


        // just checking 4 gamepads for now
        for (i = 0; i < 4; i++) {

            joyGetPos(i, &joyinfo);

            if (!hasDoneLeft && (joyinfo.wButtons != configButtonState) && joyinfo.wButtons != 0) {
                joyPosition = i;    // expect all of the following buttons to come from the same joystick
                leftButton = joyinfo.wButtons;
                configButtonState = joyinfo.wButtons;
                hasDoneLeft = 1;
                SendMessage(configButton, WM_SETTEXT, 0, (LPARAM)_T("Press Right"));
            }

            else if (!hasDoneRight && (joyinfo.wButtons != configButtonState) && joyinfo.wButtons != 0) {
                rightButton = joyinfo.wButtons;
                configButtonState = joyinfo.wButtons;
                hasDoneRight = 1;
                SendMessage(configButton, WM_SETTEXT, 0, (LPARAM)_T("Press Select"));
            }

            else if (!hasDoneSelect && (joyinfo.wButtons != configButtonState) && joyinfo.wButtons != 0) {
                selectButton = joyinfo.wButtons;
                configButtonState = joyinfo.wButtons;
                hasDoneSelect = 1;
                SendMessage(configButton, WM_SETTEXT, 0, (LPARAM)_T("Press Back"));
            }

            else if (!hasDoneBack && (joyinfo.wButtons != configButtonState) && joyinfo.wButtons != 0) {
                backButton = joyinfo.wButtons;
                configButtonState = joyinfo.wButtons;
                hasDoneBack = 1;
                SendMessage(configButton, WM_SETTEXT, 0, (LPARAM)_T("DONE"));
            }

        }


        if (hasDoneLeft && hasDoneRight && hasDoneSelect && hasDoneBack) {
            enableConfigMode = 0;
            hasDoneAllButtons = 1;
            ShowWindow(configButton, SW_HIDE);



            // ===== Save Config =====

            char buffer[MAX_PATH];
            char* poo;

            GetModuleFileNameA(NULL, buffer, MAX_PATH);

            int length = strlen(buffer);
            poo = buffer;
            poo = poo + length;

            while (length >= 0) {

                if (*poo == '\\') {
                    poo++;
                    *poo = '\0';
                    break;
                }

                length--;
                poo--;
            }

            strcat_s(buffer, "config.ini");

            char paramAsString[128];

            // joyPosition
            sprintf_s(paramAsString, "%d", joyPosition);
            WritePrivateProfileStringA("Stepmania-Switcher", "joyPosition", paramAsString, buffer);

            // leftButton
            sprintf_s(paramAsString, "%d", leftButton);
            WritePrivateProfileStringA("Stepmania-Switcher", "leftButton", paramAsString, buffer);

            // rightButton
            sprintf_s(paramAsString, "%d", rightButton);
            WritePrivateProfileStringA("Stepmania-Switcher", "rightButton", paramAsString, buffer);

            // selectButton
            sprintf_s(paramAsString, "%d", selectButton);
            WritePrivateProfileStringA("Stepmania-Switcher", "selectButton", paramAsString, buffer);

            // backButton
            sprintf_s(paramAsString, "%d", backButton);
            WritePrivateProfileStringA("Stepmania-Switcher", "backButton", paramAsString, buffer);


            // ======================

        }
    }


}




void loadConfig() {

    char buffer[MAX_PATH];
    char* poo;

    GetModuleFileNameA(NULL, buffer, MAX_PATH);

    int length = strlen(buffer);
    poo = buffer;
    poo = poo + length;

    while (length >= 0) {

        if (*poo == '\\') {
            poo++;
            *poo = '\0';
            break;
        }

        length--;
        poo--;
    }

    strcat_s(buffer, "config.ini");


    joyPosition = GetPrivateProfileIntA("Stepmania-Switcher", "joyPosition", 9999, buffer);
    leftButton = GetPrivateProfileIntA("Stepmania-Switcher", "leftButton", 9998, buffer);
    rightButton = GetPrivateProfileIntA("Stepmania-Switcher", "rightButton", 9997, buffer);
    selectButton = GetPrivateProfileIntA("Stepmania-Switcher", "selectButton", 9996, buffer);
    backButton = GetPrivateProfileIntA("Stepmania-Switcher", "backButton", 9995, buffer);


    GetPrivateProfileStringA("Stepmania-Switcher", "stepManiaPath", "poop", stepManiaPath, 256, buffer);
    GetPrivateProfileStringA("Stepmania-Switcher", "ddrExtremePath", "poop", ddrExtremePath, 256, buffer);


    hasLoadedConfig = 1;
}





void handleJoystickInput() {

    JOYINFO joyinfo;
    joyGetPos(joyPosition, &joyinfo);


    if (joyinfo.wButtons == buttonState)
        return;


    buttonState = joyinfo.wButtons;


    if (buttonState == leftButton)
    {
        currentlySelected = stepMania5Selected;
        RedrawWindow(theMainWindow, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
        selectStepMania5(theMainWindow);
    }
    else if (buttonState == rightButton)
    {
        currentlySelected = ddrExtremeSelected;
        RedrawWindow(theMainWindow, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
        selectDDRextreme(theMainWindow);
    }



    // Exit
    else if (buttonState == (backButton + leftButton))
    {
        currentlySelected = exitSelected;
        RedrawWindow(theMainWindow, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
        selectExit(theMainWindow);
    }




    // Shut down
    else if (buttonState == (backButton + rightButton))
    {
        currentlySelected = shutdownSelected;
        RedrawWindow(theMainWindow, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
        selectShutdown(theMainWindow);
    }




    else if (buttonState == selectButton)
    {
        launchSelected(theMainWindow);
    }



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
    wcex.hbrBackground = CreateSolidBrush(0x000000);
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

   // WS_OVERLAPPEDWINDOW

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   theMainWindow = hWnd;

   // SetWindowLong(hWnd, GWL_STYLE, WS_BORDER);  // With 1 point border
   // SetWindowLong(hWnd, GWL_STYLE, 0);

   // 2560 x 1600

   // SetWindowPos(hWnd, 0, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 0);

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

        HBITMAP stepmaniaImage = (HBITMAP)LoadImage(NULL, L"stepmania-logo-black-small.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

        stepMania5Button = CreateWindowEx(
            NULL,
            L"STATIC",
            NULL,
            SS_BITMAP | WS_VISIBLE | WS_CHILD,
            270,    // x position
            230,    // y position
            340,    // Button width
            340,    // Button height
            hWnd,
            (HMENU)1,
            GetModuleHandle(NULL),
            NULL);

        SendMessage(stepMania5Button, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)stepmaniaImage);




        HBITMAP ddrExtremeImage = (HBITMAP)LoadImage(NULL, L"ddr-extreme-logo-small.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

        ddrExtremeButton = CreateWindowEx(
            NULL,
            L"STATIC",
            NULL,
            SS_BITMAP | WS_VISIBLE | WS_CHILD,
            890,    // x position
            230,    // y position
            340,    // Button width
            340,    // Button height
            hWnd,
            (HMENU)2,
            GetModuleHandle(NULL),
            NULL);

        SendMessage(ddrExtremeButton, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)ddrExtremeImage);




        HBITMAP shutdownImage = (HBITMAP)LoadImage(NULL, L"shutdown-icon-smaller.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

        shutdownButton = CreateWindowEx(
            NULL,
            L"STATIC",
            NULL,
            SS_BITMAP | WS_VISIBLE | WS_CHILD,
            1330,    // x position
            718,    // y position
            150,    // Button width
            120,    // Button height
            hWnd,
            (HMENU)3,
            GetModuleHandle(NULL),
            NULL);

        SendMessage(shutdownButton, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)shutdownImage);






        HBITMAP exitImage = (HBITMAP)LoadImage(NULL, L"exit-icon-smaller.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

        exitButton = CreateWindowEx(
            NULL,
            L"STATIC",
            NULL,
            SS_BITMAP | WS_VISIBLE | WS_CHILD,
            20,    // x position
            718,    // y position
            150,    // Button width
            120,    // Button height
            hWnd,
            (HMENU)3,
            GetModuleHandle(NULL),
            NULL);

        SendMessage(exitButton, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)exitImage);




        configButton = CreateWindowExA(
            HTMAXBUTTON,
            "BUTTON",  // Predefined class; Unicode assumed
            "Pizza",      // Button text
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
            10,         // x position
            10,         // y position
            100,        // Button width
            100,        // Button height
            hWnd,     // Parent window
            (HMENU)1,
            NULL,
            NULL);      // Pointer not needed.


        ShowWindow(configButton, SW_HIDE);



        SetMenu(hWnd, NULL);

        ShowCursor(false);



        if (joySetCapture(hWnd, JOYSTICKID1, NULL, FALSE))
        {
            MessageBeep(MB_ICONEXCLAMATION);
            MessageBox(hWnd, L"Couldn't capture the joystick.", NULL,
                MB_OK | MB_ICONEXCLAMATION);
            PostMessage(hWnd, WM_CLOSE, 0, 0L);
        }


        break;
    }













    case WM_KEYUP:

        if (wParam == 0x43) {
            enableConfigMode = 1;
        }

        break;















    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
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

    HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));


    // StepMania 5
    RECT rect = { 260, 220, 620, 580 };
    FillRect(hdc, &rect, brush);



    // DDR Extreme
    rect = { 880, 220, 1240, 580 };
    FillRect(hdc, &rect, brush);



    // Shut Down
    rect = { 1320, 708, 1490, 848 };
    FillRect(hdc, &rect, brush);


    // Exit
    rect = { 10, 708, 180, 848 };
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
    else if (currentlySelected == shutdownSelected) {
        selectShutdown(hWnd);
    }
    else if (currentlySelected == exitSelected) {
        selectExit(hWnd);
    }

}




void selectStepMania5(HWND hWnd) {

    HDC hdc = GetDC(hWnd);

    RECT rect = { 260, 220, 620, 580 };
    HBRUSH brush = CreateSolidBrush(RGB(0, 255, 0));

    FillRect(hdc, &rect, brush);

    DeleteObject(brush);

    RedrawWindow(stepMania5Button, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}




void selectDDRextreme(HWND hWnd) {

    HDC hdc = GetDC(hWnd);

    RECT rect = { 880, 220, 1240, 580 };
    HBRUSH brush = CreateSolidBrush(RGB(0, 255, 0));

    FillRect(hdc, &rect, brush);

    DeleteObject(brush);

    RedrawWindow(ddrExtremeButton, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

}




void selectShutdown(HWND hWnd) {

    HDC hdc = GetDC(hWnd);

    RECT rect = { 1320, 708, 1490, 848 };
    HBRUSH brush = CreateSolidBrush(RGB(0, 255, 0));

    FillRect(hdc, &rect, brush);

    DeleteObject(brush);

    RedrawWindow(shutdownButton, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}



void selectExit(HWND hWnd) {

    HDC hdc = GetDC(hWnd);

    RECT rect = { 10, 708, 180, 848 };
    HBRUSH brush = CreateSolidBrush(RGB(0, 255, 0));

    FillRect(hdc, &rect, brush);

    DeleteObject(brush);

    RedrawWindow(exitButton, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}




void launchSelected(HWND hWnd) {


    if (currentlySelected == stepMania5Selected) {
        launchStepMania5(hWnd);
    }
    else if (currentlySelected == ddrExtremeSelected) {
        launchDDRextreme(hWnd);
    }
    else if (currentlySelected == shutdownSelected) {
        shutdownDaComputah(hWnd);
    }
    else if (currentlySelected == exitSelected) {
        exitDaProgram(hWnd);
    }


}


void launchStepMania5(HWND hWnd) {

    STARTUPINFOW si = { 0 };
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi = { 0 };


    wchar_t* wString = new wchar_t[256];
    MultiByteToWideChar(CP_ACP, 0, stepManiaPath, -1, wString, 256);


    // Create the child process
    BOOL success = CreateProcessW(
        wString,  // Path to executable
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


    wchar_t* wString = new wchar_t[256];
    MultiByteToWideChar(CP_ACP, 0, ddrExtremePath, -1, wString, 256);


    // Create the child process
    BOOL success = CreateProcessW(
        wString,  // Path to executable
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



int shutdownDaComputah(HWND hWnd) {
    // DestroyWindow(hWnd);
    // InitiateSystemShutdownA(NULL, NULL, 0, true, false);

    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;

    // Get a token for this process. 

    if (!OpenProcessToken(GetCurrentProcess(),
        TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
        return(FALSE);

    // Get the LUID for the shutdown privilege. 

    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,
        &tkp.Privileges[0].Luid);

    tkp.PrivilegeCount = 1;  // one privilege to set    
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    // Get the shutdown privilege for this process. 

    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
        (PTOKEN_PRIVILEGES)NULL, 0);

    if (GetLastError() != ERROR_SUCCESS)
        return FALSE;

    // Shut down the system and force all applications to close. 

/*
    if (!ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE,
        SHTDN_REASON_MAJOR_OPERATINGSYSTEM |
        SHTDN_REASON_MINOR_UPGRADE |
        SHTDN_REASON_FLAG_PLANNED))
        return FALSE;
*/

    if (!InitiateSystemShutdownA(NULL, NULL, 0, true, false)) {
        return FALSE;
    }


    //shutdown was successful
    return TRUE;

}


void exitDaProgram(HWND hWnd) {
    DestroyWindow(hWnd);
}
