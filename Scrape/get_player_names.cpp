#include "XLR_Parser.hpp"

std::vector<XLR_Parser::player_info> XLR_Parser::get_player_names(htmlDocPtr document_pointer)
{
	std::vector<player_info> players;
	xmlXPathObjectPtr xpath_nodes;
	xmlNodeSetPtr node_set;

	//This tag marks the start of the divs containign the teams
	const std::string xpath_expression_player_tables = "//div[@class='span4 league']";

	xpath_nodes = get_xpath_nodes(document_pointer, xpath_expression_player_tables);
	if(!xpath_nodes)
	{
		//set_failure("Error evaluating xpath expression ");
		return players;
	}
	node_set = xpath_nodes->nodesetval;

	for(int a = 0; a < node_set->nodeNr; a++)
	{
		xmlNode* div_league, *div_title, *h4_title, *a_title, *table_players, *tr_players, *a_players, *td_players;

		//navigate to where the title for the teams is
		div_league	= node_set->nodeTab[a];
		div_title	= get_next_tag(div_league->children, "div");
		h4_title	= get_next_tag(div_title->children, "h4");
		a_title		= get_next_tag(h4_title->children, "a");

		//only look at teams 2 and 3, 1 is for spec, -1 and 4 are sometimes bots
		if((strcmp((const char *)a_title->children->content, "2") == 0) || 
		   (strcmp((const char *)a_title->children->content, "3") == 0))
		{
			// start from the root again
			table_players	= get_next_tag(div_league->children, "table");
			// The first table row contains colum titles
			tr_players= get_next_tag(table_players->children, "tr");

			//each row from here should contain the information on one player
			while((tr_players = tr_players->next))
			{
				xmlChar* player_name;

				// needed, sometimes this spot is reached even though teh last row has been read.
				if(!tr_players->children)
				{
					break;
				}

				td_players = get_next_tag(tr_players->children, "td");
				td_players = td_players->next;
				td_players = td_players->next;
				
				//There are 4 different ways a player name can be stored in the html
				//in a span attribute in a link in cell, 
				//in a link in a cell,
				//in a span attribute in a cell, 
				//in a cell.
				//
				// This searches for valid content in each of those places
				// The cout statements are to indicate that an invalid location has been reached
				// this means that there is another, unaccounted for, location.
				a_players = get_next_tag(td_players->children, "a");
				if(a_players)
				{
					xmlNode* span_tag = get_next_tag(a_players->children, "span");
					
					if(span_tag)
					{
						player_name = xmlGetProp(span_tag, (const xmlChar *)"data-original-title") ;
						if(player_name != NULL)
						{
							players.push_back(player_info((const char *)player_name, true));
						}
						else
						{
							std::cout << "span in link" << std::endl;
						}
					}
					else
					{
						
						player_name = a_players->children->content;
						if(player_name != NULL)
						{
							players.push_back(player_info((const char *)player_name, true));
						}
						else
						{
							std::cout << "link" << std::endl;
						}
					}
				}
				else
				{
					xmlNode* span_tag = get_next_tag(td_players->children, "span");
					
					if(span_tag)
					{
						
						player_name = xmlGetProp(span_tag, (const xmlChar *)"data-original-title") ;
						if(player_name != NULL)
						{
							players.push_back(player_info((const char *)player_name, false));
						}
						else
						{
							std::cout << "span outside link" << std::endl;
						}
					}
					else
					{	

						player_name = td_players->children->content;
						if(player_name != NULL)
						{
							players.push_back(player_info((const char *)player_name, false));
						}
						else
						{
							std::cout << "plain" << std::endl;
						}
					}
				}
			}
		}
	}

	return players;
}
