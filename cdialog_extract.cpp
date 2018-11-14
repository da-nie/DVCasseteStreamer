//====================================================================================================
//подключаемые библиотеки
//====================================================================================================
#include "cdialog_extract.h"

#include "common.h"

//====================================================================================================
//функции обработки сообщений класса
//====================================================================================================

BEGIN_MESSAGE_MAP(CDialog_Extract,CDialog)
 ON_WM_DESTROY()
 ON_COMMAND(IDC_BUTTON_DIALOG_EXTRACT_PATH_SELECT,OnCommand_Button_PathSelect)
 ON_COMMAND(IDC_BUTTON_DIALOG_EXTRACT_DV_SELECT,OnCommand_Button_DVSelect)
 ON_COMMAND(IDC_BUTTON_DIALOG_EXTRACT_OK,OnCommand_Button_Ok)
 ON_COMMAND(IDC_BUTTON_DIALOG_EXTRACT_CANCEL,OnCommand_Button_Cancel)
END_MESSAGE_MAP()

//====================================================================================================
//конструктор класса
//====================================================================================================
CDialog_Extract::CDialog_Extract(LPCTSTR lpszTemplateName,CWnd* pParentWnd):CDialog(lpszTemplateName,pParentWnd)
{
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CDialog_Extract::~CDialog_Extract()
{
}
//====================================================================================================
//функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//нажали Enter
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_Extract::OnOK(void)
{
}
//----------------------------------------------------------------------------------------------------
//нажали ESC
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_Extract::OnCancel(void)
{
}
//----------------------------------------------------------------------------------------------------
//инициализация диалога
//----------------------------------------------------------------------------------------------------
afx_msg BOOL CDialog_Extract::OnInitDialog(void)
{
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_EXTRACT_PATH))->SetLimitText(MAX_FILE_NAME_TEXT);
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_EXTRACT_DV_FILE_NAME))->SetLimitText(MAX_FILE_NAME_TEXT);

 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_EXTRACT_PATH))->SetWindowText(sExtractSettings_Local.Path.c_str());
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_EXTRACT_DV_FILE_NAME))->SetWindowText(sExtractSettings_Local.DVFileName.c_str());

 return(CDialog::OnInitDialog());
}
//----------------------------------------------------------------------------------------------------
//запуск диалога
//----------------------------------------------------------------------------------------------------
bool CDialog_Extract::Activate(SExtractSettings& sExtractSettings)
{
 sExtractSettings_Local=sExtractSettings;
 long ret=DoModal();
 if (ret==0)
 {
  sExtractSettings=sExtractSettings_Local;
  return(true);
 }
 return(false);
}

//====================================================================================================
//функции обработки сообщений класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//уничтожение окна
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_Extract::OnDestroy(void)
{
 CDialog::OnDestroy();
}
//----------------------------------------------------------------------------------------------------
//выбрать папку
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_Extract::OnCommand_Button_PathSelect(void)
{
 std::string path;
 if (SelectDirectory(path,"Выберите папку,в которую будут извлечены файлы",this)==true)
 {
  sExtractSettings_Local.Path=path;
  uint32_t length=path.length();
  if (length>0)
  {
   if (path.c_str()[length-1]!='\\') sExtractSettings_Local.Path+="\\";
  }
  ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_EXTRACT_PATH))->SetWindowText(sExtractSettings_Local.Path.c_str());
 }
}
//----------------------------------------------------------------------------------------------------
//выбрать dv-файл
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_Extract::OnCommand_Button_DVSelect(void)
{
 char path[MAX_PATH];
 GetCurrentDirectory(MAX_PATH,path); 
 CFileDialog cFileDialog(TRUE,"","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"*.dv|*.dv||",this);
 cFileDialog.m_ofn.lpstrInitialDir=path;
 char title[]="Выбор dv-файла";
 cFileDialog.m_ofn.lpstrTitle=title;
 if (cFileDialog.DoModal()!=IDOK) return;
 char file_path[MAX_PATH];
 GetCurrentDirectory(MAX_PATH,file_path); 
 SetCurrentDirectory(path);
 uint32_t length=sizeof(file_path);
 if (length>0)
 {
  if (file_path[length-1]=='\\') file_path[length-1]=0;
 }
 sExtractSettings_Local.DVFileName=file_path;
 sExtractSettings_Local.DVFileName+="\\";
 sExtractSettings_Local.DVFileName+=cFileDialog.GetFileName();

 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_EXTRACT_DV_FILE_NAME))->SetWindowText(sExtractSettings_Local.DVFileName.c_str());
}
//----------------------------------------------------------------------------------------------------
//применить настройки
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_Extract::OnCommand_Button_Ok(void)
{ 
 EndDialog(0);
}
//----------------------------------------------------------------------------------------------------
//отменить настройки
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_Extract::OnCommand_Button_Cancel(void)
{
 EndDialog(-1);
}
