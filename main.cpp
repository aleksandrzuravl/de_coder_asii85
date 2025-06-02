#include "decoder.h"
#include "coder.h"

Mode determineMode(int argc, char* argv[]) {
    if (argc != 2) return UNKNOWN;
    
    const std::string flag(argv[1]);
    if (flag == "-e") return ENCODE;
    if (flag == "-d") return DECODE;
    return UNKNOWN;
}

void processOperation(Mode mode) {
    try {
        if (mode == ENCODE) {
            Coder encoder;
            const auto input = encoder.getUserInput();
            const auto result = encoder.encodeString(input);
            encoder.displayResult(result);
        } 
        else if (mode == DECODE) {
            Decoder decoder;
            const auto input = decoder.getUserInput();
            const auto result = decoder.decodeData(input);
            decoder.displayResult(result);
        }
    } 
    catch (const std::runtime_error& e) {
        std::cerr << "\nОшибка выполнения: " << e.what() << "\n";
    } 
    catch (const std::invalid_argument& e) {
        std::cerr << "\nОшибка ввода: " << e.what() << "\n";
    }
}

int main(int argc, char* argv[]) {
    const auto mode = determineMode(argc, argv);
    
    if (mode == UNKNOWN) {
        std::cerr << "Используйте флаг -d для декодирования или -e для кодирования.\n";
        return EXIT_FAILURE;
    }

    processOperation(mode);
    return EXIT_SUCCESS;
}
