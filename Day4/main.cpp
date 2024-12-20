#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <sstream>
#include <numeric>


using Text = std::vector<std::string>;

bool parseData(std::string const& puzzleInput,Text& text)
{
	std::fstream puzzleData(puzzleInput,std::fstream::in);
	if(!puzzleData.is_open())
	{
		std::cout<<"Can't open the puzzle input"<<std::endl;
		return false;
	}
	std::string row;
	while(std::getline(puzzleData,row))
	{
		text.push_back(row);
	}
	return true;
}

bool outOfBounds(long const x,long const y,long const w,long const h)
{
	return (x<0) || (x>=w) || (y<0) || (y>=h);
}

uint64_t firstPart(Text const& input)
{
	auto result = 0ul;
	auto const width = input.size();
	for(auto x=0ul;x<width;++x)
	{
		auto const height = input[x].length();
		for(auto y=0ul;y<height;++y)
		{
			std::string const word = "XMAS"; 
			auto dx = 0l, dy = 0l;
			if(input[x][y]=='X')
			{
				for(auto dirX = -1;dirX<=1;++dirX)
				{
					for(auto dirY = -1;dirY<=1;++dirY)
					{
						auto hit = true;
						for(auto i=0ul;i<word.length();++i)
						{
							dx = dirX*i;
							dy = dirY*i;
							if(outOfBounds(x+dx,y+dy,width,height) || input[x+dx][y+dy]!=word[i])
							{
								hit = false;
							}
						}
						if(hit)
						{
							result += 1;
						}
					}
				}	
			}

		}
	}
	return result;
}

uint64_t secondPart(Text const& input)
{
	auto result = 0ul;
	auto const width = input.size();
	for(auto x=0ul;x<width;++x)
	{	
		auto const height = input[x].length();
		for(auto y=0ul;y<height;++y)
		{
			if(input[x][y]=='A')
			{
				auto const boundsCheckLeft = !outOfBounds(x-1,y-1,width,height) && !outOfBounds(x-1,y+1,width,height);
				auto const boundsCheckRight = !outOfBounds(x+1,y-1,width,height) && !outOfBounds(x+1,y+1,width,height);

				if (boundsCheckLeft && boundsCheckRight)
				{
					char const chars[] = {
						input[x-1][y-1], // top left
						input[x+1][y-1], // top right
						input[x-1][y+1], // bottom left
						input[x+1][y+1]  // bottom right
					};
					auto const sameRows = (chars[0] == chars[2]) &&  (chars[1] == chars[3]);
					auto const sameColumns = (chars[0] == chars[1]) &&  (chars[2] == chars[3]);
					if(sameRows || sameColumns)
					{
						auto sum = 'M'+'M'+'S'+'S';
						if(std::accumulate(chars,chars+4,0) == sum)
						{
							result += 1;
						}
					}
				}
			}
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
	Text text;
	if(!parseData(argv[1],text))
	{
		return -1;
	}
	std::cout<<"First part: "<< firstPart(text) << std::endl;
	std::cout<<"Second part: "<< secondPart(text) << std::endl;
	return 0;
}




