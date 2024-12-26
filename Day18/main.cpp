#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <queue>
#include <algorithm>

using Memory = std::vector<std::string>;
constexpr auto kSize = 71ul;

struct Position
{
    Position() : x(0), y(0)
    {
    }

    Position(uint64_t const  _x,uint64_t const  _y) : x(_x), y(_y)
    {
    }

    bool inBounds(uint64_t const width, uint64_t const height) const
    {
        return y < height && x < width;
    }

    uint64_t x,y;
};

bool parseData(std::string const& puzzleInput,std::vector<Position>& bytes)
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
        auto x = std::stoul(row.substr(0,row.find(',')));
        auto y = std::stoul(row.substr(row.find(',')+1));
        bytes.push_back({x,y});
    }
    return true;
}

struct Step
{
    Step()
    : pos{0,0}
    , distance(0)
    {
    }

    Step(Position const _pos, uint64_t const _distance)
    : pos(_pos)
    , distance(_distance)
    {
    }

    bool operator<(Step const& step) const
    {
        return step.distance < distance;
    }

    Position pos;
    uint64_t distance;
};

uint64_t traverseMemory(Memory& memory)
{
    std::priority_queue<Step> pq;
    pq.push({{0,0},0});
    while(!pq.empty())
    {
        auto const step = pq.top();
        pq.pop();
        if(step.pos.x == kSize - 1 && step.pos.y == kSize - 1)
        {
            for(auto& row:memory)
            {
                std::replace(row.begin(),row.end(),'O','.');
            }
            return step.distance;
        }
        Position const newPositions[] =
        {
            {step.pos.x-1,step.pos.y},
            {step.pos.x+1,step.pos.y},
            {step.pos.x,step.pos.y-1},
            {step.pos.x,step.pos.y+1}
        };
        for(auto const& pos:newPositions)
        {
            if(pos.inBounds(kSize,kSize) && memory[pos.y][pos.x] == '.')
            {
                pq.push({pos,step.distance + 1});
                memory[pos.y][pos.x] = 'O';
            }
        }
    }
    return 0;
}

uint64_t firstPart(std::vector<Position> const& bytes)
{
    Memory memory{kSize,std::string(kSize,'.')};
    for(auto i = 0ul; i < 1024; ++i)
    {
        memory[bytes[i].y][bytes[i].x] = '#';
    }
    return traverseMemory(memory);
}

std::string secondPart(std::vector<Position> const& bytes)
{
    Memory memory {kSize,std::string(kSize,'.')};
    auto currentByte = 0ul;
    while(traverseMemory(memory) != 0)
    {
        ++currentByte;
        memory[bytes[currentByte].y][bytes[currentByte].x] = '#';
    }
    return {std::to_string(bytes[currentByte].x)+','+std::to_string(bytes[currentByte].y)};
}

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        std::cout << "Puzzle input not provided "<< std::endl;
        return -1;
    }
    std::vector<Position> bytes;
    if(!parseData(argv[1],bytes))
    {
        return -1;
    }
    std::cout << "First part: " << firstPart(bytes) << std::endl;
    std::cout << "Second part: " << secondPart(bytes) << std::endl;
    return 0;
}