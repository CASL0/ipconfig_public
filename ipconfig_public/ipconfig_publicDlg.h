
// ipconfig_publicDlg.h : ヘッダー ファイル
//

#pragma once
#include <string>
#include <list>
#include <map>
#include <vector>
#include "httpRequest.h"

// CipconfigpublicDlg ダイアログ
class CipconfigpublicDlg : public CDialogEx
{
// コンストラクション
public:
	CipconfigpublicDlg(CWnd* pParent = nullptr);	// 標準コンストラクター

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IPCONFIG_PUBLIC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート

	static const UINT MESSAGE_DOWNLOAD_PUBLIC_IP_INFO = WM_APP + 1;
// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	enum class GROUP_ID
	{
		PRIVATE_IP = 0,
		PUBLIC_IP,
		DNS_SERVER,
	};

	enum class DOWNLOAD_STATE
	{
		PUBLIC_IPv4 = 0,
		PUBLIC_IPv6,
	};

	CListCtrl m_listCtrl;
	std::map<std::wstring, std::list<std::string>> m_privateIpAddresses;
	std::map<std::wstring, std::string> m_publicIpAddresses;
	std::map<std::wstring, std::list<std::string>> m_dnsServers;
	std::list<std::string> m_privateIpList;
	int m_nextItem = 0;
	std::vector<char> m_httpBuffer;
	std::string m_httpResponseBody;
	HttpRequest m_httpRequest;
	DOWNLOAD_STATE m_state = DOWNLOAD_STATE::PUBLIC_IPv4;

	ULONG GetAdapterInfo();
	std::wstring FormatErrorMessage(ULONG errorCode) const;
	std::string SockAddrToStrAddr(SOCKADDR* addr, ADDRESS_FAMILY af) const;
	void MakeGroup(UINT groupNameResourceId, int groupId);
	void AddItemToGroup(const std::wstring& itemKey, const std::wstring& itemValue, int groupId);
	void DisplayPrivateIpAddress();
	void DisplayPublicIpAddress();
	void DisplayDnsServers();
	std::wstring Utf8ToUtf16(const std::string& src);
	void UpdateListContents();
	void OnResponse(HINTERNET hInternet, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength);
	LRESULT OnDownloadPublicIpInfo(WPARAM, LPARAM);
public:
	afx_msg void OnLvnLinkClickedListIpinfo(NMHDR* pNMHDR, LRESULT* pResult);
	static void CALLBACK cb(HINTERNET hInternet, DWORD_PTR dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength);
	afx_msg void OnBnClickedButtonUpdate();
};