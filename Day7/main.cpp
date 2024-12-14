#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <sstream>


using Equation = std::pair<uint64_t,std::vector<uint64_t>>;

bool parseData(std::string const& puzzleInput, std::vector<Equation>& equations)
{
    std::fstream puzzleData(puzzleInput,std::fstream::in);
    if(!puzzleData.is_open())
    {
        std::cout<<"Can't open the puzzle input"<<std::endl;
        return false;
    }
    std::string row;
    std::stringstream ss;
    while(std::getline(puzzleData,row))
    {
        Equation equation;
        ss.clear();
        ss.str(row);
        std::string result;
        ss >> result;
        equation.first = std::stoul(result);
        auto number = 0ul;
        while(ss >> number)
        {
            equation.second.push_back(number);
        }
        equations.push_back(equation);
    }
    return true;
}

uint64_t concatNumbers(uint64_t const firstNumber, uint64_t const secondNumber)
{
    return std::stoul(std::to_string(firstNumber) + std::to_string(secondNumber));
}

bool checkEquation(Equation const& equation,uint64_t const result,uint64_t const index,bool const concat)
{
    if(index == equation.second.size())
    {
        return equation.first == result;
    }
    if(checkEquation(equation,result + equation.second[index],index+1,concat))
    {
        return true;
    }
    if(checkEquation(equation,result * equation.second[index],index+1,concat))
    {
        return true;
    }
    if(concat)
    {
        if(checkEquation(equation,concatNumbers(result,equation.second[index]),index+1,concat))
        {
            return true;
        }
    }
    return false;
}


uint64_t firstPart(std::vector<Equation> const& equations)
{
    auto result = 0ul;
    for(auto const& equation:equations)
    {
        if(checkEquation(equation,equation.second[0],1,false))
        {   
            result += equation.first;
        }
    }
    return result;
}

uint64_t secondPart(std::vector<Equation> const& equations)
{
    auto result = 0ul;  
    for(auto const& equation:equations)
    {
        if(checkEquation(equation,equation.second[0],1,true))
        {   
            result += equation.first;
        }
    }
    return result;
}

int main(int argc,char**argv)
{
    if(argc<2)
    {
        std::cout << "Puzzle input not provided "<< std::endl;
        return -1;
    }
    std::vector<Equation> equations;
    if(!parseData(argv[1],equations))
    {
        return -1;
    }
    std::cout << "First part: " << firstPart(equations) << std::endl;
    std::cout << "Second part: " << secondPart(equations) << std::endl;
    return 0;
}