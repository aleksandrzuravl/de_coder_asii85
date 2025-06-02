#include "coder.h"
#include <stdexcept>

std::string Coder::getUserInput() const {
    std::string input;
    
    do {
        std::cout << "Введите строку для кодирования: ";
        std::getline(std::cin, input);
        
        if (input.empty()) {
            if (isTestMode()) throw std::invalid_argument("Пустой ввод.");
            std::cerr << "Ошибка: строка не может быть пустой.\n";
        }
    } while (input.empty());

    return input;
}

std::string Coder::encodeBlock(uint32_t block, int bytesCount) const {
    if (block == 0 && bytesCount == 4) return "z";
    
    char encoded[5];
    for (int i = 4; i >= 0; --i) {
        encoded[i] = (block % 85) + 33;
        block /= 85;
    }
    
    return std::string(encoded, bytesCount + 1);
}

std::string Coder::encodeString(const std::string& input) const {
    std::string result;
    uint32_t currentBlock = 0;
    int byteCount = 0;

    for (unsigned char ch : input) {
        currentBlock = (currentBlock << 8) | ch;
        if (++byteCount == 4) {
            result += encodeBlock(currentBlock);
            currentBlock = byteCount = 0;
        }
    }

    if (byteCount > 0) {
        currentBlock <<= (4 - byteCount) * 8;
        result += encodeBlock(currentBlock, byteCount);
    }

    return result;
}

void Coder::displayResult(const std::string& result) const {
    std::cout << "\nРезультат кодирования: " << result << "\n";
}
