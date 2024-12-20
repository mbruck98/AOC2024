#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <numeric>

bool parseData(std::string const& puzzleInput,std::vector<uint64_t>& stones)
{
    std::fstream puzzleData(puzzleInput,std::fstream::in);
    if(!puzzleData.is_open())
    {
        std::cout<<"Can't open the puzzle input "<<std::endl;
        return false;
    }
    auto stone = 0ul;
    while(puzzleData >> stone)
    {
        stones.push_back(stone);
    }
    return true;
}

std::pair<uint64_t,uint64_t> split(uint64_t const number)
{
    auto const strNum = std::to_string(number);
    auto const length = strNum.length() / 2;
    auto const firstNumber = std::stoull(strNum.substr(0,length));
    auto const secondNumber = std::stoull(strNum.substr(length));
    return {firstNumber,secondNumber};
}

void blink(std::pair<uint64_t,uint64_t> const stone,std::unordered_map<uint64_t,uint64_t>& stoneMap)
{
    auto const stoneValue = stone.first;
    auto const stoneCount = stone.second;
    if(stoneValue == 0)
    {
        stoneMap[1] += stoneCount;
    }
    else if(std::to_string(stoneValue).length() % 2 == 0)
    {
        auto const newStones = split(stoneValue);
        stoneMap[newStones.first] += stoneCount;
        stoneMap[newStones.second] += stoneCount;
    }
    else
    {
        stoneMap[stoneValue * 2024] += stoneCount;
    }
}

uint64_t countStones(std::vector<uint64_t> const& stones,uint64_t const count)
{
    std::unordered_map<uint64_t,uint64_t> stoneMap;
    for(auto const& stone:stones)
    {
        stoneMap[stone] = 1;
    }
    for(auto i = 0ul; i < count; ++i)
    {
        std::unordered_map<uint64_t,uint64_t> currentStones;
        for(auto const& stone:stoneMap)
        {
            blink(stone,currentStones);
        }
        stoneMap = std::move(currentStones);
    }
    return std::accumulate(stoneMap.begin(),stoneMap.end(),0ul,[](auto const total,auto const stone){return total + stone.second;});
}

uint64_t firstPart(std::vector<uint64_t> const& stones)
{
    return countStones(stones,25);
}

uint64_t secondPart(std::vector<uint64_t> const& stones)
{
    return countStones(stones,75);
}

int main(int argc,char**argv)
{
    if(argc < 2)
    {
        std::cout << "Puzzle input not provided "<< std::endl;
        return -1;
    }
    std::vector<uint64_t> stones;
    if(!parseData(argv[1],stones))
    {
        return -1;
    }
    std::cout << "First part: " << firstPart(stones) << std::endl;
    std::cout << "Second part: " << secondPart(stones) << std::endl;
    return 0;
}
