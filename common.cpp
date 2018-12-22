//====================================================================================================
//������������ ����������
//====================================================================================================

#include "common.h"

//====================================================================================================
//�������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//����� ��������
//----------------------------------------------------------------------------------------------------
bool SelectDirectory(std::string &path,const std::string &caption,CWnd *cWnd_Ptr)
{
 path="";
 LPMALLOC pShellMalloc;
 //SHBrowseForFolder ���������� PIDL. ������ ���  PIDL  
 //�������������� ���������. � �����, �� ������ ��������
 //��� ������, ��� ��� �� ������ �������� ��������� �� ������
 //shell malloc CO� ������� ����� ����� ���������� PIDL.
 if (SHGetMalloc(&pShellMalloc)!=NO_ERROR) return(false);
 
 char foldername[MAX_PATH];
 char folderpath[MAX_PATH];
 
 BROWSEINFO bi;
 memset(&bi,0,sizeof(bi));
 bi.hwndOwner=NULL;
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
