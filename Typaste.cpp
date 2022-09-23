// Typaste.cpp --- Typing + Paste
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>
// This file is public domain software.
#include "Typaste.hpp"

using namespace std;

#define DEFAULT_DELAY 20
#define DEFAULT_HOTKEY MAKEWORD('V', HOTKEYF_CONTROL)
#define HOTKEY_ID 0xDEDD
#define LoadIfFound(sound, findname, filename)    if (_wcsicmp((findname), (filename)) == 0) sound = LoadFile(findname)
#define DeleteSound(sound)  if (sound != NULL) delete[] sound

HWND s_hwndMain = NULL;
HICON s_hIcon = NULL;
HICON s_hIconSm = NULL;
INT s_nExitCode = IDCANCEL;
typaste_config config(DEFAULT_DELAY, DEFAULT_HOTKEY);

static const TCHAR s_szName[] = TEXT("Typaste");
static const WCHAR s_szEnterSoundFile[] = L"enter.wav";
static const WCHAR s_szSpaceSoundFile[] = L"space.wav";
static const WCHAR s_szModifierDownSoundFile[] = L"modifierdown.wav";
static const WCHAR s_szModifierUpSoundFile[] = L"modifierup.wav";

BOOL MyRegisterHotKey(HWND hwnd)
{
    UINT vk = LOBYTE(config.hotkey);
    UINT flags = HIBYTE(config.hotkey);

    UINT nMod = 0;
    if (flags & HOTKEYF_ALT)
        nMod |= MOD_ALT;
    if (flags & HOTKEYF_CONTROL)
        nMod |= MOD_CONTROL;
    if (flags & HOTKEYF_SHIFT)
        nMod |= MOD_SHIFT;

    UnregisterHotKey(hwnd, HOTKEY_ID);
    if (!RegisterHotKey(hwnd, HOTKEY_ID, nMod, vk))
    {
        MessageBoxW(hwnd, L"RegisterHotKey failed", NULL, MB_ICONERROR);
        return FALSE;
    }
    return TRUE;
}

LPBYTE LoadFile(LPCWSTR lpszFileName)
{
    HANDLE hFile = CreateFile(lpszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        return NULL;
    }

    DWORD dwFileSize = GetFileSize(hFile, NULL);
    DWORD dwBytesRead;
    LPBYTE buffer = new BYTE[dwFileSize];
    BOOL readSuccess = ReadFile(hFile, buffer, dwFileSize, &dwBytesRead, NULL);
    CloseHandle(hFile);
    if (readSuccess)
    {
        return buffer;
    }

    delete[] buffer;
    return NULL;
}

void FindSoundFiles(sound_config& soundConfig)
{
    soundConfig.clear();

    WCHAR szPath[MAX_PATH];
    GetModuleFileNameW(NULL, szPath, MAX_PATH);
    LPWSTR pch = wcsrchr(szPath, L'\\');
    *pch = 0;
    lstrcatW(szPath, L"\\*.wav");

    WIN32_FIND_DATAW find;
    HANDLE hFind = FindFirstFile(szPath, &find);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            LoadIfFound(soundConfig.enter, find.cFileName, s_szEnterSoundFile);
            else LoadIfFound(soundConfig.space, find.cFileName, s_szSpaceSoundFile);
            else LoadIfFound(soundConfig.modifier_down, find.cFileName, s_szModifierDownSoundFile);
            else LoadIfFound(soundConfig.modifier_up, find.cFileName, s_szModifierUpSoundFile);
            else 
            {
                soundConfig.key_list.push_back({find.cFileName, LoadFile(find.cFileName)});
            }
        } while (FindNextFileW(hFind, &find));
        FindClose(hFind);
    }

    if (soundConfig.key_list.size())
    {
        soundConfig.key = soundConfig.key_list[0];
    }
}

