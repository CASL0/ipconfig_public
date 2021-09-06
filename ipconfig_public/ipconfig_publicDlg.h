
// ipconfig_publicDlg.h : ヘッダー ファイル
//

#pragma once
#include <string>
#include <list>
#include <map>

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


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CListCtrl m_listCtrl;
	std::map<std::wstring, std::list<std::string>> m_privateIpAddresses;
	std::list<std::string> m_privateIpList;
	int m_nextItem = 0;

	ULONG GetAdapterInfo();
	std::wstring FormatErrorMessage(ULONG errorCode) const;
	std::string SockAddrToStrAddr(SOCKADDR* addr, ADDRESS_FAMILY af) const;
	void MakeGroup(UINT groupNameResourceId, int groupId);
	void AddItemToGroup(const std::wstring& itemKey, const std::wstring& itemValue, int groupId);
	void DisplayPrivateIpAddress();
	std::wstring Utf8ToUtf16(const std::string& src);
private:
	enum class GROUP_ID
	{
		PRIVATE_IP = 0,
	};
public:
	afx_msg void OnLvnLinkClickedListIpinfo(NMHDR* pNMHDR, LRESULT* pResult);
};