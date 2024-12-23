#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

constexpr long width = 101;
constexpr long height = 103;

struct Robot
{
    Robot(long const  _x,long const  _y,long const  _vx,long const  _vy)
    : x(_x)
    , y(_y)
    , vx(_vx)
    , vy(_vy)
    {
    }
    void move()
    {
        x = (x + vx) % width;
        y = (y + vy) % height;
        if(x < 0)
        {
            x = width + x;
        }
        if(y < 0)
        {
            y = height + y;
        }
    }
    long x,y,vx,vy;
};

bool parseData(std::string const& puzzleInput,std::vector<Robot>& robots)
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
        auto const px = std::stol(row.substr(2,3));
        auto const py = std::stol(row.substr(row.find_first_of(',') + 1,3));
        auto const vx = std::stol(row.substr(row.find("v=") + 2,3));
        auto const vy = std::stol(row.substr(row.find_last_of(',') + 1,3));
        robots.emplace_back(px,py,vx,vy);
    }
    return true;
}

struct Map
{
    Map(std::vector<Robot> const& robots)
    {
        update(robots);
    }

    void update(std::vector<Robot> const& robots)
    {
        for(auto& row:value)
        {
            for(auto& tile:row)
            {
                tile = 0;
            }
        }

        for(auto& robot:robots)
        {
            value[robot.y][robot.x] += 1;
        }
    }

    void print()
    {
        std::cout<<std::endl;
        for(auto i = 0ul; i < height; ++i)
        {
            for(auto j = 0ul; j<width; ++j)
            {
                if(j == width / 2)
                {
                    std::cout<<'|';
                }
                if(i == height / 2)
                {
                    std::cout<<'-';
                }
                else if(value[i][j] == 0)
                {
                std::cout<<'.';
                }
                else
                {
                    std::cout<<value[i][j];
                }
            }
            std::cout<<std::endl;
        }
        std::cout<<std::endl;
    }

    uint64_t quadrantCount(uint64_t const index)
    {
        auto count = 0ul;
        uint64_t startX,startY,endX,endY;
        switch(index)
        {
            case 0:
                startX = 0;
                startY = 0;
                endX   = width / 2;
                endY   = height / 2;
                break;
            case 1:
                startX = width / 2 + 1;
                startY = 0;
                endX   = width;
                endY   = height / 2;
                break;
            case 2:
                startX = 0;
                startY = height / 2 + 1;
                endX   = width / 2;
                endY   = height;
                break;
            case 3:
                startX = width / 2 + 1;
                startY = height / 2 + 1;
                endX   = width;
                endY   = height;
                break;
            default:
                return 0;
        }
        for(auto y = startY; y<endY; ++y)
        {
            for(auto x = startX; x < endX; ++x)
            {
                if(value[y][x])
                {
                    count += value[y][x];
                }
            }
        }
        return count;
    }

    uint64_t safetyScore()
    {
        auto score = 1ul;
        for(auto i = 0; i < 4; ++i)
        {
            auto count = quadrantCount(i);
            score *= count;
        }
        return score;
    }

    uint64_t value[height][width];
};

uint64_t firstPart(std::vector<Robot> robots)
{
    Map map(robots);
    for(auto i = 0; i < 100; ++i)
    {
        for(auto& robot:robots)
        {
            robot.move();
        }
        map.update(robots);
    }
    return map.safetyScore();
}

uint64_t secondPart(std::vector<Robot> robots)
{
    Map map(robots);
    auto lowestScore = map.safetyScore();
    auto secondsElapsed = 0ul;
    for(auto i = 0ul; i < 50000; ++i)
    {
        for(auto& robot:robots)
        {
            robot.move();
        }
        map.update(robots);
        auto const score = map.safetyScore();
        if(score < lowestScore)
        {
            lowestScore = score;
            secondsElapsed = i + 1;
            //map.print();
        }
    }
    return secondsElapsed;
}

int main(int argc,char**argv)
{
    if(argc < 2)
    {
        std::cout << "Puzzle input not provided "<< std::endl;
        return -1;
    }
    std::vector<Robot> robots;
    if(!parseData(argv[1],robots))
    {
        return -1;
    }
    std::cout << "First part: " << firstPart(robots) << std::endl;
    std::cout << "Second part: " << secondPart(robots) << std::endl;
    return 0;
}