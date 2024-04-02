#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <chrono>
using namespace std;

// Функция для кодирования данных с помощью алгоритма LZW
vector<int> LZW_encode(const string& input) {
    unordered_map<string, int> dictionary;
    vector<int> output;
    int dict_size = 256; // Начальный размер словаря

    // Инициализация словаря с ASCII символами
    for (int i = 0; i < 256; ++i) {
        string symbol(1, char(i));
        dictionary[symbol] = i;
    }

    string current_str;
    for (char symbol : input) {
        string combined = current_str + symbol;
        if (dictionary.find(combined) != dictionary.end()) {
            // Если сочетание текущей строки и символа уже есть в словаре, обновляем текущую строку
            current_str = combined;
        } else {
            // Иначе добавляем код текущей строки в выход и добавляем сочетание текущей строки и символа в словарь
            output.push_back(dictionary[current_str]);
            dictionary[combined] = dict_size++;
            current_str = string(1, symbol);
        }
    }

    // Добавляем последний код в выход
    if (!current_str.empty()) {
        output.push_back(dictionary[current_str]);
    }

    return output;
}

// Функция для декодирования данных с помощью алгоритма LZW
string LZW_decode(const vector<int>& input) {
    unordered_map<int, string> dictionary;
    int dict_size = 256; // Начальный размер словаря

    // Инициализация словаря с ASCII символами
    for (int i = 0; i < 256; ++i) {
        string symbol(1, char(i));
        dictionary[i] = symbol;
    }

    string output;
    string prev_str;
    for (int code : input) {
        string current_str;
        if (dictionary.find(code) != dictionary.end()) {
            current_str = dictionary[code];
        } else if (code == dict_size) {
            current_str = prev_str + prev_str[0];
        } else {
            throw logic_error("Неверный ввод для декодирования LZW");
        }

        // Добавляем текущую строку в выход и обновляем словарь
        output += current_str;
        if (!prev_str.empty()) {
            dictionary[dict_size++] = prev_str + current_str[0];
        }
        prev_str = current_str;
    }

    return output;
}

int main() {
    string input;
    ifstream in;
    in.open("WnP.txt");
    getline (in, input, '\0' );
    in.close();

    ofstream out;
    out.open("output.txt");
    auto start = std::chrono::high_resolution_clock::now();
    vector<int> encoded = LZW_encode(input);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    for (int code : encoded) {
        out << code << " ";
    }
    cout << endl;
    out.close();

    ofstream dout;
    dout.open("decoded.txt");
    auto start2 = std::chrono::high_resolution_clock::now();
    string decoded = LZW_decode(encoded);
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration2 = end2 - start2;
    dout << decoded;
    dout.close();

    std::cout << std::fixed;
    cout << "encoding time: " << duration.count() << endl;
    cout << "decoding tinme: " << duration2.count() << endl;

    return 0;
}