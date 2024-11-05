/*
 * Repository:  https://github.com/kingkybel/StringUtilities
 * File Name:   test/stringutil_tests.cc
 * Description: Unit tests for string utilities
 *
 * Copyright (C) 2024 Dieter J Kybelksties <github@kybelksties.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * @date: 2024-11-04
 * @author: Dieter J Kybelksties
 */

#include "ci_string.h"
#include "cstdlib"
#include "stringutil.h"

#include <dkyb/container_convert.h>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <utility>

using namespace std;
using namespace util;

string const filename = "/tmp/test.csv";

class StringUtilTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

template <typename T_>
struct SR
{
    SR(T_            source,
       T_            tp,
       StripTrimMode m,
       T_            modChars,
       char          c,
       T_            result,
       size_t        line,
       bool          differentInsensitive = false,
       const T_& resultInsensitive        = "")
        : source_(std::move(source))
        , tp_(std::move(tp))
        , m_(m)
        , modChars_(std::move(modChars))
        , c_(c)
        , result_(std::move(result))
        , line_(line)
        , differentInsensitive_(differentInsensitive)
        , resultInsensitive_(differentInsensitive ? resultInsensitive : result_)
    {
    }

    bool correctResult()
    {
        T_ actualResult = source_;
        if (tp_ == "trim")
        {
            if (m_ == util::StripTrimMode::LEFT)
            {
                trimLeft(actualResult, modChars_);
            }
            else if (m_ == util::StripTrimMode::RIGHT)
            {
                trimRight(actualResult, modChars_);
            }
            else
            {
                trim(actualResult, modChars_);
            }
        }
        else if (tp_ == "strip")
        {
            strip(actualResult, modChars_, m_);
        }
        else if (tp_ == "replace")
        {
            replaceChar(actualResult, modChars_, c_, m_);
        }
        T_   expectedResult = (typeid(T_) == typeid(ci_string) && differentInsensitive_) ? resultInsensitive_ : result_;
        bool reval          = (expectedResult == actualResult);
        if (!reval)
        {
            cout << " Line " << line_ << ": " << quoted(expectedResult) << " != " << quoted(actualResult) << " "
                 << ends;
        }
        return reval;
    }

    T_            source_;
    T_            tp_;
    StripTrimMode m_;
    T_            modChars_;
    char          c_;
    T_            result_;
    size_t        line_;
    bool          differentInsensitive_;
    T_            resultInsensitive_;
};

