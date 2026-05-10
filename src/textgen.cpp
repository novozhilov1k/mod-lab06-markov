// Copyright 2025 Your Name
#include <algorithm>
#include <cctype>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "textgen.h"

TextGenerator::TextGenerator() : rng(std::random_device{}()) {
}

TextGenerator::~TextGenerator() {
}

int TextGenerator::getRandomIndex(int max) {
  if (max <= 0) return 0;
  std::uniform_int_distribution<int> dist(0, max - 1);
  return dist(rng);
}

std::string TextGenerator::cleanWord(const std::string& word) {
  std::string result;
  size_t start = 0;
  while (start < word.length() &&
         ispunct(static_cast<unsigned char>(word[start])) &&
         word[start] != '\'' && word[start] != '-')
    start++;
  size_t end = word.length();
  while (end > start &&
         ispunct(static_cast<unsigned char>(word[end - 1])) &&
         word[end - 1] != '\'' && word[end - 1] != '-')
    end--;
  result = word.substr(start, end - start);
  std::transform(result.begin(), result.end(), result.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  return result;
}

bool TextGenerator::isEndOfSentence(const std::string& word) {
  if (word.empty()) return false;
  char last = word.back();
  return (last == '.' || last == '!' || last == '?');
}

void TextGenerator::addRecord(const prefix& pref,
                               const std::string& suffix) {
  table[pref].push_back(suffix);
}

std::string TextGenerator::selectSuffix(const prefix& pref) {
  auto it = table.find(pref);
  if (it == table.end() || it->second.empty())
    return "";
  const auto& suffixes = it->second;
  return suffixes[getRandomIndex(suffixes.size())];
}

void TextGenerator::buildTable(std::istream& input) {
  std::string rawWord;
  std::vector<std::string> allWords;

  while (input >> rawWord) {
    bool endOfSentence = isEndOfSentence(rawWord);
    std::string cleaned = cleanWord(rawWord);
    if (!cleaned.empty()) {
      allWords.push_back(cleaned);
      if (endOfSentence)
        allWords.push_back(NOPREFIX);
    }
  }

  if (allWords.size() < static_cast<size_t>(NPREF + 1))
    return;

  initialPrefix.clear();
  for (int i = 0; i < NPREF; i++)
    initialPrefix.push_back(allWords[i]);

  for (size_t i = 0; i < allWords.size() - NPREF; i++) {
    prefix currentPref;
    for (int j = 0; j < NPREF; j++)
      currentPref.push_back(allWords[i + j]);
    addRecord(currentPref, allWords[i + NPREF]);
  }
}

void TextGenerator::generateText(std::ostream& output, int maxWords) {
  if (table.empty()) return;

  prefix currentPref = initialPrefix;
  int wordCount = 0;

  if (!currentPref.empty()) {
    std::string firstWord = currentPref[0];
    if (!firstWord.empty())
      firstWord[0] =
          static_cast<char>(std::toupper(
              static_cast<unsigned char>(firstWord[0])));
    output << firstWord;
    wordCount++;
  }

  for (size_t i = 1;
       i < currentPref.size() && wordCount < maxWords; i++) {
    if (currentPref[i] != NOPREFIX) {
      output << " " << currentPref[i];
      wordCount++;
    }
  }

  while (wordCount < maxWords) {
    std::string suffix = selectSuffix(currentPref);
    if (suffix.empty()) {
      auto it = table.begin();
      std::advance(it, getRandomIndex(table.size()));
      currentPref = it->first;
      continue;
    }
    if (suffix == NOPREFIX) {
      output << ".";
      auto it = table.begin();
      std::advance(it, getRandomIndex(table.size()));
      currentPref = it->first;
      if (wordCount < maxWords) {
        std::string nextFirst = currentPref[0];
        if (!nextFirst.empty())
          nextFirst[0] =
              static_cast<char>(std::toupper(
                  static_cast<unsigned char>(nextFirst[0])));
        output << " " << nextFirst;
        wordCount++;
      }
      for (size_t i = 1;
           i < currentPref.size() && wordCount < maxWords; i++) {
        if (currentPref[i] != NOPREFIX) {
          output << " " << currentPref[i];
          wordCount++;
        }
      }
      continue;
    }
    output << " " << suffix;
    wordCount++;
    currentPref.pop_front();
    currentPref.push_back(suffix);
  }

  output << std::endl;
}
