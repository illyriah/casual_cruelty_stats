#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/HTMLparser.h>

#include <iostream>
#include <string>
#include <vector>


class urt_data
{


};

static std::string dataBuffer;
static std::string errorBuffer;
static size_t dataSize;

static size_t write_data(char *data, size_t size, size_t nmemb, std::string *dataBuffer)
{
	int realSize;

	if(dataBuffer == NULL)
	{
		return 0;
	}

	dataBuffer->append(data, size * nmemb);

	realSize = size * nmemb;
	dataSize = realSize;

	return realSize;
}

static bool initialize_curl(CURL *&conn, const std::string& url)
{
  CURLcode code;

  conn = curl_easy_init();

  if (conn == NULL)
  {
    fprintf(stderr, "Failed to create CURL connection\n");

    exit(EXIT_FAILURE);
  }

  code = curl_easy_setopt(conn, CURLOPT_ERRORBUFFER, errorBuffer.c_str());
  if (code != CURLE_OK)
  {
	  std::cerr << "Failed to set error buffer " <<  code << std::endl;

    return false;
  }

  code = curl_easy_setopt(conn, CURLOPT_URL, url.c_str());
  if (code != CURLE_OK)
  {
	  std::cerr << "Failed to set URL " << errorBuffer << std::endl;

    return false;
  }

  code = curl_easy_setopt(conn, CURLOPT_FOLLOWLOCATION, 1L);
  if (code != CURLE_OK)
  {
	  std::cerr << "Failed to set redirect option " << errorBuffer << std::endl;

    return false;
  }

  code = curl_easy_setopt(conn, CURLOPT_WRITEFUNCTION, write_data);
  if (code != CURLE_OK)
  {
	std::cerr << "Failed to set writer " << errorBuffer << std::endl;

	return false;
  }

  code = curl_easy_setopt(conn, CURLOPT_WRITEDATA, &dataBuffer);
  if (code != CURLE_OK)
  {
	std::cerr << "Failed to set write data " << errorBuffer << std::endl;

	return false;
  }

  return true;
}

xmlXPathObjectPtr get_xpath_nodes(xmlDocPtr document_pointer, std::string xpath_expression, xmlNode * new_root = NULL)
{
	xmlXPathContext * xpath_context;
	xmlXPathObjectPtr xpath_result;

	xpath_context = xmlXPathNewContext(document_pointer);
	
	if(new_root != NULL)
	{
		xpath_context->node = new_root;
	}

	if(xpath_context == NULL)
	{
		std::cerr << "Could not create xpath context" << std::endl;
		return NULL;
	}

	xpath_result = xmlXPathEvalExpression((xmlChar*) xpath_expression.c_str(), xpath_context);
	xmlXPathFreeContext(xpath_context);
	if(xpath_result == NULL)
	{
		std::cerr << "Could not evaluate xpath expression " << xpath_expression << std::endl;
		return NULL;
	}

	if(xmlXPathNodeSetIsEmpty(xpath_result->nodesetval))
	{
		std::cerr << "No results from xpath expression " << xpath_expression << std::endl;
		xmlXPathFreeObject(xpath_result);
		return NULL;
	}
		
	return xpath_result;
}

xmlNode* get_next_tag(xmlNode* current_tag, std::string tag_name)
{
	xmlNode * node_cursor;
	
	for(node_cursor = current_tag; node_cursor; node_cursor = node_cursor->next)
	{
		if(!xmlStrcmp(node_cursor->name, (const unsigned char *)tag_name.c_str()))
		{
			return node_cursor;
		}
	}

	return NULL;
}


std::string get_map_name(xmlNode * h5_tag)
{
	xmlNode * div_tag;
	std::string map_name;

	div_tag	= get_next_tag(h5_tag, "div"); 
	map_name = std::string((const char *)div_tag->children->children->content);		

	return map_name;
}

static void print_element_names(xmlNode *a_node)
{
	xmlNode * cur = NULL;

	for(cur = a_node; cur; cur = cur->next)
	{
		if(cur->type == XML_ELEMENT_NODE)
		{
			std::cout << cur->name << std::endl;
		}
		print_element_names(cur->children);
	}
}


