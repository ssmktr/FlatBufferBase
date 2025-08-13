#pragma once


// ConnectDlg 대화 상자

class ConnectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ConnectDlg)

public:
	ConnectDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~ConnectDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CONNECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	const TCHAR* GetIpAddress() { return strIPAddr.GetString(); }
	const int GetPort() { return port; }
	const TCHAR* GetNick() { return Nick.GetString(); }

private:
	void getIpAddr();
	void getNick();
	void getPort();

public:
	CIPAddressCtrl m_ctrlIpaddr;
	CEdit m_ctrlPort;
	CEdit m_ctrlNick;

	int port = 0;
	CString Nick;
	CString strIPAddr;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
