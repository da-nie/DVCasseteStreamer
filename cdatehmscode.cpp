//====================================================================================================
//подключаемые библиотеки
//====================================================================================================
#include "cdatehmscode.h"

//====================================================================================================
//конструктор класса
//====================================================================================================
CDateHMSCode::CDateHMSCode(void)
{ 
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CDateHMSCode::~CDateHMSCode()
{
}

//====================================================================================================
//закрытые функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//задать блок кода даты
//----------------------------------------------------------------------------------------------------
void CDateHMSCode::Set(SDateHMSCode *sDateHMSCode_Ptr,uint8_t id,const CDVTime &cDVTime)
{
 uint16_t year;
 uint8_t month;
 uint8_t day;
 uint8_t hour;
 uint8_t min;
 uint8_t sec;

 cDVTime.Get(year,month,day,hour,min,sec);
 sDateHMSCode_Ptr->ID=id;
 uint8_t bcd;
 bcd=CDVTime::CreateBCD(sec);
 sDateHMSCode_Ptr->UnitsOfSecond=bcd&0x0F;
 sDateHMSCode_Ptr->TensOfSecond=(bcd>>4)&0x0F;

 bcd=CDVTime::CreateBCD(min);
 sDateHMSCode_Ptr->UnitsOfMinutes=bcd&0x0F;
 sDateHMSCode_Ptr->TensOfMinutes=(bcd>>4)&0x0F;

 bcd=CDVTime::CreateBCD(hour);
 sDateHMSCode_Ptr->UnitsOfHours=bcd&0x0F;
 sDateHMSCode_Ptr->TensOfHours=(bcd>>4)&0x0F;

 sDateHMSCode_Ptr->Res1=0xff;//все единицы
 sDateHMSCode_Ptr->Res2=0x01;//все единицы
 sDateHMSCode_Ptr->Res3=0x01;//все единицы
 sDateHMSCode_Ptr->Res4=0x03;//все единицы
}

//====================================================================================================
//открытые функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//задать блок кода даты для видео данных
//----------------------------------------------------------------------------------------------------
void CDateHMSCode::SetVideo(SDateHMSCode *sDateHMSCode_Ptr,const CDVTime &cDVTime)
{
 Set(sDateHMSCode_Ptr,DATE_HMS_CODE_VIDEO_ID,cDVTime);
}


//----------------------------------------------------------------------------------------------------
//задать блок кода даты для аудио данных
//----------------------------------------------------------------------------------------------------
void CDateHMSCode::SetAudio(SDateHMSCode *sDateHMSCode_Ptr,const CDVTime &cDVTime)
{
 Set(sDateHMSCode_Ptr,DATE_HMS_CODE_AUDIO_ID,cDVTime);
}

//----------------------------------------------------------------------------------------------------
//вывести в файл параметры временного кода
//----------------------------------------------------------------------------------------------------
void CDateHMSCode::OutputToFile(FILE *file,const std::string &prefix,SDateHMSCode *sDateHMSCode_Ptr)
{
 if (file==NULL) return;
 fprintf(file,"%sDateHMSCode.Second=%c%c\r\n",prefix.c_str(),sDateHMSCode_Ptr->TensOfSecond+'0',sDateHMSCode_Ptr->UnitsOfSecond+'0');
 fprintf(file,"%sDateHMSCode.Minutes=%c%c\r\n",prefix.c_str(),sDateHMSCode_Ptr->TensOfMinutes+'0',sDateHMSCode_Ptr->UnitsOfMinutes+'0');
 fprintf(file,"%sDateHMSCode.Hours=%c%c\r\n",prefix.c_str(),sDateHMSCode_Ptr->TensOfHours+'0',sDateHMSCode_Ptr->UnitsOfHours+'0');
}


