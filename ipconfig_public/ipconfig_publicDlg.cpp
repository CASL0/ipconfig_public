
// ipconfig_publicDlg.cpp : 実装ファイル
//

#include "pch.h"
#include "framework.h"
#include "ipconfig_public.h"
#include "ipconfig_publicDlg.h"
#include "afxdialogex.h"
#include <iphlpapi.h>
#include <WS2tcpip.h>
#include <vector>
#include <memory>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CipconfigpublicDlg ダイアログ



CipconfigpublicDlg::CipconfigpublicDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IPCONFIG_PUBLIC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CipconfigpublicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_IPINFO, m_listCtrl);
}

BEGIN_MESSAGE_MAP(CipconfigpublicDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(LVN_LINKCLICK, IDC_LIST_IPINFO, &CipconfigpublicDlg::OnLvnLinkClickedListIpinfo)
	ON_MESSAGE(MESSAGE_DOWNLOAD_PUBLIC_IP_INFO, CipconfigpublicDlg::OnDownloadPublicIpInfo)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CipconfigpublicDlg::OnBnClickedButtonUpdate)
END_MESSAGE_MAP()


// CipconfigpublicDlg メッセージ ハンドラー

BOOL CipconfigpublicDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	CString resourceStr;
	(void)resourceStr.LoadStringW(AFX_IDS_APP_TITLE);
	SetWindowText(resourceStr);

	(void)m_listCtrl.SetExtendedStyle(m_listCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);
	(void)resourceStr.LoadStringW(IDS_LIST_HEADER_KEY);
	(void)m_listCtrl.InsertColumn(0, resourceStr, LVCFMT_LEFT, 150);

	(void)resourceStr.LoadStringW(IDS_LIST_HEADER_VALUE);
	(void)m_listCtrl.InsertColumn(1, resourceStr, LVCFMT_LEFT, 300);

	MakeGroup(IDS_GROUP_PRIVATE_IP, static_cast<int>(GROUP_ID::PRIVATE_IP));
	MakeGroup(IDS_GROUP_PUBLIC_IP, static_cast<int>(GROUP_ID::PUBLIC_IP));
	MakeGroup(IDS_GROUP_DNS_SERVER, static_cast<int>(GROUP_ID::DNS_SERVER));

	(void)m_listCtrl.EnableGroupView(TRUE);

	(void)resourceStr.LoadStringW(IDS_BUTTON_UPDATE);
	GetDlgItem(IDC_BUTTON_UPDATE)->SetWindowTextW(resourceStr);

	EnableDynamicLayout(TRUE);
	(void)m_pDynamicLayout->Create(this);
	(void)m_pDynamicLayout->AddItem(IDC_LIST_IPINFO, CMFCDynamicLayout::MoveNone(), CMFCDynamicLayout::SizeHorizontalAndVertical(100, 100));
	(void)m_pDynamicLayout->AddItem(IDC_BUTTON_UPDATE, CMFCDynamicLayout::MoveVertical(100), CMFCDynamicLayout::SizeNone());

	UpdateListContents();
	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CipconfigpublicDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CipconfigpublicDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

