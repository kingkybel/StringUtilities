/*
 * Repository:  https://github.com/kingkybel/StringUtilities
 * File Name:   test/decorator_tests.cc
 * Description: Unit tests for stream decoration.
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
#include "brackets.h"

#include <gtest/gtest.h>

using namespace std;
using namespace util;

class BracketsTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(BracketsTest, brackets_creation)
{
    Brackets b{util::BracketKey::SET};
    ASSERT_EQ(b.left(), "{");
    ASSERT_EQ(b.inner(), ",");
    ASSERT_EQ(b.right(), "}");

    Brackets custom{"MyQueue", string("^"), string("."), string("$")};
    ASSERT_EQ(custom.left(), "^");
    ASSERT_EQ(custom.inner(), ".");
    ASSERT_EQ(custom.right(), "$");
}

TEST_F(BracketsTest, custom_decoration)
{
    Brackets b{util::BracketKey::PIPE};
    ASSERT_EQ(b.left("=", "-"), "=|-");
    ASSERT_EQ(b.inner(".", "."), ".|.");
    ASSERT_EQ(b.right("\\", "+"), "\\|+");

    Brackets custom{"MyEBNF", "::=", "->", "<END>"};
    ASSERT_EQ(custom.left("X "), "X ::=");
    ASSERT_EQ(custom.inner("-", ">"), "-->>");
    ASSERT_EQ(custom.right("<", ">"), "<<END>>");
}