template <typename T_>
void util_string_mod_testT()
{
    typedef SR<T_> SR;
    SR             modResults[] = {
        // trivial
        SR("", "trim", StripTrimMode::ALL, "\n\t \r", char(0), "", __LINE__),
        SR("", "trim", StripTrimMode::LEFT, "\n\t \r", char(0), "", __LINE__),
        SR("", "trim", StripTrimMode::RIGHT, "\n\t \r", char(0), "", __LINE__),
        SR(" ", "trim", StripTrimMode::ALL, "\n\t \r", char(0), "", __LINE__),
        SR(" ", "trim", StripTrimMode::LEFT, "\n\t \r", char(0), "", __LINE__),
        SR(" ", "trim", StripTrimMode::RIGHT, "\n\t \r", char(0), "", __LINE__),
        SR("\t", "trim", StripTrimMode::ALL, "\n\t \r", char(0), "", __LINE__),
        SR("\t", "trim", StripTrimMode::LEFT, "\n\t \r", char(0), "", __LINE__),
        SR("\t", "trim", StripTrimMode::RIGHT, "\n\t \r", char(0), "", __LINE__),
        SR("\n", "trim", StripTrimMode::ALL, "\n\t \r", char(0), "", __LINE__),
        SR("\n", "trim", StripTrimMode::LEFT, "\n\t \r", char(0), "", __LINE__),
        SR("\n", "trim", StripTrimMode::RIGHT, "\n\t \r", char(0), "", __LINE__),

        SR("", "strip", StripTrimMode::ALL, "\n\t \r", char(0), "", __LINE__),
        SR("", "strip", StripTrimMode::LEFT, "\n\t \r", char(0), "", __LINE__),
        SR("", "strip", StripTrimMode::RIGHT, "\n\t \r", char(0), "", __LINE__),
        SR(" ", "strip", StripTrimMode::ALL, "\n\t \r", char(0), "", __LINE__),
        SR(" ", "strip", StripTrimMode::LEFT, "\n\t \r", char(0), "", __LINE__),
        SR(" ", "strip", StripTrimMode::RIGHT, "\n\t \r", char(0), "", __LINE__),
        SR("\t", "strip", StripTrimMode::ALL, "\n\t \r", char(0), "", __LINE__),
        SR("\t", "strip", StripTrimMode::LEFT, "\n\t \r", char(0), "", __LINE__),
        SR("\t", "strip", StripTrimMode::RIGHT, "\n\t \r", char(0), "", __LINE__),
        SR("\n", "strip", StripTrimMode::ALL, "\n\t \r", char(0), "", __LINE__),
        SR("\n", "strip", StripTrimMode::LEFT, "\n\t \r", char(0), "", __LINE__),
        SR("\n", "strip", StripTrimMode::RIGHT, "\n\t \r", char(0), "", __LINE__),

        SR("", "replace", StripTrimMode::ALL, "\n\t \r", '#', "", __LINE__),
        SR("", "replace", StripTrimMode::LEFT, "\n\t \r", '#', "", __LINE__),
        SR("", "replace", StripTrimMode::RIGHT, "\n\t \r", '#', "", __LINE__),
        SR(" ", "replace", StripTrimMode::ALL, "\n\t \r", '#', "#", __LINE__, false),
        SR(" ", "replace", StripTrimMode::LEFT, "\n\t \r", '#', "#", __LINE__),
        SR(" ", "replace", StripTrimMode::RIGHT, "\n\t \r", '#', "#", __LINE__),
        SR("\t", "replace", StripTrimMode::ALL, "\n\t \r", '#', "#", __LINE__),
        SR("\t", "replace", StripTrimMode::LEFT, "\n\t \r", '#', "#", __LINE__),
        SR("\t", "replace", StripTrimMode::RIGHT, "\n\t \r", '#', "#", __LINE__),
        SR("\n", "replace", StripTrimMode::ALL, "\n\t \r", '#', "#", __LINE__),
        SR("\n", "replace", StripTrimMode::LEFT, "\n\t \r", '#', "#", __LINE__),
        SR("\n", "replace", StripTrimMode::RIGHT, "\n\t \r", '#', "#", __LINE__),

        // trivial case-dependent
        SR("", "trim", StripTrimMode::ALL, "abc", char(0), "", __LINE__, false, ""),
        SR("", "trim", StripTrimMode::LEFT, "abc", char(0), "", __LINE__, false, ""),
        SR("", "trim", StripTrimMode::RIGHT, "abc", char(0), "", __LINE__, false, ""),
        SR("a", "trim", StripTrimMode::ALL, "abc", char(0), "", __LINE__, false, ""),
        SR("a", "trim", StripTrimMode::LEFT, "abc", char(0), "", __LINE__, false, ""),
        SR("a", "trim", StripTrimMode::RIGHT, "abc", char(0), "", __LINE__, false, ""),
        SR("b", "trim", StripTrimMode::ALL, "abc", char(0), "", __LINE__, false, ""),
        SR("b", "trim", StripTrimMode::LEFT, "abc", char(0), "", __LINE__, false, ""),
        SR("b", "trim", StripTrimMode::RIGHT, "abc", char(0), "", __LINE__, false, ""),
        SR("c", "trim", StripTrimMode::ALL, "abc", char(0), "", __LINE__, false, ""),
        SR("c", "trim", StripTrimMode::LEFT, "abc", char(0), "", __LINE__, false, ""),
        SR("c", "trim", StripTrimMode::RIGHT, "abc", char(0), "", __LINE__, false, ""),
        SR("A", "trim", StripTrimMode::ALL, "abc", char(0), "A", __LINE__, true, ""),
        SR("A", "trim", StripTrimMode::LEFT, "abc", char(0), "A", __LINE__, true, ""),
        SR("A", "trim", StripTrimMode::RIGHT, "abc", char(0), "A", __LINE__, true, ""),
        SR("B", "trim", StripTrimMode::ALL, "abc", char(0), "B", __LINE__, true, ""),
        SR("B", "trim", StripTrimMode::LEFT, "abc", char(0), "B", __LINE__, true, ""),
        SR("B", "trim", StripTrimMode::RIGHT, "abc", char(0), "B", __LINE__, true, ""),
        SR("C", "trim", StripTrimMode::ALL, "abc", char(0), "C", __LINE__, true, ""),
        SR("C", "trim", StripTrimMode::LEFT, "abc", char(0), "C", __LINE__, true, ""),
        SR("C", "trim", StripTrimMode::RIGHT, "abc", char(0), "C", __LINE__, true, ""),

        SR("", "strip", StripTrimMode::ALL, "abc", char(0), "", __LINE__, false, ""),
        SR("", "strip", StripTrimMode::LEFT, "abc", char(0), "", __LINE__, false, ""),
        SR("", "strip", StripTrimMode::RIGHT, "abc", char(0), "", __LINE__, false, ""),
        SR("a", "strip", StripTrimMode::ALL, "abc", char(0), "", __LINE__, false, ""),
        SR("a", "strip", StripTrimMode::LEFT, "abc", char(0), "", __LINE__, false, ""),
        SR("a", "strip", StripTrimMode::RIGHT, "abc", char(0), "", __LINE__, ""),
        SR("b", "strip", StripTrimMode::ALL, "abc", char(0), "", __LINE__, false, ""),
        SR("b", "strip", StripTrimMode::LEFT, "abc", char(0), "", __LINE__, false, ""),
        SR("b", "strip", StripTrimMode::RIGHT, "abc", char(0), "", __LINE__, false, ""),
        SR("c", "strip", StripTrimMode::ALL, "abc", char(0), "", __LINE__, false, ""),
        SR("c", "strip", StripTrimMode::LEFT, "abc", char(0), "", __LINE__, false, ""),
        SR("c", "strip", StripTrimMode::RIGHT, "abc", char(0), "", __LINE__, false, ""),
        SR("A", "strip", StripTrimMode::ALL, "abc", char(0), "A", __LINE__, true, ""),
        SR("A", "strip", StripTrimMode::LEFT, "abc", char(0), "A", __LINE__, true, ""),
        SR("A", "strip", StripTrimMode::RIGHT, "abc", char(0), "A", __LINE__, true, ""),
        SR("B", "strip", StripTrimMode::ALL, "abc", char(0), "B", __LINE__, true, ""),
        SR("B", "strip", StripTrimMode::LEFT, "abc", char(0), "B", __LINE__, true, ""),
        SR("B", "strip", StripTrimMode::RIGHT, "abc", char(0), "B", __LINE__, true, ""),
        SR("C", "strip", StripTrimMode::ALL, "abc", char(0), "C", __LINE__, true, ""),
        SR("C", "strip", StripTrimMode::LEFT, "abc", char(0), "C", __LINE__, true, ""),
        SR("C", "strip", StripTrimMode::RIGHT, "abc", char(0), "C", __LINE__, true, ""),

        SR("", "replace", StripTrimMode::ALL, "abc", '#', "", __LINE__, false, ""),
        SR("", "replace", StripTrimMode::LEFT, "abc", '#', "", __LINE__, false, ""),
        SR("", "replace", StripTrimMode::RIGHT, "abc", '#', "", __LINE__, false, ""),
        SR("a", "replace", StripTrimMode::ALL, "abc", '#', "#", __LINE__, false, "#"),
        SR("a", "replace", StripTrimMode::LEFT, "abc", '#', "#", __LINE__, false, "#"),
        SR("a", "replace", StripTrimMode::RIGHT, "abc", '#', "#", __LINE__, false, "#"),
        SR("b", "replace", StripTrimMode::ALL, "abc", '#', "#", __LINE__, false, "#"),
        SR("b", "replace", StripTrimMode::LEFT, "abc", '#', "#", __LINE__, false, "#"),
        SR("b", "replace", StripTrimMode::RIGHT, "abc", '#', "#", __LINE__, false, "#"),
        SR("c", "replace", StripTrimMode::ALL, "abc", '#', "#", __LINE__, false, "#"),
        SR("c", "replace", StripTrimMode::LEFT, "abc", '#', "#", __LINE__, false, "#"),
        SR("c", "replace", StripTrimMode::RIGHT, "abc", '#', "#", __LINE__, false, "#"),
        SR("A", "replace", StripTrimMode::ALL, "abc", '#', "A", __LINE__, true, "#"),
        SR("A", "replace", StripTrimMode::LEFT, "abc", '#', "A", __LINE__, true, "#"),
        SR("A", "replace", StripTrimMode::RIGHT, "abc", '#', "A", __LINE__, true, "#"),
        SR("B", "replace", StripTrimMode::ALL, "abc", '#', "B", __LINE__, true, "#"),
        SR("B", "replace", StripTrimMode::LEFT, "abc", '#', "B", __LINE__, true, "#"),
        SR("B", "replace", StripTrimMode::RIGHT, "abc", '#', "B", __LINE__, true, "#"),
        SR("C", "replace", StripTrimMode::ALL, "abc", '#', "C", __LINE__, true, "#"),
        SR("C", "replace", StripTrimMode::LEFT, "abc", '#', "C", __LINE__, true, "#"),
        SR("C", "replace", StripTrimMode::RIGHT, "abc", '#', "C", __LINE__, true, "#"),

        // not-so-trivial case-dependent
        SR("aABbCc", "trim", StripTrimMode::ALL, "abc", char(0), "ABbC", __LINE__, true, ""),
        /**/ SR("aABbCc", "trim", StripTrimMode::LEFT, "abc", char(0), "ABbCc", __LINE__, true, ""),
        SR("aABbCc", "trim", StripTrimMode::RIGHT, "abc", char(0), "aABbC", __LINE__, true, ""),

        SR("aABbCc", "strip", StripTrimMode::ALL, "abc", char(0), "ABC", __LINE__, true, ""),
        SR("aABbCc", "strip", StripTrimMode::LEFT, "abc", char(0), "ABbCc", __LINE__, true, ""),
        SR("aABbCc", "strip", StripTrimMode::RIGHT, "abc", char(0), "aABbC", __LINE__, true, ""),

        SR("aABbCc", "replace", StripTrimMode::ALL, "abc", '#', "#AB#C#", __LINE__, true, "######"),
        SR("aABbCc", "replace", StripTrimMode::LEFT, "abc", '#', "#ABbCc", __LINE__, true, "######"),
        SR("aABbCc", "replace", StripTrimMode::RIGHT, "abc", '#', "aABbC#", __LINE__, true, "######"),

        SR("a-A-B-b-c-C", "trim", StripTrimMode::ALL, "abc", char(0), "-A-B-b-c-C", __LINE__, true, "-A-B-b-c-"),
        /**/ SR("a-A-B-b-c-C", "trim", StripTrimMode::LEFT, "abc", char(0), "-A-B-b-c-C", __LINE__, true, "-A-B-b-c-C"),
        SR("a-A-B-b-c-C", "trim", StripTrimMode::RIGHT, "abc", char(0), "a-A-B-b-c-C", __LINE__, true, "a-A-B-b-c-"),

        SR("a-A-B-b-c-C", "strip", StripTrimMode::ALL, "abc", char(0), "-A-B---C", __LINE__, true, "-----"),
        SR("a-A-B-b-c-C", "strip", StripTrimMode::LEFT, "abc", char(0), "-A-B-b-c-C", __LINE__, true, "-A-B-b-c-C"),
        SR("a-A-B-b-c-C", "strip", StripTrimMode::RIGHT, "abc", char(0), "a-A-B-b-c-C", __LINE__, true, "a-A-B-b-c-"),

        SR("a-A-B-b-c-C", "replace", StripTrimMode::ALL, "abc", '#', "#-A-B-#-#-C", __LINE__, true, "#-#-#-#-#-#"),
        SR("a-A-B-b-c-C", "replace", StripTrimMode::LEFT, "abc", '#', "#-A-B-b-c-C", __LINE__, true, "#-A-B-b-c-C"),
        SR("a-A-B-b-c-C", "replace", StripTrimMode::RIGHT, "abc", '#', "a-A-B-b-c-C", __LINE__, true, "a-A-B-b-c-#"),
    };
    for (size_t i = 0; i < sizeof(modResults) / sizeof(SR); i++)
    {
        ASSERT_TRUE(modResults[i].correctResult());
    }
}

