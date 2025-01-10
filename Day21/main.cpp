#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <map>
#include <queue>

using KeypadMap = std::map<std::pair<char,char>,std::vector<std::string>>;

bool parseData(std::string const& puzzleInput, std::vector<std::string>& codes)
{
    std::fstream puzzleData {puzzleInput,std::fstream::in};
    if(!puzzleData.is_open())
    {
        std::cout<<"Can't open the puzzle input "<<std::endl;
        return false;
    }
    std::string code;
    while(std::getline(puzzleData,code))
    {
        codes.push_back(code);
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

    bool inBounds(uint64_t const width, uint64_t const height) const
    {
        return y < height && x < width;
    }

    uint64_t x,y;
};

struct Path
{
    Path()
    : pos{0,0}
    , distance(0)
    , pathID(0)
    {
    }

    Path(Position const _pos, uint64_t const _distance, uint64_t const _pathID)
    : pos(_pos)
    , distance(_distance)
    , pathID(_pathID)
    {
    }

    bool operator<(Path const& path) const
    {
        return path.distance < distance;
    }

    Position pos;
    uint64_t distance,pathID;
};

void fillMap(KeypadMap& map, char const* keypad, uint64_t const width, uint64_t const height)
{
    for(auto y1 = 0ul; y1 < height; ++y1)
    {
        for(auto x1 = 0ul; x1 < width; ++x1)
        {
            for(auto y2 = 0ul; y2 < height; ++y2)
            {
                for(auto x2 = 0ul; x2 < width; ++x2)
                {
                    if(keypad[y1 * width + x1] == ' ' || keypad[y2 * width + x2] == ' ')
                    {
                        continue;
                    }
                    std::priority_queue<Path> pq;
                    std::unordered_map<uint64_t,std::string> paths;
                    std::vector<std::string> shortestPaths;
                    Position start{x1,y1};
                    Position end{x2,y2};
                    pq.push({start,0,0});
                    paths[0] = "";
                    auto shortestDistance = UINT64_MAX;
                    while(!pq.empty())
                    {
                        auto const [position,distance,pathID] = pq.top();
                        pq.pop();
                        if(position == end)
                        {
                            if(shortestDistance == UINT64_MAX)
                            {
                                shortestDistance = distance;
                            }
                            if(distance == shortestDistance)
                            {
                                shortestPaths.push_back(paths[pathID]);
                            }
                            continue;
                        }
                        Position const newPositions[] =
                        {
                            {position.x-1,position.y},
                            {position.x+1,position.y},
                            {position.x,position.y-1},
                            {position.x,position.y+1}
                        };
                        for(auto i = 0; i < 4; ++i)
                        {
                            if(newPositions[i].inBounds(width,height) && distance + 1 <= shortestDistance && keypad[newPositions[i].y * width + newPositions[i].x] != ' ')
                            {
                                pq.push({newPositions[i],distance + 1,paths.size()});
                                paths[paths.size()] = paths[pathID] + "<>^v"[i];
                            }
                        }
                    }
                    map[{keypad[y1 * width + x1],keypad[y2 * width + x2]}] = shortestPaths;
                }
            }
        }
    }
}

KeypadMap getKeypadMap()
{
    KeypadMap map;
    char constexpr numpad[][3] =
    {
        {'7','8','9'},
        {'4','5','6'},
        {'1','2','3'},
        {' ','0','A'}
    };
    char constexpr dirpad[][3] =
    {
        {' ','^','A'},
        {'<','v','>'}
    };
    fillMap(map,*numpad,3,4);
    fillMap(map,*dirpad,3,2);
    return map;
}

struct SequenceInfo
{
    std::string const path;
    char const previousKey;
};

std::vector<std::string> generateSequences(std::string const& code, KeypadMap const& map)
{
    std::vector<std::string> sequences;
    std::queue<std::pair<SequenceInfo,uint64_t>> queue;
    queue.push({{"",'A'},0});
    while (!queue.empty())
    {
        auto const [sequenceInfo,index] = queue.front();
        auto const [currentPath,previousKey] = sequenceInfo;
        queue.pop();
        if(index == code.length())
        {
            sequences.push_back(currentPath);
            continue;
        }
        for(auto const& path : map.at({previousKey,code[index]}))
        {
            queue.push({{currentPath + path + 'A',code[index]},index + 1});
        }
    }
    return sequences;
}

uint64_t getSequenceLength(KeypadMap const& map, std::string const& sequence, uint64_t const depth, std::vector<std::unordered_map<std::string,uint64_t>>& sequenceLengths)
{
    auto length = 0ul;
    if(depth == 0)
    {
        return sequence.length();
    }
    if(sequenceLengths[depth].count(sequence) != 0)
    {
        return sequenceLengths[depth][sequence];
    }
    std::stringstream ss(sequence);
    std::vector<std::string> subSequences;
    std::string subSequence;
    while(std::getline(ss,subSequence,'A'))
    {
        subSequences.push_back(subSequence + 'A');
    }
    for(auto const& subSequence : subSequences)
    {
        std::vector<std::string> newSubSequences = generateSequences(subSequence,map);
        auto minimumLength = UINT64_MAX;
        for(auto const& newSequence : newSubSequences)
        {
            auto const sequenceLength = getSequenceLength(map,newSequence,depth - 1,sequenceLengths);
            minimumLength = std::min(sequenceLength,minimumLength);
        }
        length += minimumLength;
    }
    sequenceLengths[depth][sequence] = length;
    return length;
}

uint64_t sumComplexities(std::vector<std::string> const& codes, KeypadMap const& map, uint64_t const depth)
{
    auto result = 0ul;
    std::vector<std::unordered_map<std::string,uint64_t>> sequenceLengths;
    for(auto i=0ul; i <= depth; ++i)
    {
        sequenceLengths.push_back({});
    }
    for(auto const& code : codes)
    {
        std::vector<std::string> numpadSequences = generateSequences(code,map);
        auto inputLength = 0ul;
        auto minimumLength = UINT64_MAX;
        for(auto const& sequence : numpadSequences)
        {
            auto const sequenceLength = getSequenceLength(map,sequence,depth,sequenceLengths);
            minimumLength = std::min(sequenceLength,minimumLength);
        }
        inputLength += minimumLength;
        result += std::stoul(code.substr(0,3)) * inputLength;
    }
    return result;
}

uint64_t firstPart(std::vector<std::string> codes, KeypadMap const& map)
{
    return sumComplexities(codes,map,2);
}

uint64_t secondPart(std::vector<std::string> codes, KeypadMap const& map)
{
    return sumComplexities(codes,map,25);
}

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        std::cout << "Puzzle input not provided "<< std::endl;
        return -1;
    }
    std::vector<std::string> codes;
    if(!parseData(argv[1],codes))
    {
        return -1;
    }
    auto const map = getKeypadMap();
    std::cout << "First part: " << firstPart(codes,map) << std::endl;
    std::cout << "Second part: " << secondPart(codes,map) << std::endl;
    return 0;
}