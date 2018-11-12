#include "cdialog_main.h"

//====================================================================================================
//карта сообщений класса
//====================================================================================================
BEGIN_MESSAGE_MAP(CDialog_Main,CDialog)
 ON_WM_DESTROY()
 ON_COMMAND(IDC_BUTTON_MAIN_CLEAR_LOG,OnButton_ClearLog)
 ON_COMMAND(IDC_BUTTON_MAIN_INSERT,OnButton_Insert)
 ON_COMMAND(IDC_BUTTON_MAIN_EXTRACT,OnButton_Extract)
 ON_COMMAND(IDC_BUTTON_MAIN_BREAK,OnButton_Break)
 ON_WM_TIMER()
END_MESSAGE_MAP()

//====================================================================================================
//конструктор класса
//====================================================================================================
CDialog_Main::CDialog_Main(LPCTSTR lpszTemplateName, CWnd* pParentWnd):cMain_Ptr(new CMain),CDialog(lpszTemplateName,pParentWnd)
{ 
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CDialog_Main::~CDialog_Main()
{ 
}

//====================================================================================================
//закрытые функции класса
//====================================================================================================


//----------------------------------------------------------------------------------------------------
//выбор каталога
//----------------------------------------------------------------------------------------------------
afx_msg bool CDialog_Main::SelectDirectory(std::string &path,const std::string &caption)
{
 path="";
 LPMALLOC pShellMalloc;
 //SHBrowseForFolder возвращает PIDL. Память для  PIDL  
 //распределяется оболочкой. В итоге, мы должны очистить
 //эту память, так как мы должны получить указатель на объект
 //shell malloc COМ который будет позже освобожден PIDL.
 if (SHGetMalloc(&pShellMalloc)!=NO_ERROR) return(false);
 
 char foldername[MAX_PATH];
 char folderpath[MAX_PATH];
 
 BROWSEINFO bi;
 memset(&bi,0,sizeof(bi));
 bi.hwndOwner=m_hWnd;
 bi.pidlRoot=NULL;
 bi.pszDisplayName=foldername;
 bi.lpszTitle=caption.c_str();
 bi.ulFlags=BIF_NEWDIALOGSTYLE|BIF_RETURNONLYFSDIRS;
 LPITEMIDLIST pidl=SHBrowseForFolder(&bi);

 if (pidl!=NULL)
 {
  SHGetPathFromIDList(pidl,folderpath);
  path=folderpath;
  pShellMalloc->Free(pidl);
  pShellMalloc->Release();
  return(true);
 }
 pShellMalloc->Release();
 return(false);
}

//====================================================================================================
//открытые функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_Main::OnOK(void)
{
}
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_Main::OnCancel(void)
{
 //if (MessageBox("Вы уверены что хотите закрыть программу ?","Подтверждение",MB_YESNO|MB_DEFBUTTON2)!=IDYES) return;
 CDialog::OnCancel(); 
}
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
afx_msg BOOL CDialog_Main::OnInitDialog(void)
{
 //настраиваем элементы управления
 ((CEdit*)GetDlgItem(IDC_EDIT_MAIN_PREFIX_FRAME))->SetWindowText("200");
 ((CEdit*)GetDlgItem(IDC_EDIT_MAIN_PREFIX_FRAME))->SetLimitText(10);
 ((CButton*)GetDlgItem(IDC_BUTTON_MAIN_BREAK))->EnableWindow(FALSE);
 //ставим значок
 //hIcon=LoadIcon(AfxGetResourceHandle(),(LPCTSTR)IDI_ICON_MAIN);
 //SetIcon(hIcon,TRUE); 
 SetTimer(ID_TIMER_MAIN,TIMER_PERIOD_MS,NULL);
 return(CDialog::OnInitDialog());
}
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_Main::OnDestroy(void)
{
 //DestroyIcon(hIcon);
 KillTimer(ID_TIMER_MAIN);
 CDialog::OnDestroy();
}
//----------------------------------------------------------------------------------------------------
//добавить файлы в dv-видеофайл
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_Main::OnButton_Insert(void)
{
 std::string path;
 if (SelectDirectory(path,"Выберите папку, файлы из которой нужно поместить в dv-видеофайл")==false) return;

 CDVTime cDVTime;
 SYSTEMTIME st;
 GetLocalTime(&st);
 
 cDVTime.Set(st.wYear,static_cast<uint8_t>(st.wMonth),static_cast<uint8_t>(st.wDay),static_cast<uint8_t>(st.wHour),static_cast<uint8_t>(st.wMinute),static_cast<uint8_t>(st.wSecond));
  
 char str[255];
 ((CEdit*)GetDlgItem(IDC_EDIT_MAIN_PREFIX_FRAME))->GetWindowText(str,255);
 uint32_t prefix=atoi(str);

 std::string logo_file_name="minilogo.TGA";
 std::string programm_file_name="DVCasseteStreamer.exe";
 std::string output_file_name="output.dv";
 cMain_Ptr->InsertData(cDVTime,path,output_file_name,programm_file_name,logo_file_name,prefix);
}
//----------------------------------------------------------------------------------------------------
//извлечь файлы из dv-видеофайла
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_Main::OnButton_Extract(void)
{
 std::string path;
 if (SelectDirectory(path,"Выберите папку, в которую нужно извлечь файлы из dv-видеофайла")==false) return;

 std::string input_file_name="output.dv";
 cMain_Ptr->ExtractData(path,input_file_name);
}
//----------------------------------------------------------------------------------------------------
//нажата кнопка "очистить статистику"
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_Main::OnButton_ClearLog(void)
{
 ((CListBox*)GetDlgItem(IDC_LIST_MAIN_LOG))->ResetContent();
}
//----------------------------------------------------------------------------------------------------
//прервать обработку
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_Main::OnButton_Break(void)
{
 cMain_Ptr->Break();
}
//----------------------------------------------------------------------------------------------------
//событие таймера
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_Main::OnTimer(UINT nIDEvent)
{
 if (nIDEvent!=ID_TIMER_MAIN)
 {
  CDialog::OnTimer(nIDEvent);
  return;
 }
 std::string answer;
 cMain_Ptr->OnTimer(answer);  
 if (answer.length()==0) return; 
 //выводим ответ
 const uint32_t MAX_LINE_SIZE=80;
 char line[MAX_LINE_SIZE+1];
 size_t line_size=0;
 size_t length=answer.length();
 for(size_t n=0;n<length;n++)
 {
  char s=answer.c_str()[n];
  if (s=='\n' || n==length-1 || line_size==MAX_LINE_SIZE)
  {
   line[line_size]=0;
   ((CListBox*)GetDlgItem(IDC_LIST_MAIN_LOG))->AddString(line);
   line_size=0;
   if (s=='\n' || n==length-1) continue;
  }
  line[line_size]=s;
  line_size++;
 }
 //блокируем кнопки
 bool state=cMain_Ptr->IsProcessing();
 if (state==true)
 {
  ((CEdit*)GetDlgItem(IDC_BUTTON_MAIN_INSERT))->EnableWindow(FALSE);
  ((CEdit*)GetDlgItem(IDC_BUTTON_MAIN_EXTRACT))->EnableWindow(FALSE);
  ((CButton*)GetDlgItem(IDC_BUTTON_MAIN_BREAK))->EnableWindow(TRUE);
 }
 else
 {
  ((CEdit*)GetDlgItem(IDC_BUTTON_MAIN_INSERT))->EnableWindow(TRUE);
  ((CEdit*)GetDlgItem(IDC_BUTTON_MAIN_EXTRACT))->EnableWindow(TRUE);
  ((CButton*)GetDlgItem(IDC_BUTTON_MAIN_BREAK))->EnableWindow(FALSE);
 }
}