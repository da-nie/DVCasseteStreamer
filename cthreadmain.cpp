//====================================================================================================
//подключаемые библиотеки
//====================================================================================================
#include "cthreadmain.h"
#include "cmain.h"
#include <memory>
#include <string>

//====================================================================================================
//глобальные переменные
//====================================================================================================

//====================================================================================================
//функция потока
//====================================================================================================

UINT ThreadMain(LPVOID pParam)
{
 CThreadMain *cThreadMain_Ptr=reinterpret_cast<CThreadMain*>(pParam);
 if (cThreadMain_Ptr==NULL) return(0);
 cThreadMain_Ptr->Execute(); 
 return(0);
}
//====================================================================================================
//конструктор класса
//====================================================================================================
CThreadMain::CThreadMain(CMain *cMain_PtrSet)
{
 cMain_Ptr=cMain_PtrSet;
 cWinThread_Thread=NULL;
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CThreadMain::~CThreadMain()
{ 
 Stop();
}


//====================================================================================================
//закрытые функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//основной цикл класса
//----------------------------------------------------------------------------------------------------
void CThreadMain::Execute(void)
{
 while(true)
 {
  if (IsExit()==true) return;
  CMain::SMode sMode;
  cMain_Ptr->GetMode(sMode);  
  if (sMode.Mode==CMain::MODE_INSERT_TO_DV)//сборка файлов в dv
  {
   char c_dir[MAX_PATH];
   GetCurrentDirectory(MAX_PATH,c_dir);
   cDVControl.NewRecord();
   cOutputDVStreamFile.SetExit(false);
   cDVControl.LoadBackgroundImage(sMode.LogoFileName,true);
   cOutputDVStreamFile.Create(sMode.OutputFileName);
   uint32_t prefix=sMode.Prefix;
   SetCurrentDirectory(sMode.Path.c_str());
   InsertToDV(sMode.Path,"",&cOutputDVStreamFile,&cDVControl,sMode.cDVTime,prefix,sMode.OutputFileName,"",sMode.LogoFileName);
   cOutputDVStreamFile.Close();   
   SetCurrentDirectory(c_dir);
  }
  if (sMode.Mode==CMain::MODE_EXTRACT_DV)//извлечение файлов из dv
  {
   cExtractDataStreamFileWindows.SetExit(false);
   cDVControl.ExtractDV(sMode.InputFileName,sMode.Path,&cExtractDataStreamFileWindows); 
   cExtractDataStreamFileWindows.Close();
  }
  Sleep(100);
 }
}
//----------------------------------------------------------------------------------------------------
//получить, требуется ли выйти из потока
//----------------------------------------------------------------------------------------------------
bool CThreadMain::IsExit(void)
{
 if (WaitForSingleObject(cEvent_Exit.m_hObject,0)==WAIT_OBJECT_0) return(true);//требуется выйти из потока
 return(false);
}

//----------------------------------------------------------------------------------------------------
//собрать файлы в DV-видеофайл
//----------------------------------------------------------------------------------------------------
void CThreadMain::InsertToDV(const std::string &path,const std::string &save_path,IOutputDVStream *iOutputDVStream_Ptr,CDVControl *cDVControl_Ptr,CDVTime &cDVTime,uint32_t &prefix,const std::string &output_file_name,const std::string &programm_file_name,const std::string &logo_file_name)
{
 //сохраняем все файлы каталога
 WIN32_FIND_DATA wfd;
 HANDLE handle=FindFirstFile("*.*",&wfd);
 if (handle==INVALID_HANDLE_VALUE) return;
 while(1)
 {
  if (iOutputDVStream_Ptr->IsExit()==true) break;

  if (wfd.cFileName[0]!='.' && !(wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))//если это файл
  {
   std::string file_name=path;
   file_name+="\\";
   file_name+=wfd.cFileName;

   std::string title_file_name=save_path;
   title_file_name+=wfd.cFileName;

   bool use=true;
   if (title_file_name.compare(output_file_name)==0) use=false;//пропускаем имя выходного файла
   if (title_file_name.compare(programm_file_name)==0) use=false;//пропускаем своё имя
   if (title_file_name.compare(logo_file_name)==0) use=false;//пропускаем имя логотипа
   if (use==true)
   {
	cDVControl_Ptr->CreateDV(iOutputDVStream_Ptr,file_name,title_file_name,true,prefix,cDVTime);
    prefix=0;
   }
  }
  if (FindNextFile(handle,&wfd)==FALSE) break;
 }
 FindClose(handle);
 
 //обходим все каталоги
 handle=FindFirstFile("*.*",&wfd);
 if (handle==INVALID_HANDLE_VALUE) return;
 while(1)
 {
  if (iOutputDVStream_Ptr->IsExit()==true) break;

  if (wfd.cFileName[0]!='.' && (wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))//если это директория
  {
   std::string new_path=path;   
   new_path+="\\";
   new_path+=wfd.cFileName;   

   std::string new_save_path=save_path;
   new_save_path+=wfd.cFileName;
   new_save_path+="\\";

   if (SetCurrentDirectory(new_path.c_str())==FALSE) break;
   InsertToDV(new_path,new_save_path,iOutputDVStream_Ptr,cDVControl_Ptr,cDVTime,prefix,output_file_name,programm_file_name,logo_file_name);  
   SetCurrentDirectory(path.c_str());
  }
  if (FindNextFile(handle,&wfd)==FALSE) break;
 }
 FindClose(handle);
}


//====================================================================================================
//открытые функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//запустить поток
//----------------------------------------------------------------------------------------------------
void CThreadMain::Start(void)
{
 Stop();
 cEvent_Exit.ResetEvent();
 cOutputDVStreamFile.SetExit(false);
 cExtractDataStreamFileWindows.SetExit(false);
 cWinThread_Thread=AfxBeginThread((AFX_THREADPROC)ThreadMain,this);
 cWinThread_Thread->m_bAutoDelete=FALSE;
}
//----------------------------------------------------------------------------------------------------
//остановить поток
//----------------------------------------------------------------------------------------------------
void CThreadMain::Stop(void)
{ 
 if (cWinThread_Thread!=NULL)
 {
  cEvent_Exit.SetEvent();
  cOutputDVStreamFile.SetExit(true);
  cExtractDataStreamFileWindows.SetExit(true);
  WaitForSingleObject(cWinThread_Thread->m_hThread,INFINITE);
  delete(cWinThread_Thread);
  cWinThread_Thread=NULL;    
 }
}
//----------------------------------------------------------------------------------------------------
//остановить работу с DV-файлом
//----------------------------------------------------------------------------------------------------
void CThreadMain::Break(void)
{ 
 cOutputDVStreamFile.SetExit(true);
 cExtractDataStreamFileWindows.SetExit(true);
}
//----------------------------------------------------------------------------------------------------
//получить ответ
//----------------------------------------------------------------------------------------------------
void CThreadMain::GetAnswer(std::string &answer)
{ 
 cOutputDVStreamFile.GetAnswer(answer);
 std::string answer_two;
 cExtractDataStreamFileWindows.GetAnswer(answer_two);
 answer+=answer_two;
}
