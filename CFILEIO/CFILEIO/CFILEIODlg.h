// CFILEIODlg.h: 헤더 파일
#pragma once

// CCFILEIODlg 대화 상자
class CCFILEIODlg : public CDialogEx
{
// 생성입니다.
public:
	CCFILEIODlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CFILEIO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	// 파일 경로 불러오는 값
	afx_msg void OnBnClickedButtonImport();
	CEdit m_strFilePath;

	// // 선택된 파일 내용
	CEdit m_strSelectedFileContent;
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
