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

#include "vector.hpp"
#include <string>
#include <algorithm>
#include <dbg.hpp>
#include <iostream>

//
// Defines
//

void default_config_int()
{
    jules::tests::start("default_config_int");
    jules::vector<int> v;

    jules::tests::test_exception("bounds checked",
        [&]
        {
            v[0] = 0;
        });

    jules::tests::test("resize and simple routines work",
        [&]
        {
            v.resize(5);
            for (int i = 0; i != 5; i++)
                v[i] = i;

            for (int i = 0; i != 5; i++)
                std::cout << v[i] << " ";
        },
            "0 1 2 3 4 ");
    
    jules::tests::test("reserve more",
        [&]
        {
            v.reserve(8);
            std::cout << (v.capacity() >= 8);
        }, 
            "1");
        
    jules::tests::test("bounds unchecked",
        [&]
        {
            v.at_unchecked(5) = 0;
        });

    jules::tests::test("resize zeroes",
        [&]
        {
            v.resize(0);
            v.resize(4);
            auto sz = v.size();
            std::cout << sz << " ";
            for (int i = 0; i != sz; i++)
                std::cout << v[i] << " ";
        },
            "4 0 0 0 0 ");

    jules::tests::test("back and front",
        [&]
        {
            v.back() = -1;
            v.front() = -2;
            std::cout << v[0] << " " << v[v.size() - 1];
        },
            "-2 -1");

    jules::tests::test("data",
        [&]
        {
            v.resize(5);
            for (int i = 0; i != 5; i++)
                v[i] = i;

            auto data = v.data();

            for (int i = 0; i != 5; i++)
                std::cout << data[i] << " ";
        },
            "0 1 2 3 4 ");

    jules::tests::test("move ctr",
        [&]
        {
            auto mv = std::move(v);
            for (int i = 0; i != 5; i++)
                std::cout << mv[i] << " ";
        },
            "0 1 2 3 4 ");

    jules::tests::test("copy ctr",
        [&]
        {
            auto cp = v;
            for (int i = 0; i != 5; i++)
                std::cout << cp[i] << " ";
        },
            "0 1 2 3 4 ");

    jules::tests::test("list assign",
        [&]
        {
            v = { 0, -2, -5 };
            for (int i = 0; i != v.size(); i++)
                std::cout << v[i] << " ";
        },
            "0 -2 -5 ");

    jules::tests::complete();
}

void default_config_str()
{
    jules::tests::start("default_config_str");
    jules::vector<std::string> v;

    jules::tests::test_exception("bounds checked",
        [&]
        {
            v[0] = std::to_string(0);
        });

    jules::tests::test("resize and simple routines work",
        [&]
        {
            v.resize(5);
            for (int i = 0; i != 5; i++)
                v[i] = std::to_string(i);

            for (int i = 0; i != 5; i++)
                std::cout << v[i] << " ";
        },
            "0 1 2 3 4 ");
    
    jules::tests::test("reserve more",
        [&]
        {
            v.reserve(8);
            std::cout << (v.capacity() >= 8);
        }, 
            "1");

    jules::tests::test("resize zeroes",
        [&]
        {
            v.resize(0);
            v.resize(4);
            auto sz = v.size();
            std::cout << sz << " ";
            for (int i = 0; i != sz; i++)
                std::cout << v[i] << " ";
        },
            "4     ");

    jules::tests::test("back and front",
        [&]
        {
            v.back() = std::to_string(-1);
            v.front() = std::to_string(-2);
            std::cout << v[0] << " " << v[v.size() - 1];
        },
            "-2 -1");

    jules::tests::test("data",
        [&]
        {
            v.resize(5);
            for (int i = 0; i != 5; i++)
                v[i] = std::to_string(i);

            auto data = v.data();

            for (int i = 0; i != 5; i++)
                std::cout << data[i] << " ";
        },
            "0 1 2 3 4 ");

    jules::tests::test("move ctr",
        [&]
        {
            auto mv = std::move(v);
            for (int i = 0; i != 5; i++)
                std::cout << mv[i] << " ";
            
            v = std::move(mv);
            for (int i = 0; i != 5; i++)
                std::cout << v[i] << " ";
        },
            "0 1 2 3 4 0 1 2 3 4 ");

    jules::tests::test("copy ctr",
        [&]
        {
            auto cp = v;
            for (int i = 0; i != 5; i++)
                std::cout << cp[i] << " ";
        },
            "0 1 2 3 4 ");

    jules::tests::test("list assign",
        [&]
        {
            v = { "0", "-2", "-5" };
            for (int i = 0; i != v.size(); i++)
                std::cout << v[i] << " ";
        },
            "0 -2 -5 ");

    jules::tests::complete();
}

