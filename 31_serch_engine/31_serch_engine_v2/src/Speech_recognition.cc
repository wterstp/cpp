#include "../head/Speech_recognition.h"


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
