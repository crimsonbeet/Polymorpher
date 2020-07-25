
#ifndef fepparserH
#define fepparserH

#include ".\..\Include\WSIClassFactoryInterfaces.h"

class EXP CFepParser: public ISTREAMReader {
public:
	void parse(const std::string& data, IWSIMaterializer& mat) const;
	void parse(std::string::const_iterator from, std::string::const_iterator to, IWSIMaterializer&) const;
	ISTREAMWriter * create_writer() const;

	static bool Initialize(); 

	static void test(const std::string& telegram);
	static int set_useOpenProtocol(int); 

	static int _use_open_protocol; 

};

#endif //fepparserH

