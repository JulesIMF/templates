/*++

Copyright (c) 2022 JulesIMF, MIPT

Module Name:

    array_test.hpp

Abstract:

    

Author / Creation date:

    JulesIMF / 13.04.22

Revision History:

--*/


//
// Includes / usings
//

#include <functional>
#include <string>

//
// Defines
//


namespace jules::tests
{
    void test(std::string const& testName, std::function<void()> fnc, std::string const& expected = "");
    void test_exception(std::string const& testName, std::function<void()> fnc);
    void start(std::string const& suiteName);
    void complete();
}