ULONG CipconfigpublicDlg::GetAdapterInfo()
{
	ULONG bufLen = 0;
	(void)GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, nullptr, nullptr, &bufLen);
	auto adapterAddresses = std::make_unique<std::remove_pointer<PIP_ADAPTER_ADDRESSES>::type[]>(bufLen);
	if (auto ret = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, nullptr, adapterAddresses.get(), &bufLen); ret != ERROR_SUCCESS)
	{
		OutputDebugString(L"GetAdaptersAddresses failed\n");
		OutputDebugString(FormatErrorMessage(ret).c_str());
		return ret;
	}

	PIP_ADAPTER_ADDRESSES currentAddress = adapterAddresses.get();
	for (; currentAddress; currentAddress = currentAddress->Next)
	{

		//リンクが稼働中でなければ無視
		if (currentAddress->OperStatus != IfOperStatusUp)
		{
			OutputDebugString(L"not IfOperStatusUp\n");
			continue;
		}

		//ループバックは無視
		if (currentAddress->IfType == IF_TYPE_SOFTWARE_LOOPBACK)
		{
			OutputDebugString(L"IF_TYPE_SOFTWARE_LOOPBACK\n");
			continue;
		}

		CString friendlyName;
		friendlyName.Format(L"Friendly name: %ls\n", currentAddress->FriendlyName);
		OutputDebugString(friendlyName.GetString());

		//NICのプライベートIPアドレス取得
		std::list<std::string> unicastList;
		PIP_ADAPTER_UNICAST_ADDRESS unicast = currentAddress->FirstUnicastAddress;
		while (unicast)
		{
			SOCKADDR* addr = unicast->Address.lpSockaddr;
			ADDRESS_FAMILY af = addr->sa_family;

			if (std::string ipAddress(SockAddrToStrAddr(addr, af)); !ipAddress.empty())
			{
				unicastList.push_back(ipAddress.c_str());
				CStringA message;
				message.Format("Private IP address added: %s\n", ipAddress.c_str());
				OutputDebugStringA(message.GetString());
			}

			unicast = unicast->Next;
		}

		m_privateIpAddresses[currentAddress->FriendlyName] = unicastList;

		//DNSサーバーのIPアドレス取得
		std::list<std::string> dnsServerList;
		IP_ADAPTER_DNS_SERVER_ADDRESS* dnsServerAddr = currentAddress->FirstDnsServerAddress;
		while (dnsServerAddr)
		{
			ADDRESS_FAMILY af = 0;
			auto addrLength = dnsServerAddr->Address.iSockaddrLength;
			switch (addrLength)
			{
			case sizeof(SOCKADDR_IN) :
				af = AF_INET;
				break;
			case sizeof(SOCKADDR_IN6) :
				af = AF_INET6;
				break;
			default:
				break;
			}

			if (auto dns = SockAddrToStrAddr(dnsServerAddr->Address.lpSockaddr, af); !dns.empty())
			{
				dnsServerList.push_back(dns.c_str());
				CStringA message;
				message.Format("Dns server address added: %s\n", dns.c_str());
				OutputDebugStringA(message.GetString());
			}
			dnsServerAddr = dnsServerAddr->Next;
		}
		m_dnsServers[currentAddress->FriendlyName] = dnsServerList;
	}
	return ERROR_SUCCESS;
}

std::wstring CipconfigpublicDlg::FormatErrorMessage(ULONG errorCode) const
{
	constexpr DWORD BUFFERLENGTH = 1024;
	std::vector<wchar_t> buf(BUFFERLENGTH);
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, static_cast<DWORD>(errorCode), 0, buf.data(), BUFFERLENGTH - 1, 0);
	return std::wstring(buf.data()) + L"\n";
}

std::string CipconfigpublicDlg::SockAddrToStrAddr(SOCKADDR* addr, ADDRESS_FAMILY af) const
{
	if (addr == nullptr)
	{
		return std::string();
	}

	std::vector<CHAR> addrStr(NI_MAXHOST);
	switch (af)
	{
	case AF_INET:
	{
		OutputDebugString(L"IPv4\n");
		inet_ntop(af, &(reinterpret_cast<SOCKADDR_IN*>(addr))->sin_addr, addrStr.data(), NI_MAXHOST);
		break;
	}
	case AF_INET6:
	{
		OutputDebugString(L"IPv6\n");
		inet_ntop(af, &(reinterpret_cast<SOCKADDR_IN6*>(addr))->sin6_addr, addrStr.data(), NI_MAXHOST);
		break;
	}
	default:
		OutputDebugString(L"不明なアドレスファミリ\n");
		break;
	}

	return std::string(addrStr.data());
}

void CipconfigpublicDlg::MakeGroup(UINT groupNameResourceId, int groupId)
{
	LVGROUP group = { 0 };
	group.cbSize = sizeof(LVGROUP);
	group.mask = LVGF_HEADER | LVGF_GROUPID | LVGF_TASK;

	CString groupName;
	(void)groupName.LoadStringW(groupNameResourceId);
	group.pszHeader = const_cast<LPWSTR>(groupName.GetString());

	CString resourceStr;
	(void)resourceStr.LoadStringW(IDS_GROUP_TASK_HIDE);
	group.pszTask = const_cast<LPWSTR>(resourceStr.GetString());

	group.iGroupId = groupId;
	(void)m_listCtrl.InsertGroup(-1, &group);
}

