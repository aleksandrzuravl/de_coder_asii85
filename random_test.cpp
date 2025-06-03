#include <gtest/gtest.h>
#include <random>
#include <vector>
#include "decoder.h"
#include "coder.h"

class RandomCodecTest : public ::testing::Test {
protected:
    void SetUp() override {
        encoder.setTestMode(true);
        decoder.setTestMode(true);
    }

    Coder encoder;
    Decoder decoder;
    const std::string validChars = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstu";
};

// ============= Helper Functions =============
std::string generateRandomString(const std::string& validChars, int length) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, validChars.size()-1);
    
    std::string result;
    for (int i = 0; i < length; ++i) {
        result += validChars[dist(gen)];
    }
    return result;
}

std::string injectInvalidChar(const std::string& validStr) {
    static const std::vector<uint8_t> invalidChars = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
        0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13,
        0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,
        0x1E, 0x1F, 0x7F, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86,
        0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90,
        0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A,
        0x9B, 0x9C, 0x9D, 0x9E, 0x9F, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4,
        0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE,
        0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8,
        0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> charDist(0, invalidChars.size()-1);
    std::uniform_int_distribution<> posDist(0, validStr.size());

    std::string corrupted = validStr;
    corrupted.insert(posDist(gen), 1, invalidChars[charDist(gen)]);
    return corrupted;
}

// ============= Random Valid Data Test =============
TEST_F(RandomCodecTest, ValidDataRoundtrip) {
    const int NUM_TESTS = 1000;
    const int REPORT_INTERVAL = 200;

    for (int i = 0; i < NUM_TESTS; ++i) {
        // Generate random input
        std::uniform_int_distribution<> lenDist(1, 200000);
        int length = lenDist(gen);
        std::string original = generateRandomString(validChars, length);
        
        // Encode and decode
        std::string encoded = encoder.encodeString(original);
        auto decoded = decoder.decodeData(encoded);
        std::string result(decoded.begin(), decoded.end());
        
        // Verify
        EXPECT_EQ(original, result) << "Test failed at iteration " << i;
        
        if ((i+1) % REPORT_INTERVAL == 0) {
            std::cout << "Completed " << (i+1) << " valid tests\n";
        }
    }
}

// ============= Random Invalid Data Test =============
TEST_F(RandomCodecTest, InvalidDataDetection) {
    const int NUM_TESTS = 1000;
    const int REPORT_INTERVAL = 200;

    for (int i = 0; i < NUM_TESTS; ++i) {
        // Generate valid string
        std::uniform_int_distribution<> lenDist(1, 200000);
        int length = lenDist(gen);
        std::string validStr = generateRandomString(validChars, length);
        
        // Inject invalid character
        std::string invalidStr = injectInvalidChar(validStr);
        
        // Verify decoder catches the error
        EXPECT_THROW({
            try {
                decoder.decodeData(invalidStr);
            } 
            catch (const std::runtime_error& e) {
                std::cout << "Caught expected exception: " << e.what() << "\n";
                throw;
            }
        }, std::runtime_error) << "Test failed at iteration " << i;
        
        if ((i+1) % REPORT_INTERVAL == 0) {
            std::cout << "Completed " << (i+1) << " invalid tests\n";
        }
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
