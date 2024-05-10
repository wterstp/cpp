#include <iostream>
#include <map>
#include <set>
#include <queue>
#include <algorithm>
#include <vector>
#include <string>

int minEditDist(const std::string& s1, const std::string& s2) {
    int m = s1.size(), n = s2.size();
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

    for (int i = 0; i <= m; i++)
        dp[i][0] = i;
    for (int j = 0; j <= n; j++)
        dp[0][j] = j;

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (s1[i - 1] == s2[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else
                dp[i][j] = std::min({ dp[i - 1][j] + 1, dp[i][j - 1] + 1, dp[i - 1][j - 1] + 1 });
        }
    }
    return dp[m][n];
}






class SearchServer {
private:
    std::map<std::string, int> dictionary; // 单词和词频的映射

public:
    SearchServer(const std::map<std::string, int>& dict): dictionary(dict) {}

    // 根据当前输入找到建议词
    std::vector<std::string> getSuggestions(const std::string& input) {
        std::vector<std::pair<std::string, int>> suggestions;

        // 检索所有单词并计算编辑距离
        for (const auto& [word, frequency] : dictionary) {
            int dist = minEditDist(input, word);
            if (dist <= input.length()) // 简单的启发式：距离小于等于输入长度
                suggestions.emplace_back(word, frequency);
        }

        // 根据词频排序
        std::sort(suggestions.begin(), suggestions.end(),
            [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
                return a.second > b.second;
            });

        std::vector<std::string> result;
        for (auto& [word, _] : suggestions)
            result.push_back(word);
        return result;
    }
};

int main() {
    std::map<std::string, int> dict = {{"hello", 100}, {"hell", 50}, {"helium", 30}, {"hi", 60}};
    SearchServer server(dict);

    std::string input;
    while (true) {
        std::cout << "Enter prefix: ";
        std::cin >> input;
        if (input == "exit") break;

        auto suggestions = server.getSuggestions(input);
        for (const auto& word : suggestions)
            std::cout << "Suggestion: " << word << std::endl;
    }
    return 0;
}
