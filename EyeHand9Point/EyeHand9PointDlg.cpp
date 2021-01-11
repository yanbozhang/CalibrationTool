
// EyeHand9PointDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "EyeHand9Point.h"
#include "EyeHand9PointDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CEyeHand9PointDlg 对话框



CEyeHand9PointDlg::CEyeHand9PointDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EYEHAND9POINT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEyeHand9PointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CEyeHand9PointDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_WIZFINISH, &CEyeHand9PointDlg::OnBnClickedWizfinish)
	ON_BN_CLICKED(IDC_CLICK, &CEyeHand9PointDlg::OnBnClickedClick)
	ON_BN_CLICKED(IDC_FINDCORNERS, &CEyeHand9PointDlg::OnBnClickedFindcorners)
	ON_BN_CLICKED(IDC_CALCULATE, &CEyeHand9PointDlg::OnBnClickedCalculate)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CEyeHand9PointDlg 消息处理程序

BOOL CEyeHand9PointDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	GetClientRect(&windows_rect);
	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	cfont.CreatePointFont(120, _T("宋体"), NULL);
	GetDlgItem(IDC_GROUP1)->SetFont(&cfont);
	GetDlgItem(IDC_GROUP2)->SetFont(&cfont);
	GetDlgItem(IDC_GROUP3)->SetFont(&cfont);
	GetDlgItem(IDC_GROUP4)->SetFont(&cfont);
	GetDlgItem(IDC_GROUP5)->SetFont(&cfont);
	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CEyeHand9PointDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CEyeHand9PointDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		if (!img.empty())
		{
			ShowImage(img, IDC_IMG);
		}
		if (!img_temp.empty())
		{
			ShowImage(img_temp, IDC_CORNERS);
		}	
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CEyeHand9PointDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CEyeHand9PointDlg::ShowImage(Mat& imgSrc, UINT ID)
{
	CRect rect;
	//获取图片控件矩形框
	GetDlgItem(ID)->GetClientRect(&rect);
	//缩放Mat，以适应图片控件大小
	resize(imgSrc, imgSrc, cv::Size(rect.Width(), rect.Height()));
	//cv::resize(imgSrc, imgSrc, cv::Size(300, 300));
	// 转换格式 ,便于获取BITMAPINFO
	switch (imgSrc.channels())
	{
	case 1:
		cv::cvtColor(imgSrc, imgSrc, COLOR_GRAY2BGRA); // GRAY单通道
		break;
	case 3:
		cv::cvtColor(imgSrc, imgSrc, COLOR_BGR2BGRA);// BGR三通道
		break;
	default:
		break;
	}
	// 计算一个像素多少个字节
	int pixelBytes = imgSrc.channels() * (imgSrc.depth() + 1);
	// 制作bitmapinfo(数据头)
	BITMAPINFO bitInfo;
	bitInfo.bmiHeader.biBitCount = 8 * pixelBytes;
	bitInfo.bmiHeader.biWidth = imgSrc.cols;
	bitInfo.bmiHeader.biHeight = -imgSrc.rows;//注意"-"号(正数时倒着绘制)
	bitInfo.bmiHeader.biPlanes = 1;
	bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo.bmiHeader.biCompression = BI_RGB;
	bitInfo.bmiHeader.biClrImportant = 0;
	bitInfo.bmiHeader.biClrUsed = 0;
	bitInfo.bmiHeader.biSizeImage = 0;
	bitInfo.bmiHeader.biXPelsPerMeter = 0;
	bitInfo.bmiHeader.biYPelsPerMeter = 0;
	CDC* pDC = GetDlgItem(ID)->GetDC();//获取图片控件DC
	//绘图
	::StretchDIBits(
		pDC->GetSafeHdc(),
		0, 0, rect.Width(), rect.Height(),
		0, 0, imgSrc.cols, imgSrc.rows,
		imgSrc.data,
		&bitInfo,
		DIB_RGB_COLORS,
		SRCCOPY
	);
	ReleaseDC(pDC);//释放DC
}

void CEyeHand9PointDlg::OnBnClickedWizfinish()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE);//TRUE为OPEN对话框，FALSE为SAVE AS对话框
	if (dlg.DoModal() == IDOK)
		FileName = dlg.GetPathName();
	SetDlgItemText(IDC_EDIT1, (LPCTSTR)FileName);
}




void CEyeHand9PointDlg::OnBnClickedClick()
{
	// TODO: 在此添加控件通知处理程序代码
	char szStr[256] = {};
	wcstombs(szStr, FileName, FileName.GetLength());//将宽字符转换成多字符  
	const char* pBuf = szStr;
	img = imread(szStr);
	imgcopy = img.clone();
	ShowImage(img,IDC_IMG);
	char tmp[1024];
	numlog = 1;
	sprintf(tmp, "运行日志列表如下：\r\n");
	strcpy(strlog, tmp);
	sprintf(tmp, "%d.加载图片成功\r\n", numlog);
	numlog++;
	strcat(strlog, tmp);
	SetDlgItemText(IDC_EDITLOG, CA2CT(strlog));
}



