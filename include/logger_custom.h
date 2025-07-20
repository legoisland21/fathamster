#include <fstream>
#include <iostream>
#include <ctime>
#include <string>

class Logger {
private:
    std::ofstream logFile;
    const std::string fileName;
    
public:
    explicit Logger(const std::string& fileName_) : fileName(fileName_) {
        logFile.open(fileName, std::ios::app);
        if (!logFile.is_open()) {
            std::cerr << "Error: Failed to open log file: " << fileName << std::endl;
        }
    }

    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    bool LogToFile(const std::string& log, int type) {
        if (!logFile.is_open()) return false;

        time_t currentTime = time(nullptr);
        std::string timeStr = ctime(&currentTime);
        timeStr.pop_back();

        switch (type) {
            case 0: logFile << "MESSAGE: "; break;
            case 1: logFile << "INFO: "; break;
            case 2: logFile << "WARNING: "; break;
            case 3: logFile << "ERROR: "; break;
            default: return false;
        }

        logFile << "[" << timeStr << "] " << log << "\n";
        return logFile.good();
    }
};