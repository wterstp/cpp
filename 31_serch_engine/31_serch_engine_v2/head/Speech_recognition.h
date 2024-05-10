#ifndef SPEECH_RECOGNITION_H
#define SPEECH_RECOGNITION_H
#include <locale>
#include <codecvt>


  bool isChinese(char32_t ch) ;

bool isEnglish(const std::string &word);
#endif // SPEECH_RECOGNITION_H