BOOL Settings_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    SendMessageW(hwnd, WM_SETICON, ICON_BIG, (LPARAM)s_hIcon);
    SendMessageW(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)s_hIconSm);

    SetDlgItemInt(hwnd, edt1, config.min_key_delay, FALSE);
    SetDlgItemInt(hwnd, edt3, config.max_key_delay, FALSE);
    SetDlgItemInt(hwnd, edt4, config.modifier_delay, FALSE);

    SendDlgItemMessage(hwnd, edt2, HKM_SETRULES, HKCOMB_A | HKCOMB_NONE | HKCOMB_S, 0);
    SendDlgItemMessage(hwnd, edt2, HKM_SETHOTKEY, config.hotkey, 0);

    CheckRadioButton(hwnd, rad1, rad2, config.sound_config.random_key ? rad2 : rad1);

    CheckDlgButton(hwnd, chx1, config.make_typos ? BST_CHECKED : BST_UNCHECKED);
    SetDlgItemText(hwnd, edt5, config.typo_chance().c_str());

    HWND hCmb1 = GetDlgItem(hwnd, cmb1);

    for (auto& entry : config.sound_config.key_list)
    {
        ComboBox_AddString(hCmb1, entry.file_name().c_str());
    }

    if (ComboBox_FindStringExact(hCmb1, -1, config.sound_config.key.file_name().c_str()) == CB_ERR)
    {
        ComboBox_AddString(hCmb1, config.sound_config.key.file_name().c_str());
    }
    ComboBox_SetText(hCmb1, config.sound_config.key.file_name().c_str());

    SetForegroundWindow(hwnd);
    return TRUE;
}

BOOL Settings_Delete(HWND hwnd)
{
    HKEY hKey = NULL;
    RegOpenKeyExW(HKEY_CURRENT_USER,
                  L"SOFTWARE\\Katayama Hirofumi MZ\\Typaste", 0,
                  KEY_READ,
                  &hKey);
    if (!hKey)
        return FALSE;

    RegDeleteKeyW(hKey, L"MinDelay");
    RegDeleteKeyW(hKey, L"MaxDelay");
    RegDeleteKeyW(hKey, L"ModifierDelay");
    RegDeleteKeyW(hKey, L"HotKey");
    RegDeleteKeyW(hKey, L"Sound");
    RegDeleteKeyW(hKey, L"SetSound");
    RegDeleteKeyW(hKey, L"MakeTypos");
    RegDeleteKeyW(hKey, L"TypoChance");
    RegCloseKey(hKey);

    LONG nError = RegDeleteKeyW(HKEY_CURRENT_USER,
                                L"SOFTWARE\\Katayama Hirofumi MZ\\Typaste");
    return (ERROR_SUCCESS == nError);
}

BOOL Settings_Load(HWND hwnd)
{

    config.max_key_delay = DEFAULT_DELAY;
    config.min_key_delay = DEFAULT_DELAY;
    config.modifier_delay = DEFAULT_DELAY;
    config.hotkey = DEFAULT_HOTKEY;

    HKEY hKey = NULL;
    RegOpenKeyExW(HKEY_CURRENT_USER,
                  L"SOFTWARE\\Katayama Hirofumi MZ\\Typaste", 0,
                  KEY_READ,
                  &hKey);
    if (!hKey)
        return FALSE;

    DWORD dwData, cbData;
    WCHAR szText[MAX_PATH];

    cbData = sizeof(DWORD);
    if (ERROR_SUCCESS == RegQueryValueExW(hKey, L"MinDelay", NULL, NULL, (LPBYTE)&dwData, &cbData))
    {
        config.min_key_delay = dwData;
    }

    cbData = sizeof(DWORD);
    if (ERROR_SUCCESS == RegQueryValueExW(hKey, L"MaxDelay", NULL, NULL, (LPBYTE)&dwData, &cbData))
    {
        config.max_key_delay = dwData;
    }

    cbData = sizeof(DWORD);
    if (ERROR_SUCCESS == RegQueryValueExW(hKey, L"ModifierDelay", NULL, NULL, (LPBYTE)&dwData, &cbData))
    {
        config.modifier_delay = dwData;
    }

    cbData = sizeof(DWORD);
    if (ERROR_SUCCESS == RegQueryValueExW(hKey, L"HotKey", NULL, NULL, (LPBYTE)&dwData, &cbData))
    {
        config.hotkey = (WORD)dwData;
    }

    cbData = sizeof(szText);
    if (ERROR_SUCCESS == RegQueryValueExW(hKey, L"Sound", NULL, NULL, (LPBYTE)szText, &cbData))
    {
        StrTrimW(szText, L" \t\r\n");
        config.sound_config.key.file_name(szText);
        config.sound_config.key.sound(LoadFile(szText));
    }

    cbData = sizeof(DWORD);
    if (ERROR_SUCCESS == RegQueryValueExW(hKey, L"SetSound", NULL, NULL, (LPBYTE)&dwData, &cbData))
    {
        config.sound_config.random_key = !(dwData != 0);
    }

    cbData = sizeof(DWORD);
    if (ERROR_SUCCESS == RegQueryValueExW(hKey, L"MakeTypos", NULL, NULL, (LPBYTE)&dwData, &cbData))
    {
        config.make_typos = (dwData != 0);
    }

    cbData = sizeof(szText);
    if (ERROR_SUCCESS == RegQueryValueExW(hKey, L"TypoChance", NULL, NULL, (LPBYTE)szText, &cbData))
    {
        StrTrimW(szText, L" \t\r\n");
        config.typo_chance(szText);
    }

    RegCloseKey(hKey);
    return TRUE;
}

