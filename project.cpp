#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>

const std::vector<std::string> commonWords = {"a", "and", "an", "of", "in", "the"};
const int NUM_BOOKS = 64;

std::map<std::string, double> calculateWordFrequencies(const std::string& filename) {
    std::map<std::string, int> wordCount;
    std::ifstream file(filename);
    std::string word;
    int totalWords = 0;

    while (file >> word) {
        // Normalize to uppercase and remove non-alphabetic characters
        std::transform(word.begin(), word.end(), word.begin(), ::toupper);
        word.erase(std::remove_if(word.begin(), word.end(), [](char c) { return !std::isalnum(c); }), word.end());

        // Skip common words
        if (std::find(commonWords.begin(), commonWords.end(), word) == commonWords.end()) {
            wordCount[word]++;
            totalWords++;
        }
    }

    std::map<std::string, double> normalizedFrequencies;
    for (const auto& pair : wordCount) {
        normalizedFrequencies[pair.first] = static_cast<double>(pair.second) / totalWords;
    }
    return normalizedFrequencies;
}

double calculateSimilarity(const std::map<std::string, double>& freq1, const std::map<std::string, double>& freq2) {
    double similarity = 0.0;
    for (const auto& pair : freq1) {
        if (freq2.find(pair.first) != freq2.end()) {
            similarity += pair.second + freq2.at(pair.first);
        }
    }
    return similarity;
}

int main() {
    std::vector<std::map<std::string, double>> frequencies(NUM_BOOKS);
    double similarityMatrix[NUM_BOOKS][NUM_BOOKS] = {0};

    // Load frequencies for each textbook
    for (int i = 0; i < NUM_BOOKS; ++i) {
        frequencies[i] = calculateWordFrequencies("book" + std::to_string(i + 1) + ".txt");
    }

    // Calculate similarity matrix
    for (int i = 0; i < NUM_BOOKS; ++i) {
        for (int j = i + 1; j < NUM_BOOKS; ++j) {
            similarityMatrix[i][j] = calculateSimilarity(frequencies[i], frequencies[j]);
        }
    }

    // Report top ten similar pairs
    std::vector<std::pair<double, std::pair<int, int>>> similarities;
    for (int i = 0; i < NUM_BOOKS; ++i) {
        for (int j = i + 1; j < NUM_BOOKS; ++j) {
            similarities.emplace_back(similarityMatrix[i][j], std::make_pair(i, j));
        }
    }

    std::sort(similarities.rbegin(), similarities.rend());
    std::cout << "Top 10 similar pairs of textbooks:\n";
    for (int i = 0; i < 10 && i < similarities.size(); ++i) {
        std::cout << "Books " << similarities[i].second.first + 1 << " and " << similarities[i].second.second + 1 
                  << " with similarity index: " << similarities[i].first << "\n";
    }

    return 0;
}
                    