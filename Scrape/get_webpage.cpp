#include "XLR_Parser.hpp"

htmlDocPtr XLR_Parser::get_document_pointer(std::string dataBuffer)
{
	htmlDocPtr document_pointer;

	document_pointer = htmlReadDoc((xmlChar *) dataBuffer.c_str(), NULL, NULL,  HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);

	if(!document_pointer)
	{
		set_failure("could not create document poitner");
		return NULL;
	}

	return document_pointer;
}

std::string XLR_Parser::get_webpage()
{
	CURL *conn;
	CURLcode code;
	std::string dataBuffer;
	std::string errorBuffer;

	//url to xlr stats page
	static const std::string url = "http://urbanwasteland.net/stats/1/server_players";

	curl_global_init(CURL_GLOBAL_DEFAULT);
	
	initialize_curl(conn, url, dataBuffer);

	if(failure)
	{
		return error_message;
	}

	code = curl_easy_perform(conn);
	curl_easy_cleanup(conn);

	if(code != CURLE_OK)
	{
		set_failure("Failed to get " + url);
		return error_message;
	}

	return dataBuffer;
}

