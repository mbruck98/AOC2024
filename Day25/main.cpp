#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <cstdint>
#include <algorithm>

constexpr auto schematicsWidth = 5ul;
constexpr auto schematicsHeight = 6ul;

using PinHeights = std::array<uint64_t,schematicsWidth>;

bool parseData(std::string const& puzzleInput,std::vector<PinHeights>& locks,std::vector<PinHeights>& keys)
{
    std::fstream puzzleData {puzzleInput,std::fstream::in};
    if(!puzzleData.is_open())
    {
        std::cout<<"Can't open the puzzle input "<<std::endl;
        return false;
    }
    std::string row;
    std::getline(puzzleData,row);
    while(!puzzleData.eof())
    {
        if(row.length()==0)
        {
            std::getline(puzzleData,row);
        }
        else if(row[0]=='.')
        {
            PinHeights key = {0,0,0,0,0};
            for(auto y = 0ul; y < schematicsHeight; ++y)
            {
                for(auto x = 0ul; x < row.length(); ++x)
                {
                    if(row[x] == '#')
                    {
                        ++key[x];
                    }
                }
                std::getline(puzzleData,row);
            }
            std::getline(puzzleData,row);
            keys.push_back(key);
        }
        else
        {
            std::getline(puzzleData,row);
            PinHeights lock = {0,0,0,0,0};
            for(auto y = 0ul; y < schematicsHeight; ++y)
            {
                for(auto x = 0ul; x < row.length(); ++x)
                {
                    if(row[x] == '#')
                    {
                        ++lock[x];
                    }
                }
                std::getline(puzzleData,row);
            }
            locks.push_back(lock);
        }
    }
    return true;
}

uint64_t firstPart(std::vector<PinHeights> const& locks, std::vector<PinHeights> const& keys)
{
    auto result = 0ul;
    for(auto const& lock : locks)
    {
        for(auto const& key : keys)
        {
            auto lockHeight = lock.begin();
            result += std::none_of(key.begin(),key.end(),[&lockHeight](auto const& keyHeight){return *lockHeight++ + keyHeight >= schematicsHeight;});
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
    std::vector<PinHeights> locks;
    std::vector<PinHeights> keys;
    if(!parseData(argv[1],locks,keys))
    {
        return -1;
    }
    std::cout << "First part: " << firstPart(locks,keys) << std::endl;
    return 0;
}