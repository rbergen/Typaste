// AutoType
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>
// This file is public domain software.
#include "Typaste.hpp"

static paste_state state;

const UINT cMaxKL = 10;

inline void ChangeModifier(WORD modifier, bool keyUp, DWORD delay, LPBYTE sound) 
{
    if (sound)
    {
        PlaySound((LPCWSTR)sound, NULL, SND_ASYNC | SND_MEMORY | SND_NODEFAULT);
    }
    Sleep(delay);
    MyKeybdEvent(modifier, 0, keyUp ? KEYEVENTF_KEYUP : 0, 0);
    Sleep(delay);
}

inline void PressAndRelease(WORD key, DWORD pressDelay, DWORD postDelay, LPBYTE sound)
{
    if (sound)
    {
        PlaySound((LPCWSTR)sound, NULL, SND_ASYNC | SND_MEMORY | SND_NODEFAULT);
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
        hkl = GetKeyboardLayout(k);
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

UINT StartAutoType(LPCWSTR pszText, typaste_config &config)
{
    if (state.pasting)
    {
        return WM_PASTECHAR;
    }

    HKL ahkl[cMaxKL];
    HKL hklOld = GetKeyboardLayout(0);
    UINT chkl = GetKeyboardLayoutList(cMaxKL, ahkl);

    state.reset(config, pszText);
    state.original_keyboard_layout = hklOld;
    state.keyboard_layouts.push_back(hklOld);
    for (UINT i = 0; i < chkl; i++)
    {
        state.keyboard_layouts.push_back(ahkl[i]);
    }

    state.pasting = true;

    return WM_PASTECHAR;
}

UINT AutoTypeChar()
{
    if (!state.pasting || GetAsyncKeyState(VK_ESCAPE) < 0)
    {
        return WM_ENDPASTE;
    }

    typaste_config &config = state.config();
    LPBYTE sound = NULL;

    switch (state.character())
    {
    case L'\n':
        sound = config.sound_config.enter_key();
        break;
    case L' ':
        sound = config.sound_config.space_key();
        break;
    default:
        sound = config.sound_config.get_key();
        break;
    }

    switch (state.character())
    {
    case L'\r':
        NEXT_CHAR;
    case L'\n':
        PressAndRelease(VK_RETURN, config.modifier_delay, config.key_delay(), sound);
        NEXT_CHAR;
    case L'\t':
        PressAndRelease(VK_TAB, config.modifier_delay, config.key_delay(), sound);
        NEXT_CHAR;
    }

    SHORT s = -1;
    UINT i;
    for (i = 0; i < state.keyboard_layouts.size(); i++)
    {
        s = VkKeyScanExW(state.character(), state.keyboard_layouts[i]);
        if (s != -1)
            break;
    }

    if (s != -1)
    {
        SetKeyboardLayout(state.keyboard_layouts[i], config.modifier_delay);
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
        SendMessageTimeoutW(info.hwndFocus, WM_CHAR, (WPARAM)state.character(), 0,
                            SMTO_ABORTIFHUNG, 2000, &dwResult);
        Sleep(config.modifier_delay);

        NEXT_CHAR;
    }

    bool modifierWasSet = state.last_flags & 1;
    if (((flags & 1) == 1) != modifierWasSet)
    {
        ChangeModifier(VK_LSHIFT, modifierWasSet, config.modifier_delay, modifierWasSet ? state.modifier_up_sound : state.modifier_down_sound);
    }

    modifierWasSet = state.last_flags & 2;
    if (((flags & 2) == 2) != modifierWasSet)
    {
        ChangeModifier(VK_LCONTROL, modifierWasSet, config.modifier_delay, modifierWasSet ? state.modifier_up_sound : state.modifier_down_sound);
    }

    modifierWasSet = state.last_flags & 4;
    if (((flags & 4) == 4) != modifierWasSet)
    {
        ChangeModifier(VK_LMENU, modifierWasSet, config.modifier_delay, modifierWasSet ? state.modifier_up_sound : state.modifier_down_sound);
    }

    state.last_flags = flags;

    PressAndRelease(vk, config.modifier_delay, config.key_delay(), sound);

    NEXT_CHAR;
}

void EndAutoType() 
{
    if (!state.pasting)
    {
        return;
    }

    typaste_config& config = state.config();

    if (state.last_flags & 1)
    {
        ChangeModifier(VK_LSHIFT, true, config.modifier_delay, state.modifier_up_sound);
    }

    if (state.last_flags & 2)
    {
        ChangeModifier(VK_LCONTROL, true, config.modifier_delay, state.modifier_up_sound);
    }

    if (state.last_flags & 4)
    {
        ChangeModifier(VK_LMENU, true, config.modifier_delay, state.modifier_up_sound);
    }

    SetKeyboardLayout(state.original_keyboard_layout, config.modifier_delay);

    state.pasting = false;
}