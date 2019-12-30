
#define EX 1
#define OH 2
class CMyApp: public CWinApp
{

public:
	virtual BOOL InitInstance();
};
class CMainWinddow:public CWnd
{

protected:

	static const CRect m_rcSquares[9];
	int m_nGameGrid[9];
	int m_nNextChar;
	//************************************
	// Method:    DrawBoard
	// FullName:  CMainWinddow::DrawBoard
	// Access:    protected 
	// Returns:   void
	// Qualifier:
	// Parameter: CDC * pDC
	//************************************


	void DrawBoard(CDC* pDC);
	void DrawX(CDC* pDC,int nPos);
	void DrawO(CDC* pDC,int nPos);
	void CheckForGameOver();
public:
	CMainWinddow();

protected:
	virtual void PostNcDestroy();
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
protected:
	int GetRectID(CPoint piont);
	int IsWinner(void);
	BOOL IsDraw(void);

	void ResetGame(void);
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
