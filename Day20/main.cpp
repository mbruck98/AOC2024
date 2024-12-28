#include <iostream>
#include <fstream>
#include <cstdint>
#include <cmath>
#include <vector>
#include <unordered_set>

using Map = std::vector<std::string>;

bool parseData(std::string const& puzzleInput, Map& map)
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
        map.push_back(row);
    }
    return true;
}

struct Position
{
    Position() : x(0), y(0)
    {
    }

    Position(uint64_t const  _x, uint64_t const  _y) : x(_x), y(_y)
    {
    }

    bool operator==(Position const& pos) const
    {
        return pos.x == x && pos.y == y;
    }

    bool operator!=(Position const& pos) const
    {
        return !(pos==*this);
    }

    uint64_t operator()(Position const& pos) const
    {
        return pos.x + pos.y * UINT32_MAX;
    }

    uint64_t x,y;
};

std::vector<Position> getRacetrackPositions(Map const& map)
{
    std::vector<Position> positions;
    std::unordered_set<Position,Position> visited;
    Position start;
    Position end;
    for(auto y = 0ul, h = map.size(); y < h; ++y)
    {
        for(auto x = 0ul, w = map[y].size(); x < w; ++x)
        {
            if(map[y][x] == 'S')
            {
                start = {x,y};
            }
            else if(map[y][x] == 'E')
            {
                end = {x,y};
            }
        }
    }
    positions.push_back(start);
    while(start != end)
    {
        Position const newPositions[] =
        {
            {start.x-1,start.y},
            {start.x+1,start.y},
            {start.x,start.y-1},
            {start.x,start.y+1}
        };
        for(auto const& pos:newPositions)
        {
            if(map[pos.y][pos.x] != '#' && visited.count(pos) == 0)
            {
                positions.push_back(pos);
                visited.insert(pos);
                start = pos;
                break;
            }
        }
    }
    return positions;
}

uint64_t countCheats(Map const& map, uint64_t const minimumSave, uint64_t const cheatDuration)
{
    auto count = 0ul;
    auto const positions = getRacetrackPositions(map);
    for(auto pos2 = positions.cbegin() + minimumSave; pos2 != positions.cend(); ++pos2)
    {
        for(auto pos1 = positions.cbegin(); pos1 < pos2 - minimumSave; ++pos1)
        {
            uint64_t const distance = std::llabs(pos2->x - pos1->x) + std::llabs(pos2->y - pos1->y);
            if (pos2 - pos1 - distance >= minimumSave && distance <= cheatDuration)
            {
                ++count;
            }
        }
    }
    return count;
}

uint64_t firstPart(Map const& map)
{
    return countCheats(map,100,2);
}

uint64_t secondPart(Map const& map)
{
    return countCheats(map,100,20);
}

int main(int argc, char** argv)
{
    if(argc < 2)
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