BOOL Settings_Save(HWND hwnd)
{
    HKEY hCompanyKey = NULL;
    RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\Katayama Hirofumi MZ", 0, NULL, 0,
                    KEY_ALL_ACCESS, NULL, &hCompanyKey, NULL);
    if (!hCompanyKey)
        return FALSE;

    HKEY hAppKey = NULL;
    RegCreateKeyExW(hCompanyKey, L"Typaste", 0, NULL, 0,
                    KEY_ALL_ACCESS, NULL, &hAppKey, NULL);
    if (!hAppKey)
    {
        RegCloseKey(hCompanyKey);
        return FALSE;
    }

    RegSetValueExW(hAppKey, L"MinDelay", 0, REG_DWORD, (LPBYTE)&config.min_key_delay, sizeof(DWORD));
    RegSetValueExW(hAppKey, L"MaxDelay", 0, REG_DWORD, (LPBYTE)&config.max_key_delay, sizeof(DWORD));
    RegSetValueExW(hAppKey, L"ModifierDelay", 0, REG_DWORD, (LPBYTE)&config.modifier_delay, sizeof(DWORD));

    DWORD dwData = config.hotkey;
    RegSetValueExW(hAppKey, L"HotKey", 0, REG_DWORD, (LPBYTE)&dwData, sizeof(DWORD));

    DWORD cbData = ((DWORD)config.sound_config.key.file_name().length() + 1) * sizeof(WCHAR);
    RegSetValueExW(hAppKey, L"Sound", 0, REG_SZ, (LPBYTE)config.sound_config.key.file_name().c_str(), cbData);

    dwData = config.sound_config.random_key ? 0 : 1;
    RegSetValueExW(hAppKey, L"SetSound", 0, REG_DWORD, (LPBYTE)&dwData, sizeof(DWORD));

    dwData = config.make_typos ? 1 : 0;
    RegSetValueExW(hAppKey, L"MakeTypos", 0, REG_DWORD, (LPBYTE)&dwData, sizeof(DWORD));

    cbData = ((DWORD)config.typo_chance().length() + 1) * sizeof(WCHAR);
    RegSetValueExW(hAppKey, L"TypoChance", 0, REG_SZ, (LPBYTE)config.typo_chance().c_str(), cbData);

    RegCloseKey(hAppKey);
    RegCloseKey(hCompanyKey);
    return TRUE;
}

