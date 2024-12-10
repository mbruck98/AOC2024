#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <sstream>

using Report = std::vector<uint32_t>;

bool parseData(std::string const puzzleInput,std::vector<Report>& reports)
{
	std::fstream puzzleData(puzzleInput,std::fstream::in);
	if(!puzzleData.is_open())
	{
		std::cout<<"Can't open the puzzle input"<<std::endl;
		return false;
	}
	std::string line;
	std::stringstream ss;
	while(std::getline(puzzleData,line))
	{
		Report report;
		ss << line;
		uint32_t level;
		while(ss >> level)
		{
			report.push_back(level);
		}
		reports.push_back(report);
		ss.clear();
	}
	return true;
}

bool checkReport(Report const& report)
{
	long previousLevel = -1;
	bool previouslyIncreasing = false;
	for(auto level=0;level<report.size();++level)
	{
		if(previousLevel == -1) 
		{
			previousLevel = report[level];
			continue;
		}
		bool const currentlyIncreasing = previousLevel > report[level];
		uint64_t const diff = std::abs(previousLevel - report[level]);
		if (diff > 3 || diff == 0 || ((currentlyIncreasing != previouslyIncreasing) && level>1))
		{
			return false;
		}
		previousLevel = report[level];
		previouslyIncreasing = currentlyIncreasing;
	}
	return true;
}

uint64_t firstPart(std::vector<Report> const& reports)
{
	uint64_t safeReports = 0;
	for(auto& report:reports)
	{
		auto safe = checkReport(report);
		if(safe)
		{
			++safeReports;
		}
	}
	return safeReports;
}

uint64_t secondPart(std::vector<Report>& reports)
{
	uint64_t safeReports = 0;
	for(auto& report:reports)
	{
		auto safe = checkReport(report);
		if(safe)
		{
			++safeReports;
		}
		else
		{
			for(auto level=0; level<report.size();++level)
			{
				uint64_t previousNumber = report[level];
				report.erase(report.cbegin()+level);
				if(checkReport(report))
				{
					safe = true;
				}
				report.insert(report.cbegin()+level,previousNumber);
			}
			if(safe)
			{
				++safeReports;
			} 
		}
	}
	return safeReports;
} 

int main(int argc,char** argv)
{
	if(argc<2)
	{
		std::cout<<"Puzzle data not provided"<<std::endl;
		return -1;
	}
	std::vector<Report> reports;
	if(!parseData(argv[1],reports))
	{
		return -1;
	}
	std::cout<<"First part: "<< firstPart(reports) << std::endl;
	std::cout<<"Second part: "<< secondPart(reports) << std::endl;
	return 0;
}

