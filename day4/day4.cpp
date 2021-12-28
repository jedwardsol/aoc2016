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

struct Room
{
    std::string raw;
    std::string name;
    int         sector;
    std::string checksum;
};


bool valid(Room const &room)
{
    std::array<std::pair<char,int>,26>   frequency{};

    for(auto c : room.name)
    {
        if(c!='-')
        {
            frequency[c-'a'].first=c;
            frequency[c-'a'].second++;
        }
    }

    auto sorter=[](auto const &p1, auto const &p2)
    {
        if(p1.second != p2.second)
        {
            return p1.second > p2.second;       // decreasing frequency
        }
        else
        {
            return p1.first < p2.first;         // ties : increasing alphabetically
        }
    };

    std::ranges::sort(frequency, sorter);

    for(int i=0;i<5;i++)
    {
        if(room.checksum[i] != frequency[i].first)
        {
            return false;
        }
    }

    return true;
}

auto readRooms()
{
    std::ifstream           data{"data4.txt"};
    std::vector<Room>       rooms;

    Room                    room;

    while(std::getline(data,room.raw))
    {
        auto dash = room.raw.find_last_of('-');

        assert(dash!=room.raw.npos);

        room.name   = room.raw.substr(0,dash);
        dash++;

        char *end{nullptr};
        room.sector = std::strtol(room.raw.data()+dash,&end,10);

        room.checksum = room.raw.substr( 1+end-room.raw.data(), 5);

        if(valid(room))
        {
            rooms.push_back(room);
        }
    }

    return rooms;        
}



int part1(std::vector<Room>  const &rooms)
{
    int sum{};

    for(auto room : rooms)
    {
        sum+=room.sector;
    }

    return sum;
}

void part2(std::vector<Room>  const  &rooms)
{

    for(auto room : rooms)
    {
        auto decrypted{room.name};

        for(auto &c : decrypted)
        {
            if(c!='-')
            {
                c =    ((((c - 'a') + room.sector)  % 26) +'a');
            }
        }

        if(decrypted.find("north") != decrypted.npos)
        {
            std::cout << std::format("{:3} : {}\n",room.sector, decrypted);
        }
    }
}


int main()
try
{
    auto rooms = readRooms();

    std::cout << std::format("Part 1 : {}\n",part1(rooms));

    part2(rooms);


    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}