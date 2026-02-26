#include "markov.h"

// this takes words and sticks them together with spaces
std::string joinWords(const std::string words[], int startIndex, int count) {
    std::string result = "";

    for (int i = 0; i < count; i++) {
        result = result + words[startIndex + i];
        // no space after the last one
        if (i < count - 1) {
            result = result + " ";
        }
    }

    return result;
}

// opens the file and grabs all the words
int readWordsFromFile(std::string filename, std::string words[], int maxWords) {
    std::ifstream inputFile(filename);

    // if file didnt open
    if (!inputFile.is_open()) {
        return -1;
    }

    int counter = 0;

    while (counter < maxWords && inputFile >> words[counter]) {
        counter++;
    }

    inputFile.close();
    return counter;
}

int buildMarkovChain(const std::string words[], int numWords, int order,
                     std::string prefixes[], std::string suffixes[], int maxChainSize) {
    int count = 0;

    for (int i = 0; i <= numWords - order - 1; i++) {
        prefixes[count] = joinWords(words, i, order);
        suffixes[count] = words[i + order]; // the next word after
        count++;

        if (count >= maxChainSize) {
            break;
        }
    }

    return count;
}

// looks through the chain and finds a random word that goes after the prefix
std::string getRandomSuffix(const std::string prefixes[], const std::string suffixes[],
                            int chainSize, std::string currentPrefix) {
    // see how many times this prefix shows up
    int matchCount = 0;
    for (int i = 0; i < chainSize; i++) {
        if (prefixes[i] == currentPrefix) {
            matchCount++;
        }
    }

    if (matchCount == 0) {
        return "";
    }

    // pick a random one out of the matches
    int pick = rand() % matchCount;

    int matchesSoFar = 0;
    for (int i = 0; i < chainSize; i++) {
        if (prefixes[i] == currentPrefix) {
            if (matchesSoFar == pick) {
                return suffixes[i];
            }
            matchesSoFar++;
        }
    }

    return "";
}

std::string getRandomPrefix(const std::string prefixes[], int chainSize) {
    int index = rand() % chainSize;
    return prefixes[index];
}

// this is the one that actually makes the text
std::string generateText(const std::string prefixes[], const std::string suffixes[],
                         int chainSize, int order, int numWords) {
    std::string currentPrefix = getRandomPrefix(prefixes, chainSize);
    std::string result = currentPrefix;

    // this keeps track of the words we just used
    std::string recentWords[3];

    if (order == 1) {
        recentWords[0] = currentPrefix;
    } else {
        // break the prefix apart into each word
        std::string temp = currentPrefix;
        for (int w = 0; w < order; w++) {
            size_t spacePos = temp.find(" ");
            if (spacePos == std::string::npos) {
                recentWords[w] = temp;
            } else {
                recentWords[w] = temp.substr(0, spacePos);
                temp = temp.substr(spacePos + 1);
            }
        }
    }

    for (int i = 0; i < numWords - order; i++) {
        std::string newWord = getRandomSuffix(prefixes, suffixes, chainSize, currentPrefix);

        // if theres nothing left, just stop
        if (newWord == "") {
            break;
        }

        result = result + " " + newWord;

        if (order == 1) {
            recentWords[0] = newWord;
        } else {
            // slide the words over and put the new one at the end
            for (int w = 0; w < order - 1; w++) {
                recentWords[w] = recentWords[w + 1];
            }
            recentWords[order - 1] = newWord;
        }

        currentPrefix = joinWords(recentWords, 0, order);
    }

    return result;
}
