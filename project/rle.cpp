#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;

// Функция для шифрования строки с использованием RLE
string encodeRLE(const string &input) {
    string encodedString = "";
    int count = 1;
    for (size_t i = 1; i <= input.length(); i++) {
        if (i == input.length() || input[i] != input[i - 1]) {
            if (count > 4) {
                encodedString += "@" + to_string(count) + "@" + input[i - 1];
            } else {
                encodedString += input[i - 1];
            }
            count = 1;
        } else {
            count++;
        }
    }
    return encodedString;
}

// Функция для дешифровки строки, учитывая разделение серий символов с помощью знака "@"
string decodeRLE(const string &input) {
    string decodedString = "";
    size_t pos = 0;
    while (pos < input.length()) {
        if (input[pos] == '@') {
            // Если следующий символ после "@" - число, то это длина серии
            if (isdigit(input[pos + 1])) {
                size_t endPos = input.find('@', pos + 1); // Ищем конец числа
                int count = stoi(input.substr(pos + 1, endPos - pos - 1)); // Получаем длину серии
                char repeatedChar = input[endPos + 1]; // Символ, повторяющийся в серии
                decodedString.append(count, repeatedChar); // Добавляем повторяющийся символ в декодированную строку
                pos = endPos + 2; // Пропускаем число и следующий "@" после него
            } else {
                decodedString += input[pos]; // Добавляем "@" в декодированную строку
                pos++;
            }
        } else {
            decodedString += input[pos]; // Добавляем текущий символ в декодированную строку
            pos++;
        }
    }
    return decodedString;
}

int main() {
    string inp = "WnP.txt";
    string inputFile = inp;
    string encodedFile = "output.txt";
    string decodedFile = inp;

    // Чтение из файла
    ifstream fin(inputFile);
    if (!fin.is_open()) {
        cerr << "Error: Unable to open input file." << endl;
        return 1;
    }

    string inputString;
    getline(fin, inputString, '\0');
    fin.close();

    // Шифрование
    auto start = std::chrono::high_resolution_clock::now();
    string encodedString = encodeRLE(inputString);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    // Запись в файл зашифрованной строки
    ofstream fout(encodedFile);
    if (!fout.is_open()) {
        cerr << "Error: Unable to open encoded file for writing." << endl;
        return 1;
    }
    fout << encodedString;
    fout.close();

    // Дешифрование
    auto start2 = std::chrono::high_resolution_clock::now();
    string decodedString = decodeRLE(encodedString);
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration2 = end2 - start2;

    // Запись в файл дешифрованной строки
    ofstream dout(decodedFile);
    if (!dout.is_open()) {
        cerr << "Error: Unable to open decoded file for writing." << endl;
        return 1;
    }
    dout << decodedString;
    dout.close();

    cout << "RLE Encoding and Decoding completed successfully." << endl;
    std::cout << std::fixed;
    cout << "encoding time: " << duration.count() << endl;
    cout << "decoding tinme: " << duration2.count() << endl;

    return 0;
}
