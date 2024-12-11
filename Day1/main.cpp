#include <iostream>
#include <fstream>
#include <set>
#include <cstdint>
#include <algorithm>

using List = std::multiset<long>;

bool parseData(std::string const& puzzleInput,List& firstList,List& secondList)
{
	std::fstream puzzleData(puzzleInput,std::fstream::in);
	if(!puzzleData.is_open())
	{
		std::cout<<"Can't open the puzzle input" << std::endl;
		return false;
	}
	long firstNumber,secondNumber;
	while(puzzleData >> firstNumber >> secondNumber)
	{
		firstList.insert(firstNumber);
		secondList.insert(secondNumber);
	}
	return true;
}

uint64_t firstPart(List const& firstList,List const& secondList)
{
	auto result = 0ul;
	for(auto firstNumber=firstList.cbegin(),secondNumber=secondList.cbegin();firstNumber!=firstList.cend();++firstNumber,++secondNumber)
	{
		result += std::abs(*firstNumber-*secondNumber);
	}
	return result;
}

uint64_t secondPart(List const& firstList,List const& secondList)
{
	auto result = 0ul;
	for(auto currentNumber=firstList.cbegin();currentNumber!=firstList.cend();++currentNumber)
	{
		auto const occurences = std::count_if(secondList.cbegin(),secondList.cend(),[&currentNumber](long const& n){return n==*currentNumber;});
		result += occurences * *currentNumber;
	}
	return result;
}

int main(int argc,char** argv)
{
	if(argc<2)
	{
		std::cout<<"Puzzle input not provided"<<std::endl;
		return -1;
	}
	List firstList,secondList;
	if(!parseData(argv[1],firstList,secondList))
	{
		return -1;
	}
	std::cout<<"First part: "<< firstPart(firstList,secondList) << std::endl;
	std::cout<<"Second part: "<< secondPart(firstList,secondList) << std::endl;
	return 0;
}

