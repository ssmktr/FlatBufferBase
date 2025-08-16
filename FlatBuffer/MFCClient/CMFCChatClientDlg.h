#pragma once
#include "afxdialogex.h"
#include "ClientSocket.h"


// CMFCChatClientDlg 대화 상자

class CMFCChatClientDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMFCChatClientDlg)

public:
	CMFCChatClientDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMFCChatClientDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CHAT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CListBox m_ctrlListChat;
	CListBox m_ctrlListUser;
	CEdit m_ctrlEditChat;
	afx_msg void OnBnClickedSend();
	afx_msg void OnBnClickedFinish();

public:
	bool Start();
	void AddChat(const TCHAR* str);
	void AddUser(const TCHAR* name);
	void DelUser(const TCHAR* name);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	ClientSocket m_Client;
	CString m_strEditChat;
	bool bConnect = false;
	CButton m_ctrlButtonConnect;
	CButton m_ctrlButtonSend;
};
