#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <tuple>
#include <unordered_set>
#include <set>

using Map = std::vector<std::string>;

bool parseData(std::string const& puzzleInput,Map& map)
{
    std::fstream puzzleData(puzzleInput,std::fstream::in);
    if(!puzzleData.is_open())
    {
        std::cout<<"Can't open input.txt"<<std::endl;
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
    Position(): x(0),y(0)
    {}
    long x,y;  
};

struct Move
{
    Move(): dx(0),dy(0)
    {}
    Move(Position const _pos,long const _dx, long const _dy) : pos(_pos) , dx(_dx) , dy(_dy)
    {}
    bool operator==(Move const& move) const
    {
        return move.dx == dx && move.dy == dy && move.pos.x == pos.x && move.pos.y == pos.y;
    }
    uint64_t operator()(Move const& move) const  
    {
        auto hash = move.pos.x * 31u + move.pos.y * 23u + move.dx * 7u  + move.dy;
        return hash;
    }
    Position pos;
    long dx,dy;
};

struct Guard 
{
    Guard(Map const& map) : dx(-1),dy(0)
    {
        for(auto i=0ul;i<map.size();++i)
        {
            auto y = map[i].find('^');
            if(y != std::string::npos)
            {
                pos.x = i;
                pos.y = y;
                break;
            }
        } 
    }
    bool walk(Map& map)
    {
        if(map[pos.x+dx][pos.y+dy] == '#')
        {
            if(dx == 0)
            {
                dx += dy;
                dy += -dy;   
            }
            else
            {
                dy += -dx;
                dx += -dx;
            }         
        }
        else
        {
            pos.x += dx;
            pos.y += dy;
            if(map[pos.x][pos.y] != 'X')
            {
                map[pos.x][pos.y] = 'X';
                return true;
            }   
        }
        return false;
    }
    bool inBounds(Map const& map)
    {
        return pos.x + dx < static_cast<long long>(map.size()) && pos.x + dx >= 0 && pos.y + dy < static_cast<long long>(map[pos.x].size()) && pos.y + dy >= 0;
    }

    void move(Map& map,Move const& move)
    {
        pos = move.pos;
        dx = move.dx;
        dy = move.dy;
        map[move.pos.x][move.pos.y]='^';
    }
    
    Position pos;
    long dx,dy;
};


uint64_t firstPart(Map map)
{
    auto result = 0ul;
    Guard guard{map};
    while(guard.inBounds(map))
    {
        if(guard.walk(map))
        {
            result++;
        }
    }
    return result;
}

uint64_t secondPart(Map map)
{
    auto result = 0ul;
    Guard guard{map};
    Move start{guard.pos,guard.dx,guard.dy};
    std::unordered_set<Move,Move> previousMoves;
    while(guard.inBounds(map))
    {
        static_cast<void>(guard.walk(map));
    }
    guard.move(map,start);
    for(auto& row:map)
    {
        for(auto& tile:row)
        {   
            if(tile == 'X')
            {
                tile = '#';
                while(guard.inBounds(map))
                {
                    Move const currentMove{guard.pos,guard.dx,guard.dy};
                    if(previousMoves.find(currentMove) != previousMoves.cend())
                    {
                        ++result;
                        break;
                    }
                    previousMoves.insert(currentMove);
                    static_cast<void>(guard.walk(map));
                }
                tile = 'X';
                guard.move(map,start);
                previousMoves.clear();
            }
        }
    }
    return result;
}

int main(int argc,char** argv)
{
    if(argc<2)
    {
        std::cout<<"Puzzle input not provided "<<std::endl;
        return -1;
    }
    Map map;
    if(!parseData(argv[1],map))
    {
        return -1;
    }
    std::cout<<"First part: "<< firstPart(map) << std::endl;
    std::cout<<"Second part: "<< secondPart(map) << std::endl;
    return 0;
}