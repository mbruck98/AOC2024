#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <array>
#include <unordered_map>
#include <unordered_set>

bool parseData(std::string const& puzzleInput, std::vector<uint64_t>& numbers)
{
    std::fstream puzzleData {puzzleInput,std::fstream::in};
    if(!puzzleData.is_open())
    {
        std::cout<<"Can't open the puzzle input "<<std::endl;
        return false;
    }
    std::string row;
    while(std::getline(puzzleData,row))
    {
        numbers.push_back(std::stoul(row));
    }
    return true;
}

void evolveNumber(uint64_t& secretNumber)
{
    secretNumber = ((secretNumber * 64) ^ secretNumber) % 16777216;
    secretNumber = ((secretNumber / 32) ^ secretNumber) % 16777216;
    secretNumber = ((secretNumber * 2048) ^ secretNumber) % 16777216;
}

uint64_t firstPart(std::vector<uint64_t> const& numbers)
{
    auto result = 0ul;
    for(auto const number:numbers)
    {
        auto secretNumber = number;
        for(auto i=0;i<2000;++i)
        {
            evolveNumber(secretNumber);
        }
        result += secretNumber;
    }
    return result;
}

using Sequence = std::array<long,4>;

struct Hasher
{
    uint64_t operator()(Sequence const& sequence) const
    {
        auto hash = 0ull;
        for(auto const value : sequence)
        {
            hash ^= std::hash<uint64_t>{}(value) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};

uint64_t secondPart(std::vector<uint64_t> const& numbers)
{
    auto result = 0ul;
    std::unordered_map<Sequence,uint64_t,Hasher> sequencePrices;
    for(auto const& number:numbers)
    {
        std::unordered_set<Sequence,Hasher> sequences;
        auto currentNumber = number;
        for(auto i=0;i<2000;++i)
        {
            auto secretNumber = currentNumber;
            auto lastPrice = secretNumber % 10;
            Sequence sequence = {0,0,0,0};
            for(auto& priceChange:sequence)
            {
                evolveNumber(secretNumber);
                priceChange = secretNumber % 10 - lastPrice;
                lastPrice = secretNumber % 10;
            }
            evolveNumber(currentNumber);
            if(sequences.insert(sequence).second)
            {
                sequencePrices[sequence] += lastPrice;
            }
        }
    }
    for(auto const& [sequence,price]:sequencePrices)
    {
        if(price > result)
        {
            result = price;
        }
    }
    return result;
}

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        std::cout << "Puzzle input not provided "<< std::endl;
        return -1;
    }
    std::vector<uint64_t> numbers;
    if(!parseData(argv[1],numbers))
    {
        return -1;
    }
    std::cout << "First part: " << firstPart(numbers) << std::endl;
    std::cout << "Second part: " << secondPart(numbers) << std::endl;
    return 0;
}