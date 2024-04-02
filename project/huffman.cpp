#include <iostream>
#include <queue>
#include <unordered_map>
#include <fstream>
#include <chrono>

using namespace std;

// Структура для узла дерева Хаффмана
struct HuffmanNode {
    char data;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char data, int frequency) : data(data), frequency(frequency), left(nullptr), right(nullptr) {}
};

// Сравниваем узлы на основе частоты
struct Compare {
    bool operator()(HuffmanNode* left, HuffmanNode* right) {
        return left->frequency > right->frequency;
    }
};

// Функция для кодирования сообщения
unordered_map<char, string> huffmanEncode(HuffmanNode* root, string code = "") {
    unordered_map<char, string> huffmanCodes;
    if (root == nullptr)
        return huffmanCodes;

    // Если узел - лист, добавляем его код в таблицу кодирования
    if (!root->left && !root->right) {
        huffmanCodes[root->data] = code;
    }

    // Рекурсивно вызываем для левого и правого поддеревьев
    unordered_map<char, string> leftCodes = huffmanEncode(root->left, code + "0");
    unordered_map<char, string> rightCodes = huffmanEncode(root->right, code + "1");

    // Объединяем результаты
    huffmanCodes.insert(leftCodes.begin(), leftCodes.end());
    huffmanCodes.insert(rightCodes.begin(), rightCodes.end());

    return huffmanCodes;
}

// Функция для декодирования сообщения
string huffmanDecode(HuffmanNode* root, const string& encodedMessage) {
    string decodedMessage;
    HuffmanNode* current = root;

    // Проходим по закодированному сообщению
    for (char bit : encodedMessage) {
        if (bit == '0') {
            current = current->left;
        } else {
            current = current->right;
        }

        // Если дошли до листа, добавляем его данные в декодированное сообщение
        if (!current->left && !current->right) {
            decodedMessage += current->data;
            current = root; // Возвращаемся к корню для дальнейшего декодирования
        }
    }

    return decodedMessage;
}

// Функция для построения дерева Хаффмана и возврата корня
HuffmanNode* buildHuffmanTree(const unordered_map<char, int>& frequencies) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> pq;

    // Создаем листы для каждого символа и добавляем в приоритетную очередь
    for (auto& pair : frequencies) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }

    // Построение дерева
    while (pq.size() != 1) {
        HuffmanNode* left = pq.top();
        pq.pop();
        HuffmanNode* right = pq.top();
        pq.pop();

        int combinedFrequency = left->frequency + right->frequency;
        HuffmanNode* newNode = new HuffmanNode('\0', combinedFrequency);
        newNode->left = left;
        newNode->right = right;

        pq.push(newNode);
    }

    return pq.top();
}

int main() {
    string input;
    ifstream in;
    in.open("MnM.txt");
    getline (in, input, '\0' );
    in.close();
    // Вычисляем частоту каждого символа в сообщении

    auto start = std::chrono::high_resolution_clock::now();

    unordered_map<char, int> frequencies;
    for (char c : input) {
        frequencies[c]++;
    }

    // Строим дерево Хаффмана
    HuffmanNode* root = buildHuffmanTree(frequencies);

    // Получаем коды Хаффмана для каждого символа
    unordered_map<char, string> huffmanCodes = huffmanEncode(root);

    // Кодируем сообщение
    ofstream out;
    out.open("output.txt");
    string encodedMessage;
    for (char c : input) {
        encodedMessage += huffmanCodes[c];
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    out<<encodedMessage;

    auto start2 = std::chrono::high_resolution_clock::now();
    // Декодируем сообщение
    string decodedMessage = huffmanDecode(root, encodedMessage);
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration2 = end2 - start2;

    cout << "encoding time: " << duration.count() << endl;
    cout << "decoding tinme: " << duration2.count() << endl;

    return 0;
}