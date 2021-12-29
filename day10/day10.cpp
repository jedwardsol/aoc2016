#include <cassert>
#include <cstdint>

#include <array>
#include <vector>
#include <string>
#include <map>

#include <iostream>
#include <sstream>
#include <format>

#include <algorithm>
#include <numeric>
#include <ranges>

#include <regex>

using namespace std::literals;
#include "include/thrower.h"

extern std::istringstream data;




struct Bot
{
    void receive(int value)
    {
        if(low == 0)
        {
            low=value;
        }
        else
        {
            assert(high==0);

            if(value > low)
            {
                high=value;
            }
            else
            {
                high=low;
                low=value;
            }
        }
    }


    void setRecipients(int low, int high)
    {
        assert(!lowRecipient.has_value());
        assert(!highRecipient.has_value());

        lowRecipient=low;
        highRecipient=high;
    }

private:

    int                 low{};
    int                 high{};

    std::optional<int>  lowRecipient{};
    std::optional<int>  highRecipient{};
};

struct Output
{
    int number;

    void receive(int value)
    {
        assert(this->value==0);
        this->value=value;
    }

private:
    int value{};        
};



int main()
try
{
    std::map<int,Output>        outputs;
    std::map<int,Bot>           bots;
    std::string                 rule;

    std::regex                  goes {R"regex(value (\d+) goes to bot (\d+))regex"};                                // value 5 goes to bot 189
    std::regex                  gives{R"regex(bot (\d+) gives low to (\w+) (\d+) and high to (\w+) (\d+))regex"};        // bot 115 gives low to bot 82 and high to bot 181
 

    while(std::getline(data,rule))
    {
        std::smatch match;

        if(std::regex_match(rule,match, goes))
        {
            assert(match.size()==3);

            auto value         = std::stoi(match[1]);
            auto recipient     = std::stoi(match[2]);

        }
        else if(std::regex_match(rule,match, gives))
        {
            assert(match.size()==6);

            auto giver         = std::stoi(match[1]);
            auto lowType       = match[2];
            auto lowRecipient  = std::stoi(match[3]);
            auto highType      = match[4];
            auto highRecipient = std::stoi(match[5]);


        }
        else
        {
            throw_runtime_error("Bad rule "s + rule);
        }

    }




    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}