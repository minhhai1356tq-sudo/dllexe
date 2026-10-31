#include "Render.hpp"
#include <Windows.h>
#include <string>
#include <algorithm>
#include <cctype>
#include <vector>
#include <TlHelp32.h>
#include <iostream>

HWND RenderWindow;

namespace Render {
    std::vector<DWORD> GetProcessIdsByName(const std::wstring& ProcessName) {
        std::vector<DWORD> processIds;
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot == INVALID_HANDLE_VALUE) return processIds;

        PROCESSENTRY32 processEntry;
        processEntry.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(hSnapshot, &processEntry)) {
            do {
                if (ProcessName == processEntry.szExeFile)
                    processIds.push_back(processEntry.th32ProcessID);
            } while (Process32Next(hSnapshot, &processEntry));
        }
        CloseHandle(hSnapshot);
        return processIds;
    }

    BOOL CALLBACK EnumChildWindowsProc(HWND hWnd, LPARAM lParam) {
        char windowName[256] = {};
        GetWindowTextA(hWnd, windowName, sizeof(windowName));
        std::string name(windowName);

        char className[256] = {};
        GetClassNameA(hWnd, className, sizeof(className));
        std::string class_str(className);

        if (name == "_ctl.Window" && class_str.find("BlueStacksApp") != std::string::npos) {
            RenderWindow = hWnd;
            std::cout << "[INFO] Janela Render encontrada: " << name << std::endl;
            return FALSE;
        }
        else if (name == "HD-Player" && class_str.find("Qt") != std::string::npos) {
            RenderWindow = hWnd;
            std::cout << "[INFO] Janela Render encontrada: " << name << std::endl;
            return FALSE;
        }
        return TRUE;
    }

    BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam) {
        // FIX: lParam truyền thẳng DWORD value, không phải địa chỉ
        DWORD processId = static_cast<DWORD>(lParam);
        DWORD windowProcessId = 0;
        GetWindowThreadProcessId(hWnd, &windowProcessId);
        if (windowProcessId == processId) {
            EnumChildWindows(hWnd, EnumChildWindowsProc, 0);
        }
        return TRUE;
    }

    HWND FindRenderWindow() {
        RenderWindow = nullptr;  // FIX: reset để không dùng HWND stale
        auto BlueStacksProcesses = GetProcessIdsByName(L"Bluestacks.exe");
        if (BlueStacksProcesses.empty())
            BlueStacksProcesses = GetProcessIdsByName(L"HD-Player.exe");
        if (BlueStacksProcesses.empty()) {
            std::cout << "[ERROR] Nenhum processo do BlueStacks encontrado!" << std::endl;
            return nullptr;
        }
        for (const auto& ProcessId : BlueStacksProcesses) {
            HANDLE ProcessHandle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, ProcessId);
            if (ProcessHandle) {
                // FIX: truyền giá trị DWORD, không phải &ProcessId
                EnumWindows(EnumWindowsProc, static_cast<LPARAM>(ProcessId));
                CloseHandle(ProcessHandle);
            }
        }
        return RenderWindow;
    }
}
