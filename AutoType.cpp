// AutoType
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>
// This file is public domain software.
#include "Typaste.hpp"

typo_map_type typo_map({
    {'`', {'1'}},
    {'1', {'`', '2', 'q'}},
    {'2', {'1', '3', 'w', 'q'}},
    {'3', {'2', '4', 'e', 'w'}},
    {'4', {'3', '5', 'r', 'e'}},
    {'5', {'4', '6', 't', 'r'}},
    {'6', {'5', '7', 'y', 't'}},
    {'7', {'6', '8', 'u', 'y'}},
    {'8', {'7', '9', 'i', 'u'}},
    {'9', {'8', '0', 'o', 'i'}},
    {'0', {'9', '-', 'p', 'o'}},
    {'-', {'0', '=', '[', 'p'}},
    {'=', {'-', ']', '['}},

    {'q', {'1', '2', 'w', 'a'}},
    {'w', {'q', '2', '3', 'e', 's', 'a'}},
    {'e', {'w', '3', '4', 'r', 'd', 's'}},
    {'r', {'e', '4', '5', 't', 'f', 'd'}},
    {'t', {'r', '5', '6', 'y', 'g', 'f'}},
    {'y', {'t', '6', '7', 'u', 'h', 'g'}},
    {'u', {'y', '7', '8', 'i', 'j', 'h'}},
    {'i', {'u', '8', '9', 'o', 'k', 'j'}},
    {'o', {'i', '9', '0', 'p', 'l', 'k'}},
    {'p', {'o', '0', '-', '[', ';', 'l'}},
    {'[', {'p', '-', '=', ']', '\'', ';'}},
    {']', {'[', '=', '\\', '\''}},
    {'\\', {']'}},

    {'a', {'q', 'w', 's', 'z'}},
    {'s', {'a', 'w', 'e', 'd', 'x', 'z'}},
    {'d', {'s', 'e', 'r', 'f', 'c', 'x'}},
    {'f', {'d', 'r', 't', 'g', 'v', 'c'}},
    {'g', {'f', 't', 'y', 'h', 'b', 'v'}},
    {'h', {'g', 'y', 'u', 'j', 'n', 'b'}},
    {'j', {'h', 'u', 'i', 'k', 'm', 'n'}},
    {'k', {'j', 'i', 'o', 'l', ',', 'm'}},
    {'l', {'k', 'o', 'p', ';', '.', ','}},
    {';', {'l', 'p', '[', '\'', '/', '.'}},
    {'\'', {';', '[', ']', '/'}},

    {'z', {'a', 's', 'x'}},
    {'x', {'z', 's', 'd', 'c'}},
    {'c', {'x', 'd', 'f', 'v', ' '}},
    {'v', {'c', 'f', 'g', 'b', ' '}},
    {'b', {'v', 'g', 'h', 'n', ' '}},
    {'n', {'b', 'h', 'j', 'm', ' '}},
    {'m', {'n', 'j', 'k', ',', ' '}},
    {',', {'m', 'k', 'l', '.'}},
    {'.', {',', 'l', ';', '/'}},
    {'/', {';', '\'', '.'}},

    {'~', {'`', '!'}},
    {'!', {'1', '~', '@', 'Q'}},
    {'@', {'2', '!', '#', 'W', 'Q'}},
    {'#', {'3', '@', '$', 'E', 'W'}},
    {'$', {'4', '#', '%', 'R', 'E'}},
    {'%', {'5', '$', '^', 'T', 'R'}},
    {'^', {'6', '%', '&', 'Y', 'T'}},
    {'&', {'7', '^', '*', 'U', 'Y'}},
    {'*', {'8', '&', '(', 'I', 'U'}},
    {'(', {'9', '*', ')', 'O', 'I'}},
    {')', {'0', '(', '_', 'P', 'O'}},
    {'_', {'-', ')', '+', '{', 'P'}},
    {'+', {'=', '_', '}', '{'}},

    {'Q', {'q', '!', '@', 'W', 'a'}},
    {'W', {'w', 'Q', '@', '#', 'e', 's', 'a'}},
    {'E', {'e', 'W', '#', '$', 'r', 'd', 's'}},
    {'R', {'r', 'E', '$', '%', 't', 'f', 'd'}},
    {'T', {'t', 'R', '%', '^', 'y', 'g', 'f'}},
    {'Y', {'y', 'T', '^', '&', 'u', 'h', 'g'}},
    {'U', {'u', 'Y', '&', '*', 'i', 'j', 'h'}},
    {'I', {'i', 'U', '*', '(', 'o', 'k', 'j'}},
    {'O', {'o', 'I', '(', ')', 'p', 'l', 'k'}},
    {'P', {'p', 'O', ')', '_', '[', ';', 'l'}},
    {'{', {'[', 'P', '_', '+', ']', '\'', ';'}},
    {'}', {']', '{', '+', '|', '"'}},
    {'|', {'\\', '}'}},

    {'A', {'a', 'Q', 'W', 'S', 'Z'}},
    {'S', {'s', 'A', 'W', 'E', 'D', 'X', 'z'}},
    {'D', {'d', 'S', 'E', 'R', 'F', 'C', 'x'}},
    {'F', {'f', 'D', 'R', 'T', 'G', 'V', 'c'}},
    {'G', {'g', 'F', 'T', 'Y', 'H', 'B', 'v'}},
    {'H', {'h', 'G', 'Y', 'U', 'J', 'N', 'b'}},
    {'J', {'j', 'H', 'U', 'I', 'K', 'M', 'n'}},
    {'K', {'k', 'J', 'I', 'O', 'L', '<', 'm'}},
    {'L', {'l', 'K', 'O', 'P', ':', '>', ','}},
    {':', {';', 'L', 'P', '{', '"', '?', '>'}},
    {'"', {'\'', ':', '{', '}', '?'}},

    {'Z', {'z', 'A', 'S', 'X'}},
    {'X', {'x', 'Z', 'S', 'D', 'C'}},
    {'C', {'c', 'X', 'D', 'F', 'V'}},
    {'V', {'v', 'C', 'F', 'G', 'B'}},
    {'B', {'b', 'V', 'G', 'H', 'N'}},
    {'N', {'n', 'B', 'H', 'J', 'M'}},
    {'M', {'m', 'N', 'J', 'K', '<'}},
    {'<', {',', 'M', 'K', 'L', '>'}},
    {'>', {'.', '<', 'L', ':', '?'}},
    {'?', {'/', ':', '"', '>'}},
});

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

void ProcessChar(WCHAR c, typaste_config &config)
{
    LPBYTE sound = NULL;

    switch (c)
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

    switch (c)
    {
    case L'\r':
        return;
    case L'\n':
        PressAndRelease(VK_RETURN, config.modifier_delay, config.key_delay(), sound);
        return;
    case L'\b':
        PressAndRelease(VK_BACK, config.modifier_delay, config.key_delay(), sound);
        return;
    case L'\t':
        PressAndRelease(VK_TAB, config.modifier_delay, config.key_delay(), sound);
        return;
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

        return;
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
}

UINT AutoTypeChar()
{
    if (!state.pasting || GetAsyncKeyState(VK_ESCAPE) < 0)
    {
        return WM_ENDPASTE;
    }

    typaste_config &config = state.config();

    WCHAR c = state.character();

    if (config.make_typo())
    {
        WCHAR typo_c = typo_map.get_for(c);

        if (typo_c != c) 
        {
            ProcessChar(typo_c, config);
            ProcessChar('\b', config);
        }
    }

    ProcessChar(c, config);

    return state.next_character() ? WM_PASTECHAR : WM_ENDPASTE;
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