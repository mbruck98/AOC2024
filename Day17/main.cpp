#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cmath>

struct Computer
{
    Computer() : A(0), B(0), C(0), instructionPointer(0),output("")
    {}

    uint8_t getComboOperand(uint8_t const literalOperand)
    {
        if(literalOperand < 4)
        {
            return literalOperand;
        }
        switch(literalOperand)
        {
            case 4:
                return A;
            case 5:
                return B;
            case 6:
                return C;
        }
        return UINT8_MAX;
    }

    std::string const& run()
    {
        while(instructionPointer <= program.size())
        {
            runInstruction();
        }
        output.pop_back();
        return output;
    }

    void runInstruction()
    {
        auto const literalOperand = program[instructionPointer + 1];
        auto const comboOperand = getComboOperand(literalOperand);
        switch(program[instructionPointer])
        {
            case 0:
                A /= pow(2,comboOperand);
                break;
            case 1:
                B ^= literalOperand;
                break;
            case 2:
                B = comboOperand % 8;
                break;
            case 3:
                if(A != 0)
                {
                    instructionPointer = literalOperand;
                    return;
                }
                break;
            case 4:
                B ^= C;
                break;
            case 5:
                output += std::to_string(comboOperand % 8);
                output += ',';
                break;
            case 6:
                B = A / pow(2,comboOperand);
                break;
            case 7:
                C = A / pow(2,comboOperand);
                break;
            default:
                instructionPointer = UINT64_MAX;
                return;
        }
        instructionPointer += 2;
    }

    void reset()
    {
        output = "";
        instructionPointer = 0;
        A = 0;
        B = 0;
        C = 0;
    }

    std::vector<uint8_t> program;
    uint64_t A,B,C,instructionPointer;
    std::string output;
};

bool parseData(std::string const& puzzleInput,Computer& computer)
{
    std::fstream puzzleData {puzzleInput,std::fstream::in};
    if(!puzzleData.is_open())
    {
        std::cout<<"Can't open the puzzle input "<<std::endl;
        return false;
    }
    std::string row;
    std::getline(puzzleData,row);
    computer.A = std::stoul(row.substr(row.find(": ")+2));
    std::getline(puzzleData,row);
    computer.B = std::stoul(row.substr(row.find(": ")+2));
    std::getline(puzzleData,row);
    computer.C = std::stoul(row.substr(row.find(": ")+2));
    std::getline(puzzleData,row);
    std::getline(puzzleData,row);
    for(auto i=9ul; i < row.length(); i += 2)
    {
        computer.program.push_back(row[i]-'0');
    }
    return true;
}

std::string firstPart(Computer& computer)
{
    return computer.run();
}

uint64_t secondPart(Computer& computer)
{
    auto lowestA = 0ul;
    std::string expectedOutput;
    for(auto const& value:computer.program)
    {
        expectedOutput += std::to_string(value);
        expectedOutput += ",";
    }
    expectedOutput.pop_back();
    auto i = 1ul;
    auto state = 0ul;
    while(i < computer.program.size())
    {
        lowestA <<= 3;
        auto inc = 0ul;
        auto valid = false;
        for(auto j=state; j < 8; ++j)
        {
            computer.reset();
            computer.A = lowestA + j;
            auto const& output = computer.run();
            if(std::string(expectedOutput.end() - i*2-1,expectedOutput.end()) == std::string(output.end() - i*2-1,output.end()))
            {
                valid = true;
                inc = j;
                break;
            }
        }
        if(valid)
        {
            ++i;
            lowestA += inc;
            state = 0;
            continue;
        }
        --i;
        lowestA >>= 3;
        state = lowestA % 8 + 1;
        lowestA >>= 3;
    }
    return lowestA;
}

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        std::cout << "Puzzle input not provided "<< std::endl;
        return -1;
    }
    Computer computer;
    if(!parseData(argv[1],computer))
    {
        return -1;
    }
    std::cout << "First part: " << firstPart(computer) << std::endl;
    std::cout << "Second part: " << secondPart(computer) << std::endl;
    return 0;
}