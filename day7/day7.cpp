#include <cassert>
#include <cstdint>

#include <string>
#include <vector>

#include <fstream>
#include <iostream>
#include <sstream>
#include <format>

#include <algorithm>
#include <numeric>
#include <ranges>

using namespace std::literals;
#include "include/thrower.h"



// wysextplwqpvipxdv[srzvtwbfzqtspxnethm]syqbzgtboxxzpwr[kljvjjkjyojzrstfgrw]obdhcczonzvbfby[svotajtpttohxsh]cooktbyumlpxostt


struct IP7
{
    std::string supernet ;
    std::string hypernet;
};


auto readIP7()
{
    std::ifstream       data{"data7.txt"};
    std::vector<IP7>    ip7s;

    if(!data)
    {
        throw_system_error("No data");
    }

    IP7 ip7;

    while(std::getline(data,ip7.supernet))
    {
        size_t  open;

        while((open  = ip7.supernet.find('[')) !=ip7.supernet.npos)
        {
            auto close = ip7.supernet.find(']');

            ip7.hypernet+=ip7.supernet.substr(open+1,close-open-1) + "   ";

            ip7.supernet.replace(open, close-open+1,3,' ');
        }


        ip7s.push_back(ip7);
        ip7={};
    }

    
    return ip7s;
}

bool abba(std::string const &s)
{
    for(int i=0; i<s.size()-3;i++)
    {
        if(   s[i  ] == s[i+3]
           && s[i  ] != s[i+1]
           && s[i+1] == s[i+2])
        {
            return true;
        }
    }

    return false;

}

bool supportTLS(IP7 const &ip7)
{
    return abba(ip7.supernet)  && !abba(ip7.hypernet) ;
}

bool supportSSL(IP7 const &ip7)
{
    auto &s = ip7.supernet;

    for(int i=0; i<s.size()-2;i++)
    {
        if(   s[i  ] == s[i+2]
           && s[i  ] != s[i+1])
        {
            std::string bab { s[i+1], s[i], s[i+1]};

            if(ip7.hypernet.find(bab) != ip7.hypernet.npos)
            {
                return true;
            }
        }
    }

    return false;
}



int main()
try
{
    auto ip7s=readIP7();    

    std::cout << std::format("Part 1 : {}\n", std::ranges::count_if(ip7s, supportTLS));
    std::cout << std::format("Part 2 : {}\n", std::ranges::count_if(ip7s, supportSSL));

    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}