void CEyeHand9PointDlg::OnBnClickedFindcorners()
{
	// TODO: 在此添加控件通知处理程序代码
	int CornW,CornH;
	CornW = GetDlgItemInt(IDC_EDIT2);
	CornH = GetDlgItemInt(IDC_EDIT3);
	char tmp[1024];
	if (CornW == NULL || CornH == NULL)
	{
		sprintf(tmp, "%d.角点数量输入失败\r\n", numlog);
	}
	else
		sprintf(tmp, "%d.角点数量输入成功\r\n", numlog);
	numlog++;
	strcat(strlog, tmp);
	SetDlgItemText(IDC_EDITLOG, CA2CT(strlog));
	Size board_size = Size(CornW, CornH);
	Mat gray;
	if (!imgcopy.empty())
	{
		cvtColor(imgcopy, gray, COLOR_BGR2GRAY);
		bool patternfound = findChessboardCorners(gray, board_size, corners, CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE + CALIB_CB_FAST_CHECK);
		if (!patternfound)
		{
			char tmp[1024];
			sprintf(tmp, "%d.角点检测失败\r\n", numlog);
			numlog++;
			strcat(strlog, tmp);
			SetDlgItemText(IDC_EDITLOG, CA2CT(strlog));
			return;
		}
		else
		{
			char tmp[1024];
			sprintf(tmp, "%d.角点检测成功\r\n", numlog);
			numlog++;
			strcat(strlog, tmp);
			SetDlgItemText(IDC_EDITLOG, CA2CT(strlog));
			cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1), TermCriteria(TermCriteria::EPS | TermCriteria::Type::MAX_ITER, 30, 0.001));
			img_temp = imgcopy.clone();
			xstep = CornW / 3;
			ystep = CornH / 3;
			PointCount = 0;
			CalibrationPoint.clear();
			for (int i = 0; i < corners.size(); i++)
			{
				if ((CornH % 3) == 0)
				{
					if ((((i % CornW) % (xstep + 1)) == 0) && (((i / CornW) % ystep) == 0))
					{
						circle(img_temp, corners[i], 2, Scalar(0, 0, 255), 15, 8, 0);
						CalibrationPoint.insert(pair<int,int>(PointCount,i));
						PointCount++;
					}				
					else
					circle(img_temp, corners[i], 2, Scalar(255, 0, 0), 11, 8, 0);
				}
				else if ((CornH % 3) != 0)
				{
					if ((((i % CornW) % (xstep + 1)) == 0) && (((i / CornW) % (ystep + 1)) == 0))
					{
						circle(img_temp, corners[i], 2, Scalar(0, 0, 255), 15, 8, 0);
						CalibrationPoint.insert(pair<int, int>(PointCount, i));
						PointCount++;
					}
					else
				    circle(img_temp, corners[i], 2, Scalar(255, 0, 0), 11, 8, 0);
				}				
			}
			sprintf(tmp, "%d.角点标注成功\r\n", numlog);
			numlog++;
			strcat(strlog, tmp);
			SetDlgItemText(IDC_EDITLOG, CA2CT(strlog));
			ShowImage(img_temp, IDC_CORNERS);
		}
	}
	
}


