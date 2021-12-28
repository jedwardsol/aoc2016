//TODO :valarray and gslice
#include <cassert>
#include <cstdint>

#include <string>
#include <vector>

#include <iostream>
#include <sstream>
#include <format>

#include <algorithm>
#include <numeric>
#include <ranges>

using namespace std::literals;
#include "include/thrower.h"

extern std::istringstream realData;


struct Operation
{
    enum class Type
    {
        Rect,
        RotateRow,
        RotateColumn
    }       type;

    union
    {
        struct 
        {
            int width;
            int height;
        } rect;

        struct 
        {
            int offset;
            int distance;
        } rotate;

    };
};


Operation readOperation(std::string_view  line)
{
    Operation operation;

    static const auto rectPrefix  {"rect "s};
    static const auto rowPrefix   {"rotate row y="s};
    static const auto columnPrefix{"rotate column x="s};

    if(line.starts_with(rectPrefix))
    {
        operation.type = Operation::Type::Rect;

        line.remove_prefix(rectPrefix.size());

        //  WxH

        auto result = std::from_chars(line.data(), line.data()+line.size(), operation.rect.width);
        assert(result.ec == std::errc{});

        line.remove_prefix(result.ptr-line.data()+1);

        result = std::from_chars(line.data(), line.data()+line.size(), operation.rect.height);
        assert(result.ec == std::errc{});
    }
    else if(line.starts_with(rowPrefix))
    {
        operation.type = Operation::Type::RotateRow;
        line.remove_prefix(rowPrefix.size());

        // O by D

        auto result = std::from_chars(line.data(), line.data()+line.size(), operation.rotate.offset);
        assert(result.ec == std::errc{});

        line.remove_prefix(result.ptr-line.data()+4);

        result = std::from_chars(line.data(), line.data()+line.size(), operation.rotate.distance);
        assert(result.ec == std::errc{});
    }
    else if(line.starts_with(columnPrefix))
    {
        operation.type = Operation::Type::RotateColumn;
        line.remove_prefix(columnPrefix.size());

        auto result = std::from_chars(line.data(), line.data()+line.size(), operation.rotate.offset);
        assert(result.ec == std::errc{});

        line.remove_prefix(result.ptr-line.data()+4);

        result = std::from_chars(line.data(), line.data()+line.size(), operation.rotate.distance);
        assert(result.ec == std::errc{});
    }
    else
    {
        throw_runtime_error("bad operation");
    }

    return operation;
}

auto readOperations()
{
    std::string             line;
    std::vector<Operation>  operations;

    while(std::getline(realData,line))
    {
        operations.push_back(readOperation(line));        
    }

    return operations;
}


using Screen = std::array<std::array<bool,50>,6>;


void draw(Screen const &screen)
{
    for(auto row : screen)
    {
        for(auto pixel : row)
        {
            std::cout << (pixel ? '#' : ' ');
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

int count(Screen const &screen)
{
    int count{};

    for(auto row : screen)
    {
        count+= std::ranges::count(row,true);
    }

    return count;        
}



int main()
try
{
    auto operations=readOperations();


    Screen screen{};

    for(auto const &operation : operations)
    {
        switch(operation.type)
        {
        case Operation::Type::Rect:
        {
            for(int row=0;row<operation.rect.height;row++)
            {
                for(int column=0;column<operation.rect.width;column++)
                {
                    screen[row][column]=true;
                }
            }
        }
        break;

        case Operation::Type::RotateRow:
        {
            auto const row      = operation.rotate.offset;
            auto const distance = operation.rotate.distance;

            auto const originalRow=screen[row];
            auto      &newRow=screen[row];

            for(int column=0;column<50;column++)
            {
                newRow[column]=  originalRow[ (column+50 - distance) % 50];
            }
        }
        break;


        case Operation::Type::RotateColumn:
        {
            auto const column   = operation.rotate.offset;
            auto const distance = operation.rotate.distance;

            std::array<bool,6> originalColumn;

            for(int row=0;row<6;row++)
            {
                originalColumn[row]=screen[row][column];
            }

            for(int row=0;row<6;row++)
            {
                screen[row][column]=  originalColumn[ (row+6 - distance) % 6];
            }
        }
        break;
        }
    }
    draw(screen);

    std::cout << std::format("Part 1 : {}\n",count(screen));

    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}