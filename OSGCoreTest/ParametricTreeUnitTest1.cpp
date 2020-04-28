#include "ParametricTreeUnitTest1.h"
#include <gtest\gtest.h>
#include <OSGIncluding.h>
#include <ParametricComponent.h>

TEST(ParametricTree, BuildTree) {
	// Top Level
	osg::ref_ptr<ParametricComponent> pTop = new ParametricComponent();
	EXPECT_NE(pTop.get(), nullptr);
	pTop->SetParam("W = 1200\r\n");
	pTop->SetParam("D = 500\r\n");
	pTop->SetParam("H = 400\r\n");

	// Second Level
	osg::ref_ptr<ParametricComponent> pSecond1 = new ParametricComponent();
	EXPECT_NE(pSecond1.get(), nullptr);
	pSecond1->SetParam("W=Parent.W/3*2");
	pSecond1->SetParam("D=Parent.D");
	pSecond1->SetParam("H=Parent.H");

	// Second Level
	osg::ref_ptr<ParametricComponent> pSecond2 = new ParametricComponent();
	EXPECT_NE(pSecond2.get(), nullptr);
	pSecond2->SetParam("W=Parent.W/3*1");
	pSecond2->SetParam("D=Parent.D");
	pSecond2->SetParam("H=Parent.H");
	pSecond2->SetParam("PV=Parent.W * Parent.D * Parent.H");
	pSecond2->SetParam("V=W * D * H");

	pTop->addChild(pSecond1);
	pTop->addChild(pSecond2);

	// Update Top to Down
	pTop->UpdateFormulas();

	//Checking
	EXPECT_DOUBLE_EQ(atof(pTop->GetParamResult("W").c_str()), 1200.0);
	EXPECT_DOUBLE_EQ(atof(pTop->GetParamResult("D").c_str()), 500.0);
	EXPECT_DOUBLE_EQ(atof(pTop->GetParamResult("H").c_str()), 400.0);

	EXPECT_DOUBLE_EQ(atof(pSecond1->GetParamResult("W").c_str()), 800.0);
	EXPECT_DOUBLE_EQ(atof(pSecond1->GetParamResult("D").c_str()), 500.0);
	EXPECT_DOUBLE_EQ(atof(pSecond1->GetParamResult("H").c_str()), 400.0);

	EXPECT_DOUBLE_EQ(atof(pSecond2->GetParamResult("W").c_str()), 400.0);
	EXPECT_DOUBLE_EQ(atof(pSecond2->GetParamResult("D").c_str()), 500.0);
	EXPECT_DOUBLE_EQ(atof(pSecond2->GetParamResult("H").c_str()), 400.0);
	EXPECT_DOUBLE_EQ(atof(pSecond2->GetParamResult("PV").c_str()), 1200.0 * 500.0 * 400.0);
	EXPECT_DOUBLE_EQ(atof(pSecond2->GetParamResult("V").c_str()), 400.0 * 500.0 * 400.0);
}

TEST(ParametricTree, BuildTree1) {
	osg::ref_ptr<ParametricComponent> pNode0 = new ParametricComponent();
	EXPECT_NE(pNode0.get(), nullptr);
	pNode0->SetParam("W = 32");

	osg::ref_ptr<ParametricComponent> pNode1 = new ParametricComponent();
	EXPECT_NE(pNode1.get(), nullptr);
	pNode1->SetParam("W = Parent.W/2");
	pNode0->addChild(pNode1);

	osg::ref_ptr<ParametricComponent> pNode2 = new ParametricComponent();
	EXPECT_NE(pNode2.get(), nullptr);
	pNode2->SetParam("W = Parent.W/2");
	pNode1->addChild(pNode2);

	osg::ref_ptr<ParametricComponent> pNode3 = new ParametricComponent();
	EXPECT_NE(pNode3.get(), nullptr);
	pNode3->SetParam("W = Parent.W/2");
	pNode2->addChild(pNode3);

	osg::ref_ptr<ParametricComponent> pNode4 = new ParametricComponent();
	EXPECT_NE(pNode4.get(), nullptr);
	pNode4->SetParam("W = Parent.W/2");
	pNode3->addChild(pNode4);

	pNode0->UpdateFormulas();

	EXPECT_DOUBLE_EQ(atof(pNode0->GetParamResult("W").c_str()), 32);
	EXPECT_DOUBLE_EQ(atof(pNode1->GetParamResult("W").c_str()), 16);
	EXPECT_DOUBLE_EQ(atof(pNode2->GetParamResult("W").c_str()), 8);
	EXPECT_DOUBLE_EQ(atof(pNode3->GetParamResult("W").c_str()), 4);
	EXPECT_DOUBLE_EQ(atof(pNode4->GetParamResult("W").c_str()), 2);
}

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

TEST(ParametricTree, RegexTest9)
{
	std::string str("   \r\n   \r\n     \r\n");
	bool bRet = ParametricComponent::regexParseKV(str);
	EXPECT_EQ(bRet, false);

	std::string str1("");
	bool bRet1 = ParametricComponent::regexParseKV(str1);
	EXPECT_EQ(bRet1, false);
}

TEST(ParametricTree, RegexTest10)
{
	std::string strKey, strVal;
	bool bRet = ParametricComponent::regexParseFormular("Q", strKey, strVal);
	EXPECT_EQ(bRet, false);

	bool bRet1 = ParametricComponent::regexParseFormular("Q=", strKey, strVal);
	EXPECT_EQ(bRet1, false);
}
