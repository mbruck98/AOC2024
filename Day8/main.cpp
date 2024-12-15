#include <cstdint>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>

using Map = std::vector<std::string>;

bool parseData(std::string const &puzzleInput,Map &map)
{
    std::fstream puzzleData(puzzleInput,std::fstream::in);
    if (!puzzleData.is_open())
    {
        std::cout << "Can't open the puzzle input " << std::endl;
        return false;
    }
    std::string row;
    while (std::getline(puzzleData,row))
    {
        map.push_back(row);
    }
    return true;
}

struct Position
{
    Position() : x(0), y(0)
    {
    }
    Position(long long _x,long long _y) : x(_x), y(_y)
    {
    }
    bool operator==(Position const &pos) const
    {
        return pos.x == x && pos.y == y;
    }
    uint64_t operator()(Position const &pos) const
    {
        uint64_t const hash1{std::hash<uint64_t>()(pos.x)};
        uint64_t const hash2{std::hash<uint64_t>()(pos.y)};
        return hash1 ^ (hash2 << 1);
    }
    long long x,y;
};

using Locations = std::unordered_set<Position, Position>;

uint64_t createAntinodes(Position const &firstAntenna,Position const &secondAntenna,long long const mapWidth,long long const mapHeight,Locations &antinodeLocations,bool const harmonics)
{
    auto uniqueLocations = 0ul;
    auto const dx = firstAntenna.x - secondAntenna.x;
    auto const dy = firstAntenna.y - secondAntenna.y;
    auto i = 1l;
    Position firstAntinode = {firstAntenna.x + dx * i,firstAntenna.y + dy * i};
    Position secondAntinode = {secondAntenna.x - dx * i,secondAntenna.y - dy * i};
    auto inBounds = [](Position const &pos, long const width, long const height) 
    {
        return pos.y >= 0 && pos.y < height && pos.x >= 0 && pos.x < width;
    };
    if (harmonics)
    {
        while(inBounds(firstAntinode, mapWidth, mapHeight))
        {
            uniqueLocations += antinodeLocations.insert(firstAntinode).second;
            ++i;
            firstAntinode = {firstAntenna.x + dx * i,firstAntenna.y + dy * i};
        }
        i = 1;
        while(inBounds(secondAntinode, mapWidth, mapHeight))
        {
            uniqueLocations += antinodeLocations.insert(secondAntinode).second;
            ++i;
            secondAntinode = {secondAntenna.x - dx * i, secondAntenna.y - dy * i};
        }
        uniqueLocations += antinodeLocations.insert(firstAntenna).second;
        uniqueLocations += antinodeLocations.insert(secondAntenna).second;
    }
    else
    {
        if(inBounds(firstAntinode, mapWidth, mapHeight))
        {
            uniqueLocations += antinodeLocations.insert(firstAntinode).second;
        }
        if(inBounds(secondAntinode, mapWidth, mapHeight))
        {
            uniqueLocations += antinodeLocations.insert(secondAntinode).second;
        }
    }
    return uniqueLocations;
}

uint64_t calculateImpact(Map &map,bool const harmonics)
{
    auto uniqueLocations = 0ul;
    Locations antinodeLocations;
    for(auto y1 = 0ll,h = static_cast<long long>(map.size()); y1 < h; ++y1)
    { 
        for(auto x1 = 0ll,w = static_cast<long long>(map[y1].size()); x1 < w; ++x1)
        {
            if(isdigit(map[y1][x1]) || isalpha(map[y1][x1]))
            {
                for(auto x2 = x1 + 1; x2 < w; ++x2)
                {
                    if(map[y1][x1] == map[y1][x2])
                    {
                        Position firstAntenna{x1,y1};
                        Position secondAntenna{x2,y1};
                        uniqueLocations += createAntinodes(firstAntenna,secondAntenna,w,h,antinodeLocations,harmonics);
                    }
                }
                for(auto y2 = y1 + 1; y2 < h; ++y2)
                {
                    for(auto x2 = 0ll,w = static_cast<long long>(map[y2].size()); x2 < w; ++x2)
                    {
                        if(map[y1][x1] == map[y2][x2])
                        {
                            Position firstAntenna{x1,y1};
                            Position secondAntenna{x2,y2};
                            uniqueLocations += createAntinodes(firstAntenna,secondAntenna,w,h,antinodeLocations,harmonics);
                        }
                    }
                }
            }
        }
  }
  return uniqueLocations;
}

uint64_t firstPart(Map &map)
{
    return calculateImpact(map,false);
}

uint64_t secondPart(Map &map)
{
    return calculateImpact(map,true);
}

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        std::cout << "Puzzle input not provided " << std::endl;
        return -1;
    }
    Map map;
    if(!parseData(argv[1], map))
    {
        return -1;
    }
    std::cout << "First part: " << firstPart(map) << std::endl;
    std::cout << "Second part: " << secondPart(map) << std::endl;
    return 0;
}