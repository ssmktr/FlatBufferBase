
// MFCChatClientDlg.h: 헤더 파일
//
#include "ChatClient.h"

#pragma once


// CMFCChatClientDlg 대화 상자
class CMFCChatClientDlg : public CDialogEx
{
// 생성입니다.
public:
	CMFCChatClientDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCCHATCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_CtrlEditChat;
	CListBox m_ctrlListUser;
	CListBox m_ctrlListChat;
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedButtonExit();

public:
	bool Start();
	void AddChat(const TCHAR* str);
	void AddUser(const TCHAR* name);
	void DelUser(const TCHAR* name);

public:
	CClientSocket m_Client;
	CString m_strEditChat;
	bool bConnect = false;
	CButton m_ctrlButtonConnect;
	CButton m_ctrlButtonSend;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
