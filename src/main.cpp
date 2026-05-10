// Copyright 2025 Your Name
#include <fstream>
#include <iostream>
#include <string>

#include "textgen.h"

int main(int argc, char* argv[]) {
  std::string inputFile = "input.txt";
  std::string outputFile = "result/gen.txt";
  if (argc > 1) inputFile = argv[1];
  if (argc > 2) outputFile = argv[2];
  std::ifstream in(inputFile);
  if (!in.is_open()) return 1;
  TextGenerator gen;
  gen.buildTable(in);
  in.close();
  std::ofstream out(outputFile);
  if (!out.is_open()) return 1;
  gen.generateText(out, MAXGEN);
  out.close();
  return 0;
}
