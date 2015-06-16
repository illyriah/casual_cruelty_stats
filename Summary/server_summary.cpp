#include "XLR_Parser.hpp"

int main(int argc, char* argv[])
{
	XLR_Parser::UrT_stats stats = XLR_Parser::parse_XLR_stats(); 

	if(!stats.failure)
	{
		std::cout << stats.player_count << " " << stats.map_name << std::endl;	
	
	if(argc > 1)
	{
		//std::cout << "here" << std::endl;
		//std::for_each(stats.player_names.begin(), stats.player_names.end(), [](std::string player_name)
		//{
			//std::cout << player_name << std::endl;
		//});
	}
	}
	else
	{
		std::cout << stats.error_message << std::endl;
	}

	return 0;
}
