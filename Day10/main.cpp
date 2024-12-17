#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <unordered_set>

using Map = std::vector<std::vector<uint64_t>>;

bool parseData(std::string const& puzzleInput,Map& map)
{
    std::fstream puzzleData(puzzleInput,std::fstream::in);
    if(!puzzleData.is_open())
    {
        std::cout<<"Can't open the puzzle input "<<std::endl;
        return false;
    }
    std::string row;
    while(std::getline(puzzleData,row))
    {
        std::vector<uint64_t> vec;
        for(auto const& num:row)
        {
            vec.push_back(num-'0');
        }
        map.push_back(vec);
    }
    return true;
}

struct Position
{
    Position() : x(0), y(0)
    {
    }
    Position(long const  _x,long const  _y) : x(_x), y(_y)
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
    long x,y;
};

bool inBounds(Position const &pos, long const width, long const height)
{
    return pos.y >= 0 && pos.y < height && pos.x >= 0 && pos.x < width;
}

uint64_t traverseTrail(Map const& map,Position const pos,std::unordered_set<Position,Position>& trailEnds)
{
    auto uniqueTrails = 0u;
    if(map[pos.y][pos.x] == 9)
    {
        trailEnds.insert(pos);
        return 1;
    }
    else
    {
        Position const newPositions[] =
        {
            {pos.x-1,pos.y},
            {pos.x+1,pos.y},
            {pos.x,pos.y-1},
            {pos.x,pos.y+1}
        };
        for(auto const& newPos:newPositions)
        {
            if(inBounds(newPos,map.size(),map[0].size()) && map[newPos.y][newPos.x] == map[pos.y][pos.x] + 1)
            {
                uniqueTrails += traverseTrail(map,newPos,trailEnds);
            }
        }
    }
    return uniqueTrails;
}

std::pair<uint64_t,uint64_t> calculateTrail(Map const& map,Position const trailHead)
{
    std::unordered_set<Position,Position> trailEnds;
    auto const uniqueTrails = traverseTrail(map,trailHead,trailEnds);
    return {trailEnds.size(),uniqueTrails};
}

uint64_t firstPart(Map const& map)
{
    auto result = 0ul;
    for(auto y=0u;y<map.size();++y)
    {
        for(auto x=0u;x<map[y].size();++x)
        {
            if(map[y][x]==0)
            {
                Position const pos{x,y};
                result += calculateTrail(map,pos).first;
            }
        }
    }
    return result;
}

uint64_t secondPart(Map const& map)
{
    auto result = 0ul;
    for(auto y=0l;y<static_cast<long>(map.size());++y)
    {
        for(auto x=0l;x<static_cast<long>(map[y].size());++x)
        {
            if(map[y][x]==0)
            {
                Position const pos{x,y};
                result += calculateTrail(map,pos).second;
            }
        }
    }
    return result;
}

int main(int argc,char**argv)
{
    if(argc<2)
    {
        std::cout << "Puzzle input not provided "<< std::endl;
        return -1;
    }
    Map map;
    if(!parseData(argv[1],map))
    {
        return -1;
    }
    std::cout << "First part: " << firstPart(map) << std::endl;
    std::cout << "Second part: " << secondPart(map) << std::endl;
    return 0;
}