void CEyeHand9PointDlg::OnBnClickedCalculate()
{
	// TODO: 在此添加控件通知处理程序代码
	static int Edit_id[] = { IDC_POINT1,IDC_POINT2,IDC_POINT3,IDC_POINT4,IDC_POINT5,IDC_POINT6,IDC_POINT7,IDC_POINT8,IDC_POINT9 };
	CString str;
	char tmp[1024];
	//MessageBox(str);
	points_camera.clear();
	points_robot.clear();
	if (PointCount != 9)
	{
		sprintf(tmp, "%d.九点数量提取错误,提取角点数量为：%d\r\n", numlog, PointCount);
		numlog++;
		strcat(strlog, tmp);
		SetDlgItemText(IDC_EDITLOG, CA2CT(strlog));
		return;
	}
	else
	{
		sprintf(tmp, "%d.九点数量提取正确\r\n", numlog);
		numlog++;
		strcat(strlog, tmp);
		SetDlgItemText(IDC_EDITLOG, CA2CT(strlog));
	}
	for (int Cornerpoint = 0; Cornerpoint < 9; Cornerpoint++)
	{		
		GetDlgItemText(Edit_id[Cornerpoint],str);
		PointIter = CalibrationPoint.find(Cornerpoint);
		if (PointIter != CalibrationPoint.end())
		{
			int tmpPoint = PointIter->second;
			points_camera.push_back(corners[tmpPoint]);
		}
		int i = 0;
		double tmpx = 0.0, tmpy = 0.0;
		vector<string> stu;
		string ss = CW2A(str.GetString());
		stringstream sstr(ss);
		string token;
		int be = 0;
		while (getline(sstr, token, ','))
		{
			stu.push_back(token);
			if (i == 0)
				tmpx = atof(token.c_str());
			else if (i == 1)
				tmpy = atof(token.c_str());
			else break;
			i++;
		}
		points_robot.push_back(Point2f(tmpx, tmpy));
	}
	double tmpoutvalue = 1.0;
	bool robotFlag;
	//判断机器人九点输入坐标是否输入正确
	for (int i = 0; i < 9; i++)
	{
		tmpoutvalue = tmpoutvalue * points_robot[i].x * points_robot[i].y;
	}
	if (tmpoutvalue == 0) robotFlag = false;
	else robotFlag = true;
	if (robotFlag)
	{
		sprintf(tmp, "%d.角点图像坐标与机器人坐标提取完成\r\n", numlog);
		numlog++;
		strcat(strlog, tmp);
		SetDlgItemText(IDC_EDITLOG, CA2CT(strlog));
	}
	else
	{
		sprintf(tmp, "%d.角点图像坐标与机器人坐标提取失败\r\n", numlog);
		numlog++;
		strcat(strlog, tmp);
		SetDlgItemText(IDC_EDITLOG, CA2CT(strlog));
		return;
	}		
	sprintf(tmp, "%d.开始计算转换矩阵\r\n", numlog);
	numlog++;
	strcat(strlog, tmp);
	SetDlgItemText(IDC_EDITLOG, CA2CT(strlog));
	Mat warpMat = estimateRigidTransform(points_camera, points_robot, true);
	char strout[1024],tmpout[1024];
	if (warpMat.empty())
	{
		sprintf(tmp, "%d.转换矩阵计算失败\r\n", numlog);	
		numlog++;
		strcat(strlog, tmp);
		SetDlgItemText(IDC_EDITLOG, CA2CT(strlog));
		return;
	}
	else
	{
		sprintf(tmp, "%d.转换矩阵计算成功\r\n", numlog);
		numlog++;
		strcat(strlog, tmp);
		SetDlgItemText(IDC_EDITLOG, CA2CT(strlog));
	}		
	double A = warpMat.ptr<double>(0)[0];
	double B = warpMat.ptr<double>(0)[1];
	double C = warpMat.ptr<double>(0)[2];
	double D = warpMat.ptr<double>(1)[0];
	double E = warpMat.ptr<double>(1)[1];
	double F = warpMat.ptr<double>(1)[2];
	sprintf(strout,"A:%f\r\nB:%f\r\nC:%f\r\nD:%f\r\nE:%f\r\nF:%f\r\n",A,B,C,D,E,F);
	SetDlgItemText(IDC_EDITOUT, CA2CT(strout));
	sprintf(tmpout, "%f\n%f\n%f\n%f\n%f\n%f\n", A, B, C, D, E, F);
	sprintf(tmp, "%d.转换矩阵开始写入txt\r\n", numlog);
	numlog++;
	strcat(strlog, tmp);
	SetDlgItemText(IDC_EDITLOG, CA2CT(strlog));
	FILE* fp;
	fp = fopen("outdata.txt", "w");
	fprintf(fp, "%s", tmpout);
	fclose(fp);
	sprintf(tmp, "%d.转换矩阵写入txt成功\r\n", numlog);
	numlog++;
	strcat(strlog, tmp);
	SetDlgItemText(IDC_EDITLOG, CA2CT(strlog));
}

void CEyeHand9PointDlg::ChangeSize(CWnd* pWnd, int cx, int cy)
{
	if (pWnd)  //判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建	
	{
		CRect rect;   //获取控件变化前的大小  
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标
		//    cx/m_rect.Width()为对话框在横向的变化比例
		rect.left = rect.left * cx / windows_rect.Width();//调整控件大小
		rect.right = rect.right * cx / windows_rect.Width();
		rect.top = rect.top * cy / windows_rect.Height();
		rect.bottom = rect.bottom * cy / windows_rect.Height();
		pWnd->MoveWindow(rect);//设置控件大小
	}
}


void CEyeHand9PointDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (nType == 1) return;//最小化则什么都不做
	// TODO: Add your message handler code here
	CWnd* pWnd;
	pWnd = GetDlgItem(IDC_IMG);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(ID_WIZFINISH); 
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_EDIT1);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_CORNERS);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC1);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC2);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_EDIT2);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_EDIT3);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_FINDCORNERS);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC3);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC4);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC5);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC6);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC7);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC01);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC02);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC03);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC04);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC05);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC06);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC07);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC08);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC09);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_POINT1);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_POINT2);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_POINT3);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_POINT4);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_POINT5);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_POINT6);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_POINT7);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_POINT8);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_POINT9);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_EDITOUT);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_CALCULATE);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_CLICK);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_EDITLOG);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_GROUP1);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_GROUP2);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_GROUP3);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_GROUP4);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_GROUP5);
	ChangeSize(pWnd, cx, cy);
	GetClientRect(&windows_rect);// 将变化后的对话框大小设为旧大小
	Invalidate();
	UpdateWindow();
	// TODO: 在此处添加消息处理程序代码
}
