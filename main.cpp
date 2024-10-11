#include <iostream>
#include <vector>
#include <string_view>

#include "trie.cpp"

std::vector<std::string_view> SplitWords(std::string_view text) {
    std::vector<std::string_view> words;
    std::size_t start = 0;
    
    while (start < text.size()) {
        std::size_t end = text.find(' ', start);
        
        if (end == std::string_view::npos) {
            words.emplace_back(text.substr(start));
            break;
        } else {
            if (end != start) {
                words.emplace_back(text.substr(start, end - start));
            }
        }
        
        start = end + 1;
    }

    return words;
}

int main() {
    Trie trie;

    std::string command;
    std::string text;
    while (true) {
        std::cin >> command;
        if (command == "add") {
            std::cin.ignore();
            std::getline(std::cin, text);

            for (auto word : SplitWords(text)) {
                trie.Add(word);
            }
            std::cout << "[ANS]Added successfully" << std::endl;
        } else if (command == "predict") {
            bool is_new = false;
            std::cin.ignore();
            std::getline(std::cin, text);

            if (text.find(" ") != std::string::npos) {
                is_new = true;
            }
            auto splitted = SplitWords(text);
            if (splitted.size() == 0) {
                std::cout << "[ANS]Got empty input" << std::endl;
            } else {
                std::cout << "[ANS]Suggested: ";
                std::cout << trie.Predict(*(splitted.end() - 1), is_new) << std::endl;
            }
        } else if (command == "exit") {
            std::cout << "[ANS]Программа успешно завершилась." << std::endl;
            return 0;
        } else {
            std::cout << "[ANS]Неверный формат запроса" << std::endl;
            std::cout << "[ANS]Была получена комманда " << command << std::endl;
        }
    }
}
