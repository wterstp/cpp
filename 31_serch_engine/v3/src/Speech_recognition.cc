#include "../head/Speech_recognition.h"
#include<vector>
#include<algorithm>

  bool  isChinese(char32_t ch) {
    return ch >= 0x4E00 && ch <= 0x9FFF;
}

bool isEnglish(const std::string &word)
{ // 判断是否为英文
  for (char ch : word)
  {
    if (!((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')))
    {
      return false;
    }
  }
  return true;
}

int minEditDist(const std::string& s1, const std::string& s2) {
    // 使用 codecvt_utf8 转换 s1 和 s2 到 Unicode 码点序列
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    std::u32string u32s1 = converter.from_bytes(s1);
    std::u32string u32s2 = converter.from_bytes(s2);

    int m = u32s1.size(), n = u32s2.size();
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

    for (int i = 0; i <= m; i++)
        dp[i][0] = i;
    for (int j = 0; j <= n; j++)
        dp[0][j] = j;

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (u32s1[i - 1] == u32s2[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else
                dp[i][j] = std::min({dp[i - 1][j] + 1, dp[i][j - 1] + 1, dp[i - 1][j - 1] + 1});
        }
    }
    return dp[m][n];
}