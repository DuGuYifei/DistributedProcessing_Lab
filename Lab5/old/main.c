#include <Windows.h>
#include <stdio.h>
#include <ctype.h>

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

int main()
{
    HANDLE hClip, hNewClip;
    char *buffer, *newBuffer;
    BOOL isChange = FALSE;
    while (1)
    {
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
        {
            break;
        }
        if (!OpenClipboard(NULL))
        {
            printf("Unable to open clipboard.\n");
            return 1;
        }
        // If data is pure text
        if (!IsClipboardFormatAvailable(CF_TEXT))
        {
            Sleep(100);
            continue;
        }
        hClip = GetClipboardData(CF_TEXT);
        if (!hClip)
        {
            printf("Unable to get clipboard data.\n");
            CloseClipboard();
            return 1;
        }
        buffer = (char *)GlobalLock(hClip);
        if (!buffer)
        {
            printf("Unable to lock clipboard.\n");
            CloseClipboard();
            return 1;
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
        Sleep(100);
    }
    return 0;
}
