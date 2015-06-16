#include "XLR_Parser.hpp"

std::string XLR_Parser::get_map_name(htmlDocPtr document_pointer)
{
	std::string map_name;
	xmlXPathObjectPtr xpath_nodes;
	xmlNodeSetPtr node_set;
	static const std::string xpath_expression_map = "//div[@class='server-info-box']/h5";

	xpath_nodes = get_xpath_nodes(document_pointer, xpath_expression_map);
	if(xpath_nodes == NULL)
	{
		set_failure("Error evaluating xpath expression");
		return error_message;
	}
	node_set = xpath_nodes->nodesetval;

	for(int a = 0; a < node_set->nodeNr; a++)
	{
		xmlNode* h5_tag = node_set->nodeTab[a];
	
		if(strcmp((const char *)h5_tag->children->content, "CURRENT MAP") == 0)
		{
			xmlNode * div_tag;

			div_tag	= get_next_tag(h5_tag, "div"); 
			map_name = std::string((const char *)div_tag->children->children->content);		
			//TODO what if this point is never reached
			break;
		}
	}

	return map_name;
}
