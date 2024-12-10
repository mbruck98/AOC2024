#include <iostream>
#include <fstream>
#include <cstdint>
#include <algorithm>
#include <vector>
#include <set>
#include <unordered_map>
#include <optional>

using PageOrderRules = std::unordered_map<uint64_t,std::pair<std::set<uint64_t>,std::set<uint64_t>>>;
using Update = std::vector<uint64_t>;

bool parseData(std::string const puzzleInput,PageOrderRules& rules,std::vector<Update>& updates)
{
	std::fstream puzzleData(puzzleInput,std::fstream::in);
	if(!puzzleData.is_open())
	{
		std::cout<<"Can't open input.txt";
		return false;
	}
	std::string row;
	while(std::getline(puzzleData,row))
	{
		if(row.length() == 5)
		{
			uint64_t firstNumber,secondNumber;
			firstNumber = std::atoi(row.substr(0,2).c_str());
			secondNumber = std::atoi(row.substr(3).c_str());
			rules[firstNumber].first.insert(secondNumber);
			rules[secondNumber].second.insert(firstNumber);
		}
		else if(row.length() == 0)
		{
			continue;
		}
		else
		{	
			std::vector<uint64_t> update;
			for(auto i=0; i<=row.length();i += 3)
			{
				update.emplace_back(std::atoi(row.substr(i,2).c_str()));
			}
			updates.push_back(update);
		}		
	}
	return true;
}

std::optional<std::pair<uint64_t,uint64_t>> checkUpdate(PageOrderRules const& rules,Update const& update)
{
	for(auto i=0;i<update.size();++i)
	{
		if(rules.find(update[i]) == rules.cend())
		{
			continue;
		}
		for(auto j=0;j<i;++j)
		{
			if(rules.at(update[j]).second.find(update[i]) != rules.at(update[j]).second.cend())
			{
				return std::pair{i,j};
			} 
		}	
		for(auto k=i+1;k<update.size();++k)
		{
			if(rules.at(update[k]).first.find(update[i]) != rules.at(update[k]).first.cend())
			{
				return std::pair{i,k};
			}
		}
	}
	return std::nullopt;
}

uint64_t firstPart(PageOrderRules const& rules,std::vector<Update> const& updates)
{
	uint64_t result = 0;
	for(auto const& update:updates)
	{
		if(!checkUpdate(rules,update).has_value())
		{
			result += update[update.size()/2];
		}
	}
	return result;
}

uint64_t secondPart(PageOrderRules const& rules,std::vector<Update>& updates)
{
	uint64_t result = 0;
	for(auto& update:updates)
	{
		auto check = checkUpdate(rules,update);
		if(check.has_value())
		{
			while(check.has_value())
			{
				std::swap(update[check.value().first],update[check.value().second]);
				check = checkUpdate(rules,update);
			}
			result += update[update.size()/2];
		}
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
	std::vector<Update> updates;
	PageOrderRules rules;
	if(!parseData(argv[1],rules,updates))
	{
		return -1;
	}
	std::cout<<"First part: "<< firstPart(rules,updates) << std::endl;
	std::cout<<"Second part: "<< secondPart(rules,updates) << std::endl;
	return 0;
}

