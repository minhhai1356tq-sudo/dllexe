#ifndef ADB_HPP
#define ADB_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace FWork {
    class Adb {
    public:
        Adb(const std::string& path);
        void Kill();
        uint32_t Start(std::string processName, std::string moduleName);
    private:
        void ExecuteAdbCommand(const std::string& command);
        std::vector<std::string> split(const std::string& s, char delimiter);
        std::string _path;
    };
}
#endif
