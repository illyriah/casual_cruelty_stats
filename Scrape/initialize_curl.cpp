#include "XLR_Parser.hpp"

void XLR_Parser::initialize_curl(CURL *&conn, const std::string& url, std::string &dataBuffer)
{
	CURLcode code;

	conn = curl_easy_init();

	if (conn == NULL)
	{
		set_failure("Failed to create CURL connection");
		return;
	}

	code = curl_easy_setopt(conn, CURLOPT_ERRORBUFFER, errorBuffer.c_str());
	if (code != CURLE_OK)
	{
		set_failure("Failed to set error buffer");
		return;
	}

	code = curl_easy_setopt(conn, CURLOPT_URL, url.c_str());
	if (code != CURLE_OK)
	{
		set_failure("Failed to set URL");
		return;
	}

	code = curl_easy_setopt(conn, CURLOPT_FOLLOWLOCATION, 1L);
	if (code != CURLE_OK)
	{
		set_failure("Failed to set redirect option");
		return;
	}

	code = curl_easy_setopt(conn, CURLOPT_WRITEFUNCTION, write_data);
	if (code != CURLE_OK)
	{
		set_failure("Failed to set writer");
		return;
	}

	code = curl_easy_setopt(conn, CURLOPT_WRITEDATA, &dataBuffer);
	if (code != CURLE_OK)
	{
		set_failure("Failed to set write data");
		return;
	}

	return;
}

size_t XLR_Parser::write_data(char *data, size_t size, size_t nmemb, std::string &dataBuffer)
{
	int realSize;

	dataBuffer.append(data, size * nmemb);
	realSize = size * nmemb;

	return realSize;
}
