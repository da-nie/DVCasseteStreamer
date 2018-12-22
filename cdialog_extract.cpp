//====================================================================================================
//������������ ����������
//====================================================================================================
#include "cdialog_extract.h"

#include "common.h"

//====================================================================================================
//������� ��������� ��������� ������
//====================================================================================================

BEGIN_MESSAGE_MAP(CDialog_Extract,CDialog)
 ON_WM_DESTROY()
 ON_COMMAND(IDC_BUTTON_DIALOG_EXTRACT_PATH_SELECT,OnCommand_Button_PathSelect)
 ON_COMMAND(IDC_BUTTON_DIALOG_EXTRACT_DV_SELECT,OnCommand_Button_DVSelect)
 ON_COMMAND(IDC_BUTTON_DIALOG_EXTRACT_OK,OnCommand_Button_Ok)
 ON_COMMAND(IDC_BUTTON_DIALOG_EXTRACT_CANCEL,OnCommand_Button_Cancel)
END_MESSAGE_MAP()

//====================================================================================================
//����������� ������
//====================================================================================================
CDialog_Extract::CDialog_Extract(LPCTSTR lpszTemplateName,CWnd* pParentWnd):CDialog(lpszTemplateName,pParentWnd)
{
}
//====================================================================================================
//���������� ������
//====================================================================================================
CDialog_Extract::~CDialog_Extract()
{
}
//====================================================================================================
//������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//������ Enter
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_Extract::OnOK(void)
{
}
//----------------------------------------------------------------------------------------------------
//������ ESC
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_Extract::OnCancel(void)
{
}
//----------------------------------------------------------------------------------------------------
//������������� �������
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
//������ �������
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
//������� ��������� ��������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//����������� ����
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_Extract::OnDestroy(void)
{
 CDialog::OnDestroy();
}
//----------------------------------------------------------------------------------------------------
//������� �����
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_Extract::OnCommand_Button_PathSelect(void)
{
 std::string path;
 if (SelectDirectory(path,"�������� �����,� ������� ����� ��������� �����",this)==true)
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
//������� dv-����
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_Extract::OnCommand_Button_DVSelect(void)
{
 char path[MAX_PATH];
 GetCurrentDirectory(MAX_PATH,path); 
 CFileDialog cFileDialog(TRUE,"","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"*.dv|*.dv||",this);
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
 sExtractSettings_Local.DVFileName=file_path;
 sExtractSettings_Local.DVFileName+="\\";
 sExtractSettings_Local.DVFileName+=cFileDialog.GetFileName();

 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_EXTRACT_DV_FILE_NAME))->SetWindowText(sExtractSettings_Local.DVFileName.c_str());
}
//----------------------------------------------------------------------------------------------------
//��������� ���������
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_Extract::OnCommand_Button_Ok(void)
{ 
 EndDialog(0);
}
//----------------------------------------------------------------------------------------------------
//�������� ���������
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_Extract::OnCommand_Button_Cancel(void)
{
 EndDialog(-1);
}
