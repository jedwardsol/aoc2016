#include <cassert>
#include <cstdint>

#include <string>
#include <map>
#include <array>

#include <iostream>
#include <sstream>
#include <format>

#include <algorithm>
#include <numeric>
#include <ranges>

using namespace std::literals;
#include "include/thrower.h"

extern std::istringstream realData;



int main()
try
{
    std::array<std::map<char,int>,8> counts;

    std::string line;

    while(std::getline(realData,line))
    {
        if(!line.empty())
        {
            assert(line.size()==8);

            for(int i=0;i<8;i++)
            {
                counts[i][line[i]]++;
            }
        }
    }

    std::string mostCommon{};
    std::string leastCommon{};

    for(auto &map : counts)
    {
        auto c = std::ranges::max_element(map, [](auto const &lhs, auto const &rhs){return lhs.second < rhs.second;});
        mostCommon+=c->first;

        c = std::ranges::min_element(map, [](auto const &lhs, auto const &rhs){return lhs.second < rhs.second;});
        leastCommon+=c->first;
    }

    std::cout << std::format("Part 1 : {}\nPart 2 : {}\n",mostCommon,leastCommon);


    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}