#include "AIEngine.h"
#include <iostream>
#include <windows.h>
#include <string>
#include <array>

AIEngine::AIEngine(const std::string& path) : pathToStockfish(path), isRunning(false) {}

AIEngine::~AIEngine() {
    if (isRunning) {
        stop();
    }
}

void AIEngine::start() {
    // Mở Stockfish thông qua CreateProcess và các pipes
    std::cout << "Starting Stockfish..." << std::endl;

    // Tạo các pipe cho đầu vào và đầu ra của Stockfish
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    HANDLE hChildStdOutRead = NULL;
    HANDLE hChildStdOutWrite = NULL;
    HANDLE hChildStdInRead = NULL;
    HANDLE hChildStdInWrite = NULL;

    // Tạo pipe để giao tiếp với Stockfish
    if (!CreatePipe(&hChildStdOutRead, &hChildStdOutWrite, &saAttr, 0)) {
        std::cerr << "Stdout pipe creation failed!" << std::endl;
        return;
    }

    if (!CreatePipe(&hChildStdInRead, &hChildStdInWrite, &saAttr, 0)) {
        std::cerr << "Stdin pipe creation failed!" << std::endl;
        return;
    }

    // Đảm bảo các bộ ghi được truyền qua pipes
    SetHandleInformation(hChildStdOutRead, HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(hChildStdInWrite, HANDLE_FLAG_INHERIT, 0);

    // Cấu hình tiến trình con
    STARTUPINFO siStartInfo;
    PROCESS_INFORMATION piProcInfo;

    ZeroMemory(&siStartInfo, sizeof(siStartInfo));
    siStartInfo.cb = sizeof(siStartInfo);
    ZeroMemory(&piProcInfo, sizeof(piProcInfo));

    // Lệnh để khởi động Stockfish
    std::string command = pathToStockfish + " uci";

    // Tạo tiến trình con
    if (!CreateProcess(NULL, (LPSTR)command.c_str(), NULL, NULL, TRUE, 0, NULL, NULL, &siStartInfo, &piProcInfo)) {
        std::cerr << "CreateProcess failed (" << GetLastError() << ")." << std::endl;
        return;
    }

    // Đóng các handle không cần thiết
    CloseHandle(piProcInfo.hProcess);
    CloseHandle(piProcInfo.hThread);
    CloseHandle(hChildStdOutWrite);
    CloseHandle(hChildStdInRead);

    isRunning = true;

    // Gửi lệnh UCI cho Stockfish
    sendCommand("uci");
    sendCommand("uciok");
}

void AIEngine::stop() {
    // Gửi lệnh "quit" để dừng Stockfish
    sendCommand("quit");
    isRunning = false;
}

void AIEngine::sendCommand(const std::string& command) {
    if (!isRunning) {
        std::cerr << "Stockfish is not running!" << std::endl;
        return;
    }

    // Gửi lệnh tới Stockfish qua pipe stdin
    DWORD written;
    WriteFile(hChildStdInWrite, command.c_str(), command.length(), &written, NULL);
    WriteFile(hChildStdInWrite, "\n", 1, &written, NULL);  // Gửi ký tự newline

    std::cout << "Sent command: " << command << std::endl;
}

std::string AIEngine::getResponse() {
    // Nhận phản hồi từ Stockfish qua pipe stdout
    char buffer[128];
    std::string response = "";
    DWORD bytesRead;

    while (ReadFile(hChildStdOutRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
        if (bytesRead == 0) {
            break;
        }
        buffer[bytesRead] = '\0';  // Null-terminate the string
        response += buffer;
    }

    return response;
}

std::string AIEngine::getBestMove(const std::string& position, int timeLimit) {
    if (!isRunning) {
        start();  // Bắt đầu Stockfish nếu chưa chạy
    }

    // Gửi lệnh "position" để thiết lập trạng thái bàn cờ cho Stockfish
    sendCommand("position " + position);

    // Gửi lệnh "go" để yêu cầu Stockfish tìm kiếm nước đi tốt nhất trong thời gian giới hạn
    sendCommand("go movetime " + std::to_string(timeLimit));

    // Nhận phản hồi về nước đi tốt nhất
    return getResponse();
}

