#include "adb.hpp"
#include <windows.h>
#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include <sstream>
#include <algorithm>

namespace FWork {

    Adb::Adb(const std::string& path) : _path(path) {}

    void Adb::Kill() {
        ExecuteAdbCommand("kill-server");
    }

    void Adb::ExecuteAdbCommand(const std::string& command) {
        std::string full_cmd = "\"" + _path + "\" " + command;
        
        STARTUPINFOA si = { sizeof(si) };
        PROCESS_INFORMATION pi;
        si.dwFlags = STARTF_USESHOWWINDOW;
        si.wShowWindow = SW_HIDE;

        if (CreateProcessA(NULL, (LPSTR)full_cmd.c_str(), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
    }

    std::vector<std::string> Adb::split(const std::string& s, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    uint32_t Adb::Start(std::string processName, std::string moduleName) {
        std::string cmd = "\"" + _path + "\" shell pidof " + processName;
        std::string output;
        char buffer[128];

        FILE* pipe = _popen(cmd.c_str(), "r");
        if (!pipe) return 0;

        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            output += buffer;
        }
        _pclose(pipe);

        output.erase(std::remove(output.begin(), output.end(), '\n'), output.end());
        output.erase(std::remove(output.begin(), output.end(), '\r'), output.end());

        if (output.empty()) return 0;

        try {
            return std::stoul(output);
        } catch (...) {
            return 0;
        }
    }
}
