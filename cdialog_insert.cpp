//====================================================================================================
//������������ ����������
//====================================================================================================
#include "cdialog_insert.h"

#include "common.h"

//====================================================================================================
//������� ��������� ��������� ������
//====================================================================================================

BEGIN_MESSAGE_MAP(CDialog_Insert,CDialog)
 ON_WM_DESTROY()
 ON_COMMAND(IDC_BUTTON_DIALOG_INSERT_LOGO_SELECT,OnCommand_Button_LogoSelect)
 ON_COMMAND(IDC_BUTTON_DIALOG_INSERT_PATH_SELECT,OnCommand_Button_PathSelect)
 ON_COMMAND(IDC_BUTTON_DIALOG_INSERT_DV_SELECT,OnCommand_Button_DVSelect)
 ON_COMMAND(IDC_BUTTON_DIALOG_INSERT_OK,OnCommand_Button_Ok)
 ON_COMMAND(IDC_BUTTON_DIALOG_INSERT_CANCEL,OnCommand_Button_Cancel)
END_MESSAGE_MAP()

//====================================================================================================
//����������� ������
//====================================================================================================
CDialog_Insert::CDialog_Insert(LPCTSTR lpszTemplateName,CWnd* pParentWnd):CDialog(lpszTemplateName,pParentWnd)
{
}
//====================================================================================================
//���������� ������
//====================================================================================================
CDialog_Insert::~CDialog_Insert()
{
}
//====================================================================================================
//������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//������ Enter
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_Insert::OnOK(void)
{
}
//----------------------------------------------------------------------------------------------------
//������ ESC
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_Insert::OnCancel(void)
{
}
//----------------------------------------------------------------------------------------------------
//������������� �������
//----------------------------------------------------------------------------------------------------
afx_msg BOOL CDialog_Insert::OnInitDialog(void)
{
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_INSERT_LOGO_FILE_NAME))->SetLimitText(MAX_FILE_NAME_TEXT);
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_INSERT_PATH))->SetLimitText(MAX_FILE_NAME_TEXT);
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_INSERT_DV_FILE_NAME))->SetLimitText(MAX_FILE_NAME_TEXT);
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_INSERT_GLOBAL_PREFIX))->SetLimitText(MAX_PREFIX_TEXT);
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_INSERT_LOCAL_PREFIX))->SetLimitText(MAX_PREFIX_TEXT);

 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_INSERT_LOGO_FILE_NAME))->SetWindowText(sInsertSettings_Local.LogoFileName.c_str());
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_INSERT_PATH))->SetWindowText(sInsertSettings_Local.Path.c_str());
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_INSERT_DV_FILE_NAME))->SetWindowText(sInsertSettings_Local.DVFileName.c_str());

 char g_prefix[255];
 char l_prefix[255];
 sprintf(g_prefix,"%i",sInsertSettings_Local.GlobalPrefix);
 sprintf(l_prefix,"%i",sInsertSettings_Local.LocalPrefix);

 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_INSERT_GLOBAL_PREFIX))->SetWindowText(g_prefix);
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_INSERT_LOCAL_PREFIX))->SetWindowText(l_prefix);
 return(CDialog::OnInitDialog());
}
//----------------------------------------------------------------------------------------------------
//������ �������
//----------------------------------------------------------------------------------------------------
bool CDialog_Insert::Activate(SInsertSettings& sInsertSettings)
{
 sInsertSettings_Local=sInsertSettings;
 long ret=DoModal();
 if (ret==0)
 {
  sInsertSettings=sInsertSettings_Local;
  return(true);
 }
 return(false);
}

//====================================================================================================
//������� ��������� ��������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//����������� ����
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_Insert::OnDestroy(void)
{
 CDialog::OnDestroy();
}
//----------------------------------------------------------------------------------------------------
//������� ���� ��������
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_Insert::OnCommand_Button_LogoSelect(void)
{
 char path[MAX_PATH];
 GetCurrentDirectory(MAX_PATH,path); 
 CFileDialog cFileDialog(TRUE,"","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"*.tga|*.tga||",this);
 cFileDialog.m_ofn.lpstrInitialDir=path;
 char title[]="����� ����� ��������";
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
 sInsertSettings_Local.LogoFileName=file_path;
 sInsertSettings_Local.LogoFileName+="\\";
 sInsertSettings_Local.LogoFileName+=cFileDialog.GetFileName();

 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_INSERT_LOGO_FILE_NAME))->SetWindowText(sInsertSettings_Local.LogoFileName.c_str());
}
//----------------------------------------------------------------------------------------------------
//������� �����
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_Insert::OnCommand_Button_PathSelect(void)
{
 std::string path;
 if (SelectDirectory(path,"�������� �����, ����� �� ������� ����� ��������� � DV-���������",this)==true)
 {
  sInsertSettings_Local.Path=path;
  uint32_t length=path.length();
  if (length>0)
  {
   if (path.c_str()[length-1]!='\\') sInsertSettings_Local.Path+="\\";
  }
  ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_INSERT_PATH))->SetWindowText(sInsertSettings_Local.Path.c_str());
 }
}
//----------------------------------------------------------------------------------------------------
//������� dv-����
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_Insert::OnCommand_Button_DVSelect(void)
{
 char path[MAX_PATH];
 GetCurrentDirectory(MAX_PATH,path); 
 CFileDialog cFileDialog(FALSE,"","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"*.dv|*.dv||",this);
 cFileDialog.m_ofn.lpstrInitialDir=path;
 char title[]="����� dv-�����";
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
 sInsertSettings_Local.DVFileName=file_path;
 sInsertSettings_Local.DVFileName+=cFileDialog.GetFileName();

 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_INSERT_DV_FILE_NAME))->SetWindowText(sInsertSettings_Local.DVFileName.c_str());
}
//----------------------------------------------------------------------------------------------------
//��������� ���������
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_Insert::OnCommand_Button_Ok(void)
{ 
 char prefix[MAX_PREFIX_TEXT+1];

 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_INSERT_GLOBAL_PREFIX))->GetWindowText(prefix,MAX_PREFIX_TEXT);
 sInsertSettings_Local.GlobalPrefix=atoi(prefix);

 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_INSERT_LOCAL_PREFIX))->GetWindowText(prefix,MAX_PREFIX_TEXT);
 sInsertSettings_Local.LocalPrefix=atoi(prefix);
 
 EndDialog(0);
}
//----------------------------------------------------------------------------------------------------
//�������� ���������
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_Insert::OnCommand_Button_Cancel(void)
{
 EndDialog(-1);
}
