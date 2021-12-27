#include <cassert>
#include <cstdint>

#include <array>
#include <vector>
#include <string>
#include <set>

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

    auto operator<=>(Location const &) const noexcept = default;
};

enum Direction
{
    North,East,South,West

};


int main()
try
{
    std::string         token;
    Location            location {0,0};
    Direction           direction{North};
    std::set<Location>  visited{location};    


    while(data>>token)
    {
        assert(token[0]=='L' || token[0]=='R');
        assert(std::isdigit(token[1]));

        if(token[0]=='L')
        {
            direction = static_cast<Direction>((static_cast<int>(direction) + 4 - 1) % 4);
        }
        else
        {
            direction = static_cast<Direction>((static_cast<int>(direction) + 1) % 4);
        }

        token.erase(token.begin());

        auto distance = stoi(token);

        int dr{};
        int dc{};

        switch(direction)
        {
        case North:     dr=-1; break;
        case South:     dr=+1; break;
        case East:      dc=+1; break;
        case West:      dc=-1; break;
        }

        for(int i=0;i<distance;i++)
        {
            location.row+=dr;
            location.column+=dc;
 
            if(visited.contains(location))
            {
                std::cout << "Part 2 : " << std::abs(location.row)+std::abs(location.column) << "\n";
            }
            else
            {
                visited.insert(location);
            }
        }
     }

    std::cout << "Part 1 : " << std::abs(location.row)+std::abs(location.column) << "\n";


    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}