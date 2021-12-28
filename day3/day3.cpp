#include <cassert>
#include <cstdint>

#include <array>
#include <vector>
#include <string>

#include <fstream>
#include <iostream>
#include <sstream>
#include <format>

#include <algorithm>
#include <numeric>
#include <ranges>

using namespace std::literals;
#include "include/thrower.h"


struct Triangle
{
    std::array<int,3> side;
};

auto readTriangles()
{
    std::ifstream           data{"data3.txt"};
    std::vector<Triangle>   triangles;

    Triangle t;

    while(data >> t.side[0] >> t.side[1] >> t.side[2])
    {
        triangles.push_back(t);
    }

    return triangles;        
}

auto extractFromColumns(std::vector<Triangle> const &originalTriangles)
{
    std::vector<Triangle>   triangles;

    for(int row=0; row<originalTriangles.size(); row+=3)
    {
        for(auto column=0;column<3;column++)
        {
            triangles.push_back(    {originalTriangles[row  ].side[column],
                                     originalTriangles[row+1].side[column],
                                     originalTriangles[row+2].side[column]});
        }
    }

    return triangles;        
}


int main()
try
{
    auto originalTriangles = readTriangles();
    auto moreTriangles     = extractFromColumns(originalTriangles);       // no need to store them really


    auto isTriangle = [](Triangle t) 
    { 
        std::ranges::sort(t.side); 
        return t.side[0] + t.side[1] > t.side[2];
    };


    std::cout << std::format("Part 1 : {}\n",std::ranges::count_if(originalTriangles, isTriangle));
    std::cout << std::format("Part 2 : {}\n",std::ranges::count_if(moreTriangles, isTriangle));

    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}