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

std::regex const            goes {R"regex(value (\d+) goes to bot (\d+))regex"};                                        // value 5 goes to bot 189
std::regex const            gives{R"regex(bot (\d+) gives low to (\w+) (\d+) and high to (\w+) (\d+))regex"};           // bot 115 gives low to bot 82 and high to bot 181

struct Output
{
    int number;

    void receive(int value)
    {
        assert(this->value==0);
        this->value=value;
    }

    int getValue()
    {
        assert(value!=0);
        return value;
    }

private:
    int value{};        
};


struct Bot
{
    int                 number{};

    void receive(int value, std::map<int,Bot> &bots, std::map<int,Output> &outputs)
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

            send(lowRecipient,low,bots,outputs);
            send(highRecipient,high,bots,outputs);
    
        }
    }

    static void send(std::optional<int> &recipient, int value, std::map<int,Bot> &bots, std::map<int,Output> &outputs)
    {
        assert(recipient.has_value());

        if(recipient.value() >= 1000)
        {
            outputs[recipient.value()-1000].receive(value);
        }
        else
        {
            bots[recipient.value()].receive(value,bots,outputs);
        }
    }

    void setRecipients(int low, int high)
    {
        assert(!lowRecipient.has_value());
        assert(!highRecipient.has_value());

        lowRecipient=low;
        highRecipient=high;
    }

    bool part1_17_61()
    {
        assert(low!=0);
        assert(high!=0);

        return(low==17 && high==61);

    }


private:

    int                 low{};
    int                 high{};

    std::optional<int>  lowRecipient{};
    std::optional<int>  highRecipient{};
};




auto getRules()
{
    std::vector<std::string>        rules;
    std::string                     rule;

    while(std::getline(data,rule))
    {
        assert(   std::regex_match(rule,gives)
               || std::regex_match(rule,goes));


        rules.push_back(rule);
    }

    return rules;
}


int main()
try
{
    auto rules=getRules();
    

    std::map<int,Output>        outputs;
    std::map<int,Bot>           bots;

 
    for(auto &rule : rules)
    {
        std::smatch match;

        if(std::regex_match(rule,match, gives))
        {
            assert(match.size()==6);

            auto giver         = std::stoi(match[1]);
            auto lowType       = match[2];
            auto lowRecipient  = std::stoi(match[3]);
            auto highType      = match[4];
            auto highRecipient = std::stoi(match[5]);

            if(lowType =="output") lowRecipient  += 1000;
            if(highType=="output") highRecipient += 1000;

            auto &bot = bots[giver];

            bot.number=giver;

            bot.setRecipients(lowRecipient,highRecipient);
        }
    }

    for(auto &rule : rules)
    {
        std::smatch match;

        if(std::regex_match(rule,match, goes))
        {
            assert(match.size()==3);

            auto value         = std::stoi(match[1]);
            auto recipient     = std::stoi(match[2]);

            assert(bots.contains(recipient));

            auto &bot = bots[recipient];

            bot.receive(value,bots,outputs);
        }
    }

    for(auto &[number, bot] : bots)
    {
        if(bot.part1_17_61())
        {
            std::cout << std::format("Part 1 : bot {}\n",number);
        }
    }

    std::cout << std::format("Part 2 : {}\n",  outputs[0].getValue()
                                             * outputs[1].getValue()
                                             * outputs[2].getValue());

    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}