TEST_F(StringUtilTest, util_ci_traits_test)
{
    ci_char_traits tr;
    ASSERT_TRUE(tr.eq('a', 'a'));
    ASSERT_TRUE(tr.eq('a', 'A'));
    ASSERT_TRUE(tr.ne('a', 'B'));
    ASSERT_TRUE(tr.lt('a', 'B'));

    ASSERT_EQ(tr.compare(nullptr, nullptr, 2), 0);
    ASSERT_EQ(tr.compare(nullptr, "a", 2), -1);
    ASSERT_EQ(tr.compare("Aa", "aA", 2), 0);
    ASSERT_EQ(tr.compare("Aa", "bA", 2), -1);
    ASSERT_EQ(tr.compare("Ba", "aA", 2), 1);
    ASSERT_EQ(tr.compare("aaaBa", "AAAaA", 5), 4);
    ASSERT_EQ(tr.compare("aaaaa", "AAABA", 5), -4);

    ASSERT_EQ(tr.compare(nullptr, nullptr, 0), 0);
    ASSERT_EQ(tr.compare(nullptr, "a", 0), 0);
    ASSERT_EQ(tr.compare("Aa", "aA", 0), 0);
    ASSERT_EQ(tr.compare("Aa", "bA", 0), 0);
    ASSERT_EQ(tr.compare("Ba", "aA", 0), 0);
    ASSERT_EQ(tr.compare("aaaBa", "AAAaA", 0), 0);
    ASSERT_EQ(tr.compare("aaaaa", "AAABA", 0), 0);

    ASSERT_EQ(tr.compare(nullptr, nullptr, 1), 0);
    ASSERT_EQ(tr.compare(nullptr, "a", 1), -1);
    ASSERT_EQ(tr.compare("Aa", "aA", 1), 0);
    ASSERT_EQ(tr.compare("Aa", "bA", 1), -1);
    ASSERT_EQ(tr.compare("Ba", "aA", 1), 1);
    ASSERT_EQ(tr.compare("aaaBa", "AAAaA", 1), 0);
    ASSERT_EQ(tr.compare("aaaaa", "AAABA", 1), 0);

    ASSERT_EQ(tr.compare(nullptr, nullptr, 10), 0);
    ASSERT_EQ(tr.compare(nullptr, "a", 10), -1);
    ASSERT_EQ(tr.compare("Aa", "aA", 10), 0);
    ASSERT_EQ(tr.compare("Aa", "bA", 10), -1);
    ASSERT_EQ(tr.compare("Ba", "aA", 10), 1);
    ASSERT_EQ(tr.compare("aaaBa", "AAAaA", 10), 4);
    ASSERT_EQ(tr.compare("aaaaa", "AAABA", 10), -4);
}

