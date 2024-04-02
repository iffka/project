#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

// Функция кодирования алгоритма LZ78
vector<pair<int, char>> lz78_encode(const string& input) {
    unordered_map<string, int> dictionary;
    vector<pair<int, char>> encoded_data;

    int dict_size = 256; // Начальный размер словаря (ASCII символы)

    string current;
    for (char c : input) {
        current += c;
        // Если текущая последовательность символов не найдена в словаре
        if (dictionary.find(current) == dictionary.end()) {
            // Записываем в код последовательность, которая уже есть в словаре
            encoded_data.push_back({dictionary[current.substr(0, current.length() - 1)], c});
            // Добавляем новую последовательность в словарь
            dictionary[current] = dict_size++;
            current = "";
        }
    }

    // Обработка последнего символа
    if (!current.empty()) {
        encoded_data.push_back({dictionary[current], '\0'});
    }

    return encoded_data;
}

// Функция декодирования алгоритма LZ78
string lz78_decode(const vector<pair<int, char>>& encoded_data) {
    unordered_map<int, string> dictionary;
    string decoded_data;

    int dict_size = 256; // Начальный размер словаря (ASCII символы)

    for (const auto& p : encoded_data) {
        string entry;
        // Если код равен 0, значит это символ из алфавита
        if (p.first == 0) {
            decoded_data += p.second;
            entry = p.second;
        } else {
            // Иначе это комбинация из словаря
            entry = dictionary[p.first];
            entry += p.second;
            decoded_data += entry;
        }
        // Добавляем новую комбинацию в словарь
        dictionary[dict_size++] = entry;
    }

    return decoded_data;
}

// Функция чтения из файла и записи в файл закодированных данных
void lz78_encode_file(const string& input_filename, const string& output_filename) {
    ifstream input_file(input_filename);
    ofstream output_file(output_filename, ios::binary);

    if (!input_file.is_open()) {
        cerr << "Error: Could not open input file." << endl;
        return;
    }

    if (!output_file.is_open()) {
        cerr << "Error: Could not open output file." << endl;
        return;
    }

    string input_data((istreambuf_iterator<char>(input_file)), istreambuf_iterator<char>());

    auto encoded_data = lz78_encode(input_data);

    // Запись количества закодированных символов в начале файла
    int encoded_size = encoded_data.size();
    output_file.write(reinterpret_cast<const char*>(&encoded_size), sizeof(encoded_size));

    // Запись закодированных данных
    for (const auto& p : encoded_data) {
        output_file.write(reinterpret_cast<const char*>(&p.first), sizeof(p.first));
        output_file.write(reinterpret_cast<const char*>(&p.second), sizeof(p.second));
    }

    input_file.close();
    output_file.close();
}

// Функция декодирования из файла
void lz78_decode_file(const string& input_filename, const string& output_filename) {
    ifstream input_file(input_filename, ios::binary);
    ofstream output_file(output_filename);

    if (!input_file.is_open()) {
        cerr << "Error: Could not open input file." << endl;
        return;
    }

    if (!output_file.is_open()) {
        cerr << "Error: Could not open output file." << endl;
        return;
    }

    int encoded_size;
    input_file.read(reinterpret_cast<char*>(&encoded_size), sizeof(encoded_size));

    vector<pair<int, char>> encoded_data(encoded_size);
    for (int i = 0; i < encoded_size; ++i) {
        input_file.read(reinterpret_cast<char*>(&encoded_data[i].first), sizeof(encoded_data[i].first));
        input_file.read(reinterpret_cast<char*>(&encoded_data[i].second), sizeof(encoded_data[i].second));
    }

    string decoded_data = lz78_decode(encoded_data);
    output_file << decoded_data;

    input_file.close();
    output_file.close();
}

int main() {
    // Пример использования: кодирование и декодирование файла
    auto start = std::chrono::high_resolution_clock::now();
    lz78_encode_file("WnP.txt", "output.bin");
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    auto start2 = std::chrono::high_resolution_clock::now();
    lz78_decode_file("output.bin", "decoded.txt");
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration2 = end2 - start2;

    std::cout << std::fixed;
    cout << "encoding time: " << duration.count() << endl;
    cout << "decoding tinme: " << duration2.count() << endl;

    return 0;
}
