#include "stdafx.h"
//------------------------------------------------------------------------------
#include "cdocument_main.h"
#include "clistview_main.h"
#include "cframewnd_main.h"

//------------------------------------------------------------------------------
class CWinApp_Main:public CWinApp
{
 protected:
  //-Переменные класса-------------------------------------------------------
  //-Функции класса----------------------------------------------------------
  //-Прочее------------------------------------------------------------------
 public:
  //-Конструктор класса------------------------------------------------------
  CWinApp_Main(void);
  //-Деструктор класса-------------------------------------------------------
  ~CWinApp_Main();
  //-Переменные класса-------------------------------------------------------
  //-Функции класса----------------------------------------------------------
  virtual BOOL InitInstance(void);
  int ExitInstance(void);
  //-Прочее------------------------------------------------------------------
};
//-Конструктор класса--------------------------------------------------------
CWinApp_Main::CWinApp_Main(void)
{
}
//-Деструктор класса---------------------------------------------------------
CWinApp_Main::~CWinApp_Main()
{
}
//-Функции класса------------------------------------------------------------
BOOL CWinApp_Main::InitInstance(void)
{ 
 CWinApp::InitInstance();

 char FileName[MAX_PATH];
 GetModuleFileName(NULL,FileName,MAX_PATH);
 //отматываем до черты
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
 CSingleDocTemplate* pDocTemplate;//новый шаблон 
 pDocTemplate=new CSingleDocTemplate(NULL,RUNTIME_CLASS(CDocument_Main),RUNTIME_CLASS(CFrameWnd_Main),RUNTIME_CLASS(CListView_Main));
 if (pDocTemplate==NULL)
 {
  MessageBox(NULL,"Не могу создать шаблон!","Ошибка",MB_OK);
  return(false);
 }
 AddDocTemplate(pDocTemplate);//добавить шаблон 
 CCommandLineInfo cmdInfo;//класс команд 
 ParseCommandLine(cmdInfo);//разбор командной строки 
 if (!ProcessShellCommand(cmdInfo)) return(FALSE);//запуск команд на выполнение 
 m_pMainWnd->SetWindowText("DV Cassete Streamer v1.0");
 m_pMainWnd->ShowWindow(SW_SHOW);//показать окно 
 m_pMainWnd->UpdateWindow();//запустить цикл обработки сообщений 
 return(TRUE);
}
int CWinApp_Main::ExitInstance(void)
{
 return(CWinApp::ExitInstance());
}

CWinApp_Main cWinApp_Main;