void Settings_OnOK(HWND hwnd)
{
    config.min_key_delay = GetDlgItemInt(hwnd, edt1, NULL, FALSE);
    config.max_key_delay = GetDlgItemInt(hwnd, edt3, NULL, FALSE);
    config.modifier_delay = GetDlgItemInt(hwnd, edt4, NULL, FALSE);
    config.hotkey = (WORD)SendDlgItemMessage(hwnd, edt2, HKM_GETHOTKEY, 0, 0);

    WCHAR szText[MAX_PATH];
    GetDlgItemText(hwnd, cmb1, szText, ARRAYSIZE(szText));
    StrTrimW(szText, L" \t\r\n");
    config.sound_config.key.file_name(szText);
    config.sound_config.key.sound(LoadFile(szText));

    config.sound_config.random_key = IsDlgButtonChecked(hwnd, rad2) == BST_CHECKED;

    config.make_typos = IsDlgButtonChecked(hwnd, chx1) == BST_CHECKED;
    
    GetDlgItemText(hwnd, edt5, szText, ARRAYSIZE(szText));
    StrTrimW(szText, L" \t\r\n");
    wstring sTypoChance = config.typo_chance(szText);
    SetDlgItemText(hwnd, edt5, sTypoChance.c_str());

    Settings_Save(hwnd);

    EndDialog(hwnd, IDOK);
}

#ifndef ARRAYSIZE
    #define ARRAYSIZE(array) (sizeof(array) / sizeof(array[0]))
#endif

void Settings_OnPsh1(HWND hwnd)
{
    WCHAR szText[128];
    LoadStringW(NULL, IDS_DELETESETTINGS, szText, ARRAYSIZE(szText));

    if (IDOK == MessageBoxW(hwnd, szText, s_szName, MB_ICONINFORMATION))
    {
        Settings_Delete(hwnd);
        Settings_Load(hwnd);
        Settings_Save(hwnd);

        EndDialog(hwnd, psh1);
    }
}

void Settings_OnPsh2(HWND hwnd)
{
    OPENFILENAMEW ofn;
    WCHAR szPath[MAX_PATH] = L"";

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = OPENFILENAME_SIZE_VERSION_400W;
    ofn.lpstrFilter = L"Sound File (*.wav)\0*.wav\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = szPath;
    ofn.nMaxFile = ARRAYSIZE(szPath);
    ofn.lpstrTitle = L"Choose a sound file";
    ofn.Flags = OFN_EXPLORER | OFN_ENABLESIZING | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST |
                OFN_HIDEREADONLY | OFN_LONGNAMES;
    ofn.lpstrDefExt = L"wav";
    if (GetOpenFileNameW(&ofn))
    {
        SetDlgItemTextW(hwnd, cmb1, szPath);
    }
}

void Settings_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id)
    {
    case IDOK:
        Settings_OnOK(hwnd);
        break;
    case IDCANCEL:
        EndDialog(hwnd, id);
        break;
    case psh1:
        Settings_OnPsh1(hwnd);
        break;
    case psh2:
        Settings_OnPsh2(hwnd);
        break;
    }
}

INT_PTR CALLBACK
SettingsDialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        HANDLE_MSG(hwnd, WM_INITDIALOG, Settings_OnInitDialog);
        HANDLE_MSG(hwnd, WM_COMMAND, Settings_OnCommand);
    default:
        break;
    }
    return 0;
}

BOOL OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    s_hwndMain = hwnd;

    FindSoundFiles(config.sound_config);
    Settings_Load(hwnd);
    MyRegisterHotKey(hwnd);

    SendMessageW(hwnd, WM_SETICON, ICON_BIG, (LPARAM)s_hIcon);
    SendMessageW(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)s_hIconSm);

    return TRUE;
}

void OnPsh2(HWND hwnd)
{
    WCHAR szPath[MAX_PATH], szText[64];

    GetModuleFileNameW(NULL, szPath, MAX_PATH);
    LPWSTR pch = wcsrchr(szPath, L'\\');
    *pch = 0;

    LoadStringW(NULL, IDS_README, szText, 64);
    wcscat_s(szPath, szText);

    ShellExecuteW(hwnd, NULL, szPath, NULL, NULL, SW_SHOWNORMAL);
}

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    EndAutoType();

    switch (id)
    {
    case IDOK:
        s_nExitCode = IDOK;
        DestroyWindow(hwnd);
        break;
    case IDCANCEL:
        s_nExitCode = IDCANCEL;
        DestroyWindow(hwnd);
        break;
    case psh1:
        s_nExitCode = psh1;
        DestroyWindow(hwnd);
        break;
    case psh2:
        OnPsh2(hwnd);
        break;
    }
}

