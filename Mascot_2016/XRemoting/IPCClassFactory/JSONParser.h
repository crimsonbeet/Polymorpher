/*
 * JSONParser.h; Created on: Mar 20, 2013; Author: alex bourakov
 *
 */


#include ".\..\Include\WSIClassFactoryInterfaces.h"
#include ".\JSONWriter.h"

#ifndef jsonparserH
#define jsonparserH

class EXP CJSONParser: public ISTREAMReader {
public:
	void parse(const std::string& wsi, IWSIMaterializer& mat) const;
	void parse(std::string::const_iterator from, std::string::const_iterator to, IWSIMaterializer&) const;
	ISTREAMWriter * create_writer() const;
	int Encoding() const;
};

#endif //jsonparserH

