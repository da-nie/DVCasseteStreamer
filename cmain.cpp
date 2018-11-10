//====================================================================================================
//������������ ����������
//====================================================================================================
#include "cmain.h"

//====================================================================================================
//����������� ������
//====================================================================================================
CMain::CMain(void):cThreadMain_Ptr(new CThreadMain(this))
{ 
 sProtected.sMode.Mode=MODE_WAIT;

 cThreadMain_Ptr->Start();
}
//====================================================================================================
//���������� ������
//====================================================================================================
CMain::~CMain()
{
 cThreadMain_Ptr->Stop();
}

//====================================================================================================
//�������� ������� ������
//====================================================================================================

//====================================================================================================
//�������� ������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//������� �������
//----------------------------------------------------------------------------------------------------
void CMain::OnTimer(std::string &answer)
{
 cThreadMain_Ptr->GetAnswer(answer);
}

//----------------------------------------------------------------------------------------------------
//������� ������ � dv-����
//----------------------------------------------------------------------------------------------------
void CMain::InsertData(CDVTime &cDVTime,const std::string &path,const std::string &output_file_name,const std::string &programm_file_name,const std::string &logo_file_name,uint32_t prefix)
{
 CRAIICCriticalSection cRAIICCriticalSection(&sProtected.cCriticalSection);
 {
  sProtected.sMode.Mode=MODE_INSERT_TO_DV;
  sProtected.sMode.cDVTime=cDVTime;
  sProtected.sMode.LogoFileName=logo_file_name;
  sProtected.sMode.ProgrammFileName=programm_file_name;
  sProtected.sMode.Path=path;
  sProtected.sMode.Prefix=prefix;
  sProtected.sMode.OutputFileName=output_file_name;  
 } 
}
//----------------------------------------------------------------------------------------------------
//������� ������ �� dv-�����  
//----------------------------------------------------------------------------------------------------
void CMain::ExtractData(const std::string &path,const std::string &input_file_name)
{
 CRAIICCriticalSection cRAIICCriticalSection(&sProtected.cCriticalSection);
 {
  sProtected.sMode.Mode=MODE_EXTRACT_DV;
  sProtected.sMode.Path=path;
  sProtected.sMode.InputFileName=input_file_name;  
 } 
}
//----------------------------------------------------------------------------------------------------
//�������� ����� ������
//----------------------------------------------------------------------------------------------------
void CMain::GetMode(SMode &sMode)
{
 CRAIICCriticalSection cRAIICCriticalSection(&sProtected.cCriticalSection);
 {
  sMode=sProtected.sMode;
  sProtected.sMode.Mode=MODE_WAIT;
 } 
}