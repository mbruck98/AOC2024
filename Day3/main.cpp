#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdint>
#include <regex>


bool parseData(std::string const& puzzleInput,std::string& memory)
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
	auto result = 0ul;
	std::regex const pattern {"mul\\((\\d{1,3}),\\s*(\\d{1,3})\\)"};
	auto const begin = std::sregex_iterator(memory.cbegin(), memory.cend(), pattern);
	auto const end = std::sregex_iterator();
	for (auto i = begin; i != end; ++i) {
		auto commaIndex = 0ul;
		auto const mul = i->str();
		std::string strNum;
		for(auto j=4;j<7;++j)
		{
			if(mul[j] == ',')
			{
				break;
			}
			strNum += mul[j];
			commaIndex = j+1;
		}
		auto const firstNumber = std::atoi(strNum.c_str());
		strNum = "";
		for(auto j=commaIndex+1;j<commaIndex+4;++j)
		{
			if(mul[j] == ')')
			{
				break;
			}
			strNum += mul[j];
		}
		auto const secondNumber = std::atoi(strNum.c_str());
		result += firstNumber * secondNumber;
    }
	return result;
}

uint64_t secondPart(std::string const& memory)
{
	auto result = 0ul;
	struct Index
	{
		Index(long const _value,bool const _enable) 
		: value(_value),enable(_enable)
		{};
		long value;
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
	auto nextDisableIndex = std::find_if(indices.cbegin(),indices.cend(),[](Index const& i){return !i.enable;});
	auto currentIndex = std::find_if(indices.cbegin(),indices.cend(),[&nextDisableIndex](Index const& i){return (i.enable)&&(i.value>nextDisableIndex->value);});
	result += firstPart(memory.substr(0,nextDisableIndex->value));
	while(currentIndex != indices.cend())
	{	
		nextDisableIndex = std::find_if(indices.cbegin(),indices.cend(),[&currentIndex](Index const& i){return (!i.enable)&&(i.value>currentIndex->value);});
		auto size = 0ul;
		if(nextDisableIndex == indices.cend())
		{
			size = memory.size() - currentIndex->value;
		}
		else
		{
			size = nextDisableIndex->value - currentIndex->value;
		} 
		auto const chunk = memory.substr(currentIndex->value,size);
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

