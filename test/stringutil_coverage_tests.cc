#include "stringutil.h"
#include <gtest/gtest.h>

#include <set>
#include <string>
#include <vector>

using namespace util;

class StringUtilCoverageTest : public ::testing::Test
{
};

TEST_F(StringUtilCoverageTest, scan_bool_string_accepts_common_values)
{
    bool value = false;
    EXPECT_TRUE(scanBoolString(std::string{"TrUe"}, value));
    EXPECT_TRUE(value);

    EXPECT_TRUE(scanBoolString(std::string{"OFF"}, value));
    EXPECT_FALSE(value);

    value = true;
    EXPECT_FALSE(scanBoolString(std::string{"maybe"}, value));
    EXPECT_FALSE(value);
}

TEST_F(StringUtilCoverageTest, substr_from_to_inclusive_handles_bounds)
{
    const std::string text{"abcdef"};

    EXPECT_EQ(substr_from_to_incl(text, 1, 3), "bcd");
    EXPECT_EQ(substr_from_to_incl(text, 2, 100), "cdef");
    EXPECT_EQ(substr_from_to_incl(text, 7, 7), "");
    EXPECT_EQ(substr_from_to_incl(text, 4, 3), "");
}

TEST_F(StringUtilCoverageTest, split_into_vector_handles_empty_segments)
{
    const auto by_char = splitIntoVector(std::string{",a,,b,"}, ',');
    const std::vector<std::string> expected_by_char{"", "a", "", "b", ""};
    EXPECT_EQ(by_char, expected_by_char);

    const auto by_string = splitIntoVector(std::string{"||a||||b||"}, std::string{"||"});
    const std::vector<std::string> expected_by_string{"", "a", "", "b", ""};
    EXPECT_EQ(by_string, expected_by_string);

    const auto no_separator = splitIntoVector(std::string{"no-separator"}, '/');
    ASSERT_EQ(no_separator.size(), 1UL);
    EXPECT_EQ(no_separator[0], "no-separator");
}

TEST_F(StringUtilCoverageTest, split_into_set_deduplicates_and_keeps_empty)
{
    const auto values = splitIntoSet(std::string{"a,,a"}, ',');
    const std::set<std::string> expected{"", "a"};
    EXPECT_EQ(values, expected);
}

TEST_F(StringUtilCoverageTest, directional_replace_char_changes_expected_regions)
{
    std::string left{"  a b  "};
    replaceCharLeft(left, std::string{" "}, '_');
    EXPECT_EQ(left, "__a b  ");

    std::string right{"  a b  "};
    replaceChar(right, std::string{" "}, '_', StripTrimMode::RIGHT);
    EXPECT_EQ(right, "  a b__");

    std::string all_replace{"   "};
    replaceChar(all_replace, std::string{" "}, '_', StripTrimMode::RIGHT);
    EXPECT_EQ(all_replace, "___");
}

TEST_F(StringUtilCoverageTest, classify_number_string_covers_main_classes)
{
    EXPECT_EQ(classifyNumberString(std::string{""}), NumberClass::NONE);
    EXPECT_EQ(classifyNumberString(std::string{"12a"}), NumberClass::NONE);

    EXPECT_EQ(classifyNumberString(std::string{"-12"}), NumberClass::INT);
    EXPECT_EQ(classifyNumberString(std::string{"+12"}), NumberClass::INT);

    EXPECT_EQ(classifyNumberString(std::string{"12.3"}), NumberClass::FLOAT);
    EXPECT_EQ(classifyNumberString(std::string{"12e3"}), NumberClass::FLOAT);

    EXPECT_EQ(classifyNumberString(std::string{"18446744073709551615"}), NumberClass::UINT);
    EXPECT_EQ(classifyNumberString(std::string{"184467440737095516150"}), NumberClass::FLOAT);
}
