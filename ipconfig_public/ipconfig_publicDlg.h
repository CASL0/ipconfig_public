
// ipconfig_publicDlg.h : ヘッダー ファイル
//

#pragma once


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
};
