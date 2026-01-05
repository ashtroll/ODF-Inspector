#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <string>
#include <sstream>
#include <iostream>
#include <functional>
#include "../include/ODFInspector.h"

#pragma comment(lib, "comctl32.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// Window controls
HWND hMainWindow;
HWND hTextBox;
HWND hStatusBar;
HWND hTabControl;
HWND hOpenButton;

// Tab pages
enum TabPages { TAB_SUMMARY, TAB_STRUCTURE, TAB_METADATA, TAB_CONTENT, TAB_STYLES };

std::string currentFilePath;
std::unique_ptr<ODFInspector> inspector;

// Capture output to string
std::string captureInspectorOutput(std::function<void()> func) {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    func();
    std::cout.rdbuf(old);
    return buffer.str();
}

void UpdateDisplay(int tabIndex) {
    if (!inspector || !inspector->isValidODF()) {
        SetWindowTextA(hTextBox, "No valid ODF file loaded");
        return;
    }

    std::string output;
    
    switch (tabIndex) {
        case TAB_SUMMARY:
            output = captureInspectorOutput([&]() { inspector->displaySummary(); });
            break;
        case TAB_STRUCTURE:
            output = captureInspectorOutput([&]() { inspector->displayStructure(); });
            break;
        case TAB_METADATA:
            output = captureInspectorOutput([&]() { inspector->displayMetadata(); });
            break;
        case TAB_CONTENT:
            output = captureInspectorOutput([&]() { inspector->displayContent(); });
            break;
        case TAB_STYLES:
            output = captureInspectorOutput([&]() { inspector->displayStyles(); });
            break;
    }
    
    SetWindowTextA(hTextBox, output.c_str());
}

void OpenODFFile() {
    OPENFILENAMEA ofn;
    char szFile[260] = {0};
    
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hMainWindow;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "ODF Files\0*.ODT;*.ODS;*.ODP;*.ODG\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    
    if (GetOpenFileNameA(&ofn)) {
        currentFilePath = szFile;
        inspector = std::make_unique<ODFInspector>(currentFilePath);
        
        if (inspector->load()) {
            std::string statusText = "Loaded: " + currentFilePath;
            SetWindowTextA(hStatusBar, statusText.c_str());
            
            int currentTab = TabCtrl_GetCurSel(hTabControl);
            UpdateDisplay(currentTab);
        } else {
            std::string error = "Error loading file: " + inspector->getLastError();
            MessageBoxA(hMainWindow, error.c_str(), "Error", MB_OK | MB_ICONERROR);
        }
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            // Create Open button
            hOpenButton = CreateWindowA("BUTTON", "Open ODF File",
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                10, 10, 120, 30,
                hwnd, (HMENU)1, NULL, NULL);
            
            // Create tab control
            INITCOMMONCONTROLSEX icex;
            icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
            icex.dwICC = ICC_TAB_CLASSES;
            InitCommonControlsEx(&icex);
            
            hTabControl = CreateWindowA(WC_TABCONTROLA, "",
                WS_VISIBLE | WS_CHILD | TCS_TABS,
                10, 50, 960, 30,
                hwnd, (HMENU)2, NULL, NULL);
            
            // Add tabs
            TCITEMA tie;
            tie.mask = TCIF_TEXT;
            
            tie.pszText = (LPSTR)"Summary";
            TabCtrl_InsertItem(hTabControl, TAB_SUMMARY, &tie);
            
            tie.pszText = (LPSTR)"Structure";
            TabCtrl_InsertItem(hTabControl, TAB_STRUCTURE, &tie);
            
            tie.pszText = (LPSTR)"Metadata";
            TabCtrl_InsertItem(hTabControl, TAB_METADATA, &tie);
            
            tie.pszText = (LPSTR)"Content";
            TabCtrl_InsertItem(hTabControl, TAB_CONTENT, &tie);
            
            tie.pszText = (LPSTR)"Styles";
            TabCtrl_InsertItem(hTabControl, TAB_STYLES, &tie);
            
            // Create text box for output
            hTextBox = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "",
                WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
                10, 90, 960, 480,
                hwnd, (HMENU)3, NULL, NULL);
            
            // Set monospace font
            HFONT hFont = CreateFontA(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, FIXED_PITCH | FF_MODERN, "Consolas");
            SendMessage(hTextBox, WM_SETFONT, (WPARAM)hFont, TRUE);
            
            // Create status bar
            hStatusBar = CreateWindowExA(0, STATUSCLASSNAMEA, "",
                WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
                0, 0, 0, 0,
                hwnd, (HMENU)4, NULL, NULL);
            
            SetWindowTextA(hStatusBar, "Ready - Open an ODF file to inspect");
            
            return 0;
        }
        
        case WM_COMMAND:
            if (LOWORD(wParam) == 1) { // Open button
                OpenODFFile();
            }
            return 0;
        
        case WM_NOTIFY: {
            if (((LPNMHDR)lParam)->idFrom == 2 && ((LPNMHDR)lParam)->code == TCN_SELCHANGE) {
                int tabIndex = TabCtrl_GetCurSel(hTabControl);
                UpdateDisplay(tabIndex);
            }
            return 0;
        }
        
        case WM_SIZE: {
            // Resize status bar
            SendMessage(hStatusBar, WM_SIZE, 0, 0);
            
            // Get client area
            RECT rcClient;
            GetClientRect(hwnd, &rcClient);
            
            // Get status bar height
            RECT rcStatus;
            GetWindowRect(hStatusBar, &rcStatus);
            int statusHeight = rcStatus.bottom - rcStatus.top;
            
            // Resize controls
            SetWindowPos(hTabControl, NULL, 10, 50, rcClient.right - 20, 30, SWP_NOZORDER);
            SetWindowPos(hTextBox, NULL, 10, 90, rcClient.right - 20, 
                         rcClient.bottom - 100 - statusHeight, SWP_NOZORDER);
            
            return 0;
        }
        
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "ODFInspectorWindow";
    
    WNDCLASSA wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    
    RegisterClassA(&wc);
    
    hMainWindow = CreateWindowExA(
        0,
        CLASS_NAME,
        "ODF Inspector",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1000, 650,
        NULL,
        NULL,
        hInstance,
        NULL
    );
    
    if (hMainWindow == NULL) {
        return 0;
    }
    
    ShowWindow(hMainWindow, nCmdShow);
    
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return 0;
}
