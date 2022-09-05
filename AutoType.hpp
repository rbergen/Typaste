inline void WaitModifierRelease(DWORD dwDelay)
{
    while (GetAsyncKeyState(VK_SHIFT) < 0 ||
           GetAsyncKeyState(VK_CONTROL) < 0 ||
           GetAsyncKeyState(VK_MENU) < 0)
    {
        Sleep(dwDelay);
    }
}

static inline void MyKeybdEvent(WORD wVk, WORD wScan, DWORD dwFlags, ULONG_PTR dwExtra)
{
    INPUT input;
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = wVk;
    input.ki.wScan = wScan;
    input.ki.dwFlags = dwFlags;
    input.ki.time = 0;
    input.ki.dwExtraInfo = dwExtra;
    SendInput(1, &input, sizeof(input));
}

inline void CtrlV(DWORD dwDelay)
{
    MyKeybdEvent(VK_LCONTROL, 0, 0, 0);
    Sleep(dwDelay);

    MyKeybdEvent(L'V', 0, 0, 0);
    Sleep(dwDelay);

    MyKeybdEvent(L'V', 0, KEYEVENTF_KEYUP, 0);
    Sleep(dwDelay);

    MyKeybdEvent(VK_LCONTROL, 0, KEYEVENTF_KEYUP, 0);
    Sleep(dwDelay);
}

UINT StartAutoType(LPCWSTR pszText, typaste_config &config);
UINT AutoTypeChar();
void EndAutoType();

#define NEXT_CHAR   return state.next_character() ? WM_PASTECHAR : WM_ENDPASTE
