#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

using XY = std::pair<long long,long long>;

struct Machine
{
    Machine(XY const buttonA,XY const buttonB,XY const _prize)
    : A(buttonA)
    , B(buttonB)
    , prize(_prize)
    {}
    XY A,B,prize;
};

bool parseData(std::string const& puzzleInput, std::vector<Machine>& machines)
{
    std::fstream puzzleData(puzzleInput,std::fstream::in);
    if(!puzzleData.is_open())
    {
        std::cout<<"Can't open the puzzle input "<<std::endl;
        return false;
    }
    std::string row;
    while(!puzzleData.eof())
    {
        std::getline(puzzleData,row);
        XY buttonA;
        buttonA.first=std::stoul(row.substr(12,2));
        buttonA.second=std::stoul(row.substr(18,2));
        std::getline(puzzleData,row);
        XY buttonB;
        buttonB.first=std::stoul(row.substr(12,2));
        buttonB.second=std::stoul(row.substr(18,2));
        std::getline(puzzleData,row);
        XY prize;
        prize.first=std::stoul(row.substr(9,5));
        prize.second=std::stoul(row.substr(row.find("Y=",0)+2));
        machines.emplace_back(buttonA,buttonB,prize);
        std::getline(puzzleData,row);
    }
    return true;
}

XY solve(Machine const& machine, long long const offset)
{
    if(((machine.prize.second + offset) * machine.A.first - (machine.prize.first + offset) * machine.A.second) % (machine.B.second * machine.A.first - machine.B.first * machine.A.second))
    {
        return {0,0};
    }
    auto const bTimes = ((machine.prize.second + offset) * machine.A.first - (machine.prize.first+offset) * machine.A.second) / (machine.B.second * machine.A.first - machine.B.first * machine.A.second);
    if(((machine.prize.first + offset) - bTimes * machine.B.first) % machine.A.first)
    {
        return {0,0};
    }
    auto const aTimes = ((machine.prize.first + offset) - bTimes * machine.B.first) / machine.A.first;
    return {aTimes,bTimes};
}

uint64_t firstPart(std::vector<Machine> const& machines)
{
    auto result = 0ul;
    for(auto const& machine:machines)
    {
        auto const solution = solve(machine,0);
        result += 3 * solution.first + solution.second;
    }
    return result;
}

uint64_t secondPart(std::vector<Machine> const& machines)
{
    auto result = 0ul;
    for(auto const& machine:machines)
    {
        auto const solution = solve(machine,10000000000000);
        result += 3 * solution.first + solution.second;
    }
    return result;
}

int main(int argc,char**argv)
{
    if(argc < 2)
    {
        std::cout << "Puzzle input not provided "<< std::endl;
        return -1;
    }
    std::vector<Machine> machines;
    if(!parseData(argv[1],machines))
    {
        return -1;
    }
    std::cout << "First part: " << firstPart(machines) << std::endl;
    std::cout << "Second part: " << secondPart(machines) << std::endl;
    return 0;
}