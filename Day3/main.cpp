#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdint>
#include <regex>
#include <string>

bool parseData(std::string const puzzleInput,std::string& memory)
{
	std::fstream const puzzleData(puzzleInput,std::fstream::in);
	if(!puzzleData.is_open())
	{
		std::cout<<"Can't open the puzzle input"<<std::endl;
		return false;
	}
	std::stringstream ss; 
	ss << puzzleData.rdbuf();
	memory = ss.str();	
	return true;			
}

uint64_t firstPart(std::string const& memory)
{
	uint64_t result = 0;
	std::regex const pattern {"mul\\((\\d{1,3}),\\s*(\\d{1,3})\\)"};
	auto const begin = std::sregex_iterator(memory.cbegin(), memory.cend(), pattern);
    auto const end = std::sregex_iterator();
    for (auto i = begin; i != end; ++i) {
		uint64_t firstNumber,secondNumber;
		uint64_t commaIndex = 0;
		std::string const mul = i->str();
		std::string strNum;
		for(auto i=4;i<7;i++)
		{
			if(mul[i] == ',')
			{
				break;
			}
			strNum += mul[i];
			commaIndex = i+1;
		}
		firstNumber = std::atoi(strNum.c_str());
		strNum = "";
		for(uint64_t i=commaIndex+1;i<commaIndex+4;i++)
		{
			if(mul[i] == ')')
			{
				break;
			}
			strNum += mul[i];
		}
		secondNumber = std::atoi(strNum.c_str());
		result += firstNumber * secondNumber;
    }
	return result;
}

uint64_t secondPart(std::string const& memory)
{
	uint64_t result = 0;
	struct Index
	{
		Index(int64_t const _value,bool const _enable) 
		: value(_value),enable(_enable)
		{};
		int64_t value;
		bool enable;

	};
	std::vector<Index> indices;
	std::regex pattern ("do\\(\\)");
	auto begin = std::sregex_iterator(memory.cbegin(), memory.cend(), pattern);
    auto const end = std::sregex_iterator();
	for (auto i = begin; i != end; ++i) 
	{
		indices.push_back({i->position(),true});
	}
	pattern = std::regex("don't\\(\\)");
	begin = std::sregex_iterator(memory.cbegin(), memory.cend(), pattern);
	for (auto i = begin; i != end; ++i) 
	{
		indices.push_back({i->position(),false});
	}
	std::sort(indices.begin(),indices.end(),[](Index const& i1,Index const& i2){return i1.value<i2.value;});
	auto currentIndex = std::find_if(indices.cbegin(),indices.cend(),[](Index const& i){return i.enable;});
	result += firstPart(memory.substr(0,currentIndex->value));
	while(currentIndex != indices.cend())
	{	
		auto nextDisableIndex = std::find_if(indices.cbegin(),indices.cend(),[&currentIndex](Index const& i){return (!i.enable)&&(i.value>currentIndex->value);});
		uint64_t size = 0;
		if(nextDisableIndex == indices.cend())
		{
			size = memory.size() - currentIndex->value;
		}
		else
		{
			size = nextDisableIndex->value - currentIndex->value;
		} 
		std::string const chunk = memory.substr(currentIndex->value,size);
		result += firstPart(chunk);
		currentIndex = std::find_if(indices.cbegin(),indices.cend(),[&nextDisableIndex](Index const& i){return (i.enable)&&(i.value>nextDisableIndex->value);}); 
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
	std::string memory;
	if(!parseData(argv[1],memory))
	{
		return -1;
	}
	std::cout<<"First part: "<< firstPart(memory) << std::endl;
	std::cout<<"Second part: "<< secondPart(memory) << std::endl;
	return 0;
}

