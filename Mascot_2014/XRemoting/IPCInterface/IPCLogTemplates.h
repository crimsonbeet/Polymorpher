// Created by Alex Nikol Bourakov (ANB) around April/07

#pragma once

#ifndef ipclogtemplatesH
#define ipclogtemplatesH

template<typename F, typename N>
void LOGFailure(const F& Func, const N& Name, const std::string& Msg = "Error") {
	std::string error = FormatSystemError(GetLastError());
	std::ostringstream ostr;
	ostr << Func << '(' << Msg << '=' << Name << ')' << ' ' << "syserror" << ':' << ' ' << error;

	IPCLog(ostr.str()); 
}

template<typename F, typename N, typename S>
void LOGMessage(const F& Func, const N& Name, const S& Stat, const std::string& Msg = "succeeded with:") {
	std::string error = FormatSystemError(GetLastError());
	std::ostringstream ostr;
	ostr << Func << '(' << Name << ')' << Msg << ' ' << Stat;

	IPCLog(ostr.str()); 
}

#endif //ipclogtemplatesH
