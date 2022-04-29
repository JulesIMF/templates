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

#include "array.hpp"
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

int main()
{
    try
    {
        jules::array<bool, 5, jules::storage::on_heap> arr = {false, false, true, false};
        arr.resize(5);
        arr.resize(0);
        arr.resize(5);

        for (int i = 0; i != 5; i++)
            arr[i] = i & 1;

        for (int i = 0; i != 5; i++)
            std::cout << arr[i] << " ";

        printf("\n");

        arr = { true, true, false, true };

        for (int i = 0; i != arr.size(); i++)
            std::cout << arr[i] << " ";

        printf("\n");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        while (true)
        {       
        }        
    }
}