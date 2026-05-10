// Copyright 2025 Your Name
#ifndef INCLUDE_TEXTGEN_H_
#define INCLUDE_TEXTGEN_H_

#include <deque>
#include <map>
#include <random>
#include <string>
#include <vector>

const int NPREF = 2;
const int MAXGEN = 1500;
const char NOPREFIX[] = "\n";

typedef std::deque<std::string> prefix;
typedef std::map<prefix, std::vector<std::string>> statetab;

class TextGenerator {
 private:
  statetab table;
  prefix initialPrefix;
  std::mt19937 rng;

  int getRandomIndex(int max);
  std::string cleanWord(const std::string& word);
  bool isEndOfSentence(const std::string& word);

 public:
  TextGenerator();
  ~TextGenerator();

  void buildTable(std::istream& input);
  void addRecord(const prefix& pref, const std::string& suffix);
  void generateText(std::ostream& output, int maxWords = MAXGEN);

  prefix getInitialPrefix() const { return initialPrefix; }
  const statetab& getTable() const { return table; }
  std::string selectSuffix(const prefix& pref);
};

#endif  // INCLUDE_TEXTGEN_H_