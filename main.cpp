#include "stdafx.h"
//------------------------------------------------------------------------------
#include "cdocument_main.h"
#include "clistview_main.h"
#include "cframewnd_main.h"

//------------------------------------------------------------------------------
class CWinApp_Main:public CWinApp
{
 protected:
  //-���������� ������-------------------------------------------------------
  //-������� ������----------------------------------------------------------
  //-������------------------------------------------------------------------
 public:
  //-����������� ������------------------------------------------------------
  CWinApp_Main(void);
  //-���������� ������-------------------------------------------------------
  ~CWinApp_Main();
  //-���������� ������-------------------------------------------------------
  //-������� ������----------------------------------------------------------
  virtual BOOL InitInstance(void);
  int ExitInstance(void);
  //-������------------------------------------------------------------------
};
//-����������� ������--------------------------------------------------------
CWinApp_Main::CWinApp_Main(void)
{
}
//-���������� ������---------------------------------------------------------
CWinApp_Main::~CWinApp_Main()
{
}
//-������� ������------------------------------------------------------------
BOOL CWinApp_Main::InitInstance(void)
{ 
 CWinApp::InitInstance();

 char FileName[MAX_PATH];
 GetModuleFileName(NULL,FileName,MAX_PATH);
 //���������� �� �����
 long size=strlen(FileName);
 if (size>0) size--;
 while(size>0)
 {
  unsigned char s=FileName[size];
  if (s==0 || s=='\\')
  {
   FileName[size+1]=0;
   break;
  }
  size--;
 }
 SetCurrentDirectory(FileName);
 CSingleDocTemplate* pDocTemplate;//����� ������ 
 pDocTemplate=new CSingleDocTemplate(NULL,RUNTIME_CLASS(CDocument_Main),RUNTIME_CLASS(CFrameWnd_Main),RUNTIME_CLASS(CListView_Main));
 if (pDocTemplate==NULL)
 {
  MessageBox(NULL,"�� ���� ������� ������!","������",MB_OK);
  return(false);
 }
 AddDocTemplate(pDocTemplate);//�������� ������ 
 CCommandLineInfo cmdInfo;//����� ������ 
 ParseCommandLine(cmdInfo);//������ ��������� ������ 
 if (!ProcessShellCommand(cmdInfo)) return(FALSE);//������ ������ �� ���������� 
 m_pMainWnd->SetWindowText("DV Cassete Streamer v1.0");
 m_pMainWnd->ShowWindow(SW_SHOW);//�������� ���� 
 m_pMainWnd->UpdateWindow();//��������� ���� ��������� ��������� 
 return(TRUE);
}
int CWinApp_Main::ExitInstance(void)
{
 return(CWinApp::ExitInstance());
}

CWinApp_Main cWinApp_Main;


