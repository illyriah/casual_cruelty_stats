#include "UrT_XLR_Parser.hpp"

void UrT_XLR_Parser::set_failure(std::string custom_message)
{
	errorBuffer = custom_message + errorBuffer;
	failure = true;
}

UrT_XLR_Parser::UrT_stats UrT_XLR_Parser::get_failure()
{
	UrT_stats return_failure;
	return_failure.failure = true;
	return_failure.error_message = errorBuffer;

	return return_failure;
}

xmlNode* UrT_XLR_Parser::get_next_tag(xmlNode*& current_tag, const std::string& tag_name)
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

xmlXPathObjectPtr UrT_XLR_Parser::get_xpath_nodes(xmlDocPtr document_pointer, const std::string& xpath_expression, xmlNode * new_root)
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
		set_failure("Could not create xpath context");
		return NULL;
	}

	xpath_result = xmlXPathEvalExpression((xmlChar*) xpath_expression.c_str(), xpath_context);
	xmlXPathFreeContext(xpath_context);
	if(xpath_result == NULL)
	{
		set_failure("Could not evaluate xpath expression " + xpath_expression);
		return NULL;
	}

	return xpath_result;
}
