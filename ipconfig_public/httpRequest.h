#pragma once
#include <string>
#include <memory>
#include <Windows.h>
#include <winhttp.h>

class HttpRequest
{
private:
	std::wstring m_userAgent;
	std::unique_ptr<void, decltype(&WinHttpCloseHandle)> m_session = { nullptr, WinHttpCloseHandle };
	std::unique_ptr<void, decltype(&WinHttpCloseHandle)> m_connect = { nullptr, WinHttpCloseHandle };
	std::unique_ptr<void, decltype(&WinHttpCloseHandle)> m_request = { nullptr, WinHttpCloseHandle };
public:
	HttpRequest(const std::wstring& userAgent = L"http request") :m_userAgent(userAgent) {};
	DWORD RequestUri(const std::wstring& url, WINHTTP_STATUS_CALLBACK callback, DWORD_PTR callbackParam);
	void close();

private:
	DWORD AnalyzeUrl(const std::wstring& url, std::wstring& hostName, std::wstring& path, INTERNET_PORT& port) const;
};