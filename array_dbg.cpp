/*++

Copyright (c) 2022 JulesIMF, MIPT

Module Name:

    array_dbg.cpp

Abstract:

    

Author / Creation date:

    JulesIMF / 13.04.22

Revision History:

--*/


//
// Includes / usings
//

#include "array.hpp"
#include <dbg.hpp>
#include <iostream>

//
// Defines
//

void on_stack()
{
    jules::tests::start("on_stack");
    jules::array<int, 5, jules::storage::on_stack> arr;

    jules::tests::test_exception("bounds checked",
        [&]
        {
            arr[0] = 0;
        });

    jules::tests::test("bounds unchecked",
        [&]
        {
            arr.at_unchecked(0) = 0;
        });
    
    jules::tests::test("resize and simple routines work",
        [&]
        {
            arr.resize(5);
            for (int i = 0; i != 5; i++)
                arr[i] = i;
            
            for (int i = 0; i != 5; i++)
                std::cout << arr[i] << " ";
        },
        "0 1 2 3 4 ");
    
    jules::tests::test("resize zeroes",
        [&]
        {
            arr.resize(0);
            arr.resize(4);
            auto sz = arr.size();
            std::cout << sz << " ";
            for (int i = 0; i != sz; i++)
                std::cout << arr[i] << " ";
        },
        "4 0 0 0 0 ");
        
    jules::tests::test("back and front",
        [&]
        {
            arr.back() = -1;
            arr.front() = -2;
            std::cout << arr[0] << " " << arr[arr.size() - 1];
        },
        "-2 -1");
    
    jules::tests::test("data",
        [&]
        {
            arr.resize(5);
            for (int i = 0; i != 5; i++)
                arr[i] = i;

            auto data = arr.data();

            for (int i = 0; i != 5; i++)
                std::cout << data[i] << " ";
        },
        "0 1 2 3 4 ");
    
    jules::tests::test("move ctr",
        [&]
        {
            auto mv = std::move(arr);
            for (int i = 0; i != 5; i++)
                std::cout << mv[i] << " ";
        },
        "0 1 2 3 4 ");
    
    jules::tests::test("list assign",
        [&]
        {
            arr = {0, -2, -5};
            for (int i = 0; i != arr.size(); i++)
                std::cout << arr[i] << " ";
        },
            "0 -2 -5 ");



    jules::tests::complete();
}

void on_heap()
{
    jules::tests::start("on_heap");
    jules::array<int, 5, jules::storage::on_heap> arr;

    jules::tests::test_exception("bounds checked",
        [&]
        {
            arr[0] = 0;
        });

    jules::tests::test("bounds unchecked",
        [&]
        {
            arr.at_unchecked(0) = 0;
        });

    jules::tests::test("resize and simple routines work",
        [&]
        {
            arr.resize(5);
            for (int i = 0; i != 5; i++)
                arr[i] = i;

            for (int i = 0; i != 5; i++)
                std::cout << arr[i] << " ";
        },
            "0 1 2 3 4 ");

    jules::tests::test("resize zeroes",
        [&]
        {
            arr.resize(0);
            arr.resize(4);
            auto sz = arr.size();
            std::cout << sz << " ";
            for (int i = 0; i != sz; i++)
                std::cout << arr[i] << " ";
        },
            "4 0 0 0 0 ");

    jules::tests::test("back and front",
        [&]
        {
            arr.back() = -1;
            arr.front() = -2;
            std::cout << arr[0] << " " << arr[arr.size() - 1];
        },
            "-2 -1");

    jules::tests::test("data",
        [&]
        {
            arr.resize(5);
            for (int i = 0; i != 5; i++)
                arr[i] = i;

            auto data = arr.data();

            for (int i = 0; i != 5; i++)
                std::cout << data[i] << " ";
        },
            "0 1 2 3 4 ");

    jules::tests::test("move ctr",
        [&]
        {
            auto mv = std::move(arr);
            for (int i = 0; i != 5; i++)
                std::cout << mv[i] << " ";
        },
            "0 1 2 3 4 ");

    jules::tests::test("list assign",
        [&]
        {
            arr = { 0, -2, -5 };
            for (int i = 0; i != arr.size(); i++)
                std::cout << arr[i] << " ";
        },
            "0 -2 -5 ");



    jules::tests::complete();
}

void bool_specialization()
{
    jules::tests::start("bool_specialization");
    jules::array<bool, 25, jules::storage::on_stack> arr;

    jules::tests::test_exception("bounds checked",
        [&]
        {
            arr[0] = false;
        });

    jules::tests::test("bounds unchecked",
        [&]
        {
            arr.at_unchecked(0) = false;
        });
    
    jules::tests::test("resize and simple routines work",
        [&]
        {
            arr.resize(arr.max_size());
            for (int i = 0; i != arr.size(); i++)
                arr[i] = i & 1;
            
            for (int i = 0; i != arr.size(); i++)
                std::cout << arr[i] << " ";
        },
        "0 1 0 1 0 1 0 1 "
        "0 1 0 1 0 1 0 1 "
        "0 1 0 1 0 1 0 1 0 ");
    
    jules::tests::test("resize zeroes",
        [&]
        {
            arr.resize(0);
            arr.resize(4);
            auto sz = arr.size();
            std::cout << sz << " ";
            for (int i = 0; i != sz; i++)
                std::cout << arr[i] << " ";
        },
        "4 0 0 0 0 ");
        
    jules::tests::test("back and front",
        [&]
        {
            arr.back() = true;
            arr.front() = false;
            for (int i = 0; i != arr.size(); i++)
                std::cout << arr[i] << " ";
        },
        "0 0 0 1 ");
    
    jules::tests::test("move ctr",
        [&]
        {
            auto mv = std::move(arr);
            for (int i = 0; i != arr.size(); i++)
                std::cout << mv[i] << " ";
        },
        "0 0 0 1 ");
    
    jules::tests::test("list assign",
        [&]
        {
            arr = {true, true, false, true};
            for (int i = 0; i != arr.size(); i++)
                std::cout << arr[i] << " ";
        },
            "1 1 0 1 ");

    auto const carr = arr;
    jules::tests::test("const ref works",
        [&]
        {
            for (int i = 0; i != carr.size(); i++)
                std::cout << carr[i] << " ";
        },
        "1 1 0 1 ");

    jules::tests::complete();
}

int main()
{
    on_stack();
    on_heap();
    bool_specialization();
}