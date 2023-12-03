
// CFILEIODlg.cpp: 구현 파일
#include "pch.h"
#include "framework.h"
#include "CFILEIO.h"
#include "CFILEIODlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCFILEIODlg 대화 상자
CCFILEIODlg::CCFILEIODlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CFILEIO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCFILEIODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_IMPORT01, m_strFilePath);
	DDX_Control(pDX, IDC_EDIT_IMPORT01, m_strFilePath);
	DDX_Control(pDX, IDC_EDIT_IMPORT02, m_strSelectedFileContent);
}

BEGIN_MESSAGE_MAP(CCFILEIODlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, &CCFILEIODlg::OnBnClickedButtonImport)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CCFILEIODlg::OnBnClickedButtonSave)
END_MESSAGE_MAP()


// CCFILEIODlg 메시지 처리기
BOOL CCFILEIODlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는 프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면 아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는 프레임워크에서 이 작업을 자동으로 수행합니다.

void CCFILEIODlg::OnPaint()
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

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서 이 함수를 호출합니다.
HCURSOR CCFILEIODlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCFILEIODlg::OnBnClickedButtonImport()
{
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, _T("All files (*.*)|*.*||"));

	if (dlg.DoModal() == IDOK)
	{
		CString strFilePath = dlg.GetPathName(); // 선택된 파일의 경로 가져오기

		CEdit* editCtrlPath = (CEdit*)GetDlgItem(IDC_EDIT_IMPORT01); // 연관된 텍스트 상자 업데이트 (IDC_EDIT_IMPORT01)
		if (editCtrlPath)
			editCtrlPath->SetWindowText(strFilePath);

		CEdit* editCtrlContent = (CEdit*)GetDlgItem(IDC_EDIT_IMPORT02); // 선택한 파일의 내용 읽고 표시하기
		if (editCtrlContent)
		{
			CStdioFile file;
			if (file.Open(strFilePath, CFile::modeRead | CFile::typeText | CFile::shareDenyNone))
			{
				CString strContent;
				CString strLine;

				while (file.ReadString(strLine))
					strContent += strLine + _T("\r\n"); // 줄 바꿈과 함께 각 줄을 추가합니다.

				editCtrlContent->SetWindowText(strContent);

				file.Close();
			}
		}
	}
}

void CCFILEIODlg::OnBnClickedButtonSave()
{
	CString strFilePath;
	CFileDialog dlg(FALSE, NULL, NULL, OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST, _T("All Files (*.*)|*.*||"));

	if (dlg.DoModal() == IDOK)													// 파일 대화상자 열기
	{
		strFilePath = dlg.GetPathName();										// 선택된 파일 경로 가져오기
		CFile file;																// 파일 쓰기를 시도
		if (file.Open(strFilePath, CFile::modeCreate | CFile::modeWrite))
		{
			CEdit* editCtrlContent = (CEdit*)GetDlgItem(IDC_EDIT_IMPORT02);		// 현재 텍스트 상자의 내용 가져오기 (IDC_EDIT_IMPORT02)
			if (editCtrlContent)
			{
				CString strContent;
				editCtrlContent->GetWindowText(strContent);
				CStringA narrowStrContent(strContent);							// 넓은 문자열을 좁은 문자열로 변환
				const char* pBuffer = narrowStrContent.GetBuffer();				// 내용을 바이너리 데이터로 변환
				UINT nLength = narrowStrContent.GetLength() * sizeof(char);
				file.Write(pBuffer, nLength); // 파일에 바이너리 데이터 쓰기
				file.Close();
				CString message = _T("The file was successfully saved."); // 성공 메시지 표시
				AfxMessageBox(message, MB_ICONINFORMATION);
				SetTimer(1, 3000, NULL); // 3초 후 메시지 상자를 자동으로 닫기 위한 타이머 설정
			}
		}
		else
			AfxMessageBox(_T("There was a problem saving the file."), MB_ICONERROR); // 파일 열기 실패 메시지
	}
}

void CCFILEIODlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)										// 타이머 ID가 메시지 박스를 닫기 위해 설정한 것과 일치하는지 확인
	{
		CWnd* pMsgBox = CWnd::FindWindow(NULL, _T("The file was saved successfully."));
		if (pMsgBox && pMsgBox->IsWindowVisible())			// 메시지 박스를 찾았고 여전히 표시 중인지 확인
			pMsgBox->PostMessage(WM_CLOSE, 0, 0);			// 메시지 박스를 닫음
		KillTimer(1);										// 타이머 중지
	}
	CDialogEx::OnTimer(nIDEvent);
}