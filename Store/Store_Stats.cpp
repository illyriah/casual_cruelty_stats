#include <pqxx/pqxx>
#include <chrono>
#include <thread>
#include <sstream>
#include <iostream>

#include "XLR_Parser.hpp"
using namespace pqxx;
using namespace std;

int main(void)
{
	string sql;
	std::string previous_map;
	vector<XLR_Parser::player_info> previous_players;
	XLR_Parser::UrT_stats stats;

	std::chrono::duration<double> time_between_calls(30);
	std::chrono::duration<double> time_elapsed;
	std::chrono::duration<double> time_to_sleep;
	std::chrono::steady_clock::time_point start;
	std::chrono::steady_clock::time_point end;
	std::chrono::minutes match_duration(14);
	std::chrono::steady_clock::time_point match_start_time;

	connection C("dbname=casual_cruelty_stats user=postgres password=hostaddr=127.0.0.1 port=5432");
	if(C.is_open()) 
	{
		cout << "Opened database successfully: " << C.dbname() << endl;
	} 
	else 
	{
		cout << "Can't open database" << endl;
		return 1;
	}
	
	while(true)
	{
		start = std::chrono::steady_clock::now();
		stats = XLR_Parser::parse_XLR_stats(); 
		if(!stats.failure)
		{
			if(stats.map_name.compare(previous_map) != 0 && match_duration < (std::chrono::steady_clock::now() - match_start_time))
			{
				work W(C);
				sql = "SELECT add_map('" + stats.map_name + "')"; 
				W.exec(sql);
				cout << sql << endl;
				W.commit();
				match_start_time = std::chrono::steady_clock::now();
				std::this_thread::sleep_for(time_between_calls);
				previous_map = stats.map_name;
			}
			
			if(stats.map_name.compare(previous_map) != 0 && match_duration > (std::chrono::steady_clock::now() - match_start_time))
			{
				cout << "b3 fuckup" << endl;
			}
			else
			{

				work W(C);

				//if cannot find previous player in current list, that player left
				for(auto past_player: previous_players)
				{
					auto it = find_if(stats.players.begin(), stats.players.end(), [&](XLR_Parser::player_info previous_player)
					{
						return (past_player.name.compare(previous_player.name) == 0);
					});

					if(it == stats.players.end())
					{
						std::stringstream ss;
						ss << std::boolalpha << "SELECT player_disconnect('" <<  past_player.name << "')";
						cout << ss.str() << endl;
						W.exec(ss.str());
					}	
				}

				//if cannot find current player in previous list, that player joined
				for(auto current_player: stats.players)
				{
					auto it = find_if(previous_players.begin(), previous_players.end(), [&](XLR_Parser::player_info previous_player)
					{
						return (current_player.name.compare(previous_player.name) == 0);
					});

					if(it == previous_players.end())
					{
						std::stringstream ss;
						ss << std::boolalpha << "SELECT player_connect('" <<  current_player.name << "', " << current_player.regular << ", " << current_player.RSM_member <<  ")";
						cout << ss.str() << endl;
						W.exec(ss.str());
					}	
				}

				previous_players = stats.players;
				
				W.commit();
				previous_map = stats.map_name;
		
				cout << "cycle" << endl;
			}
		}
		else
		{
			cout << "offline" << endl;
		}

		end= std::chrono::steady_clock::now();
		time_elapsed = end - start;
		time_to_sleep = time_between_calls - time_elapsed;
		std::this_thread::sleep_for(time_to_sleep);
	}

	C.disconnect();

	return 0;
}

