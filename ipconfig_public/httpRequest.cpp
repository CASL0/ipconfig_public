#include "pch.h"
#include "httpRequest.h"
#include <vector>

DWORD HttpRequest::RequestUri(const std::wstring& url, WINHTTP_STATUS_CALLBACK callback, DWORD_PTR callbackParam)
{
	//URLの解析
	std::wstring hostName, path;
	INTERNET_PORT port = 0;
	if (auto ret = AnalyzeUrl(url, hostName, path, port);ret != ERROR_SUCCESS)
	{
		OutputDebugString(L"不正なURL\n");
		return ret;
	}

	//セッションハンドルの生成
	m_session.reset(
		WinHttpOpen(m_userAgent.c_str(), WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, WINHTTP_FLAG_ASYNC)
	);
	if (m_session == nullptr)
	{
		OutputDebugString(L"WinHttpOpen failed\n");
		return GetLastError();
	}

	//接続ハンドルの生成
	m_connect.reset(WinHttpConnect(m_session.get(), hostName.c_str(), port, 0));
	if (m_connect == nullptr)
	{
		OutputDebugString(L"WinHttpConnect failed\n");
		return GetLastError();
	}

	DWORD schemeFlag = port == INTERNET_DEFAULT_HTTPS_PORT ? WINHTTP_FLAG_SECURE : 0;

	//リクエストハンドルの生成
	m_request.reset(
		WinHttpOpenRequest(
			m_connect.get(), L"GET", path.c_str(), 
			nullptr, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, schemeFlag)
	);
	if (m_request == nullptr)
	{
		OutputDebugString(L"WinHttpOpenRequest failed\n");
		return GetLastError();
	}

	//コールバックの設定
	if (WinHttpSetStatusCallback(m_request.get(), callback, WINHTTP_CALLBACK_FLAG_ALL_NOTIFICATIONS, 0) == WINHTTP_INVALID_STATUS_CALLBACK)
	{
		OutputDebugString(L"WinHttpSetStatusCallback failed\n");
		return GetLastError();
	}

	if (!WinHttpSendRequest(m_request.get(), WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, callbackParam))
	{
		OutputDebugString(L"WinHttpSendRequest failed\n");
		return GetLastError();
	}

	return ERROR_SUCCESS;
}

void HttpRequest::close()
{
	if (m_request != nullptr)
	{
		WinHttpSetStatusCallback(m_request.get(), nullptr, 0, 0);
		m_request.reset();
	}

	if (m_connect != nullptr)
	{
		m_connect.reset();
	}

	if(m_session!=nullptr)
	{
		m_session.reset();
	}

}

DWORD HttpRequest::AnalyzeUrl(const std::wstring& url, std::wstring& hostName, std::wstring& path, INTERNET_PORT& port) const
{
	OutputDebugString(L"Analyze URL: ");
	OutputDebugString(url.c_str());
	OutputDebugString(L"\n");
	constexpr DWORD URL_MAX_LENGTH = 2048;
	URL_COMPONENTS urlComponents = { 0 };
	urlComponents.dwStructSize = sizeof(URL_COMPONENTS);
	std::vector<WCHAR> hostNameBuff(URL_MAX_LENGTH);
	std::vector<WCHAR> pathBuff(URL_MAX_LENGTH);
	urlComponents.lpszHostName = hostNameBuff.data();
	urlComponents.lpszUrlPath = pathBuff.data();
	urlComponents.dwHostNameLength = URL_MAX_LENGTH;
	urlComponents.dwUrlPathLength = URL_MAX_LENGTH;
	if (!WinHttpCrackUrl(url.c_str(), 0, 0, &urlComponents))
	{
		OutputDebugString(L"WinHttpCrackUrl failed\n");
		return GetLastError();
	}

	hostName = urlComponents.lpszHostName;
	path = urlComponents.lpszUrlPath;
	port = urlComponents.nPort;
	return ERROR_SUCCESS;
}
