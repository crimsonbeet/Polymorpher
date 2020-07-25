// Created by Alex Nikol Bourakov (ANB) around April/07


#ifndef wsiparserH
#define wsiparserH

#include ".\..\Include\WSIClassFactoryInterfaces.h"
#include ".\WSIWriter.h"

class EXP CWSIParser: public ISTREAMReader {
public:
	void parse(const std::string& wsi, IWSIMaterializer& mat) const;
	void parse(std::string::const_iterator from, std::string::const_iterator to, IWSIMaterializer&) const;
	ISTREAMWriter * create_writer() const;
	int Encoding() const;
};

class EXP CCRFParser: public CWSIParser {
public:
	ISTREAMWriter * create_writer() const;
};

class EXP CRawParser: public ISTREAMReader {
public:
	void parse(const std::string& wsi, IWSIMaterializer& mat) const;
	void parse(std::string::const_iterator from, std::string::const_iterator to, IWSIMaterializer&) const;
	ISTREAMWriter * create_writer() const;
};

#endif //wsiparserH

