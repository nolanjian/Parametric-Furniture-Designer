


#include <iostream>
#include <gtest\gtest.h>

#include <chrono>
#include <filesystem>

#include "ParametricTreeUnitTest1.h"


int main()
{
    

    auto tp = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    

    //std::string outputXML = "--gtest_output=xml:" + std::filesystem::current_path().string() + "\\" + std::to_string(tp) + ".xml";
    std::string outputXML = "--gtest_output=\"xml:" + std::filesystem::current_path().string() + "\\" +  std::to_string(tp) + ".xml\"";

    int argc = 1;
    char* argv[1];
    argv[0] = (char*)outputXML.c_str();


    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
    getchar();
    return 0;
}