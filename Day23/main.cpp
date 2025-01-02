#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <set>
#include <algorithm>

using ConnectionSet = std::set<std::string>;
using ConnectionMap = std::unordered_map<std::string,ConnectionSet>;

bool parseData(std::string const& puzzleInput, ConnectionMap& connectionMap)
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
        auto const PC1 = row.substr(0,2);
        auto const PC2 = row.substr(3,2);
        connectionMap[PC1].insert(PC2);
        connectionMap[PC2].insert(PC1);
    }
    return true;
}

uint64_t firstPart(ConnectionMap const& connectionMap)
{
    std::set<ConnectionSet> threePCs;
    for(auto const& [PC1,connectionsPC1] : connectionMap)
    {
        for(auto const& PC2 : connectionsPC1)
        {
            for(auto const& PC3 : connectionMap.at(PC2))
            {
                if(connectionsPC1.count(PC3) != 0)
                {
                    ConnectionSet connection{PC1,PC2,PC3};
                    if(std::any_of(connection.cbegin(),connection.cend(),[](auto const& pc){return pc[0]=='t';}))
                    {
                        threePCs.insert(connection);
                    }
                }
            }
        }
    }
    return threePCs.size();
}

std::string secondPart(ConnectionMap const& connectionMap)
{
    std::string password = "";
    ConnectionSet largestSet;
    auto checkPC = [](ConnectionMap const& connectionMap,ConnectionSet const& connection,std::string const& targetPC)
    {
        for(auto const& PC : connection)
        {
            if(connectionMap.at(PC).count(targetPC) == 0)
            {
                return false;
            }
        }
        return true;
    };
    for(auto const& [PC1,connectionsPC1] : connectionMap)
    {
        ConnectionSet currentSet;
        currentSet.insert(PC1);
        for(auto const& PC2 : connectionsPC1)
        {
            bool valid = checkPC(connectionMap,currentSet,PC2);
            for(auto const& PC_N : currentSet)
            {
                if(checkPC(connectionMap,connectionMap.at(PC2),PC_N))
                {
                    valid = false;
                    break;
                }
            }
            if(valid)
            {
                currentSet.insert(PC2);
            }
        }
        if(currentSet.size() > largestSet.size())
        {
            largestSet = currentSet;
        }
    }
    for(auto const& PC : largestSet)
    {
        password += PC + ",";
    }
    password.pop_back();
    return password;
}

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        std::cout << "Puzzle input not provided "<< std::endl;
        return -1;
    }
    ConnectionMap connectionMap;
    if(!parseData(argv[1],connectionMap))
    {
        return -1;
    }
    std::cout << "First part: " << firstPart(connectionMap) << std::endl;
    std::cout << "Second part: " << secondPart(connectionMap) << std::endl;
    return 0;
}