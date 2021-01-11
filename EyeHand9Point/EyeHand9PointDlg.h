
// EyeHand9PointDlg.h: 头文件
//
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
#pragma once


// CEyeHand9PointDlg 对话框
class CEyeHand9PointDlg : public CDialogEx
{
// 构造
public:
	CEyeHand9PointDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EYEHAND9POINT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedWizfinish();
	void ShowImage(Mat& imgSrc, UINT ID);
	CString FileName;//加载图像路径
	afx_msg void OnBnClickedClick();
	afx_msg void OnBnClickedFindcorners();
	Mat img,imgcopy,img_temp;//加载的图像
	vector<Point2f>corners;
	vector<Point2f>points_camera;
	vector<Point2f>points_robot;
	int xstep,ystep;
	afx_msg void OnBnClickedCalculate();
	char strlog[4096];
	int numlog;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void ChangeSize(CWnd* pWnd, int cx, int cy);
	CRect windows_rect;//初始化窗体大小
	CFont cfont;//初始化字体
	map<int, int>CalibrationPoint;
	map<int, int>::iterator PointIter;
	int PointCount;
};
