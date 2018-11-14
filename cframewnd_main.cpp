#include "cframewnd_main.h"

//====================================================================================================
//карта сообщений класса
//====================================================================================================
BEGIN_MESSAGE_MAP(CFrameWnd_Main,CFrameWnd)
 ON_WM_CREATE()
 ON_WM_TIMER()
 ON_WM_DESTROY()
 ON_COMMAND(IDC_TOOLBAR_MAIN_INSERT,OnCommand_ToolBar_Main_Insert)
 ON_COMMAND(IDC_TOOLBAR_MAIN_EXTRACT,OnCommand_ToolBar_Main_Extract)
 ON_COMMAND(IDC_TOOLBAR_MAIN_STOP,OnCommand_ToolBar_Main_Stop)
 ON_COMMAND(IDC_TOOLBAR_MAIN_VERIFY,OnCommand_ToolBar_Main_Verify)
 ON_COMMAND(IDC_TOOLBAR_MAIN_CLEAR,OnCommand_ToolBar_Main_Clear)
END_MESSAGE_MAP()
//====================================================================================================
//конструктор класса
//====================================================================================================
CFrameWnd_Main::CFrameWnd_Main(void)
{
 char path[MAX_PATH];
 GetCurrentDirectory(MAX_PATH,path);
 sInsertSettings.DVFileName=path;
 sInsertSettings.DVFileName+="\\output.dv";
 sInsertSettings.GlobalPrefix=200;
 sInsertSettings.LocalPrefix=0;
 sInsertSettings.LogoFileName=path;
 sInsertSettings.LogoFileName+="\\minilogo.tga";
 sInsertSettings.Path=path;
 sInsertSettings.Path+="\\";

 sExtractSettings.DVFileName=path;
 sExtractSettings.DVFileName+="\\output.dv";
 sExtractSettings.Path=path;
 sExtractSettings.Path+="\\";

 LastProcessingState=true;
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CFrameWnd_Main::~CFrameWnd_Main()
{
}
//====================================================================================================
//закрытые функции класса
//====================================================================================================

//====================================================================================================
//открытые функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//создание рамки
//----------------------------------------------------------------------------------------------------
afx_msg int CFrameWnd_Main::OnCreate(LPCREATESTRUCT lpCreateStruct)
{ 
 //установим иконку
 hIcon=LoadIcon(AfxGetResourceHandle(),(LPCTSTR)IDI_ICON_MAIN);
 SetIcon(hIcon,TRUE);

 //создадим панель инструментов
 cToolBar_Main.Create(this,WS_CHILD|WS_VISIBLE|CBRS_SIZE_FIXED|CBRS_TOP|CBRS_TOOLTIPS);
 //загрузим картинки для панели

 //используем BITMAP, чтобы сделать 24 бита RGB в панели инструментов, вместо 16 цветов при обычном создании из ресурса
 HBITMAP hBitmap=(HBITMAP)LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_TOOLBAR_MAIN),IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_LOADMAP3DCOLORS); 
 cToolBar_Main.SetBitmap(hBitmap);

 UINT ButtonIndex[TOOLBAR_MAIN_BUTTON_AMOUNT];
 ButtonIndex[0]=IDC_TOOLBAR_MAIN_INSERT;
 ButtonIndex[1]=0;
 ButtonIndex[2]=IDC_TOOLBAR_MAIN_EXTRACT;
 ButtonIndex[3]=0;
 ButtonIndex[4]=IDC_TOOLBAR_MAIN_VERIFY;
 ButtonIndex[5]=0;
 ButtonIndex[6]=0;
 ButtonIndex[7]=IDC_TOOLBAR_MAIN_STOP;
 ButtonIndex[8]=0;
 ButtonIndex[9]=0;
 ButtonIndex[10]=IDC_TOOLBAR_MAIN_CLEAR;
 
 cToolBar_Main.SetButtons(ButtonIndex,TOOLBAR_MAIN_BUTTON_AMOUNT);
 
 //настроим кнопки панели
 for(uint32_t n=0;n<TOOLBAR_MAIN_BUTTON_AMOUNT;n++) cToolBar_Main.SetButtonStyle(n,TBBS_BUTTON);

 cToolBar_Main.SetButtonInfo(1,ID_SEPARATOR,TBBS_SEPARATOR,24);
 cToolBar_Main.SetButtonInfo(3,ID_SEPARATOR,TBBS_SEPARATOR,24);
 cToolBar_Main.SetButtonInfo(5,ID_SEPARATOR,TBBS_SEPARATOR,24); 
 cToolBar_Main.SetButtonInfo(6,ID_SEPARATOR,TBBS_SEPARATOR,24);
 cToolBar_Main.SetButtonInfo(8,ID_SEPARATOR,TBBS_SEPARATOR,24); 
 cToolBar_Main.SetButtonInfo(9,ID_SEPARATOR,TBBS_SEPARATOR,24);

 //название панели
 cToolBar_Main.SetWindowText("Главная паналь управления");
 //устанавливаем новые размеры кнопок
 SIZE Size_Button;
 Size_Button.cx=40;
 Size_Button.cy=40;
 SIZE Size_Image;
 Size_Image.cx=32;
 Size_Image.cy=32;
 cToolBar_Main.SetSizes(Size_Button,Size_Image);
 cToolBar_Main.EnableDocking(CBRS_ALIGN_ANY); 
 
 //запускаем таймер
 SetTimer(ID_TIMER_MAIN,TIMER_PERIOD_MS,NULL); 
 int ret=CFrameWnd::OnCreate(lpCreateStruct);
 return(ret);
}
//----------------------------------------------------------------------------------------------------
//уничтожение рамки
//----------------------------------------------------------------------------------------------------
afx_msg void CFrameWnd_Main::OnDestroy(void)
{
 DestroyIcon(hIcon);
 KillTimer(ID_TIMER_MAIN);
}
//----------------------------------------------------------------------------------------------------
//событие таймера
//----------------------------------------------------------------------------------------------------
afx_msg void CFrameWnd_Main::OnTimer(UINT nIDEvent)
{
 if (nIDEvent==ID_TIMER_MAIN)
 { 
  CDocument_Main *cDocument_Main_Ptr=(CDocument_Main*)GetActiveDocument();  
  if (cDocument_Main_Ptr!=NULL) 
  {
   cDocument_Main_Ptr->OnTimer();
   bool state=cDocument_Main_Ptr->IsProcessing();
   if (state!=LastProcessingState)
   {
    if (state==true)
    {	
     cToolBar_Main.SetButtonStyle(0,TBBS_DROPDOWN|TBBS_MARKED);
     cToolBar_Main.SetButtonStyle(2,TBBS_DROPDOWN|TBBS_MARKED);
     cToolBar_Main.SetButtonStyle(4,TBBS_DROPDOWN|TBBS_MARKED);
     cToolBar_Main.SetButtonStyle(7,TBBS_BUTTON);
    }
    else
    {
     cToolBar_Main.SetButtonStyle(0,TBBS_BUTTON);
     cToolBar_Main.SetButtonStyle(2,TBBS_BUTTON);
     cToolBar_Main.SetButtonStyle(4,TBBS_BUTTON);
     cToolBar_Main.SetButtonStyle(7,TBBS_DROPDOWN|TBBS_MARKED);
    }	
   }
   LastProcessingState=state;
  }
 }
 CFrameWnd::OnTimer(nIDEvent);
}

