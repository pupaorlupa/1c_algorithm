#include <unordered_map>
#include <vector>

struct Trie {
private:
  struct Vertex {
    std::unordered_map<char, int> to;
    int terminal_score;
    int best_next_index;
    int best_next_score;
    char letter;

    Vertex(int index, char letter) : terminal_score(0), best_next_index(index), best_next_score(0), letter(letter) {}
  };

public:
  struct TrieState {
    std::string prefix;
    
    int vertex_index = 0;

    void Clear() {
      vertex_index = 0;
      prefix.clear();
    }
  };

  Trie() {
    trie.push_back(Vertex(0, '\0'));
  }

  /// Добавляет вершину в бор и пересчитывает максимумы предков в каждой вершине на пути.
  void Add(const std::string_view word) {
    state.Clear();

    std::vector<int> history; // будет явно запоминать историю использованных вершин для пересчета максимума, потому что рекурсия это плохо
                              // TODO: использовать StackVector длины ~6, потому что как правило слова не длинные
    history.reserve(word.size() + 1);

    int current_vertex = 0;
    for (size_t i = 0; i < word.size(); ++i) {
      history.push_back(current_vertex);

      auto place = trie[current_vertex].to.try_emplace(word[i]);
      if (place.second) {
        (*place.first).second = trie.size();
        trie.push_back(Vertex(trie.size(), word[i]));
      }
      current_vertex = (*(place.first)).second;
    }

    // последняя вершина - терминальная
    history.push_back(current_vertex);
    auto& vert = trie[current_vertex];
    ++vert.terminal_score;
    if (vert.best_next_score < vert.terminal_score) {
      vert.best_next_index = current_vertex;
      vert.best_next_score = vert.terminal_score;
    }
    
    // выполним обновление максимумов
    for (int i = history.size() - 2; i >= 0; --i) {
      int current_vertex_index = history[i];
      auto& vert = trie[current_vertex_index];

      int next_score = trie[history[i + 1]].best_next_score;
      if (vert.best_next_score < next_score) {
        vert.best_next_score = next_score;
        vert.best_next_index = history[i + 1];
      } else {
        break;
      }
    }
  }

  std::string Predict(std::string_view word, bool is_new) {
    if (is_new) {
      state.Clear();
    }

    // проходимся по бору и копим префикс
    int current_vertex_index = state.vertex_index;
    for (auto letter : word) {
      auto& vert = trie[current_vertex_index];
      auto next_vert = vert.to.find(letter);
      if (next_vert != vert.to.end()) {
        state.prefix += letter; 
        current_vertex_index = (*next_vert).second;
        state.vertex_index = current_vertex_index;
      } else {
        state.Clear();
        return "NO PREDICTIONS";
      }
    }

    std::string ret_prefix = state.prefix;

    // делаем предсказание
    while (current_vertex_index != trie[current_vertex_index].best_next_index) {
      current_vertex_index = trie[current_vertex_index].best_next_index;
      ret_prefix += trie[current_vertex_index].letter;
    }
    return ret_prefix;
  }

private:

  TrieState state;
  std::vector<Vertex> trie;
};