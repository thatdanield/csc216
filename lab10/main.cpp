#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <fstream>
#include <map>

using namespace std;
int main() {
    vector<string> commonWords = {
        "a", "an", "and", "are", "as", "at", "be", "but", "by", "for",
        "from", "had", "has", "have", "he", "i", "in", "is", "it", "of",
        "on", "or", "that", "the", "they", "this", "to", "was", "were",
        "with", "you", "your"
    };
    cout << "Input file name: " << endl;
    string filename;
    cin >> filename;
    ifstream file(filename);

    ofstream outputFile(filename + ".index");

    map<string, vector<int>> invertedFileIndex;
    string word;
    int wordIndex = 0;
    while(file >> word) {
        for (size_t i = 0; i < word.length(); i++) {
            if (ispunct(word[i])) {
                word.erase(i, 1);
                i--;
            }
        }
        for (char& c : word) {
            c = tolower(c);
        }
        if (!word.empty()
                && find(commonWords.begin(), commonWords.end(), word) == commonWords.end()) {
            invertedFileIndex[word].push_back(wordIndex);
        }
        wordIndex++;
    }

    for (pair<string, vector<int>> entry : invertedFileIndex) {
        string line = entry.first + ": [";
        for (size_t i = 0; i < entry.second.size(); i++) {
            if (i > 0) {
                line += ", ";
            }
            line += to_string(entry.second[i]);
        }
        line += "]\n";
        cout << line;
        outputFile << line;
    }


    return 0;
}
