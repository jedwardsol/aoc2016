#include <cassert>
#include <cstdint>

#include <array>
#include <vector>
#include <string>

#include <fstream>
#include <iostream>
#include <sstream>
#include <format>


using namespace std::literals;
#include "include/thrower.h"
#include "include/stopwatch.h"

auto getString()
{
    std::ifstream   data{"data9.txt"};
    
    if(!data)
    {
        throw_system_error("no file");
    }

    std::string     s;
    std::getline(data,s);

    return s;        
}


auto decode(std::string_view  s)      //  (AxB)
{
    auto x    =s.find('x');    assert(x!=s.npos);

    int numChars{};
    int numRepeats{};
    
    auto result = std::from_chars(s.data()+1, s.data()+x,    numChars);         assert(result.ec == std::errc{});
    result = std::from_chars(s.data()+x+1, s.data()+s.size(),numRepeats);       assert(result.ec == std::errc{});

    return std::make_pair(numChars,numRepeats);
}


void expandOnce(std::string &string)
{
    size_t   walk{};

    while(walk < string.size())
    {
        auto open=string.find('(',walk);

        if(open==string.npos)
        {
            return;
        }

        auto close=string.find(')',open);    assert(close!=string.npos);

        auto [numChars, numRepeats] = decode( std::string_view{string.data()+open,close-open});


        std::string expansion;

        for(int i=0;i<numRepeats;i++)
        {
            expansion += string.substr(close+1, numChars);
        }

        auto lenToReplace=close-open + 1 + numChars;

        string.replace(open, lenToReplace, expansion);

        walk = open + expansion.size();
    }
}



// brute force - but remove everything before the 1st ( since it is irrelevent
// this keeps the max length of the string reasonable (10^5) even though the final expanded length would be 10^9)
size_t expandAndTrim(std::string &string)
{
    size_t      trimmed{};
    size_t      maxLen {};
    int         i{};    

    auto trim=[&]()
    {
        maxLen=std::max(maxLen,string.size());

        auto open=string.find('(');

        if(open==string.npos)
        {
            open=string.size();
        }

        trimmed+=open;
        
        string.erase(0,open);
    };

    trim();

    while(string.size())
    {
        auto open=string.find('(');

        assert(open==0);


        auto close=string.find(')',open);    assert(close!=string.npos);

        auto [numChars, numRepeats] = decode( std::string_view{string.data()+open,close-open});


        std::string expansion;

        for(int i=0;i<numRepeats;i++)
        {
            expansion += string.substr(close+1, numChars);
        }

        auto lenToReplace=close-open + 1 + numChars;

        string.replace(open, lenToReplace, expansion);

        trim();

        i++;
    }


    return trimmed;
}



int main()
try
{
    std::string test{"X(8x2)(3x3)ABCY"};
    expandOnce(test);
    assert(test=="X(3x3)ABC(3x3)ABCY");

    test = "(27x12)(20x12)(13x14)(7x10)(1x12)A";
    assert(expandAndTrim(test)==241920);

    test = "(25x3)(3x3)ABC(2x3)XY(5x2)PQRSTX(18x9)(3x2)TWO(5x7)SEVEN";
    assert(expandAndTrim(test)==445);




    auto const initialString{getString()};

    auto part1{initialString};
    auto part2{initialString};
    expandOnce(part1);
    std::cout << std::format("Part 1 : {}\n",part1.size());

    Stopwatch sw;
    auto result = expandAndTrim(part2);

    std::cout << std::format("Part 2 : {} in {}s\n",result, sw.seconds());



    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}