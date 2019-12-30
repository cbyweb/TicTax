#include <afxwin.h>
#include "TicTax.h"
CMyApp app;

const CRect CMainWinddow::m_rcSquares[9]={

	CRect(16,16,112,112),
	CRect(128,16,224,112),
	CRect(240,16,336,112),
	CRect(16,128,112,224),
	CRect(128,128,224,224),
	CRect(240,128,336,224),
	CRect(16,240,112,336),
	CRect(128,240,224,336),
	CRect(240,240,336,336)
};
BOOL CMyApp::InitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	this->m_pMainWnd = new CMainWinddow;
	this->m_pMainWnd->ShowWindow(this->m_nCmdShow);
	this->m_pMainWnd->UpdateWindow();
	return TRUE;
	//return CWinApp::InitInstance();
}


CMainWinddow::CMainWinddow()//构造函数
{
	/*for (int i=0;i<9;++i)
	{
		m_ngamegrid[i] = 0;
	}
*/
	::ZeroMemory(m_nGameGrid,9*sizeof(int));
	m_nNextChar = EX;
	//1.注册窗口类
	CString  strWndClass = AfxRegisterWndClass(CS_DBLCLKS,
		AfxGetApp()->LoadStandardCursor(IDC_ARROW),
		(HBRUSH)(COLOR_3DFACE+1),
		AfxGetApp()->LoadStandardIcon(IDI_WINLOGO));

	//2.创建窗口类
	CreateEx(0,strWndClass,_T("Tic-tac-toe"),WS_OVERLAPPED|WS_SYSMENU|WS_CAPTION|WS_MINIMIZEBOX,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,NULL,NULL);
	CRect rect(0,0,352,352);
	CalcWindowRect(&rect);
	
	SetWindowPos(NULL,0,0,rect.Width(),rect.Height(),SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);
}

void CMainWinddow::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类

	delete this;

	CWnd::PostNcDestroy();
}
BEGIN_MESSAGE_MAP(CMainWinddow, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


void CMainWinddow::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()
	DrawBoard(&dc);
}
void CMainWinddow::DrawBoard(CDC* pDC)
{
	CPen pen(PS_SOLID, 16,	RGB(0,0,0));
	CPen* pOlder = pDC->SelectObject(&pen);

	pDC->MoveTo(120,16);
	pDC->LineTo(120,336);

	pDC->MoveTo(232,16);
	pDC->LineTo(232,336);

	pDC->MoveTo(16,120);
	pDC->LineTo(336,120);

	pDC->MoveTo(16,232);
	pDC->LineTo(336,232);

	for (int i=0;i<9;++i)
	{
		if (m_nGameGrid[i] == EX)
		{
			DrawX(pDC,i);
		}else if (m_nGameGrid[i] == OH)
		{
			DrawO(pDC,i);
		}
	}
	
	pDC->SelectObject(pOlder);

}

void CMainWinddow::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if(m_nNextChar!=EX)
		return;

	int nPos = GetRectID(point);
	if (nPos == -1||m_nGameGrid[nPos]!=0)
	{
		return;
	}
	m_nGameGrid[nPos] = EX;
	m_nNextChar = OH;
	CClientDC dc(this);
	DrawX(&dc,nPos);

	CheckForGameOver();

	CWnd::OnLButtonDown(nFlags, point);
}


void CMainWinddow::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_nNextChar != OH)
		return;

	int nPos = GetRectID(point);
	if (nPos == -1||m_nGameGrid[nPos]!=0)
	{
		return;
	}
	m_nGameGrid[nPos] = OH;
	m_nNextChar = EX;
	CClientDC dc(this);
	DrawO(&dc,nPos);

	CheckForGameOver();

	CWnd::OnRButtonDown(nFlags, point);
}
void CMainWinddow::DrawX(CDC* pDC,int nPos)
{

	CPen pen(PS_SOLID,16,RGB(255,0,0));
	CPen* pOlder = pDC->SelectObject(&pen);

	CRect rect = m_rcSquares[nPos];
	//pDC->Rectangle(&rect);
	rect.DeflateRect(16,16);
	pDC->MoveTo(rect.left,rect.top);
	pDC->LineTo(rect.right,rect.bottom);
	pDC->MoveTo(rect.left,rect.bottom);
	pDC->LineTo(rect.right,rect.top);

	pDC->SelectObject(pOlder);
}
void CMainWinddow::DrawO(CDC* pDC,int nPos)
{
	CPen pen(PS_SOLID,16,RGB(0,0,255));
	CPen* pOldpen = pDC->SelectObject(&pen);
	pDC->SelectStockObject(NULL);

	CRect rect = m_rcSquares[nPos];
	rect.DeflateRect(16,16);
	pDC->Ellipse(&rect);
	
	pDC->SelectObject(pOldpen);
}

int CMainWinddow::GetRectID(CPoint point)
{

	for(int i=0;i<9;i++)
	{
		if(m_rcSquares[i].PtInRect(point))
		{
			return i;
		}
	}
	return -1;
}
void CMainWinddow::CheckForGameOver()
{
	int nWinner;

	if (nWinner = IsWinner())
	{
		CString string =(nWinner == EX) ? _T("X wins!") : _T("O wins!");
		MessageBox(string,_T("Game Over"),MB_ICONEXCLAMATION|MB_OK);
		ResetGame();
	}else if (IsDraw())
	{
		MessageBox(_T("It's a draw!"),_T("Game Over"),MB_ICONEXCLAMATION|MB_OK);
		ResetGame();

	}
}

int CMainWinddow::IsWinner(void)
{
	static int nPattern[8][3]=
	{
		0,1,2,
		3,4,5,
		6,7,8,
		0,3,6,
		1,4,7,
		2,5,8,
		0,4,8,
		2,4,6
	};
	for(int i=0;i<8;++i)
	{
		if ((m_nGameGrid[nPattern[i][0]] == EX)&&
			(m_nGameGrid[nPattern[i][1]] == EX)&&
			(m_nGameGrid[nPattern[i][2]] == EX))
		{
			return EX;
		}
		if ((m_nGameGrid[nPattern[i][0]] == OH)&&
			(m_nGameGrid[nPattern[i][1]] == OH)&&
			(m_nGameGrid[nPattern[i][2]] == OH))
		{
			return OH;
		}
	}
	return 0;
}


BOOL CMainWinddow::IsDraw(void)
{
	for (int i=0 ; i<9; ++i)
	{
		if (m_nGameGrid[i] == 0)
		{
			return FALSE;
		}
	}
	return TRUE;
}


void CMainWinddow::ResetGame(void)
{
	m_nNextChar = EX;
	::ZeroMemory(m_nGameGrid,9*sizeof(int));
	Invalidate();
}


void CMainWinddow::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CClientDC dc(this);
	if (dc.GetPixel(point) == RGB(0,0,0))
	{
		ResetGame();
	}

	CWnd::OnLButtonDblClk(nFlags, point);
}