void CipconfigpublicDlg::AddItemToGroup(const std::wstring& itemKey, const std::wstring& itemValue, int groupId)
{
	LVITEM item = { 0 };
	item.mask = LVIF_TEXT | LVIF_GROUPID;
	item.iItem = m_nextItem;
	item.iSubItem = 0;
	item.pszText = const_cast<LPWSTR>(itemKey.c_str());
	item.iGroupId = groupId;
	(void)m_listCtrl.InsertItem(&item);
	(void)m_listCtrl.SetItemText(m_nextItem, 1, const_cast<LPWSTR>(itemValue.c_str()));
	m_nextItem++;
}

void CipconfigpublicDlg::DisplayPrivateIpAddress()
{
	for (const auto& [key, value] : m_privateIpAddresses)
	{
		for (const auto& elem : value)
		{
			AddItemToGroup(key, Utf8ToUtf16(elem), static_cast<int>(GROUP_ID::PRIVATE_IP));
		}
	}
}

void CipconfigpublicDlg::DisplayPublicIpAddress()
{
	for (const auto& [key, value] : m_publicIpAddresses)
	{
		AddItemToGroup(key, Utf8ToUtf16(value), static_cast<int>(GROUP_ID::PUBLIC_IP));
	}
}

void CipconfigpublicDlg::DisplayDnsServers()
{
	for (const auto& [key, value] : m_dnsServers)
	{
		for (const auto& elem : value)
		{
			AddItemToGroup(key, Utf8ToUtf16(elem), static_cast<int>(GROUP_ID::DNS_SERVER));
		}
	}
}

std::wstring CipconfigpublicDlg::Utf8ToUtf16(const std::string& src)
{
	auto bufLen = MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, nullptr, 0);
	std::vector<wchar_t> buffer(bufLen);
	(void)MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, buffer.data(), bufLen);
	return std::wstring(buffer.data());
}

void CipconfigpublicDlg::UpdateListContents()
{
	m_state = DOWNLOAD_STATE::PUBLIC_IPv4;
	PostMessage(MESSAGE_DOWNLOAD_PUBLIC_IP_INFO, static_cast<WPARAM>(m_state), 0);

	if (auto ret = GetAdapterInfo(); ret != ERROR_SUCCESS)
	{
		OutputDebugString(L"プライベートIPアドレスの取得に失敗\n");
	}

	DisplayPrivateIpAddress();
	DisplayDnsServers();

}

void CipconfigpublicDlg::OnLvnLinkClickedListIpinfo(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;

	PNMLVLINK pLinkInfo = reinterpret_cast<PNMLVLINK>(pNMHDR);
	auto groupId = pLinkInfo->iSubItem;

	LVGROUP group = { 0 };
	group.cbSize = sizeof(LVGROUP);
	group.mask = LVGF_TASK;

	CString task;
	if (m_listCtrl.GetGroupState(groupId, LVGS_COLLAPSED))
	{
		(void)task.LoadStringW(IDS_GROUP_TASK_HIDE);
		group.pszTask = const_cast<LPWSTR>(task.GetString());
		ListView_SetGroupState(m_listCtrl.m_hWnd, groupId, LVGS_COLLAPSED, 0);
	}
	else
	{
		(void)task.LoadStringW(IDS_GROUP_TASK_COLLAPSED);
		group.pszTask = const_cast<LPWSTR>(task.GetString());
		ListView_SetGroupState(m_listCtrl.m_hWnd, groupId, LVGS_COLLAPSED, LVGS_COLLAPSED);
	}
	(void)m_listCtrl.SetGroupInfo(groupId, &group);
}

void CALLBACK CipconfigpublicDlg::cb(HINTERNET hInternet, DWORD_PTR dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength)
{
	if (dwContext == NULL)
	{
		return;
	}

	auto dlg = reinterpret_cast<CipconfigpublicDlg*>(dwContext);
	dlg->OnResponse(hInternet, dwInternetStatus, lpvStatusInformation, dwStatusInformationLength);
}

