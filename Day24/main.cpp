#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <set>
#include <map>
#include <unordered_map>

struct Gate
{
    std::string a,b,op;
};

using WireMap = std::unordered_map<std::string,uint8_t>;
using GateMap = std::map<std::string,Gate>;

bool parseData(std::string const& puzzleInput,WireMap& wireMap,GateMap& gateMap)
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
        if(row.length()==0)
        {
            break;
        }
        wireMap[row.substr(0,3)] = std::stoul(row.substr(5));
    }
    while(std::getline(puzzleData,row))
    {
        Gate gate;
        auto const firstSpace = row.find(' ');
        auto const secondSpace = row.find(' ',firstSpace + 1);
        gate.a = row.substr(0,3);
        gate.op = row.substr(firstSpace + 1,secondSpace - firstSpace - 1);
        gate.b = row.substr(secondSpace + 1,3);
        auto const output = row.substr(row.length() - 3,3);
        gateMap[output] = gate;
    }
    return true;
}

uint8_t getWireValue(WireMap& wireMap,GateMap const& gatemap,std::string const& wire)
{
    if(wireMap.count(wire) == 0)
    {
        auto const gate = gatemap.at(wire);
        auto const a = getWireValue(wireMap,gatemap,gate.a);
        auto const b = getWireValue(wireMap,gatemap,gate.b);
        auto value = 0ul;
        if(gate.op == "AND")
        {
            value = a & b;
        }
        else if(gate.op == "OR")
        {
            value = a | b;
        }
        else
        {
            value = a ^ b;
        }
        wireMap[wire] = value;
    }
    return wireMap[wire];
}

uint64_t firstPart(WireMap& wireMap,GateMap const& gateMap)
{
    std::string result = "";
    for(auto const& [output,gate]:gateMap)
    {
        if(output[0]=='z')
        {
            auto const wireValue = getWireValue(wireMap,gateMap,output);
            result = std::to_string(wireValue) + result;
        }
    }
    return std::stoul(result,0,2);
}

std::string secondPart(GateMap const& gateMap)
{
    std::string result = "";
    std::set<std::string> faultyOutputs;
    for(auto const& [output,gate]:gateMap)
    {
        bool const xy = (gate.a[0] == 'x' && gate.b[0] == 'y') || (gate.a[0] == 'y' && gate.b[0] == 'x');
        bool const x00y00 = (gate.a == "x00" && gate.b == "y00") || (gate.a == "y00" && gate.b != "x00");
        if(gate.op != "XOR"  && output[0]=='z' && output != "z45")
        {
            faultyOutputs.insert(output);
        }
        if(gate.op == "XOR" && output[0]!='z' && !xy)
        {
            faultyOutputs.insert(output);
        }
        if(gate.op == "XOR" && xy)
        {
            bool faulty = true;
            for(auto const& [out,gate2]:gateMap)
            {
                if(gate2.op == "XOR" && (gate2.a == output || gate2.b == output))
                {
                    faulty = false;
                }
            }
            if(faulty && !x00y00)
            {
                faultyOutputs.insert(output);
            }
        }
        if(gate.op == "AND")
        {
            bool faulty = true;
            for(auto const& [out,gate2]:gateMap)
            {
                if(gate2.op == "OR" && (gate2.a == output || gate2.b == output))
                {
                    faulty = false;
                }
            }
            if(faulty && !x00y00)
            {
                faultyOutputs.insert(output);
            }
        }
    }
    for(auto const& output:faultyOutputs)
    {
        result += output + ',';
    }
    result.pop_back();
    return result;
}

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        std::cout << "Puzzle input not provided "<< std::endl;
        return -1;
    }
    WireMap wireMap;
    GateMap gateMap;
    if(!parseData(argv[1],wireMap,gateMap))
    {
        return -1;
    }
    std::cout << "First part: " << firstPart(wireMap,gateMap) << std::endl;
    std::cout << "Second part: " << secondPart(gateMap) << std::endl;
    return 0;
}