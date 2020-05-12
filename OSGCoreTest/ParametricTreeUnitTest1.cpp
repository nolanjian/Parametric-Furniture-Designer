#include "ParametricTreeUnitTest1.h"
#include <gtest\gtest.h>
#include <OSGIncluding.h>
#include <ParametricComponent.h>

using namespace PFDCore;

TEST(ParametricTree, BuildTree) {
	// Top Level
	osg::ref_ptr<ParametricComponent> pTop = new ParametricComponent();
	EXPECT_NE(pTop.get(), nullptr);
	pTop->SetParam(L"W = 1200\r\n");
	pTop->SetParam(L"D = 500\r\n");
	pTop->SetParam(L"H = 400\r\n");

	// Second Level
	osg::ref_ptr<ParametricComponent> pSecond1 = new ParametricComponent();
	EXPECT_NE(pSecond1.get(), nullptr);
	pSecond1->SetParam(L"W=Parent.W/3*2");
	pSecond1->SetParam(L"D=Parent.D");
	pSecond1->SetParam(L"H=Parent.H");

	// Second Level
	osg::ref_ptr<ParametricComponent> pSecond2 = new ParametricComponent();
	EXPECT_NE(pSecond2.get(), nullptr);
	pSecond2->SetParam(L"W=Parent.W/3*1");
	pSecond2->SetParam(L"D=Parent.D");
	pSecond2->SetParam(L"H=Parent.H");
	pSecond2->SetParam(L"PV=Parent.W * Parent.D * Parent.H");
	pSecond2->SetParam(L"V=W * D * H");

	pTop->addChild(pSecond1);
	pTop->addChild(pSecond2);

	// Update Top to Down
	pTop->UpdateFormulas();

	//Checking
	EXPECT_DOUBLE_EQ(_wtof(pTop->GetParamResult(L"W").c_str()), 1200.0);
	EXPECT_DOUBLE_EQ(_wtof(pTop->GetParamResult(L"D").c_str()), 500.0);
	EXPECT_DOUBLE_EQ(_wtof(pTop->GetParamResult(L"H").c_str()), 400.0);

	EXPECT_DOUBLE_EQ(_wtof(pSecond1->GetParamResult(L"W").c_str()), 800.0);
	EXPECT_DOUBLE_EQ(_wtof(pSecond1->GetParamResult(L"D").c_str()), 500.0);
	EXPECT_DOUBLE_EQ(_wtof(pSecond1->GetParamResult(L"H").c_str()), 400.0);

	EXPECT_DOUBLE_EQ(_wtof(pSecond2->GetParamResult(L"W").c_str()), 400.0);
	EXPECT_DOUBLE_EQ(_wtof(pSecond2->GetParamResult(L"D").c_str()), 500.0);
	EXPECT_DOUBLE_EQ(_wtof(pSecond2->GetParamResult(L"H").c_str()), 400.0);
	EXPECT_DOUBLE_EQ(_wtof(pSecond2->GetParamResult(L"PV").c_str()), 1200.0 * 500.0 * 400.0);
	EXPECT_DOUBLE_EQ(_wtof(pSecond2->GetParamResult(L"V").c_str()), 400.0 * 500.0 * 400.0);
}

TEST(ParametricTree, BuildTree1) {
	osg::ref_ptr<ParametricComponent> pNode0 = new ParametricComponent();
	EXPECT_NE(pNode0.get(), nullptr);
	pNode0->SetParam(L"W = 32");

	osg::ref_ptr<ParametricComponent> pNode1 = new ParametricComponent();
	EXPECT_NE(pNode1.get(), nullptr);
	pNode1->SetParam(L"W = Parent.W/2");
	pNode0->addChild(pNode1);

	osg::ref_ptr<ParametricComponent> pNode2 = new ParametricComponent();
	EXPECT_NE(pNode2.get(), nullptr);
	pNode2->SetParam(L"W = Parent.W/2");
	pNode1->addChild(pNode2);

	osg::ref_ptr<ParametricComponent> pNode3 = new ParametricComponent();
	EXPECT_NE(pNode3.get(), nullptr);
	pNode3->SetParam(L"W = Parent.W/2");
	pNode2->addChild(pNode3);

	osg::ref_ptr<ParametricComponent> pNode4 = new ParametricComponent();
	EXPECT_NE(pNode4.get(), nullptr);
	pNode4->SetParam(L"W = Parent.W/2");
	pNode3->addChild(pNode4);

	pNode0->UpdateFormulas();

	EXPECT_DOUBLE_EQ(_wtof(pNode0->GetParamResult(L"W").c_str()), 32);
	EXPECT_DOUBLE_EQ(_wtof(pNode1->GetParamResult(L"W").c_str()), 16);
	EXPECT_DOUBLE_EQ(_wtof(pNode2->GetParamResult(L"W").c_str()), 8);
	EXPECT_DOUBLE_EQ(_wtof(pNode3->GetParamResult(L"W").c_str()), 4);
	EXPECT_DOUBLE_EQ(_wtof(pNode4->GetParamResult(L"W").c_str()), 2);
}