void CipconfigpublicDlg::OnResponse(HINTERNET hInternet, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength)
{
	HINTERNET requestHandle = hInternet;
	switch (dwInternetStatus)
	{
	case WINHTTP_CALLBACK_STATUS_DATA_AVAILABLE:
	{
		auto size = *(reinterpret_cast<LPDWORD>(lpvStatusInformation));
		if (size == 0)
		{	//これ以上読み込むデータがないのでコールバック終了
			std::string publicIpInfo;
			publicIpInfo.swap(m_httpResponseBody);
			if (m_state == DOWNLOAD_STATE::PUBLIC_IPv4)
			{
				m_publicIpAddresses[L"IPv4"] = publicIpInfo.c_str();
				m_state = DOWNLOAD_STATE::PUBLIC_IPv6;
				PostMessage(MESSAGE_DOWNLOAD_PUBLIC_IP_INFO, static_cast<WPARAM>(m_state), 0);
			}
			else if (m_state == DOWNLOAD_STATE::PUBLIC_IPv6)
			{
				if (m_publicIpAddresses.at(L"IPv4") != publicIpInfo)
				{
					m_publicIpAddresses[L"IPv6"] = publicIpInfo.c_str();
				}
				DisplayPublicIpAddress();
			}
			return;
		}

		m_httpBuffer.resize(size + 1);

		if (!WinHttpReadData(requestHandle, m_httpBuffer.data(), size, nullptr))
		{
			OutputDebugString(L"WinHttpReadData failed\n");
			return;
		}
		// → WINHTTP_CALLBACK_STATUS_READ_COMPLETE
	}
	break;
	case WINHTTP_CALLBACK_STATUS_HEADERS_AVAILABLE:
	{
		DWORD statusCode = 0;
		DWORD statusCodeSize = sizeof(DWORD);
		if (!WinHttpQueryHeaders(requestHandle, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX, &statusCode, &statusCodeSize, WINHTTP_NO_HEADER_INDEX))
		{
			OutputDebugString(L"WinHttpQueryHeaders failed\n");
			return;
		}

		OutputDebugString(L"Status Code: ");
		OutputDebugString(std::to_wstring(statusCode).c_str());
		OutputDebugString(L"\n");
		switch (statusCode)
		{
		case 200:
			break;
		default:
			//200応答以外の場合は終了
			return;
		}

		if (!WinHttpQueryDataAvailable(requestHandle, nullptr))
		{
			OutputDebugString(L"WinHttpQueryDataAvailable failed\n");
			return;
		}
		// → WINHTTP_CALLBACK_STATUS_DATA_AVAILABLE
		break;
	}
	case WINHTTP_CALLBACK_STATUS_READ_COMPLETE:
		if (lpvStatusInformation && dwStatusInformationLength)
		{
			char* buffer = reinterpret_cast<char*>(lpvStatusInformation);
			buffer[dwStatusInformationLength] = '\0';
			m_httpResponseBody += buffer;

			// レスポンスデータデータ問い合わせ
			if (!WinHttpQueryDataAvailable(requestHandle, nullptr))
			{
				OutputDebugString(L"WinHttpQueryDataAvailable failed\n");
				return;
			}
			// → WINHTTP_CALLBACK_STATUS_DATA_AVAILABLE
		}
		break;
	case WINHTTP_CALLBACK_STATUS_SENDREQUEST_COMPLETE:
	{
		// WinHttpSendRequestの完了通知

		if (!WinHttpReceiveResponse(requestHandle, nullptr))
		{
			OutputDebugString(L"WinHttpReceiveResponse failed\n");
			return;
		}
		// → WINHTTP_CALLBACK_STATUS_HEADERS_AVAILABLE
		break;
	}
	case WINHTTP_CALLBACK_STATUS_NAME_RESOLVED:
		OutputDebugString(L"名前解決\n");
		OutputDebugString(reinterpret_cast<LPWSTR>(lpvStatusInformation));
		OutputDebugString(L"\n");
		break;
	default:
		break;
	}

}

LRESULT CipconfigpublicDlg::OnDownloadPublicIpInfo(WPARAM wParam, LPARAM)
{
	auto state = static_cast<DOWNLOAD_STATE>(wParam);
	switch (state)
	{
	case DOWNLOAD_STATE::PUBLIC_IPv4:
		m_httpRequest.close();
		m_httpRequest.RequestUri(L"https://api.ipify.org", cb, reinterpret_cast<DWORD_PTR>(this));
		break;
	case DOWNLOAD_STATE::PUBLIC_IPv6:
		m_httpRequest.close();
		m_httpRequest.RequestUri(L"https://api64.ipify.org", cb, reinterpret_cast<DWORD_PTR>(this));
		break;
	default:
		break;
	}
	return ERROR_SUCCESS;
}

void CipconfigpublicDlg::OnBnClickedButtonUpdate()
{
	m_privateIpAddresses.clear();
	m_publicIpAddresses.clear();
	m_dnsServers.clear();
	(void)m_listCtrl.DeleteAllItems();
	m_nextItem = 0;
	UpdateListContents();
}
