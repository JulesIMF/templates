/*++

Copyright (c) 2022 JulesIMF, MIPT

Module Name:

    test.cpp

Abstract:

    

Author / Creation date:

    JulesIMF / 05.04.22

Revision History:

--*/


//
// Includes / usings
//

#include "vector.hpp"
#include <iostream>
#include <string>
#include <array>
#include <cassert>
#include <vector>

//
// Defines
//

struct Lol
{
    Lol() : lol(this) {};
    Lol(Lol const&) = delete;
    Lol(Lol&&) = delete;
    Lol* lol;
    void check() { assert(this == lol); };
};

int* arr()
{
    return new int[5];
}

int main()
{
    jules::vector<std::string> v = {std::string("11"), "211", "311"};
    auto dump = [&]
    {
        std::cout << "Contains: ";
        for (auto const& s: v)
            std::cout << s << " ";

        std::cout << "\n";
        
        printf("Size:     %zu\n"
               "Capacity: %zu\n"
               "Data:     %p\n",
               v.size(),
               v.capacity(),
               v.data());
    };

    // dump();
    
    v.clear();

    v = {"aaa"};

    //dump();

    std::string s = "bbb";
    v.insert(v.end(), s);
    v.insert(v.begin(), "ccc");
    // for (auto it = v.begin(); it != v.end(); ++it)
    //     std::cout << *it;
                
    for (auto const& s : v)
        std::cout << s;
}