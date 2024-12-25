#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <queue>
#include <unordered_set>
#include <unordered_map>

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

    Position(long const  _x,long const  _y) : x(_x), y(_y)
    {
    }

    bool operator==(Position const& pos) const
    {
        return pos.x == x && pos.y == y;
    }

    uint64_t operator()(Position const& pos) const
    {
        return pos.x + pos.y * UINT32_MAX;
    }

    long x,y;
};

struct Step
{
    Step()
    : pos{0,0}
    , dir(0)
    , score(0)
    , pathID(0)
    {
    }

    Step(Position const _pos, uint64_t const _dir, uint64_t const _score, uint64_t const _pathID)
    : pos(_pos)
    , dir(_dir)
    , score(_score)
    , pathID(_pathID)
    {
    }

    bool operator<(Step const& step) const
    {
        return step.score < score;
    }

    Position pos;
    uint64_t dir,score,pathID;
};

std::pair<uint64_t,uint64_t> traverseMap(Map const& map, bool const countTiles)
{
    auto lowestScore = UINT64_MAX;
    Position startPos;
    Position endPos;
    long const dx[] = {1, 0, -1, 0};
    long const dy[] = {0, 1, 0, -1};
    std::priority_queue<Step> pq;
    std::unordered_set<Position,Position> bestTiles;
    std::unordered_map<uint64_t,std::vector<Position>> paths;
    std::unordered_map<Position,std::unordered_map<uint64_t,uint64_t>,Position> scores;
    for(auto y = 0ll, h = static_cast<long long>(map.size()); y < h; ++y)
    {
        for(auto x = 0ll, w = static_cast<long long>(map[y].size()); x < w; ++x)
        {
            if(map[y][x] == 'S')
            {
                startPos = {x,y};
            }
            else if(map[y][x] == 'E')
            {
                endPos = {x,y};
            }
            for(auto dir = 0ul; dir < 4; ++dir)
            {
                scores[{x,y}][dir] = UINT64_MAX;
            }
        }
    }
    pq.push({startPos,0,0,0});
    while(!pq.empty())
    {
        auto const step = pq.top();
        pq.pop();
        paths[step.pathID].push_back(step.pos);
        if(step.pos == endPos)
        {
            if(lowestScore == UINT64_MAX)
            {
                lowestScore = step.score;
            }
            if(step.score == lowestScore && countTiles)
            {
                bestTiles.insert(paths[step.pathID].begin(),paths[step.pathID].end());
            }
            continue;
        }
        Position const forwardPos {step.pos.x + dx[step.dir], step.pos.y + dy[step.dir]};
        if(map[forwardPos.y][forwardPos.x] != '#' && step.score + 1 <= scores[forwardPos][step.dir])
        {
            pq.push({forwardPos,step.dir, step.score + 1,step.pathID});
            scores[forwardPos][step.dir] = step.score + 1;
        }
        for (auto i = -1l; i <= 1; i += 2)
        {
            auto const newDir = (step.dir + i + 4) % 4;
            Position const sidewaysPos {step.pos.x + dx[newDir], step.pos.y + dy[newDir]};
            if(map[sidewaysPos.y][sidewaysPos.x] != '#' && step.score + 1001 <= scores[sidewaysPos][newDir])
            {
                pq.push({sidewaysPos,newDir,step.score + 1001,paths.size()});
                scores[sidewaysPos][newDir] = step.score + 1001;
                if(countTiles)
                {
                    paths[paths.size()] = paths[step.pathID];
                }
            }
        }
    }
    return {lowestScore,bestTiles.size()};
}

uint64_t firstPart(Map const& map)
{
    return traverseMap(map,false).first;
}

uint64_t secondPart(Map const& map)
{
    return traverseMap(map,true).second;
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
