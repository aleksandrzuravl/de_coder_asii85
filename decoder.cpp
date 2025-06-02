#include "decoder.h"
#include <stdexcept>
#include <iostream>  
#include <cctype> 

void Decoder::setTestMode(bool mode) { 
    testMode = mode; 
}

bool Decoder::isTestMode() const { 
    return testMode; 
}

std::string Decoder::getUserInput() const {
    std::string input;
    
    do {
        std::cout << "Введите данные для декодирования: ";
        std::getline(std::cin, input);
        
        if (input.empty()) {
            if (isTestMode()) throw std::invalid_argument("Пустой ввод.");
            std::cerr << "Ошибка: данные не могут быть пустыми.\n";
        }
    } while (input.empty());

    return input;
}

std::vector<uint8_t> Decoder::decodeData(const std::string& input) const {
    std::vector<uint8_t> output;
    uint32_t block = 0;
    int charCount = 0;

    for (char ch : input) {
        if (isspace(ch)) continue;
        
        if (ch == 'z') {
            if (charCount != 0) {
                throw std::runtime_error("Символ 'z' не может быть в середине блока");
            }
            output.insert(output.end(), 4, 0);
            continue;
        }

        if (ch < '!' || ch > 'u') {
            throw std::runtime_error("Недопустимый символ в ASCII85 строке");
        }

        block = block * 85 + (ch - 33);
        if (++charCount == 5) {
            for (int i = 0; i < 4; ++i) {
                output.push_back((block >> (24 - i*8)) & 0xFF);
            }
            block = charCount = 0;
        }
    }

    if (charCount > 0) {
        while (charCount++ < 5) block = block * 85 + 84;
        for (int i = 0; i < charCount - 2; ++i) {
            output.push_back((block >> (24 - i*8)) & 0xFF);
        }
    }

    return output;
}

void Decoder::displayResult(const std::vector<uint8_t>& result) const {
    if (result.empty()) {
        throw std::invalid_argument("Некорректные данные. Результат декодирования пуст.");
    }
    
    std::string message(result.begin(), result.end());
    std::cout << "\nРезультат декодирования: " << message << "\n";
}