TEST_F(StringUtilTest, util_container_conversion_test)
{
    set<string> sSet;
    ASSERT_TRUE(sSet.empty());
    vector<string> sVec = toVector(sSet);
    ASSERT_TRUE(sVec.empty());
    sSet = toSet(sVec);
    ASSERT_TRUE(sSet.empty());

    sSet.insert("1stString");
    sSet.insert("2ndString");
    ASSERT_TRUE(!sSet.empty());
    ASSERT_EQ(sSet.size(), 2UL);
    sVec = toVector(sSet);
    ASSERT_TRUE(!sVec.empty());
    ASSERT_EQ(sVec.size(), 2UL);
    sSet = toSet(sVec);
    ASSERT_TRUE(!sSet.empty());
    ASSERT_EQ(sSet.size(), 2UL);
}

template <typename T_>
void util_string_testT()
{
    T_ trimstring;
    trim(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "");
    trimstring = " ";
    trim(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "");
    trimstring = "\t";
    trim(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "");
    trimstring = "\n";
    trim(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "");
    trimstring = "\r";
    trim(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "");
    trimstring = " \r\n ";
    trim(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "");
    trimstring = " \r\t\t \n ";
    trim(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "");
    trimstring = "a";
    trim(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "a");
    trimstring = "\ta";
    trim(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "a");
    trimstring = "a\t";
    trim(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "a");
    trimstring = "\n\t";
    trim(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "");
    trimstring = "\n";
    trim(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "");
    trimstring = "\r";
    trim(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "");
    trimstring = " \r\n ";
    trim(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "");
    trimstring = " \r\t\t \n ";
    trim(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "");
    trimstring = " \r\ta\t \n ";
    trim(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "a");
    trimstring = "\na";
    trim(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "a");
    trimstring = "a\t   ";
    trim(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "a");
    trimstring = "\t\t\t\ta     ";
    trim(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "a");

    T_ source = "123/3456/7890a";
    // ("split " << source << " into vector by '/'");
    vector<T_> result = splitIntoVector(source, '/');
    ASSERT_EQ(result.size(), 3UL);
    ASSERT_EQ(result[0], "123");
    ASSERT_EQ(result[1], "3456");
    ASSERT_EQ(result[2], "7890a");

    // ("split " << source << " into vector by '.'");
    result = splitIntoVector(source, '.');
    ASSERT_EQ(result.size(), 1UL);
    ASSERT_EQ(result[0], "123/3456/7890a");

    // ("split " << source << " into vector by \"37\"");
    result = splitIntoVector(source, '7');
    ASSERT_EQ(result.size(), 2UL);
    ASSERT_EQ(result[0], "123/3456/");
    ASSERT_EQ(result[1], "890a");

    source = "1/4/7/1/7";
    // ("split " << source << " into vector by \"/\"");
    result = splitIntoVector(source, "/");
    ASSERT_EQ(result.size(), 5UL);
    ASSERT_EQ(result[0], "1");
    ASSERT_EQ(result[1], "4");
    ASSERT_EQ(result[2], "7");
    ASSERT_EQ(result[3], "1");
    ASSERT_EQ(result[4], "7");

    source = "123/456/789/123/789";
    // ("split " + source + " into set by '/'");
    set<T_> resultSet = splitIntoSet(source, '/');
    ASSERT_EQ(resultSet.size(), 3UL);
    result = toVector(resultSet);
    ASSERT_EQ(result.size(), 3UL);
    ASSERT_EQ(result[0], "123");
    ASSERT_EQ(result[1], "456");
    ASSERT_EQ(result[2], "789");

    // ("split " << source << " into set by '.'");
    resultSet = splitIntoSet(source, '.');
    ASSERT_EQ(resultSet.size(), 1UL);
    result = toVector(resultSet);
    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0], "123/456/789/123/789");

    // ("split " << source << " into set by \"37\"");
    resultSet = splitIntoSet(source, "1");
    ASSERT_EQ(resultSet.size(), 3UL);
    result = toVector(resultSet);
    ASSERT_EQ(result.size(), 3UL);
    ASSERT_NE(resultSet.find(""), resultSet.end());
    ASSERT_NE(resultSet.find("23/456/789/"), resultSet.end());
    ASSERT_NE(resultSet.find("23/789"), resultSet.end());

    const T_ stripStr  = " _ 123.456/789-0ab/_ _";
    T_       stripable = stripStr;

    // ("strip " << stripable << " of \".\"");
    strip(stripable, ".");
    ASSERT_EQ(stripable, " _ 123456/789-0ab/_ _");
    stripable = stripStr;
    // ("strip " << stripable << " of \"/\"");
    strip(stripable, "/");
    ASSERT_EQ(stripable, " _ 123.456789-0ab_ _");
    stripable = stripStr;
    // ("strip " << stripable << " of \"./\"");
    strip(stripable, "./");
    ASSERT_EQ(stripable, " _ 123456789-0ab_ _");

    stripable = stripStr;
    // ("trim " << stripable << " of \" \"");
    trim(stripable, " ");
    ASSERT_EQ(stripable, "_ 123.456/789-0ab/_ _");
    stripable = stripStr;
    // ("trim " << stripable << " of \"_\"");
    trim(stripable, "_");
    ASSERT_EQ(stripable, " _ 123.456/789-0ab/_ ");

    stripable = stripStr;
    // ("trim " << stripable << " of \" _\"");
    trim(stripable, " _");
    ASSERT_EQ(stripable, "123.456/789-0ab/");

    stripable = stripStr; // " _ 123.456/789-0ab/_ _"
    // ("replaceChar " << stripable << " chars \"_\" with '#'");
    replaceChar(stripable, "_", '#');
    ASSERT_EQ(stripable, " # 123.456/789-0ab/# #");
    stripable = stripStr; // " _ 123.456/789-0ab/_ _"
    // ("replaceChar " << stripable << " chars \" _\" with '#'");
    replaceChar(stripable, "_ ", '#');
    ASSERT_EQ(stripable, "###123.456/789-0ab/###");

    ASSERT_EQ(toLower(T_("SoMeStRiNg")), T_("somestring"));
    ASSERT_EQ(toUpper(T_("SoMeStRiNg")), T_("SOMESTRING"));
}

