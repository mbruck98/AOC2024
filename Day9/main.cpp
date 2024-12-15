#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <algorithm>

bool parseData(std::string const& puzzleInput,std::string& diskMap)
{
    std::fstream puzzleData(puzzleInput,std::fstream::in);
    if(!puzzleData.is_open())
    {
        std::cout<<"Can't open the puzzle input "<<std::endl;
        return false;
    }
    puzzleData >> diskMap;
    return true;
}

uint64_t calculateChecksum(std::wstring const& diskMap)
{
    auto checksum = 0ul;
    for(auto j = 0ul; j < diskMap.size(); ++j)
    {
        if(diskMap[j] != '.')
        {
            checksum += (diskMap[j] - '0') * j;
        }
    }
    return checksum;
}

std::wstring::iterator findEmptyChunk(std::wstring& diskMap,uint64_t const size)
{
    for(auto [start,count] = std::tuple{diskMap.begin(),0ul}; start != diskMap.end(); ++start)
    {
        if(*start=='.')
        {
            ++count;
        }
        else
        {
            count = 0;
        }
        if(count == size)
        {
            return start - count + 1;
        }
    }
    return diskMap.end();
}

std::wstring transformDiskMap(std::string const& diskMap) 
{
    std::wstring transformedMap;
    wchar_t fileID = '0';
    for(auto i = 0ul; i < diskMap.size(); i += 2)
    {
        for(auto j = 0l; j < diskMap[i] - '0'; ++j)
        {
            transformedMap += fileID;
        }
        for(auto j = 0l; j < diskMap[i+1] - '0'; ++j)
        {
            transformedMap += '.';
        }
        ++fileID;
    }
    return transformedMap;
}

uint64_t firstPart(std::string const& diskMap)
{
    auto result = 0ul;  
    auto map = transformDiskMap(diskMap);
    for(auto i = map.size()-1; i != 0; --i)
    {
        auto const firstFreeSpace = map.find_first_of('.');
        if(firstFreeSpace != std::string::npos && map[i] != '.' && i > firstFreeSpace)
        {
            std::swap(map[i],map[firstFreeSpace]);
        }
    }
    result += calculateChecksum(map);
    return result;
}

uint64_t secondPart(std::string const& diskMap)
{
    auto result = 0ul;  
    auto map = transformDiskMap(diskMap);
    auto fileID = map[map.find_last_not_of('.')];
    while(fileID >= '0')
    {
        auto const fileIndex = map.find_last_of(fileID);
        auto count = 0ul;
        while(map[fileIndex-count]==fileID)
        {
            ++count;
        }
        auto freeChunk = findEmptyChunk(map,count);
        if(freeChunk != map.end() && freeChunk < map.begin() + fileIndex)
        {   
            for(auto [i,oldChunk] = std::tuple{0ul,map.begin() + fileIndex}; i < count; ++freeChunk, --oldChunk, ++i)
            {
                std::swap(*freeChunk, *oldChunk);
            }
        }
        --fileID;
    }
    result += calculateChecksum(map);
    return result;
}

int main(int argc,char**argv)
{
    if(argc < 2)
    {
        std::cout << "Puzzle input not provided "<< std::endl;
        return -1;
    }
    std::string diskMap;
    if(!parseData(argv[1],diskMap))
    {
        return -1;
    }
    std::cout << "First part: " << firstPart(diskMap) << std::endl;
    std::cout << "Second part: " << secondPart(diskMap) << std::endl;
    return 0;
}