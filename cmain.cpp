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
 sProtected.IsProcessing=false;
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
//�������� � �������� �����
//----------------------------------------------------------------------------------------------------
void CMain::GetAndClearAnswer(std::string &answer)
{
 cThreadMain_Ptr->GetAndClearAnswer(answer);
}

//----------------------------------------------------------------------------------------------------
//������� ������ � dv-����
//----------------------------------------------------------------------------------------------------
void CMain::InsertData(CDVTime &cDVTime,const CDialog_Insert::SInsertSettings &sInsertSettings)
{
 CRAIICCriticalSection cRAIICCriticalSection(&sProtected.cCriticalSection);
 {
  sProtected.sMode.Mode=MODE_INSERT_TO_DV;
  sProtected.sMode.cDVTime=cDVTime;
  sProtected.sMode.sInsertSettings=sInsertSettings;
 } 
}
//----------------------------------------------------------------------------------------------------
//������� ������ �� dv-�����  
//----------------------------------------------------------------------------------------------------
void CMain::ExtractData(const CDialog_Extract::SExtractSettings &sExtractSettings)
{
 CRAIICCriticalSection cRAIICCriticalSection(&sProtected.cCriticalSection);
 {
  sProtected.sMode.Mode=MODE_EXTRACT_DV;
  sProtected.sMode.sExtractSettings=sExtractSettings;
 } 
}
//----------------------------------------------------------------------------------------------------
//��������� ������ � dv-�����
//----------------------------------------------------------------------------------------------------
void CMain::VerifyData(const CDialog_Extract::SExtractSettings &sExtractSettings)
{
 CRAIICCriticalSection cRAIICCriticalSection(&sProtected.cCriticalSection);
 {
  sProtected.sMode.Mode=MODE_VERIFY_DV;
  sProtected.sMode.sExtractSettings=sExtractSettings;
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
 } 
}
//----------------------------------------------------------------------------------------------------
//�������� ����� ������ (���������� ����� ��������)
//----------------------------------------------------------------------------------------------------
void CMain::ClearMode(void)
{
 CRAIICCriticalSection cRAIICCriticalSection(&sProtected.cCriticalSection);
 {
  sProtected.sMode.Mode=MODE_WAIT;
 } 
}
//----------------------------------------------------------------------------------------------------
//�������� ����� ������ � �������� ���
//----------------------------------------------------------------------------------------------------
void CMain::GetAndClearMode(SMode &sMode)
{
 CRAIICCriticalSection cRAIICCriticalSection(&sProtected.cCriticalSection);
 {
  sMode=sProtected.sMode;
  sProtected.sMode.Mode=MODE_WAIT;
 } 
}
//----------------------------------------------------------------------------------------------------
//������, ������������ �� ���������
//----------------------------------------------------------------------------------------------------
void CMain::SetProcessingState(bool state)
{
 CRAIICCriticalSection cRAIICCriticalSection(&sProtected.cCriticalSection);
 {
  sProtected.IsProcessing=state;
 }
}
//----------------------------------------------------------------------------------------------------
//��������, ������������ �� ���������
//----------------------------------------------------------------------------------------------------
bool CMain::IsProcessing(void)
{
 CRAIICCriticalSection cRAIICCriticalSection(&sProtected.cCriticalSection);
 {
  return(sProtected.IsProcessing);
 }
}
//----------------------------------------------------------------------------------------------------
//�������� ���������
//----------------------------------------------------------------------------------------------------
void CMain::Break(void)
{
 cThreadMain_Ptr->Break();
}