void iterator_tests()
{
    jules::tests::start("iterator_tests");

    jules::vector<int> v = { 0, 4, 1, 3, 2 };
    jules::tests::test("list init",
        [&]
        {
            for (int i = 0; i != v.size(); i++)
                std::cout << v[i] << " ";
        },
            "0 4 1 3 2 ");
    
    jules::tests::test("range based for",
        [&]
        {
            for (auto x : v)
                std::cout << x << " ";
        },
            "0 4 1 3 2 ");
    
    jules::tests::test("range based for ref",
        [&]
        {
            for (auto& x : v)
                std::cout << x << " ";
        },
            "0 4 1 3 2 ");

    jules::tests::test("range based for const ref",
        [&]
        {
            for (auto const& x : v)
                std::cout << x << " ";
        },
            "0 4 1 3 2 ");

    jules::tests::test("reverse iterators",
        [&]
        {
            for (auto rit = v.rbegin(); rit != v.rend(); ++rit)
            {
                std::cout << *rit << " ";
            }
        },
            "2 3 1 4 0 ");
    
    jules::tests::test("std sort",
        [&]
        {
            std::sort(v.begin(), v.end());
            for (auto const& x : v)
                std::cout << x << " ";
        }, 
            "0 1 2 3 4 ");

    jules::tests::test("std sort reverse",
        [&]
        {
            std::sort(v.rbegin(), v.rend());
            for (auto const& x : v)
                std::cout << x << " ";
        }, 
            "4 3 2 1 0 ");

    jules::tests::test("std copy",
        [&]
        {
            jules::vector<int> cp(v.size());
            std::copy(v.begin(), v.end(), cp.begin());
            for (auto const& x : cp)
                std::cout << x << " ";

        }, 
            "4 3 2 1 0 ");

    jules::tests::test("emplace back",
        [&]
        {
            jules::vector<std::string> v;
            v.emplace_back(3, 'a');
            std::cout << v.back();
        }, 
            "aaa");

    jules::tests::complete();
}

void emplace_insert_remove()
{
    jules::tests::start("emplace_insert_remove");

    jules::vector<std::string> v;
    jules::tests::test("emplace back",
        [&]
        {
            auto s = v.emplace_back(3, 'a');
            std::cout << v.back();
            std::cout << s;
        }, 
            "aaa""aaa");
    
    jules::tests::test("push back",
        [&]
        {
            v.push_back("bbb");
            for (auto const& s : v)
                std::cout << s << " ";
        }, 
            "aaa bbb ");
    
    jules::tests::test("pop back",
        [&]
        {
            v.pop_back();
            std::cout << v.back();
        }, 
            "aaa");
        
    jules::tests::test("insert",
        [&]
        {
            std::string s = "bbb";
            v.insert(v.end(), s);
            v.insert(v.begin(), "ccc");

            v.insert(v.begin() + 1, 2, "ddd");
                
            for (auto const& s : v)
                std::cout << s;
            
        }, 
            "ccc"
            "ddd"
            "ddd"
            "aaa"
            "bbb");
    
    jules::tests::test("insert range",
        [&]
        {
            v = {"a", "r", "e", "ya"};
            std::vector<std::string> range = {"r", "an", "ge"};
            v.insert(v.begin() + 1, range.begin(), range.end());
            for (auto& s : v)
                std::cout << s;
        }, 
            "arangereya");
    
    jules::tests::test("insert range",
        [&]
        {
            v.insert(v.end(), {" АХАХАХ ", "ЕГОР ГДЕ БАБЛО??"});
            for (auto& s : v)
                std::cout << s;
        }, 
            "arangereya АХАХАХ ЕГОР ГДЕ БАБЛО??");

    jules::tests::test("insert range",
        [&]
        {
            v.erase(v.begin() + 1, v.end() - 2);
            for (auto& s : v)
                std::cout << s;
        }, 
            "a АХАХАХ ЕГОР ГДЕ БАБЛО??");

    jules::tests::complete();
}

void strange_tests()
{
    jules::tests::start("strange_tests");

    jules::tests::complete();
}

int main()
{
    default_config_int();
    default_config_str();
    iterator_tests();
    emplace_insert_remove();
    // strange_tests();
}