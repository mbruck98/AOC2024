#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <unordered_set>
#include <queue>
#include <numeric>

using Garden = std::vector<std::string>;

bool parseData(std::string const& puzzleInput,Garden& garden)
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
        garden.push_back(row);
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

    bool inBounds(long const width, long const height) const
    {
        return y >= 0 && y < height && x >= 0 && x < width;
    }

    long x,y;
};

struct Direction
{
    enum class Value : uint8_t
    {
        Up,
        Down,
        Left,
        Right
    };

    Direction(Position const plot,Position const neighbour)
    {
        if(plot.x == neighbour.x)
        {
            if(plot.y>neighbour.y)
            {
                value = Value::Up;
            }
            else
            {
                value = Value::Down;
            }
        }
        else
        {
            if(plot.x>neighbour.x)
            {
                value = Value::Left;
            }
            else
            {
                value = Value::Right;
            }
        }
    }

    bool operator==(Direction const& dir) const 
    {
        return value == dir.value;
    }
  
    Value value;
};

struct Region
{
    Region(uint64_t const _area, uint64_t const _perimiter) 
    : area(_area)
    , perimiter(_perimiter)
    {
    }

    uint64_t area,perimiter;
};

uint64_t calculateFenceCost(Garden const& garden,bool const discounted)
{
    std::unordered_set<Position,Position> visitedPlots;
    std::vector<Region> regions;
    for(auto y = 0ll,h = static_cast<long long>(garden.size()); y < h; ++y)
    { 
        for(auto x = 0ll,w = static_cast<long long>(garden[y].size()); x < w; ++x)
        {
            Position const position(x,y);
            if(visitedPlots.insert(position).second)
            {
                auto const plantType = garden[y][x];
                std::queue<Position> positionsToCheck;
                std::vector<std::pair<Position,Direction>> fences;
                positionsToCheck.push(position);
                Region region{1,0};
                while (!positionsToCheck.empty())
                {
                    Position const& pos = positionsToCheck.front();
                    Position const newPositions[] =
                    {
                        {pos.x,pos.y+1},
                        {pos.x,pos.y-1},
                        {pos.x-1,pos.y},
                        {pos.x+1,pos.y}
                    };
                    for(auto const& newPos:newPositions)
                    {
                        if(newPos.inBounds(w,h) && garden[newPos.y][newPos.x] == plantType)
                        {
                            if(visitedPlots.insert(newPos).second)
                            {
                                ++region.area;
                                positionsToCheck.push(newPos);  
                            }
                        }
                        else
                        {
                            ++region.perimiter;
                            Direction const fenceDirection{pos,newPos};
                            fences.emplace_back(pos,fenceDirection);
                        }
                    }
                    positionsToCheck.pop();
                }
                if(discounted)
                {
                    for(auto fence1 = fences.cbegin(); fence1 != fences.cend(); ++fence1)
                    {
                        for(auto fence2 = fences.cbegin() + 1; fence2 != fences.cend(); ++fence2)
                        {
                            if(fence1->first.x == fence2->first.x && fence1->first.y == fence2->first.y + 1 && fence1->second == fence2->second)
                            {
                                --region.perimiter;
                            }
                        }
                    }
                    for(auto fence1 = fences.cbegin(); fence1 != fences.cend(); ++fence1)
                    {
                        for(auto fence2 = fences.cbegin() + 1; fence2 != fences.cend(); ++fence2)
                        {
                            if(fence1->first.y == fence2->first.y && fence1->first.x == fence2->first.x + 1 && fence1->second == fence2->second)
                            {
                                --region.perimiter;
                            }
                        }
                    }
                    if(region.perimiter != 4)
                    {
                        region.perimiter -= region.perimiter % 2;
                    }
                }
                regions.push_back(region);
            }
        }
    }
    return std::accumulate(regions.cbegin(),regions.cend(),0ul,[](auto const total,auto const region){return total + region.area*region.perimiter;});
}

uint64_t firstPart(Garden const& garden)
{
    return calculateFenceCost(garden,false);
}

uint64_t secondPart(Garden const& garden)
{
    return calculateFenceCost(garden,true);
}

int main(int argc,char**argv)
{
    if(argc < 2)
    {
        std::cout << "Puzzle input not provided "<< std::endl;
        return -1;
    }
    Garden garden;
    if(!parseData(argv[1],garden))
    {
        return -1;
    }
    std::cout << "First part: " << firstPart(garden) << std::endl;
    std::cout << "Second part: " << secondPart(garden) << std::endl;
    return 0;
}

