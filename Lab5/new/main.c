#include <Windows.h>
#include <stdio.h>
#include <ctype.h>

HANDLE hClip, hNewClip;
char *buffer, *newBuffer;
BOOL isChange = FALSE;

BOOL IsBankCardNumber(char *str)
{
    int len = strlen(str);
    if (len < 12 || len > 19)
    {
        return 0;
    }
    for (int i = len - 1; i >= 0; i--)
    {
        if (!isdigit(str[i]))
        {
            return 0;
        }
    }
    return 1;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        SetClipboardViewer(hwnd);
        printf("Clipboard viewer added.\n");
        break;

    case WM_DESTROY:
        ChangeClipboardChain(hwnd, hwnd);
        PostQuitMessage(0);
        break;

    case WM_DRAWCLIPBOARD:
        printf("Clipboard changed.\n");
        if (!OpenClipboard(NULL))
        {
            printf("Unable to open clipboard.\n");
            break;
        }
        // If data is pure text
        if (!IsClipboardFormatAvailable(CF_TEXT))
        {
            CloseClipboard();
            Sleep(100);
            break;
        }
        hClip = GetClipboardData(CF_TEXT);
        if (!hClip)
        {
            printf("Unable to get clipboard data.\n");
            CloseClipboard();
            break;
        }
        buffer = (char *)GlobalLock(hClip);
        if (!buffer)
        {
            printf("Unable to lock clipboard.\n");
            CloseClipboard();
            break;
        }

        if (IsBankCardNumber(buffer))
        {
            strcpy(buffer, "Don't copy bank card number");
            hNewClip = GlobalAlloc(GMEM_MOVEABLE, strlen(buffer) + 1);
            newBuffer = (char *)GlobalLock(hNewClip);
            strcpy(newBuffer, buffer);
            GlobalUnlock(hNewClip);
            isChange = TRUE;
        }
        GlobalUnlock(hClip);
        if (isChange)
        {
            printf("%s\n", newBuffer);
            EmptyClipboard();
            SetClipboardData(CF_TEXT, hNewClip);
            GlobalFree(hClip);
            isChange = FALSE;
        }
        if (!CloseClipboard())
        {
            printf("Unable to close clipboard.\n");
            return 1;
        }
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int main()
{
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "Lab5";
    RegisterClass(&wc);

    // create a virtual window to receive messages
    HWND hwnd = CreateWindow("Lab5", "Clipboard Viewer", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, GetModuleHandle(NULL), NULL);
    printf("window created.\n");
    if (!hwnd)
    {
        printf("Unable to create window.\n");
        return 1;
    }

    MSG msg;
    while (1)
    {
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
        {
            break;
        }

        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return 0;
}