int main(void)
{
	CURL *conn;
	CURLcode code;
	htmlDocPtr document_pointer;
	xmlXPathObjectPtr xpath_nodes;
	xmlNodeSetPtr node_set;
	

	const std::string url = "http://urbanwasteland.net/stats/1/server_players";
	const std::string xpath_expression_map = "//div[@class='server-info-box']/h5";
	const std::string xpath_expression_player_tables = "//div[@class='span4 league']";
	//const std::string xpath_expression_players = "//*[contains(@href, '/stats/1/player_stats/view/')]";

	curl_global_init(CURL_GLOBAL_DEFAULT);
	
	if(!initialize_curl(conn, url))
	{
		std::cerr << "Failed to retrieve " << url << " " << errorBuffer << std::endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << "Initialized!!" << std::endl;
	}

	code = curl_easy_perform(conn);
	curl_easy_cleanup(conn);

	if(code != CURLE_OK)
	{
		std::cerr << " Failed to get " << url << " " << errorBuffer << std::endl;
		exit(EXIT_FAILURE);
	}
	
	document_pointer = htmlReadDoc((xmlChar *) dataBuffer.c_str(), NULL, NULL,  HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);


	
	xpath_nodes = get_xpath_nodes(document_pointer, xpath_expression_map);
	if(!xpath_nodes)
	{
		std::cerr << "Error evaluating xpath expression" << std::endl;
		exit(EXIT_FAILURE);
	}

	node_set = xpath_nodes->nodesetval;


	for(int a = 0; a < node_set->nodeNr; a++)
	{
		xmlNode* h5_tag = node_set->nodeTab[a];
	
		if(strcmp((const char *)h5_tag->children->content, "CURRENT MAP") == 0)
		{
			xmlNode * div_tag;
			std::string map_name;

			div_tag	= get_next_tag(h5_tag, "div"); 
			map_name = std::string((const char *)div_tag->children->children->content);		

			break;
		}
	}


	xpath_nodes = get_xpath_nodes(document_pointer, xpath_expression_player_tables);
	if(!xpath_nodes)
	{
		std::cerr << "Error evaluating xpath expression" << std::endl;
		exit(EXIT_FAILURE);
	}

	node_set = xpath_nodes->nodesetval;


	std::vector<std::string> names;
	for(int a = 0; a < node_set->nodeNr; a++)
	{
		xmlNode* div_league, *div_title, *h4_title, *a_title, *table_players, *tr_players, *a_players, *td_players;

		div_league	= node_set->nodeTab[a];
		div_title	= get_next_tag(div_league->children, "div");
		h4_title	= get_next_tag(div_title->children, "h4");
		a_title		= get_next_tag(h4_title->children, "a");


		if((strcmp((const char *)a_title->children->content, "2") == 0) || 
		   (strcmp((const char *)a_title->children->content, "3") == 0))
		{
			table_players	= get_next_tag(div_league->children, "table");
			tr_players= get_next_tag(table_players->children, "tr");

			while(tr_players = tr_players->next)
			{
				xmlChar* player_name;

				if(!tr_players->children)
				{
					break;
				}

				td_players = get_next_tag(tr_players->children, "td");
				td_players = td_players->next;
				td_players = td_players->next;
				
				a_players = get_next_tag(td_players->children, "a");
				if(a_players)
				{
					xmlNode* span_tag = get_next_tag(a_players->children, "span");
					
					if(span_tag)
					{
						player_name = xmlGetProp(span_tag, (const xmlChar *)"data-original-title") ;
						names.push_back(std::string((const char *)player_name));
					}
					else
					{
						player_name = a_players->children->content;
						names.push_back(std::string((const char *)player_name));
					}
				}
				else
				{
					player_name = td_players->children->content;
					names.push_back(std::string((const char *)player_name));
				}
			}
		}
	}

	for(auto player : names)
	{
		std::cout << player << std::endl;
	}

	return 0;
}
