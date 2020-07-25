
#ifndef xmlparserH
#define xmlparserH

#include ".\..\Include\WSIClassFactoryInterfaces.h"
#include ".\XMLWriter.h"

class EXP CXMLParser: public ISTREAMReader {
public:
	void parse(const std::string& xml, IWSIMaterializer& mat) const;
	void parse(std::string::const_iterator from, std::string::const_iterator to, IWSIMaterializer&) const;
	ISTREAMWriter * create_writer() const;
	int Encoding() const;
};

class EXP CXMLMProConfigParser: public CXMLParser {
public:
	void parse(const std::string& xml, IWSIMaterializer& mat) const;
	void parse(std::string::const_iterator from, std::string::const_iterator to, IWSIMaterializer&) const;
	ISTREAMWriter * create_writer() const;
};

#endif //xmlparserH

