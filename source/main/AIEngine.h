#ifndef AIENGINE_H
#define AIENGINE_H

#include <string>
#include <windows.h>

class AIEngine {
public:
    // Constructor và Destructor
    AIEngine(const std::string& path);
    ~AIEngine();

    // Các phương thức khởi động và dừng Stockfish
    void start();
    void stop();

    // Gửi lệnh tới Stockfish
    void sendCommand(const std::string& command);

    // Nhận phản hồi từ Stockfish
    std::string getResponse();

    // Tính toán nước đi tốt nhất từ Stockfish
    std::string getBestMove(const std::string& position, int timeLimit);

private:
    // Đường dẫn tới Stockfish
    std::string pathToStockfish;

    // Cờ cho biết Stockfish có đang chạy hay không
    bool isRunning;

    // Handles cho các pipe (stdin, stdout)
    HANDLE hChildStdOutRead;
    HANDLE hChildStdOutWrite;
    HANDLE hChildStdInRead;
    HANDLE hChildStdInWrite;
};

#endif // AIENGINE_H

