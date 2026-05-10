#include "textgen.h"
#include <gtest/gtest.h>
#include <sstream>
#include <cctype>

TEST(TextGeneratorTest, InitPrefixCorrectSize) {
    TextGenerator gen;
    std::stringstream ss("cat dog bird fish cow");
    gen.buildTable(ss);
    prefix p = gen.getInitialPrefix();
    ASSERT_EQ(p.size(), (size_t)NPREF);
    EXPECT_EQ(p[0], "cat");
    EXPECT_EQ(p[1], "dog");
}

TEST(TextGeneratorTest, AddAndFindSuffix) {
    TextGenerator gen;
    prefix pr = {"sun", "moon"};
    gen.addRecord(pr, "stars");
    const auto& tab = gen.getTable();
    auto it = tab.find(pr);
    ASSERT_NE(it, tab.end());
    EXPECT_EQ(it->second.size(), 1);
    EXPECT_EQ(it->second[0], "stars");
}

TEST(TextGeneratorTest, OnlyOneSuffixReturnsIt) {
    TextGenerator gen;
    gen.addRecord({"red", "blue"}, "green");
    EXPECT_EQ(gen.selectSuffix({"red", "blue"}), "green");
}

TEST(TextGeneratorTest, MultipleSuffixesReturnValid) {
    TextGenerator gen;
    gen.addRecord({"up", "down"}, "left");
    gen.addRecord({"up", "down"}, "right");
    gen.addRecord({"up", "down"}, "center");
    std::string res = gen.selectSuffix({"up", "down"});
    EXPECT_TRUE(res == "left" || res == "right" || res == "center");
}

TEST(TextGeneratorTest, GenerateExactWordCount) {
    TextGenerator gen;
    std::stringstream ss("a1 a2 a3 a4 a5 a6 a7");
    gen.buildTable(ss);
    std::stringstream out;
    gen.generateText(out, 4);
    std::string txt = out.str();
    std::stringstream cnt(txt);
    int words = 0;
    std::string w;
    while (cnt >> w) words++;
    EXPECT_EQ(words, 4);
}

TEST(TextGeneratorTest, EmptyStreamMakesEmptyTable) {
    TextGenerator gen;
    std::stringstream ss("");
    gen.buildTable(ss);
    EXPECT_TRUE(gen.getTable().empty());
}

TEST(TextGeneratorTest, TwoWordsNotEnough) {
    TextGenerator gen;
    std::stringstream ss("hello world");
    gen.buildTable(ss);
    EXPECT_TRUE(gen.getTable().empty());
}

TEST(TextGeneratorTest, UnknownPrefixReturnsEmpty) {
    TextGenerator gen;
    EXPECT_EQ(gen.selectSuffix({"xyz", "abc"}), "");
}

TEST(TextGeneratorTest, SamePrefixStoresAllSuffixes) {
    TextGenerator gen;
    gen.addRecord({"go", "to"}, "school");
    gen.addRecord({"go", "to"}, "park");
    gen.addRecord({"go", "to"}, "shop");
    auto it = gen.getTable().find({"go", "to"});
    ASSERT_NE(it, gen.getTable().end());
    EXPECT_EQ(it->second.size(), 3);
}

TEST(TextGeneratorTest, OutputStartsWithCapital) {
    TextGenerator gen;
    std::stringstream ss("begin middle end extra more");
    gen.buildTable(ss);
    std::stringstream out;
    gen.generateText(out, 3);
    std::string res = out.str();
    ASSERT_FALSE(res.empty());
    EXPECT_TRUE(std::isupper(static_cast<unsigned char>(res[0])));
}