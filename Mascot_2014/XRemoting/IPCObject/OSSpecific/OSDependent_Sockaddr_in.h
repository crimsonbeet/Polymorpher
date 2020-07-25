// Created by Alex Nikol Bourakov (ANB) around April/07
#ifndef OSDependent_Sockaddr_in_h
#define OSDependent_Sockaddr_in_h

struct CSockaddrLess: std::binary_function<sockaddr_in, sockaddr_in, bool> {
	bool operator()(const sockaddr_in& x, const sockaddr_in& y) const {
		return
#ifdef _WIN32_WINDOWS
			(x.sin_addr.S_un.S_addr < y.sin_addr.S_un.S_addr) ||
			(x.sin_addr.S_un.S_addr == y.sin_addr.S_un.S_addr && x.sin_port < y.sin_port);
#else
			(x.sin_addr.s_addr < y.sin_addr.s_addr) ||
			(x.sin_addr.s_addr == y.sin_addr.s_addr && x.sin_port < y.sin_port);
#endif
	}
};


struct in_addr_helper {
	union {
		struct {
			unsigned char s_b1, s_b2, s_b3, s_b4;
		} S_un_b;
		struct {
			unsigned short s_w1, s_w2;
		} S_un_w;
		unsigned long S_addr;
		struct in_addr S_in_addr;
	} S_un;
	bool is_aloopback() {
		bool ok = true;
		if(S_un.S_un_b.s_b1 != 127) {
			ok = false;
		}
		else
		if(S_un.S_un_b.s_b2 != 0) {
			ok = false;
		}
		else
		if(S_un.S_un_b.s_b3 != 0) {
			ok = false;
		}
		else
		if(S_un.S_un_b.s_b4 != 1) {
			ok = false;
		}
		return ok;
	}
	static in_addr make_loopback() {
		in_addr_helper x;
		x.S_un.S_un_b.s_b1 = 127;
		x.S_un.S_un_b.s_b4 = 1;

		return x.S_un.S_in_addr;
	}
	bool is_inaddr_any() {
		return S_un.S_addr == 0;
	}
	in_addr_helper() {
		S_un.S_addr = 0;
	}
	in_addr_helper(const in_addr& sin_addr) {
		S_un.S_in_addr = sin_addr;
	}
};


std::string/*999.999.999.999:99999*/ EXPORTIPCINTERFACE sockaddr_in_tostring(const sockaddr_in& addr);
sockaddr_in EXPORTIPCINTERFACE sockaddr_in_fromstring(const std::string& addr/*999.999.999.999:99999*/); 

in_addr EXPORTIPCINTERFACE inet_addr_fromstring(const std::string& addr/*999.999.999.999:99999*/, u_short *pport = 0); 

bool EXPORTIPCINTERFACE check_loopaddress(const sockaddr_in& addr); 
bool EXPORTIPCINTERFACE is_loopaddress(const std::string& addr/*999.999.999.999:99999*/); 

#endif /*OSDependent_Sockaddr_in_h*/

