#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <sstream>
#include <unordered_map>

using DesignMap = std::unordered_map<std::string,uint64_t>;

bool parseData(std::string const& puzzleInput,std::vector<std::string>& patterns, std::vector<std::string>& designs)
{
    std::fstream puzzleData {puzzleInput,std::fstream::in};
    if(!puzzleData.is_open())
    {
        std::cout<<"Can't open the puzzle input "<<std::endl;
        return false;
    }
    std::stringstream ss;
    std::string row;
    std::getline(puzzleData,row);
    ss << row;
    std::string pattern;
    while(ss >> pattern)
    {
        if(!ss.eof())
        {
            pattern.pop_back();
        }
        patterns.push_back(pattern);
    }
    std::getline(puzzleData,row);
    while(std::getline(puzzleData,row))
    {
        designs.push_back(row);
    }
    return true;
}

uint64_t possibleCombinations(std::string const& design,std::vector<std::string> const& patterns, DesignMap& possibleDesigns)
{
    if(possibleDesigns.find(design) != possibleDesigns.end())
    {
        return possibleDesigns[design];
    }
    auto count = 0ul;
    for(auto const& pattern:patterns)
    {
        if(design.find(pattern) == 0)
        {
            auto const newString = design.substr(pattern.length());
            if(newString.empty())
            {
                ++count;
                continue;
            }
            count += possibleCombinations(newString,patterns,possibleDesigns);
        }
    }
    possibleDesigns[design] = count;
    return count;
}

uint64_t firstPart(std::vector<std::string> const& patterns, std::vector<std::string> const& designs, DesignMap& possibleDesigns)
{
    auto result = 0ul;
    for(auto const& design:designs)
    {
        if(possibleCombinations(design,patterns,possibleDesigns) > 0)
        {
            ++result;
        }
    }
    return result;
}

uint64_t secondPart(std::vector<std::string> const& patterns, std::vector<std::string> const& designs, DesignMap& possibleDesigns)
{
    auto result = 0ul;
    for(auto const& design:designs)
    {
        result += possibleCombinations(design,patterns,possibleDesigns);
    }
    return result;
}

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        std::cout << "Puzzle input not provided "<< std::endl;
        return -1;
    }
    std::vector<std::string> patterns,designs;
    DesignMap possibleDesigns;
    if(!parseData(argv[1],patterns,designs))
    {
        return -1;
    }
    std::cout << "First part: " << firstPart(patterns,designs,possibleDesigns) << std::endl;
    std::cout << "Second part: " << secondPart(patterns,designs,possibleDesigns) << std::endl;
    return 0;
}