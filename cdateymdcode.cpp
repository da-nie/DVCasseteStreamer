//====================================================================================================
//подключаемые библиотеки
//====================================================================================================
#include "cdateymdcode.h"

//====================================================================================================
//конструктор класса
//====================================================================================================
CDateYMDCode::CDateYMDCode(void)
{ 
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CDateYMDCode::~CDateYMDCode()
{
}

//====================================================================================================
//закрытые функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//задать блок кода даты
//----------------------------------------------------------------------------------------------------
void CDateYMDCode::Set(SDateYMDCode *sDateYMDCode_Ptr,uint8_t id,const CDVTime &cDVTime)
{
 uint16_t year;
 uint8_t month;
 uint8_t day;
 uint8_t hour;
 uint8_t min;
 uint8_t sec;

 cDVTime.Get(year,month,day,hour,min,sec);
 sDateYMDCode_Ptr->ID=id;

 uint8_t bcd;
 bcd=CDVTime::CreateBCD(day);
 sDateYMDCode_Ptr->UnitsOfDay=bcd&0x0F;
 sDateYMDCode_Ptr->TensOfDay=(bcd>>4)&0x0F;

 bcd=CDVTime::CreateBCD(month);
 sDateYMDCode_Ptr->UnitsOfMonth=bcd&0x0F;
 sDateYMDCode_Ptr->TensOfMonth=(bcd>>4)&0x0F;

 bcd=CDVTime::CreateBCD(year%100);
 sDateYMDCode_Ptr->UnitsOfYear=bcd&0x0F;
 sDateYMDCode_Ptr->TensOfYear=(bcd>>4)&0x0F;

 sDateYMDCode_Ptr->Res1=0xff;//все единицы
 sDateYMDCode_Ptr->Res2=0x03;
 sDateYMDCode_Ptr->Res3=0x07;
}

//====================================================================================================
//открытые функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//задать блок кода даты для видео данных
//----------------------------------------------------------------------------------------------------
void CDateYMDCode::SetVideo(SDateYMDCode *sDateYMDCode_Ptr,const CDVTime &cDVTime)
{
 Set(sDateYMDCode_Ptr,DATE_YMD_CODE_VIDEO_ID,cDVTime);
}


//----------------------------------------------------------------------------------------------------
//задать блок кода даты для аудио данных
//----------------------------------------------------------------------------------------------------
void CDateYMDCode::SetAudio(SDateYMDCode *sDateYMDCode_Ptr,const CDVTime &cDVTime)
{
 Set(sDateYMDCode_Ptr,DATE_YMD_CODE_AUDIO_ID,cDVTime);
}

//----------------------------------------------------------------------------------------------------
//вывести в файл параметры кода даты
//----------------------------------------------------------------------------------------------------
void CDateYMDCode::OutputToFile(FILE *file,const std::string &prefix,SDateYMDCode *sDateYMDCode_Ptr)
{
 if (file==NULL) return;
 fprintf(file,"%sDateYMDCode.Day=%c%c\r\n",prefix.c_str(),sDateYMDCode_Ptr->TensOfDay+'0',sDateYMDCode_Ptr->UnitsOfDay+'0');
 fprintf(file,"%sDateYMDCode.Month=%c%c\r\n",prefix.c_str(),sDateYMDCode_Ptr->TensOfMonth+'0',sDateYMDCode_Ptr->UnitsOfMonth+'0');
 fprintf(file,"%sDateYMDCode.Year=%c%c\r\n",prefix.c_str(),sDateYMDCode_Ptr->TensOfYear+'0',sDateYMDCode_Ptr->UnitsOfYear+'0');
}




