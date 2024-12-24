#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <optional>
#include <regex>

using Map = std::vector<std::string>;

bool parseData(std::string const& puzzleInput, Map& map, std::string& moves)
{
    std::fstream puzzleData {puzzleInput,std::fstream::in};
    if(!puzzleData.is_open())
    {
        std::cout<<"Can't open the puzzle input "<<std::endl;
        return false;
    }
    std::string row;
    std::getline(puzzleData,row);
    while(row.length() > 0)
    {
        map.push_back(row);
        std::getline(puzzleData,row);
    }
    while(!puzzleData.eof())
    {
        moves += puzzleData.get();
    }
    moves.erase(std::remove(moves.begin(),moves.end(),'\n'),moves.end());
    moves.pop_back();
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
    uint64_t x,y;
};

bool checkMove(Map const& map, Position const pos, char const direction, long const dx, long const dy)
{
    Position const newPos {pos.x+dx,pos.y+dy};
    Position otherHalf {newPos};
    switch(map[newPos.y][newPos.x])
    {
        case '#':
            return false;
        case 'O':
            return checkMove(map,newPos,direction,dx,dy);
        case '[':
            ++otherHalf.x;
            break;
        case ']':
            --otherHalf.x;
            break;
        default:
            return true;
    }
    if(!checkMove(map,otherHalf,direction,dx,dy))
    {
        return false;
    }
    if(dy != 0 && !checkMove(map,newPos,direction,dx,dy))
    {
        return false;
    }
    return true;
}

void move(Map& map, Position const pos, char const direction, long const dx, long const dy)
{
    Position const newPos {pos.x+dx,pos.y+dy};
    if(map[newPos.y][newPos.x]=='O')
    {
        move(map,newPos,direction,dx,dy);
    }
    if(map[newPos.y][newPos.x]=='[')
    {
        Position const otherHalf {newPos.x+1,newPos.y};
        move(map,otherHalf,direction,dx,dy);
        move(map,newPos,direction,dx,dy);
    }
    else if(map[newPos.y][newPos.x]==']')
    {
        Position const otherHalf {newPos.x-1,newPos.y};
        move(map,otherHalf,direction,dx,dy);
        move(map,newPos,direction,dx,dy);
    }
    std::swap(map[newPos.y][newPos.x],map[pos.y][pos.x]);
}

std::optional<Position> moveRobot(Map& map, Position const pos, char const direction)
{
    auto dx=0l, dy=0l;
    switch(direction)
    {
        case '^':
            dy=-1;
            break;
        case 'v':
            dy=1;
            break;
        case '<':
            dx=-1;
            break;
        case '>':
            dx=1;
            break;
    }
    if(!checkMove(map,pos,direction,dx,dy))
    {
        return std::nullopt;
    }
    move(map,pos,direction,dx,dy);
    Position const newPos {pos.x+dx,pos.y+dy};
    return newPos;
}

uint64_t sumBoxes(Map const& map)
{
    auto sum = 0ul;
    for(auto y = 0ul, h = map.size(); y < h; ++y)
    {
        for(auto x = 0ul, w = map[y].size(); x < w; ++x)
        {
            if(map[y][x]=='O' || map[y][x]=='[')
            {
                sum += y*100 + x;
            }
        }
    }
    return sum;
}

uint64_t firstPart(Map map, std::string const& moves)
{
    Position robotPos;
    for(auto y = 0ul, h = map.size(); y < h; ++y)
    {
        for(auto x = 0ul, w = map[y].size(); x < w; ++x)
        {
            if(map[y][x]=='@')
            {
                robotPos = {x,y};
            }
        }
    }
    for(auto const& direction:moves)
    {
        auto const newPos = moveRobot(map,robotPos,direction);
        if(newPos.has_value())
        {
            robotPos = newPos.value();
        }
    }
    return sumBoxes(map);
}

uint64_t secondPart(Map& map, std::string const& moves)
{
    for(auto& row:map)
    {
        row = std::regex_replace(row,std::regex{"[.]"},"..");
        row = std::regex_replace(row,std::regex{"#"},"##");
        row = std::regex_replace(row,std::regex{"O"},"[]");
        row = std::regex_replace(row,std::regex{"@"},"@.");
    }
    return firstPart(map,moves);
}

int main(int argc, char**argv)
{
    if(argc < 2)
    {
        std::cout << "Puzzle input not provided "<< std::endl;
        return -1;
    }
    Map map;
    std::string moves;
    if(!parseData(argv[1],map,moves))
    {
        return -1;
    }
    std::cout << "First part: " << firstPart(map,moves) << std::endl;
    std::cout << "Second part: " << secondPart(map,moves) << std::endl;
    return 0;
}