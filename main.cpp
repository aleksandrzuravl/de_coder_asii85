#include "decoder.h"
#include "coder.h"



int main(int argc, char* argv[]) {

    if (argc == 2) {
    
        std::string flag = argv[1];
        
        try {
            if (flag == "-d") {
            
                Decoder decoder;
                std::string value = decoder.getUserInput();
                std::vector<uint8_t> result = decoder.decodeData(value);
                decoder.displayResult(result);
                
            } else if (flag == "-e") {
            
                Coder coder;
                std::string value = coder.getUserInput();
                std::string result = coder.encodeString(value);
                coder.encodeString(result);
                
            } else {
                std::cerr << "Unknown flag. Use -d for decode or -e for encode.\n";
                return 1;
            }
        } catch (const std::runtime_error& e) {
            std::cerr << "\nError: " << e.what() << "\n";
        } catch (const std::invalid_argument& e) {
            std::cerr << "\nUser interaction error: " << e.what() << "\n";
        }
    }
    return 0;
}














