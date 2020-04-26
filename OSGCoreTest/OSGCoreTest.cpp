
#include "windows.h"

#include <iostream>
#include <gtest\gtest.h>

#include <OSGIncluding.h>

#include <ParametricComponent.h>



TEST(ParametricTree, CalTest) {
    osg::ref_ptr<ParametricComponent> pc = new ParametricComponent();
    EXPECT_NE(pc.get(), nullptr);

    pc->SetParam(" A = 800 ");
    pc->SetParam("     \r\n       B       \r\n      =               \r\n           800        \r\n         ");
    pc->SetParam(" W=800 ");

	std::string key1(" D ");
	std::string val1("  \r\n  \r\n  900\r\n  \r\n  ");
    pc->SetParam(key1, val1);

	std::string key2(" H ");
	std::string val2("  \r\n  \r\n  900\r\n  \r\n  ");
    pc->SetParam(key2, val2);

    EXPECT_STREQ(pc->GetParam("A").c_str(), "800");
    EXPECT_STREQ(pc->GetParam("B").c_str(), "800");
    EXPECT_STREQ(pc->GetParam("W").c_str(), "800");
    EXPECT_STREQ(pc->GetParam("D").c_str(), "900");
    EXPECT_STREQ(pc->GetParam("H").c_str(), "900");
}

TEST(ParametricTree, RegexTest1)
{
    std::string strKey, strVal;
    ParametricComponent::regexParseFormular(" A = 800 ", strKey, strVal);
    EXPECT_STREQ(strKey.c_str(), "A");
    EXPECT_STREQ(strVal.c_str(), "800");
}

TEST(ParametricTree, RegexTest2)
{
	std::string strKey, strVal;
	ParametricComponent::regexParseFormular("     \r\n       B       \r\n      =               \r\n           800        \r\n         ",
        strKey, strVal);
	EXPECT_STREQ(strKey.c_str(), "B");
	EXPECT_STREQ(strVal.c_str(), "800");
}

TEST(ParametricTree, RegexTest3)
{
	std::string strKey, strVal;
	ParametricComponent::regexParseFormular(" W=800 ", strKey, strVal);
	EXPECT_STREQ(strKey.c_str(), "W");
	EXPECT_STREQ(strVal.c_str(), "800");
}

TEST(ParametricTree, RegexTest4)
{
	std::string strKey, strVal;
	ParametricComponent::regexParseFormular("Q=800", strKey, strVal);
	EXPECT_STREQ(strKey.c_str(), "Q");
	EXPECT_STREQ(strVal.c_str(), "800");
}

TEST(ParametricTree, RegexTest5)
{
    std::string str(" 18 ");

	ParametricComponent::regexParseKV(str);
	EXPECT_STREQ(str.c_str(), "18");
}

TEST(ParametricTree, RegexTest6)
{
	std::string str("   \r\n   \r\n   W1\r\n");

	ParametricComponent::regexParseKV(str);
	EXPECT_STREQ(str.c_str(), "W1");
}

TEST(ParametricTree, RegexTest7)
{
	std::string str("   \r\n   \r\n   W1\r\n=");

	ParametricComponent::regexParseKV(str);
	EXPECT_STREQ(str.c_str(), "W1");
}

TEST(ParametricTree, RegexTest8)
{
	std::string str("   \r\n   \r\n   W1=  \r\n");

	ParametricComponent::regexParseKV(str);
	EXPECT_STREQ(str.c_str(), "W1");
}

int main()
{
    ::testing::InitGoogleTest();
    RUN_ALL_TESTS();
    return 0;
}