void OnHotKey(HWND hwnd, int idHotKey, UINT fuModifiers, UINT vk)
{
    if (idHotKey != HOTKEY_ID)
        return;

    EndAutoType();

    if (!IsClipboardFormatAvailable(CF_UNICODETEXT) ||
        !OpenClipboard(hwnd))
    {
        UnregisterHotKey(hwnd, HOTKEY_ID);
        WaitModifierRelease(config.modifier_delay);
        CtrlV(config.modifier_delay);
        MyRegisterHotKey(hwnd);
        return;
    }

    LPWSTR pszClone = NULL;
    if (HGLOBAL hText = GetClipboardData(CF_UNICODETEXT))
    {
        if (LPWSTR pszText = (LPWSTR)GlobalLock(hText))
        {
            pszClone = _wcsdup(pszText);
            GlobalUnlock(hText);
        }
    }

    if (!pszClone)
    {
        UnregisterHotKey(hwnd, HOTKEY_ID);
        WaitModifierRelease(config.modifier_delay);
        CtrlV(config.modifier_delay);
        MyRegisterHotKey(hwnd);
        return;
    }

    CloseClipboard();

    WaitModifierRelease(config.modifier_delay);

    PostMessage(hwnd, StartAutoType(pszClone, config), 0, 0);
}

void OnDestroy(HWND hwnd)
{
    EndAutoType();

    Settings_Save(hwnd);

    auto soundConfig = config.sound_config;
    DeleteSound(soundConfig.enter);
    DeleteSound(soundConfig.space);
    DeleteSound(soundConfig.modifier_down);
    DeleteSound(soundConfig.modifier_up);
    for (auto item : soundConfig.key_list) 
    {
        DeleteSound(item.second);
    }

    PostQuitMessage(0);
}

#ifndef HANDLE_WM_HOTKEY
    #define HANDLE_WM_HOTKEY(hwnd,wParam,lParam,fn) \
        ((fn)((hwnd),(int)(wParam),(UINT)LOWORD(lParam),(UINT)HIWORD(lParam)),(LRESULT)0)
#endif

INT_PTR CALLBACK
MainDialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        HANDLE_MSG(hwnd, WM_INITDIALOG, OnInitDialog);
        HANDLE_MSG(hwnd, WM_COMMAND, OnCommand);
        HANDLE_MSG(hwnd, WM_HOTKEY, OnHotKey);
        HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);
        case WM_PASTECHAR:
            PostMessage(hwnd, AutoTypeChar(), 0, 0);
            return TRUE;
        case WM_ENDPASTE:
            EndAutoType();
            return TRUE;
    }
    return 0;
}

INT WINAPI
WinMain(HINSTANCE   hInstance,
        HINSTANCE   hPrevInstance,
        LPSTR       lpCmdLine,
        INT         nCmdShow)
{
    {
        WCHAR szText[64];
        LoadStringW(NULL, IDS_TITLETEXT, szText, ARRAYSIZE(szText));
        if (HWND hwnd = FindWindowW(L"#32770", szText))
        {
            /* already exists */
            SetForegroundWindow(hwnd);
            return 0;
        }
    }

    InitCommonControls();

    s_hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(1));
    s_hIconSm = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(1), IMAGE_ICON,
        GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), 0);

    for (;;)
    {
        HWND hwnd = CreateDialog(hInstance, MAKEINTRESOURCE(1), NULL,
                                 MainDialogProc);
        if (!hwnd)
        {
            MessageBoxW(NULL, L"CreateDialog failed", NULL, MB_ICONERROR);
            return -2;
        }

        ShowWindow(hwnd, nCmdShow);
        UpdateWindow(hwnd);

        // message loop
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0))
        {
            if (IsDialogMessage(hwnd, &msg))
                continue;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (s_nExitCode != psh1)
            break;

        DialogBox(hInstance, MAKEINTRESOURCE(2), NULL, SettingsDialogProc);
    }

    return 0;
}