template <typename T_>
void util_string_left_right_testT()
{
    T_ trimstring;
    trimLeft(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "");
    trimstring = "";
    trimRight(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "");

    trimstring = " ";
    trimLeft(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "");
    trimstring = " ";
    trimRight(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "");

    trimstring = "\t";
    trimLeft(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "");
    trimstring = "\t";
    trimRight(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "");

    trimstring = "\t";
    trimLeft(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "");
    trimstring = "\t";
    trimRight(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "");

    trimstring = "\t";
    trim(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "");
    trimstring = "\n";
    trim(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "");
    trimstring = "\r";
    trim(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "");
    trimstring = " \r\n ";
    trim(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "");
    trimstring = " \r\t\t \n ";
    trim(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "");
    trimstring = "a";
    trim(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "a");
    trimstring = "\ta";
    trim(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "a");
    trimstring = "a\t";
    trim(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "a");
    trimstring = "\ta\n";
    trim(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "a");
    trimstring = "\na";
    trim(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "a");
    trimstring = "a\t   ";
    trim(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "a");
    trimstring = "\t\t\t\ta     ";
    trim(trimstring, " \n\t\r");
    ASSERT_EQ(trimstring, "a");

    T_ source = "123/3456/7890a";
    // ("split " << source << " into vector by '/'");
    vector<T_> result = splitIntoVector(source, '/');
    ASSERT_EQ(result.size(), 3UL);
    ASSERT_EQ(result[0], "123");
    ASSERT_EQ(result[1], "3456");
    ASSERT_EQ(result[2], "7890a");

    // ("split " << source << " into vector by '.'");
    result = splitIntoVector(source, '.');
    ASSERT_EQ(result.size(), 1UL);
    ASSERT_EQ(result[0], "123/3456/7890a");

    // ("split " << source << " into vector by \"37\"");
    result = splitIntoVector(source, "34");
    ASSERT_EQ(result.size(), 2UL);
    ASSERT_EQ(result[0], "123/");
    ASSERT_EQ(result[1], "56/7890a");

    source = "1/4/7/1/7";
    // ("split " << source << " into vector by \"/\"");
    result = splitIntoVector(source, "/");
    ASSERT_EQ(result.size(), 5UL);
    ASSERT_EQ(result[0], "1");
    ASSERT_EQ(result[1], "4");
    ASSERT_EQ(result[2], "7");
    ASSERT_EQ(result[3], "1");
    ASSERT_EQ(result[4], "7");

    source = "123/456/789/123/789";
    // ("split " + source + " into set by '/'");
    set<T_> resultSet = splitIntoSet(source, '/');
    ASSERT_EQ(resultSet.size(), 3UL);
    result = toVector(resultSet);
    ASSERT_EQ(result.size(), 3UL);
    ASSERT_EQ(result[0], "123");
    ASSERT_EQ(result[1], "456");
    ASSERT_EQ(result[2], "789");

    // ("split " << source << " into set by '.'");
    resultSet = splitIntoSet(source, '.');
    ASSERT_EQ(resultSet.size(), 1UL);
    result = toVector(resultSet);
    ASSERT_EQ(result.size(), 1UL);
    ASSERT_EQ(result[0], "123/456/789/123/789");

    // (split  source  into set by "/");
    resultSet = splitIntoSet(source, "/");
    ASSERT_EQ(resultSet.size(), 3UL);
    ASSERT_EQ(resultSet.find("/"), resultSet.end());
    ASSERT_NE(resultSet.find("123"), resultSet.end());
    ASSERT_NE(resultSet.find("456"), resultSet.end());
    ASSERT_NE(resultSet.find("789"), resultSet.end());

    const T_ stripStr  = " _ 123.456/789-0ab/_ _";
    T_       stripable = stripStr;

    // ("strip " << stripable << " of \".\"");
    strip(stripable, ".");
    ASSERT_EQ(stripable, " _ 123456/789-0ab/_ _");
    stripable = stripStr;
    // ("strip " << stripable << " of \"/\"");
    strip(stripable, "/");
    ASSERT_EQ(stripable, " _ 123.456789-0ab_ _");
    stripable = stripStr;
    // ("strip " << stripable << " of \"./\"");
    strip(stripable, "./");
    ASSERT_EQ(stripable, " _ 123456789-0ab_ _");

    stripable = stripStr;
    // ("trim " << stripable << " of \" \"");
    trim(stripable, " ");
    ASSERT_EQ(stripable, "_ 123.456/789-0ab/_ _");
    stripable = stripStr;
    // ("trim " << stripable << " of \"_\"");
    trim(stripable, "_");
    ASSERT_EQ(stripable, " _ 123.456/789-0ab/_ ");

    stripable = stripStr;
    // ("trim " << stripable << " of \" _\"");
    trim(stripable, " _");
    ASSERT_EQ(stripable, "123.456/789-0ab/");

    stripable = stripStr;
    // ("replaceChar " << stripable << " chars \"_\" with '#'");
    replaceChar(stripable, "_", '#');
    ASSERT_EQ(stripable, " # 123.456/789-0ab/# #");
    stripable = stripStr;
    // ("replaceChar " << stripable << " chars \" _\" with '#'");
    replaceChar(stripable, "_ ", '#');
    ASSERT_EQ(stripable, "###123.456/789-0ab/###");
}

