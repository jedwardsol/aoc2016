#include <cassert>
#include <cstdint>

#include <array>
#include <vector>
#include <string>

#include <iostream>
#include <sstream>
#include <format>

#include <thread>
#include <mutex>


using namespace std::literals;
#include "include/thrower.h"
#include "wrap_bcrypt.h"

auto const base{"reyedfim"s};
//auto const base{"abc"s};

std::string         password1(8,'_');
std::string         password2(8,'_');
std::mutex          passwordLock;
std::atomic_int     digits1{};
std::atomic_int     digits2{};


void hasher(int start, int increment)
{
    int index{start};

    while(digits2 < 8)
    {
        BCrypt::Hash hasher;

        auto const doorId = base+std::to_string(index);
        auto const digest = hasher.append(doorId).digest();

        if(digest.starts_with("00000"))
        {
            std::unique_lock _{passwordLock};

            if(digits1 < 8)
            {
                password1[digits1] = digest[5];
                digits1++;
            }

            if(digits2 < 8)
            {
                int index = digest[6] - '0';

                if(index < 8)
                {
                    password2[index]=digest[7];
                }
                digits2++;
            }

            std::cout << password1 << ' ' << password2 << "\n";
        }                                           
        index+=increment;
    }
}


int main()
try
{
    constexpr int numThreads=6;

    std::array<std::thread,numThreads>   threads;

    for(int i=0;i<numThreads;i++)
    {
        threads[i] = std::thread{hasher,i,numThreads};
    }

    for(auto &thread:threads)
    {
        thread.join();
    }


    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}