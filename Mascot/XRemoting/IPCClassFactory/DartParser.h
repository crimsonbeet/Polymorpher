
#ifndef dartparserH
#define dartparserH

#include ".\..\Include\WSIClassFactory.h"

class EXP CDartParser: public ISTREAMReader {
public:
	void parse(const std::string& xml, IWSIMaterializer& mat) const;
	void parse(std::string::const_iterator from, std::string::const_iterator to, IWSIMaterializer&) const;
	ISTREAMWriter * create_writer() const;

	typedef bool delegate_validator_type(const substring_locator& tag_id, const wsi_object& obj, bool is_last);

	static void SetValidator(delegate_validator_type *validator); 

private:
	static delegate_validator_type *_s_validator; 
};

#endif //dartparserH