TEST(ParametricTree, CalTest) {
	osg::ref_ptr<ParametricComponent> pc = new ParametricComponent();
	EXPECT_NE(pc.get(), nullptr);

	pc->SetParam(L" A = 800 ");
	pc->SetParam(L"     \r\n       B       \r\n      =               \r\n           800        \r\n         ");
	pc->SetParam(L" W=800 ");

	std::wstring key1(L" D ");
	std::wstring val1(L"  \r\n  \r\n  900\r\n  \r\n  ");
	pc->SetParam(key1, val1);

	std::wstring key2(L" H ");
	std::wstring val2(L"  \r\n  \r\n  900\r\n  \r\n  ");
	pc->SetParam(key2, val2);

	EXPECT_STREQ(pc->GetParam(L"A").c_str(), L"800");
	EXPECT_STREQ(pc->GetParam(L"B").c_str(), L"800");
	EXPECT_STREQ(pc->GetParam(L"W").c_str(), L"800");
	EXPECT_STREQ(pc->GetParam(L"D").c_str(), L"900");
	EXPECT_STREQ(pc->GetParam(L"H").c_str(), L"900");
}

TEST(ParametricTree, RegexTest1)
{
	std::wstring strKey, strVal;
	ParametricComponent::regexParseFormular(L" A = 800 ", strKey, strVal);
	EXPECT_STREQ(strKey.c_str(), L"A");
	EXPECT_STREQ(strVal.c_str(), L"800");
}

TEST(ParametricTree, RegexTest2)
{
	std::wstring strKey, strVal;
	ParametricComponent::regexParseFormular(L"     \r\n       B       \r\n      =               \r\n           800        \r\n         ",
		strKey, strVal);
	EXPECT_STREQ(strKey.c_str(), L"B");
	EXPECT_STREQ(strVal.c_str(), L"800");
}

TEST(ParametricTree, RegexTest3)
{
	std::wstring strKey, strVal;
	ParametricComponent::regexParseFormular(L" W=800 ", strKey, strVal);
	EXPECT_STREQ(strKey.c_str(), L"W");
	EXPECT_STREQ(strVal.c_str(), L"800");
}

TEST(ParametricTree, RegexTest4)
{
	std::wstring strKey, strVal;
	ParametricComponent::regexParseFormular(L"Q=800", strKey, strVal);
	EXPECT_STREQ(strKey.c_str(), L"Q");
	EXPECT_STREQ(strVal.c_str(), L"800");
}

TEST(ParametricTree, RegexTest5)
{
	std::wstring str(L" 18 ");

	ParametricComponent::regexParseKV(str);
	EXPECT_STREQ(str.c_str(), L"18");
}

TEST(ParametricTree, RegexTest6)
{
	std::wstring str(L"   \r\n   \r\n   W1\r\n");

	ParametricComponent::regexParseKV(str);
	EXPECT_STREQ(str.c_str(), L"W1");
}

TEST(ParametricTree, RegexTest7)
{
	std::wstring str(L"   \r\n   \r\n   W1\r\n=");

	ParametricComponent::regexParseKV(str);
	EXPECT_STREQ(str.c_str(), L"W1");
}

TEST(ParametricTree, RegexTest8)
{
	std::wstring str(L"   \r\n   \r\n   W1=  \r\n");

	ParametricComponent::regexParseKV(str);
	EXPECT_STREQ(str.c_str(), L"W1");
}

TEST(ParametricTree, RegexTest9)
{
	std::wstring str(L"   \r\n   \r\n     \r\n");
	bool bRet = ParametricComponent::regexParseKV(str);
	EXPECT_EQ(bRet, false);

	std::wstring str1(L"");
	bool bRet1 = ParametricComponent::regexParseKV(str1);
	EXPECT_EQ(bRet1, false);
}

TEST(ParametricTree, RegexTest10)
{
	std::wstring strKey, strVal;
	bool bRet = ParametricComponent::regexParseFormular(L"Q", strKey, strVal);
	EXPECT_EQ(bRet, false);

	bool bRet1 = ParametricComponent::regexParseFormular(L"Q=", strKey, strVal);
	EXPECT_EQ(bRet1, false);
}
