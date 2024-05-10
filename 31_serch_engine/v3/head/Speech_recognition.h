#ifndef SPEECH_RECOGNITION_H
#define SPEECH_RECOGNITION_H
#include <locale>
#include <codecvt>


  bool isChinese(char32_t ch) ;

bool isEnglish(const std::string &word);
int minEditDist(const std::string& s1, const std::string& s2);
#endif // SPEECH_RECOGNITION_H