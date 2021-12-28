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


int main()
try
{
    auto operations=readOperations();

    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}