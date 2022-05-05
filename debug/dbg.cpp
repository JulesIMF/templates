/*++

Copyright (c) 2022 JulesIMF, MIPT

Module Name:

    dbg.cpp

Abstract:



Author / Creation date:

    JulesIMF / 13.04.22

Revision History:

--*/


//
// Includes / usings
//

#include <string>
#include <iostream>
#include <sstream>
#include <functional>

//
// Defines
//

static int nTests = 0;
static std::string currentSuite = "unnamed";

namespace TerminalColor
{
    // 
    // Restoration escape seq
    //
    auto static    Default = "\e[0m";

    //
    // Regular text
    //

    auto static    Black   = "\e[0;30m";
    auto static    Red     = "\e[0;31m";
    auto static    Green   = "\e[0;32m";
    auto static    Yellow  = "\e[0;33m";
    auto static    Blue    = "\e[0;34m";
    auto static    Purple  = "\e[0;35m";
    auto static    Cyan    = "\e[0;36m";
    auto static    White   = "\e[0;37m";

    //
    // Bold text
    //

    auto static    BlackB  = "\e[1;30m";
    auto static    RedB    = "\e[1;31m";
    auto static    GreenB  = "\e[1;32m";
    auto static    YellowB = "\e[1;33m";
    auto static    BlueB   = "\e[1;34m";
    auto static    PurpleB = "\e[1;35m";
    auto static    CyanB   = "\e[1;36m";
    auto static    WhiteB  = "\e[1;37m";
    

    //
    // Underlined text
    // 
    
    auto static    BlackU  = "\e[4;30m";
    auto static    RedU    = "\e[4;31m";
    auto static    GreenU  = "\e[4;32m";
    auto static    YellowU = "\e[4;33m";
    auto static    BlueU   = "\e[4;34m";
    auto static    PurpleU = "\e[4;35m";
    auto static    CyanU   = "\e[4;36m";
    auto static    WhiteU  = "\e[4;37m";
}

namespace jules::tests
{
    static void delimiter_()
    {
        printf("%s-----------------------------------------------------\n",
            ::TerminalColor::Default);
    }

    static void onFail_()
    {
        delimiter_();
        printf("Terminating suite %s%s%s on %stest #%d%s...\n",
            ::TerminalColor::YellowB,
            currentSuite.c_str(),
            ::TerminalColor::Default,
            ::TerminalColor::Purple,
            nTests,
            ::TerminalColor::Default);
        
        std::exit(-1);
    }

    static void onTestBegin_(std::string const& testName)
    {
        nTests++;
        printf("Running %stest #%d%s (%s%s%s)...\n", 
            ::TerminalColor::Purple, 
            nTests, 
            ::TerminalColor::Default,
            ::TerminalColor::CyanU,
            testName.c_str(),
            ::TerminalColor::Default);
    }

    void start(std::string const& suiteName)
    {
        currentSuite = suiteName;

        printf("\n%sRunning suite %s%s%s...%s\n",
            ::TerminalColor::BlueB,
            ::TerminalColor::YellowB,
            currentSuite.c_str(),
            ::TerminalColor::BlueB,
            ::TerminalColor::Default);
        
        delimiter_();
    }

    void test(std::string const& testName, std::function<void()> fnc, std::string const& expected)
    {        
        onTestBegin_(testName);

        std::stringstream buffer;
        std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
        try
        {
            fnc();
        }
        catch(const std::exception& e)
        {
            printf("%sFailed!%s Caught unexpected std::exception%s \"%s\".\n",
                ::TerminalColor::RedB,
                ::TerminalColor::Red,
                ::TerminalColor::Default,
                e.what());
            
            onFail_();
        }
        catch (...)
        {
            printf("%sFailed!%s Caught unexpected exception of unknown type.%s\n",
                ::TerminalColor::RedB,
                ::TerminalColor::Red,
                ::TerminalColor::Default);

            onFail_();
        }
        
        bool ok = buffer.str() == expected;
        if (ok)
        {
            printf("%sTest #%d passed! Expected and got %s\"%s\".\n",
                ::TerminalColor::Green,
                nTests,
                ::TerminalColor::Default,
                expected.c_str());
        }
        else
        {
            printf("%sFailed!%s Expected %s\"%s\"%s, got %s\"%s\"%s.\n",
                ::TerminalColor::RedB,
                ::TerminalColor::Default,
                ::TerminalColor::Green,
                expected.c_str(),
                ::TerminalColor::Default,
                ::TerminalColor::Red,
                buffer.str().c_str(),
                ::TerminalColor::Default);
            
            onFail_();
        }
        
        std::cout.rdbuf(old);
        delimiter_();
    }

    void test_exception(std::string const& testName, std::function<void()> fnc)
    {
        onTestBegin_(testName);
        std::stringstream buffer;
        std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

        try
        {
            fnc();
        }
        catch (const std::exception& e)
        {
            printf("%sTest #%d passed! Caught an std::exception %s\"%s\".\n",
                ::TerminalColor::Green,
                nTests,
                ::TerminalColor::Default,
                e.what());
            
            std::cout.rdbuf(old);
            delimiter_();
            return;
            
        }
        catch (...)
        {
            printf("%sTest #%d passed! Caught an exception of unknown type%s.\n",
                ::TerminalColor::Green,
                nTests,
                ::TerminalColor::Default);
            std::cout.rdbuf(old);
            delimiter_();
            return;
        }

        printf("%sFailed!%s No exception caught. Output was %s\"%s\".\n",
            ::TerminalColor::RedB,
            ::TerminalColor::Red,
            ::TerminalColor::Default,
            buffer.str().c_str());

        onFail_();
    }

    void complete()
    {
        if (nTests)
        {
            printf("%s%d%s test%s from suite %s%s%s passed!%s\n\n",
                ::TerminalColor::PurpleB,
                nTests,
                ::TerminalColor::GreenB,
                ((nTests == 1) ? "" : "s"),
                ::TerminalColor::YellowB,
                currentSuite.c_str(),
                ::TerminalColor::GreenB,
                ::TerminalColor::Default);
        }

        else
        {
            printf("%sEmpty suite completed!%s\n\n",
                ::TerminalColor::GreenB,
                ::TerminalColor::Default);
        }

        
        nTests = 0;
        currentSuite = "unnamed";
    }
}