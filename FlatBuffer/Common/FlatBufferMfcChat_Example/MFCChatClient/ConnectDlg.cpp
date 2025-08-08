// ConnectDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MFCChatClient.h"
#include "ConnectDlg.h"
#include "afxdialogex.h"


// ConnectDlg 대화 상자

IMPLEMENT_DYNAMIC(ConnectDlg, CDialogEx)

ConnectDlg::ConnectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CONNECT, pParent)
{

}

ConnectDlg::~ConnectDlg()
{
}

void ConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS, m_ctrlIpaddr);
	DDX_Control(pDX, IDC_EDIT_PORT, m_ctrlPort);
	DDX_Control(pDX, IDC_EDIT_NICK, m_ctrlNick);
}

BEGIN_MESSAGE_MAP(ConnectDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &ConnectDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// ConnectDlg 메시지 처리기
BOOL ConnectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ctrlIpaddr.SetAddress(127, 0, 0, 1);
	m_ctrlPort.SetWindowTextA(_T("31400"));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void ConnectDlg::OnBnClickedOk()
{
	getIpAddr();
	getPort();
	getNick();
	
	CDialogEx::OnOK();
}

void ConnectDlg::getIpAddr()
{
	BYTE ipFirst, ipSecond, ipThird, ipForth;
	m_ctrlIpaddr.GetAddress(ipFirst, ipSecond, ipThird, ipForth);
	strIPAddr.Format(_T("%d.%d.%d.%d"), ipFirst, ipSecond, ipThird, ipForth);
}

void ConnectDlg::getNick()
{
	m_ctrlNick.GetWindowTextA(Nick);
}

void ConnectDlg::getPort()
{
	TCHAR buf[10] = { 0, };
	m_ctrlPort.GetWindowText(buf, 10);
	port = _ttoi(buf);
}

