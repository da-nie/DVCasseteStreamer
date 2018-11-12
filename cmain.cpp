//====================================================================================================
//подключаемые библиотеки
//====================================================================================================
#include "cmain.h"

//====================================================================================================
//конструктор класса
//====================================================================================================
CMain::CMain(void):cThreadMain_Ptr(new CThreadMain(this))
{ 
 sProtected.sMode.Mode=MODE_WAIT;
 sProtected.IsProcessing=false;
 cThreadMain_Ptr->Start();
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CMain::~CMain()
{
 cThreadMain_Ptr->Stop();
}

//====================================================================================================
//закрытые функции класса
//====================================================================================================

//====================================================================================================
//открытые функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//событие таймера
//----------------------------------------------------------------------------------------------------
void CMain::OnTimer(std::string &answer)
{
 cThreadMain_Ptr->GetAndClearAnswer(answer);
}

//----------------------------------------------------------------------------------------------------
//собрать данные в dv-файл
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
//извлечь данные из dv-файла  
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
//получить режим работы
//----------------------------------------------------------------------------------------------------
void CMain::GetMode(SMode &sMode)
{
 CRAIICCriticalSection cRAIICCriticalSection(&sProtected.cCriticalSection);
 {
  sMode=sProtected.sMode;
 } 
}
//----------------------------------------------------------------------------------------------------
//очистить режим работы (установить режим ожидания)
//----------------------------------------------------------------------------------------------------
void CMain::ClearMode(void)
{
 CRAIICCriticalSection cRAIICCriticalSection(&sProtected.cCriticalSection);
 {
  sProtected.sMode.Mode=MODE_WAIT;
 } 
}
//----------------------------------------------------------------------------------------------------
//получить режим работы и очистить его
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
//задать, производится ли обработка
//----------------------------------------------------------------------------------------------------
void CMain::SetProcessingState(bool state)
{
 CRAIICCriticalSection cRAIICCriticalSection(&sProtected.cCriticalSection);
 {
  sProtected.IsProcessing=state;
 }
}
//----------------------------------------------------------------------------------------------------
//получить, производится ли обработка
//----------------------------------------------------------------------------------------------------
bool CMain::IsProcessing(void)
{
 CRAIICCriticalSection cRAIICCriticalSection(&sProtected.cCriticalSection);
 {
  return(sProtected.IsProcessing);
 }
}
//----------------------------------------------------------------------------------------------------
//прервать обработку
//----------------------------------------------------------------------------------------------------
void CMain::Break(void)
{
 cThreadMain_Ptr->Break();
}