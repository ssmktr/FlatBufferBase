
// MFCChatClientDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFCChatClient.h"
#include "MFCChatClientDlg.h"
#include "ConnectDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCChatClientDlg 대화 상자



CMFCChatClientDlg::CMFCChatClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCCHATCLIENT_DIALOG, pParent)
	, m_strEditChat(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCChatClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_CHAT, m_CtrlEditChat);
	DDX_Control(pDX, IDC_LIST_USER, m_ctrlListUser);
	DDX_Control(pDX, IDC_LIST_CHAT, m_ctrlListChat);
	DDX_Text(pDX, IDC_EDIT_CHAT, m_strEditChat);
	DDX_Control(pDX, IDC_BUTTON_SEND, m_ctrlButtonSend);
}

BEGIN_MESSAGE_MAP(CMFCChatClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CMFCChatClientDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CMFCChatClientDlg::OnBnClickedButtonExit)
END_MESSAGE_MAP()


// CMFCChatClientDlg 메시지 처리기

BOOL CMFCChatClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	if (!Start())
		return false;

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCChatClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCChatClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCChatClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


bool CMFCChatClientDlg::Start()
{
	ConnectDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		if (!m_Client.Create())
		{
			AfxMessageBox(_T("소켓 생성에 실패하였습니다!"));
			return false;
		}

		if (!m_Client.Connect(dlg.GetIpAddress(), dlg.GetPort()))
		{
			AfxMessageBox(_T("서버와 연결되지 않았습니다!"));
			return false;
		}

		flatbuffers::FlatBufferBuilder builder;
		{
			auto name = builder.CreateString(dlg.GetNick());
			auto data = fbs::CreateC2S_CONNECT_REQ(builder, name);
			auto root = fbs::CreateRoot(builder, fbs::Packet_C2S_CONNECT_REQ, data.Union());
			builder.Finish(root);
			m_Client.WritePacket(CMessage(builder.GetBufferPointer(), builder.GetSize()));
		}

		bConnect = true;
		m_CtrlEditChat.SetFocus();
		m_ctrlButtonSend.EnableWindow(TRUE);

		return true;
	}

	return false;
}

void CMFCChatClientDlg::OnBnClickedButtonSend()
{
	UpdateData(TRUE);

	flatbuffers::FlatBufferBuilder builder;
	auto message = builder.CreateString(m_strEditChat.GetString());
	auto data = fbs::CreateC2S_CHAT_REQ(builder, message);
	auto root = fbs::CreateRoot(builder, fbs::Packet_C2S_CHAT_REQ, data.Union());
	builder.Finish(root);

	m_Client.WritePacket(CMessage(builder.GetBufferPointer(), builder.GetSize()));

	m_strEditChat = _T("");
	m_CtrlEditChat.SetFocus();

	UpdateData(FALSE);
}


void CMFCChatClientDlg::OnBnClickedButtonExit()
{
	if (bConnect)
	{
		flatbuffers::FlatBufferBuilder builder;
		{
			auto data = fbs::CreateC2S_LEAVE_REQ(builder);
			auto root = fbs::CreateRoot(builder, fbs::Packet_C2S_LEAVE_REQ, data.Union());
			builder.Finish(root);
			m_Client.WritePacket(CMessage(builder.GetBufferPointer(), builder.GetSize()));
		}

		Sleep(500);	// 객체가 패킷보다 먼저 삭제되는것을 방지

		m_Client.ShutDown();
		m_Client.Close();
	}

	DestroyWindow();
}

void CMFCChatClientDlg::AddChat(const TCHAR* str)
{
	m_ctrlListChat.AddString(str);
	m_ctrlListChat.SetCurSel(m_ctrlListChat.GetCount() - 1);
}

void CMFCChatClientDlg::AddUser(const TCHAR* name)
{
	if (m_ctrlListUser.FindString(-1, name) == LB_ERR)
		m_ctrlListUser.AddString(name);

	m_ctrlListUser.SetCurSel(m_ctrlListUser.GetCount() - 1);
}

void CMFCChatClientDlg::DelUser(const TCHAR* name)
{
	m_ctrlListUser.DeleteString(m_ctrlListUser.FindString(-1, name));
	m_ctrlListUser.SetCurSel(m_ctrlListUser.GetCount() - 1);
}


BOOL CMFCChatClientDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && 
		pMsg->hwnd == GetDlgItem(IDC_EDIT_CHAT)->m_hWnd && 
		pMsg->wParam == VK_RETURN)
	{
		OnBnClickedButtonSend();
		return true;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

