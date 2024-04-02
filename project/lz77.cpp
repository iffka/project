#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>

using namespace std;

struct Token {
    int distance;
    int length;
    char nextChar;
};

vector<Token> compress(const string& input) {
    vector<Token> tokens;
    int i = 0;
    while (i < input.size()) {
        Token token;
        token.distance = 0;
        token.length = 0;
        token.nextChar = input[i];

        // Находим наилучшее совпадение с подстрокой
        for (int j = i - 1; j >= 0 && j >= i - 255; --j) {
            int len = 0;
            while (input[i + len] == input[j + len] && i + len < input.size() && len < 255) {
                ++len;
            }
            if (len > token.length) {
                token.distance = i - j;
                token.length = len;
                token.nextChar = input[i + len];
            }
        }

        tokens.push_back(token);
        i += token.length + 1;
    }
    return tokens;
}

string decompress(const vector<Token>& tokens) {
    string decompressed;
    for (const auto& token : tokens) {
        if (token.length == 0) {
            decompressed += token.nextChar;
        } else {
            int startIndex = decompressed.size() - token.distance;
            for (int i = 0; i < token.length; ++i) {
                decompressed += decompressed[startIndex + i];
            }
            decompressed += token.nextChar;
        }
    }
    return decompressed;
}

int main() {
    string input;
    ifstream in;
    in.open("WnP.txt");
    getline (in, input, '\0' );
    in.close();

    // Сжатие
    auto start = std::chrono::high_resolution_clock::now();
    vector<Token> compressed = compress(input);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    // Вывод сжатых данных
    ofstream out;
    out.open("output.txt");
    for (const auto& token : compressed) {
        out << "(" << token.distance << "," << token.length << "," << token.nextChar << ")";
    }
    out.close();
    cout << endl;

    // Декомпрессия
    auto start2 = std::chrono::high_resolution_clock::now();
    string decompressed = decompress(compressed);
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration2 = end2 - start2;

    std::cout << std::fixed;
    cout << "encoding time: " << duration.count() << endl;
    cout << "decoding tinme: " << duration2.count() << endl;

    return 0;
}