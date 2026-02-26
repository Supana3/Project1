#include <iostream>
#include "markov.h"

int main() {
    srand(time(0)); // makes it random each time

    std::string filename;
    int order;
    int numWords;

    std::cout << "Enter input filename: ";
    std::cin >> filename;

    std::cout << "Enter order (1, 2, or 3): ";
    std::cin >> order;

    std::cout << "Enter number of words to generate: ";
    std::cin >> numWords;

    // has to be static or it crashes with big arrays
    static std::string words[100000];
    static std::string prefixes[100000];
    static std::string suffixes[100000];

    int wordCount = readWordsFromFile(filename, words, 100000);

    if (wordCount == -1) {
        std::cout << "Error: Could not open file " << filename << std::endl;
        return 1;
    }

    std::cout << "Read " << wordCount << " words from " << filename << std::endl;

    int chainSize = buildMarkovChain(words, wordCount, order, prefixes, suffixes, 100000);

    if (chainSize == 0) {
        std::cout << "Error: Could not build chain. File might be too short." << std::endl;
        return 1;
    }

    std::string output = generateText(prefixes, suffixes, chainSize, order, numWords);

    std::cout << std::endl;
    std::cout << "Generated text:" << std::endl;
    std::cout << output << std::endl;

    return 0;
}
