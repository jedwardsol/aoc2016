#include <cassert>
#include <cstdint>

#include <array>
#include <vector>
#include <string>

#include <iostream>
#include <sstream>
#include <format>

#include <algorithm>
#include <numeric>
#include <ranges>

using namespace std::literals;
#include "include/thrower.h"

extern std::istringstream data;

struct Location
{
    int row{};
    int column{};
};


void solvePad(std::vector<std::string> const &pad, Location start, std::istringstream  &data)
{
    data.clear();
    data.seekg(0);

    std::string     line;
    Location        key{start};

    while(std::getline(data,line))
    {
        if(!line.empty())
        {
            for(auto c : line)
            {
                int dr{};
                int dc{};

                switch(c)
                {
                    case 'U':  dr=-1; break;
                    case 'D':  dr=+1; break;
                    case 'L':  dc=-1; break;
                    case 'R':  dc=+1; break;
                    default:    throw_runtime_error("bad key");
                }

                if(pad[key.row+dr][key.column+dc] != ' ')
                {
                    key.row+=dr;
                    key.column+=dc;
                }
            }

            std::cout << pad[key.row][key.column];
        }
    }
    std::cout << "\n";
}

int main()
try
{
    std::vector<std::string>  pad1
    {
        "     ",
        " 123 ",
        " 456 ",
        " 789 ",
        "     "
    };

    std::vector<std::string>  pad2
    {
        "       ",
        "   1   ",
        "  234  ",
        " 56789 ",
        "  ABC  ",
        "   D   ",
        "       "
    };

    solvePad(pad1,{2,2},data);
    solvePad(pad2,{3,1},data);

    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}