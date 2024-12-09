#include "GetLastLine.h"
#include <filesystem>
#include <fstream>
#include <regex>
#include <deque>
#include <thread>
#include <sstream>

namespace fs = std::filesystem;

string cleanLine(const string& line) {
    regex timestampRegex("^\\[[0-9]{2}:[0-9]{2}:[0-9]{2}\\]");
    regex nameRegex("<(.*?)>");

    string cleanedLine = regex_replace(line, timestampRegex, "");
    cleanedLine = regex_replace(cleanedLine, nameRegex, "$1");
    cleanedLine = regex_replace(cleanedLine, regex("^\\s+"), "");
    cleanedLine = regex_replace(cleanedLine, regex("\\s+$"), "");

    return cleanedLine;
}

string getLastLine(const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) return "";

    string line;
    deque<string> lines;

    while (getline(file, line)) {
        lines.push_back(line);
        if (lines.size() > 1) {
            lines.pop_front();
        }
    }

    file.close();
    return lines.empty() ? "" : cleanLine(lines.back());
}

int extractIdFromMessage(const std::string& message) {
    std::regex acceptRegex(R"(\/accept\s+help\s+(\d+))", std::regex_constants::icase);
    std::smatch match;

    if (std::regex_search(message, match, acceptRegex) && match.size() > 1) {
        return std::stoi(match[1].str());
    }

    return -1;
}

void monitorLogFile(CChat* playerChat, CInput* playerInput) {
    if (playerChat == nullptr) return;

    fs::path logFilePath(playerChat->m_szLogPath);

    if (!fs::exists(logFilePath) || !fs::is_regular_file(logFilePath)) return;

    auto lastWriteTime = fs::last_write_time(logFilePath);

    while (playerChat != nullptr) {
        auto currentWriteTime = fs::last_write_time(logFilePath);

        if (currentWriteTime != lastWriteTime) {
            lastWriteTime = currentWriteTime;

            auto logContent = getLastLine(playerChat->m_szLogPath);
            int id = extractIdFromMessage(logContent);

            std::this_thread::sleep_for(std::chrono::seconds(std::rand() % 3 + 1));

            if (id != -1) playerInput->Send("/accept help " + std::to_string(id));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}