//====================================================================================================
//функции обработки сообщений
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//нажата кнопка "Собрать файлы в DV-видеофайл" главной панели инструментов 
//----------------------------------------------------------------------------------------------------
afx_msg void CFrameWnd_Main::OnCommand_ToolBar_Main_Insert(void)
{
 CDialog_Insert cDialog_Insert((LPSTR)IDD_DIALOG_INSERT,this);
 if (cDialog_Insert.Activate(sInsertSettings)==false) return;

 CDocument_Main *cDocument_Main_Ptr=(CDocument_Main*)GetActiveDocument();
 if (cDocument_Main_Ptr!=NULL) cDocument_Main_Ptr->Insert(sInsertSettings);
}
//----------------------------------------------------------------------------------------------------
//нажата кнопка "Извлечь файлы из DV-видеофайла" главной панели инструментов 
//----------------------------------------------------------------------------------------------------
afx_msg void CFrameWnd_Main::OnCommand_ToolBar_Main_Extract(void)
{
 CDialog_Extract cDialog_Extract((LPSTR)IDD_DIALOG_EXTRACT,this);
 if (cDialog_Extract.Activate(sExtractSettings)==false) return;

 CDocument_Main *cDocument_Main_Ptr=(CDocument_Main*)GetActiveDocument();
 if (cDocument_Main_Ptr!=NULL) cDocument_Main_Ptr->Extract(sExtractSettings);
}
//----------------------------------------------------------------------------------------------------
//нажата кнопка "Остановить обработку" главной панели инструментов 
//----------------------------------------------------------------------------------------------------
afx_msg void CFrameWnd_Main::OnCommand_ToolBar_Main_Stop(void)
{
 CDocument_Main *cDocument_Main_Ptr=(CDocument_Main*)GetActiveDocument();
 if (cDocument_Main_Ptr!=NULL) cDocument_Main_Ptr->Break();
}
//----------------------------------------------------------------------------------------------------
//нажата кнопка "Проверить файлы в DV-видеофайле" главной панели инструментов 
//----------------------------------------------------------------------------------------------------
afx_msg void CFrameWnd_Main::OnCommand_ToolBar_Main_Verify(void)
{
 CDialog_Extract cDialog_Extract((LPSTR)IDD_DIALOG_EXTRACT,this);
 if (cDialog_Extract.Activate(sExtractSettings)==false) return;

 CDocument_Main *cDocument_Main_Ptr=(CDocument_Main*)GetActiveDocument();
 if (cDocument_Main_Ptr!=NULL) cDocument_Main_Ptr->Verify(sExtractSettings);
}
//----------------------------------------------------------------------------------------------------
//нажата кнопка "очистить статистику" главной панели инструментов 
//----------------------------------------------------------------------------------------------------
void CFrameWnd_Main::OnCommand_ToolBar_Main_Clear(void)
{
 CListCtrl &cListCtrl=((CListView_Main*)GetActiveView())->GetListCtrl(); 
 cListCtrl.DeleteAllItems();
}
//====================================================================================================
//Прочее
//====================================================================================================
IMPLEMENT_DYNCREATE(CFrameWnd_Main,CFrameWnd)

