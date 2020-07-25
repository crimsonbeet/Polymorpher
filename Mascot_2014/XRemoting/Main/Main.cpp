

#include ".\..\Include\stdafx_OS9.h"


#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#include <fcntl.h>
#include <stdlib.h>
#include <assert.h>


#include <ctime>
#include <ios>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <set>
#include <utility>
#include <stack>
#include <vector>
#include <algorithm>
#include <functional>
#include <limits>
#include <queue>
#include <deque>


struct TAMessage {
	std::vector<int> _values;
};

struct TBMessage {
	std::vector<TAMessage> _messages;
};






#ifdef WSI_NAMESPACE
#undef WSI_NAMESPACE
#endif
#ifdef autocreateserialization_not_necessaryH
#undef autocreateserialization_not_necessaryH
#endif

#define WSI_NAMESPACE XSimpleProtocol

#define WSICLASSFACTORY_IMPL




#include ".\..\Include\WSIClassFactory.h"



BEGIN_WSI_SERIALIZATION_OBJECT(TAMessage)
SET_OBJECT_IDENTIFICATION(A)
CONTAINS_FLAT_MEMBER(_values, V)
END_WSI_SERIALIZATION_OBJECT()




BEGIN_WSI_SERIALIZATION_OBJECT(TBMessage)
SET_OBJECT_IDENTIFICATION(B)
CONTAINS_OBJECT_MEMBER(_messages, B)
END_WSI_SERIALIZATION_OBJECT()


#include ".\..\Include\WSIParser.h"

IWSIMarshaller * CreateIWSIMarshaller();

void DestroyIWSIMarshaller(IWSIMarshaller *marsh);


int main(int argc, char **argv) {
	TBMessage remoobj;

	std::string config = "><$B ><A V={3 2 1} $";

	IWSIMarshaller *pMarshaller = CreateIWSIMarshaller();
	if(pMarshaller) {
		IWSIMarshaller& marshaller = *pMarshaller;

		CWSITBMessage sr(remoobj);
		sr.register_with(marshaller);

		marshaller(CWSIParser(), config);
	}
	DestroyIWSIMarshaller(pMarshaller);
	return 0;
}