TEST_F(StringUtilTest, util_ci_string_test)
{
    ci_string trimstring;
    trim(trimstring, "aBZd");
    ASSERT_EQ(trimstring, ci_string(""));
    trimstring = "aA";
    trim(trimstring, "aBZd");
    ASSERT_EQ(trimstring, ci_string(""));
    trimstring = "BaAb";
    trim(trimstring, "aBZd");
    ASSERT_EQ(trimstring, ci_string(""));

    trimstring = "zBaAZb";
    trim(trimstring, "aBZd");
    ASSERT_EQ(trimstring, ci_string(""));

    trimstring = "zBadDDdAZb";
    trim(trimstring, "aBZd");
    ASSERT_EQ(trimstring, ci_string(""));

    trimstring = "zB<SOMETHING>adDD</SOMETHING>dAZb";
    trim(trimstring, "aBZd");
    ASSERT_EQ(trimstring, ci_string("<SOMETHING>adDD</SOMETHING>"));

    ci_string source = "123a456B789c78A";
    // ("split " << source << " into vector by 'a'");
    vector<ci_string> result = splitIntoVector(source, 'a');
    ASSERT_EQ(result.size(), 3UL);
    ASSERT_EQ(result[0], ci_string("123"));
    ASSERT_EQ(result[1], ci_string("456B789c78"));
    ASSERT_EQ(result[2], ci_string(""));

    source = "123a456B789c78A";
    // ("split " << source << " into vector by 'A'");
    result = splitIntoVector(source, 'A');
    ASSERT_EQ(result.size(), 3UL);
    ASSERT_EQ(result[0], ci_string("123"));
    ASSERT_EQ(result[1], ci_string("456B789c78"));
    ASSERT_EQ(result[2], ci_string(""));

    source                   = "xxxAXxXbxXxC";
    set<ci_string> resultSet = splitIntoSet(source, ci_string("xxx"));
    ASSERT_EQ(resultSet.size(), 4UL);
    ASSERT_NE(resultSet.find(""), resultSet.end());
    ASSERT_NE(resultSet.find("a"), resultSet.end());
    ASSERT_NE(resultSet.find("b"), resultSet.end());
    ASSERT_NE(resultSet.find("c"), resultSet.end());

    ci_string const stripStr  = "abCaaAxxxabcxxxcBA";
    ci_string       stripable = stripStr;

    // ("strip " << stripable << " of \"abc\"");
    strip(stripable, ci_string("abc"));
    ASSERT_EQ(stripable, ci_string("xxxxxx"));

    stripable = stripStr;
    // ("replaceChar " << stripable << " chars \"abc\" with '#'");
    replaceChar(stripable, "abc", '#');
    ASSERT_EQ(stripable, ci_string("######xxx###xxx###"));
}

TEST_F(StringUtilTest, util_string_test)
{
    util_string_testT<string>();
    util_string_testT<ci_string>();
    util_string_mod_testT<string>();
    util_string_mod_testT<ci_string>();
    util_string_left_right_testT<string>();
    util_string_left_right_testT<ci_string>();
}
