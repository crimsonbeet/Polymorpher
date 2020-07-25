
#ifndef httpparserH
#define httpparserH

#include ".\..\Include\WSIClassFactoryInterfaces.h"

class EXP CHttpParser: public ISTREAMReader {
public:
	void parse(const std::string& str, IWSIMaterializer& mat) const;
	void parse(std::string::const_iterator from, std::string::const_iterator to, IWSIMaterializer&) const;
	ISTREAMWriter * create_writer() const;
	int Encoding() const;
};

#endif //httpparserH

