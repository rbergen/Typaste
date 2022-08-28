// AutoType
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>
// This file is public domain software.
#include "Typaste.hpp"

const UINT cMaxKL = 10;

inline void ChangeModifier(WORD modifier, bool keyUp, DWORD delay, LPCWSTR sound) 
{
    if (sound && *sound)
    {
        PlaySoundW(sound, NULL, SND_ASYNC | SND_FILENAME | SND_NODEFAULT);
    }
    Sleep(delay);
    MyKeybdEvent(modifier, 0, keyUp ? KEYEVENTF_KEYUP : 0, 0);
    Sleep(delay);
}

inline void PressAndRelease(WORD key, DWORD pressDelay, DWORD postDelay, LPCWSTR sound)
{
    if (sound && *sound)
    {
        PlaySoundW(sound, NULL, SND_ASYNC | SND_FILENAME | SND_NODEFAULT);
    }
    Sleep(pressDelay);
    MyKeybdEvent(key, 0, 0, 0);
    Sleep(pressDelay);
    MyKeybdEvent(key, 0, KEYEVENTF_KEYUP, 0);
    Sleep(postDelay);
}

inline void SetKeyboardLayout(HKL layout, DWORD delay)
{
    HKL hkl;

    for (INT k = 0; k < cMaxKL; ++k)
    {
        hkl = GetKeyboardLayout(0);
        if (hkl == layout)
            break;

        Sleep(delay);
        MyKeybdEvent(VK_LSHIFT, 0, 0, 0);
        Sleep(delay);
        MyKeybdEvent(VK_LMENU, 0, 0, 0);
        Sleep(delay);
        MyKeybdEvent(VK_LMENU, 0, KEYEVENTF_KEYUP, 0);
        Sleep(delay);
        MyKeybdEvent(VK_LSHIFT, 0, KEYEVENTF_KEYUP, 0);
        Sleep(delay);
    }
}

void AutoType(const WCHAR *pszText, typaste_config &config)
{
    HKL ahkl[cMaxKL];
    UINT chkl = GetKeyboardLayoutList(cMaxKL, ahkl);
    HKL hklOld = GetKeyboardLayout(0);
    BYTE lastFlags = 0;
    LPCWSTR pszModifierDownSound = config.sound_config.modifier_down_key();
    LPCWSTR pszModifierUpSound = config.sound_config.modifier_up_key();

    for (; *pszText ;++pszText)
    {
        if (GetAsyncKeyState(VK_ESCAPE) < 0)
            break;

        LPCWSTR pszSound = NULL;

        switch (*pszText)
        {
        case L'\n':
            pszSound = config.sound_config.enter_key();
            break;
        case L' ':
            pszSound = config.sound_config.space_key();
            break;
        default:
            pszSound = config.sound_config.get_key();
            break;
        }

        switch (*pszText)
        {
        case L'\r':
            continue;
        case L'\n':
            PressAndRelease(VK_RETURN, config.modifier_delay, config.key_delay(), pszSound);
            continue;
        case L'\t':
            PressAndRelease(VK_TAB, config.modifier_delay, config.key_delay(), pszSound);
            continue;
        }

        SHORT s = -1;
        UINT i;
        for (i = 0; i < chkl; ++i)
        {
            s = VkKeyScanExW(*pszText, ahkl[i]);
            if (s != -1)
                break;
        }

        if (s != -1)
        {
            SetKeyboardLayout(ahkl[i], config.modifier_delay);
        }

        BYTE vk = LOBYTE(s);
        BYTE flags = HIBYTE(s);

        if (s == -1)
        {
            HWND hwnd = GetForegroundWindow();
            DWORD tid = GetWindowThreadProcessId(hwnd, NULL);
            GUITHREADINFO info = { sizeof(info) };
            GetGUIThreadInfo(tid, &info);
            DWORD_PTR dwResult;
            SendMessageTimeoutW(info.hwndFocus, WM_CHAR, *pszText, 0,
                                SMTO_ABORTIFHUNG, 2000, &dwResult);
            Sleep(config.modifier_delay);

            continue;
        }

        bool modifierWasSet = lastFlags & 1;
        if (((flags & 1) == 1) != modifierWasSet)
        {
            ChangeModifier(VK_LSHIFT, modifierWasSet, config.modifier_delay, modifierWasSet ? pszModifierUpSound : pszModifierDownSound);
        }

        modifierWasSet = lastFlags & 2;
        if (((flags & 2) == 2) != modifierWasSet)
        {
            ChangeModifier(VK_LCONTROL, modifierWasSet, config.modifier_delay, modifierWasSet ? pszModifierUpSound : pszModifierDownSound);
        }

        modifierWasSet = lastFlags & 4;
        if (((flags & 4) == 4) != modifierWasSet)
        {
            ChangeModifier(VK_LMENU, modifierWasSet, config.modifier_delay, modifierWasSet ? pszModifierUpSound : pszModifierDownSound);
        }

        lastFlags = flags;

        PressAndRelease(vk, config.modifier_delay, config.key_delay(), pszSound);
    }

    if (lastFlags & 1)
    {
        ChangeModifier(VK_LSHIFT, true, config.modifier_delay, pszModifierUpSound);
    }

    if (lastFlags & 2)
    {
        ChangeModifier(VK_LCONTROL, true, config.modifier_delay, pszModifierUpSound);
    }

    if (lastFlags & 4)
    {
        ChangeModifier(VK_LMENU, true, config.modifier_delay, pszModifierUpSound);
    }

    SetKeyboardLayout(hklOld, config.modifier_delay);
}