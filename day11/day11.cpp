#include <cassert>
#include <cstdint>

#include <array>
#include <map>
#include <queue>

#include <iostream>
#include <sstream>
#include <format>

#include <algorithm>
#include <numeric>
#include <ranges>

using namespace std::literals;
#include "include/thrower.h"

using  Floor = uint16_t;                    // even bits = microchips
                                            // odd  bits = generators


constexpr Floor allGenerators 
{
    0b10'10'10'10'10'10'10'10,    
};


struct State
{
    int                 elevatorFloor;
    std::array<Floor,4> floors;

    auto operator<=>(State const &) const noexcept = default;
};

struct Visit
{
    bool visited{};
    int  distance{};        // number of moves to get here
};



/*

Test input

    The first floor contains a hydrogen-compatible microchip and a lithium-compatible microchip.
    The second floor contains a hydrogen generator.
    The third floor contains a lithium generator.
    The fourth floor contains nothing relevant.

    F4 .  .  .  .  .  
    F3 .  .  .  LG .  
    F2 .  HG .  .  .  
    F1 E  .  HM .  LM 
*/

constexpr State testInitialState 
{
    0,
    {
        0b01'01,    
        0b10'00,    
        0b00'10,    
        0b00'00,    
    }
};

constexpr State testFinalState 
{
    3,
    {
        0b00'00,    
        0b00'00,    
        0b00'00,    
        0b11'11,    
    }
};



/* 

Real input

    The first floor contains a thulium generator, a thulium-compatible microchip, a plutonium generator, and a strontium generator.
    The second floor contains a plutonium-compatible microchip and a strontium-compatible microchip.
    The third floor contains a promethium generator, a promethium-compatible microchip, a ruthenium generator, and a ruthenium-compatible microchip.
    The fourth floor contains nothing relevant.

          Thul    Plut    Strn    Prom    Ruth
    F4    .  .    .  .    .  .    .  .    .  .
    F3    .  .    .  .    .  .    G  M    G  M
    F2    .  .    .  M    .  M    .  .    .  .
    F1    G  M    G  .    G  .    .  .    .  .


*/




constexpr State realInitialState 
{
    0,
    {
        0b11'10'10'00'00,    
        0b00'01'01'00'00,    
        0b00'00'00'11'11,    
        0b00'00'00'00'00,    
    }
};

constexpr State realFinalState 
{
    3,
    {
        0b00'00'00'00'00,    
        0b00'00'00'00'00,    
        0b00'00'00'00'00,    
        0b11'11'11'11'11,    
    }
};


uint16_t    microchip(int i)
{
    return 1u << (i*2);
}

uint16_t    generator(int i)
{
    return 1u << ((i*2)+1);
}




/* could precompute some masks */
bool isValidFloor(Floor const &floor, int numPairs)
{
    /*     if a chip (even bit) is on.
       and the corresponding generator (odd bit) is off
       and another generator (odd bit)  is on
       then 
            invalid
       else
            valid
    */

    for(int pair=0;pair<numPairs;pair++)
    {
        if(    (floor & microchip(pair))
           && !(floor & generator(pair))) 
        {
            if(floor & allGenerators)
            {
                return false;
            }
        }
    }


    return true;
}


bool isValidState(State const &state, int numPairs)
{
    for(auto floor : state.floors)
    {
        if(!isValidFloor(floor, numPairs))
        {
            return false;
        }
    }

    return true;
}



int solve(State initialState, State finalState, int numPairs)
{
    std::map<State,Visit>           progress;
    std::priority_queue<State>      toVisit;


    progress[initialState].visited  = true;
    progress[initialState].distance = 0;


    return 0;
}


int main()
try
{

    auto testSolution = solve(testInitialState,testFinalState, 2);

    assert(testSolution==11);

    auto realSolution = solve(realInitialState,realFinalState,5);


    


    return 0;
}
catch(std::exception const &e)
{
    std::cout << e.what() << '\n';
}