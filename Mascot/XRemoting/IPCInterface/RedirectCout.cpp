// Created by Alex Nikol Bourakov (ANB) around April/07

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "stdafx_interface.h"


//#if defined(__BORLANDC__)
//#include <Stlport\stdio_streambuf>
//#pragma comment(lib, "stlp.lib")
//#endif


BOOL _g_captstdcout_shutdown = FALSE;
BOOL _g_captstdcout_active = FALSE;
BOOL _g_abandoncaptstdcout_active = FALSE;
BOOL _g_captstdcout_error = FALSE;

wsi_gate _g_captstdcout_gate;

return_t __stdcall CaptureStandardOutput(LPVOID) {
	int fdpipe[2];
	if(_g_captstdcout_error || OSPipe(fdpipe) == -1) {
		LOGFailure(std::string("CaptureStandardOutput"), std::string("CreatePipe"));
		_g_captstdcout_error = TRUE;
		return 0;
	}

	_g_captstdcout_gate.lock();

	enum { READ_FD, WRITE_FD };

#if defined(_UCC)
	std::basic_filebuf<char> buf_out(fdpipe[WRITE_FD]);
	std::basic_filebuf<char> buf_in(fdpipe[READ_FD]);
	std::ostream pipe_out(&buf_out);
	std::istream pipe_in(&buf_in);
#elif defined(__BORLANDC__)
//	_SgI::stdio_ostreambuf buf_out(fdopen(fdpipe[WRITE_FD], "w"));
//	_SgI::stdio_istreambuf buf_in(fdopen(fdpipe[READ_FD], "r"));
//	std::ostream pipe_out(&buf_out);
//	std::istream pipe_in(&buf_in);
	std::ofstream pipe_out(fdpipe[WRITE_FD]);
	std::ifstream pipe_in(fdpipe[READ_FD]);
#elif defined(_MSC_VER)
	std::ofstream pipe_out(_fdopen(fdpipe[WRITE_FD], "w"));
	std::ifstream pipe_in(_fdopen(fdpipe[READ_FD], "r"));
#else
	__gnu_cxx::stdio_filebuf<char> buf_out(fdopen(fdpipe[WRITE_FD], "w"), std::ios_base::out, 1);
	__gnu_cxx::stdio_filebuf<char> buf_in(fdopen(fdpipe[READ_FD], "r"), std::ios_base::in, 1);
	std::ostream pipe_out(&buf_out);
	std::istream pipe_in(&buf_in);
#endif

	std::streambuf *old_buf = std::cout.rdbuf(pipe_out.rdbuf());

	std::cout << "std::cout has been redirected" << std::endl;

	_g_captstdcout_active = TRUE;

#if defined(_WIN32_WINDOWS)
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
#endif

	char buffer[4096];
	while(pipe_in.good() && !_g_captstdcout_shutdown) {
		char *c = buffer;
		bool done = false;
		while(!done && pipe_in.get(*c).good()) {
			switch(*c) {
				case 0xA: done = true;
				case 0xD:
				default: ++c;
					break;
			}
			if((c - buffer) > 4094) {
				done = true;
			}
		}
		*c = 0;
		if(c > buffer) {
			BOOL __stdcall IPCLogEx(const std::string& msg, BOOL no_cout);
			if(!IPCLogEx(buffer, TRUE)) {
				break;
			}
		}
	}

#if defined(_WIN32_WINDOWS)
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
#endif

	_g_captstdcout_gate.unlock();

	while(pipe_in.rdbuf()->in_avail()) pipe_in.ignore(pipe_in.rdbuf()->in_avail());
	std::cout.rdbuf(old_buf);

#if defined(_WIN32_WINDOWS)
	pipe_out.close();
	pipe_in.close();
#else
	buf_out.close();
	buf_in.close();
#endif

	return 0;
}


return_t __stdcall AbandoneCaptureStandardOutput(LPVOID) {
	while(CountActiveWorkItems() > 2) {
		OSSleep(100);
	}
	_g_captstdcout_shutdown = TRUE;

//	Make read pipe complete.
	std::cout << ">< XTerminate " << std::endl;

//	Gate must be locked if thread has been started.
//	Thread releases the gate on exit.
//	Do allocate the gate.
//	It will suspend the current thread until gate is released(that is thread exited).
	_g_captstdcout_gate.lock();
	_g_captstdcout_gate.unlock();

	_g_captstdcout_active = FALSE;
	_g_abandoncaptstdcout_active = FALSE;
	_g_captstdcout_shutdown = FALSE;

	return 0;
}


return_t EXP __stdcall RedirectStandardOutput() {
	return_t rc = 0;
	if(!_g_captstdcout_active) {
		rc = QueueWorkItem(CaptureStandardOutput);
		if(rc) {
			while(!_g_captstdcout_active && !_g_captstdcout_error) {
				OSWait();
			}
		}
	}
	return rc;
}

return_t EXP __stdcall RestoreStandardOutput() {
	return_t rc = 0;
	if(!_g_abandoncaptstdcout_active) {
		if(_g_captstdcout_active) {
			_g_abandoncaptstdcout_active = TRUE;
			rc = QueueWorkItem(AbandoneCaptureStandardOutput);
		}
	}
	return rc;
}
