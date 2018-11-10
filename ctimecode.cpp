//====================================================================================================
//подключаемые библиотеки
//====================================================================================================
#include "ctimecode.h"

//====================================================================================================
//конструктор класса
//====================================================================================================
CTimeCode::CTimeCode(void)
{ 
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CTimeCode::~CTimeCode()
{
}

//====================================================================================================
//закрытые функции класса
//====================================================================================================

//====================================================================================================
//открытые функции класса
//====================================================================================================


//----------------------------------------------------------------------------------------------------
//задать блок временного кода
//----------------------------------------------------------------------------------------------------
void CTimeCode::Set(UTimeCode *uTimeCode_Ptr,const CDVTime &cDVTime_VideoTime,uint32_t frame,bool is_pal)
{
 uint16_t year;
 uint8_t month;
 uint8_t day;
 uint8_t hour;
 uint8_t min;
 uint8_t sec;

 cDVTime_VideoTime.Get(year,month,day,hour,min,sec);
 uint8_t f=frame%CDVTime::NTSC_FRAME_RATE;
 if (is_pal==true) f=frame%CDVTime::PAL_FRAME_RATE;

 if (is_pal==true)
 {
  STimeCodePAL *sTimeCodePAL_Ptr=&uTimeCode_Ptr->sTimeCodePAL;
  sTimeCodePAL_Ptr->ID=TIME_CODE_ID;
  uint8_t bcd;
  bcd=CDVTime::CreateBCD(f);
  sTimeCodePAL_Ptr->UnitsOfFrames=bcd&0x0F;
  sTimeCodePAL_Ptr->TensOfFrames=(bcd>>4)&0x0F;
  sTimeCodePAL_Ptr->Arb=1;
  sTimeCodePAL_Ptr->CF=1;

  bcd=CDVTime::CreateBCD(sec);
  sTimeCodePAL_Ptr->UnitsOfSecond=bcd&0x0F;
  sTimeCodePAL_Ptr->TensOfSecond=(bcd>>4)&0x0F;
  sTimeCodePAL_Ptr->BGF0=1;

  bcd=CDVTime::CreateBCD(min);
  sTimeCodePAL_Ptr->UnitsOfMinutes=bcd&0x0F;
  sTimeCodePAL_Ptr->TensOfMinutes=(bcd>>4)&0x0F;
  sTimeCodePAL_Ptr->BGF2=1;

  bcd=CDVTime::CreateBCD(hour);
  sTimeCodePAL_Ptr->UnitsOfHours=bcd&0x0F;
  sTimeCodePAL_Ptr->TensOfHours=(bcd>>4)&0x0F;
  sTimeCodePAL_Ptr->BGF1=1;
  sTimeCodePAL_Ptr->PC=1;
 }
 else
 {
  STimeCodeNTSC *sTimeCodeNTSC_Ptr=&uTimeCode_Ptr->sTimeCodeNTSC;
  sTimeCodeNTSC_Ptr->ID=TIME_CODE_ID;
  uint8_t bcd;
  bcd=CDVTime::CreateBCD(f);
  sTimeCodeNTSC_Ptr->UnitsOfFrames=bcd&0x0F;
  sTimeCodeNTSC_Ptr->TensOfFrames=(bcd>>4)&0x0F;
  sTimeCodeNTSC_Ptr->DF=0;
  sTimeCodeNTSC_Ptr->CF=1;

  bcd=CDVTime::CreateBCD(sec);
  sTimeCodeNTSC_Ptr->UnitsOfSecond=bcd&0x0F;
  sTimeCodeNTSC_Ptr->TensOfSecond=(bcd>>4)&0x0F;
  sTimeCodeNTSC_Ptr->PC=1;

  bcd=CDVTime::CreateBCD(min);
  sTimeCodeNTSC_Ptr->UnitsOfMinutes=bcd&0x0F;
  sTimeCodeNTSC_Ptr->TensOfMinutes=(bcd>>4)&0x0F;
  sTimeCodeNTSC_Ptr->BGF0=1;

  bcd=CDVTime::CreateBCD(hour);
  sTimeCodeNTSC_Ptr->UnitsOfHours=bcd&0x0F;
  sTimeCodeNTSC_Ptr->TensOfHours=(bcd>>4)&0x0F;
  sTimeCodeNTSC_Ptr->BGF1=1;
  sTimeCodeNTSC_Ptr->BGF2=1;
 }


}
//----------------------------------------------------------------------------------------------------
//вывести в файл параметры временного кода
//----------------------------------------------------------------------------------------------------
void CTimeCode::OutputToFile(FILE *file,const std::string &prefix,bool is_pal,UTimeCode *uTimeCode_Ptr)
{
 if (file==NULL) return;
 if (is_pal==true)
 {
  STimeCodePAL *sTimeCodePAL_Ptr=&uTimeCode_Ptr->sTimeCodePAL;
  fprintf(file,"%sTimeCode.Frames=%c%c\r\n",prefix.c_str(),sTimeCodePAL_Ptr->TensOfFrames+'0',sTimeCodePAL_Ptr->UnitsOfFrames+'0');
  fprintf(file,"%sTimeCode.Second=%c%c\r\n",prefix.c_str(),sTimeCodePAL_Ptr->TensOfSecond+'0',sTimeCodePAL_Ptr->UnitsOfSecond+'0');
  fprintf(file,"%sTimeCode.Minutes=%c%c\r\n",prefix.c_str(),sTimeCodePAL_Ptr->TensOfMinutes+'0',sTimeCodePAL_Ptr->UnitsOfMinutes+'0');
  fprintf(file,"%sTimeCode.Hours=%c%c\r\n",prefix.c_str(),sTimeCodePAL_Ptr->TensOfHours+'0',sTimeCodePAL_Ptr->UnitsOfHours+'0');
  fprintf(file,"%sTimeCode.CF=%i\r\n",prefix.c_str(),sTimeCodePAL_Ptr->CF);
  fprintf(file,"%sTimeCode.PC=%i\r\n",prefix.c_str(),sTimeCodePAL_Ptr->PC);
  fprintf(file,"%sTimeCode.BGF0=%i\r\n",prefix.c_str(),sTimeCodePAL_Ptr->BGF0);
  fprintf(file,"%sTimeCode.BGF1=%i\r\n",prefix.c_str(),sTimeCodePAL_Ptr->BGF1);
  fprintf(file,"%sTimeCode.BGF2=%i\r\n",prefix.c_str(),sTimeCodePAL_Ptr->BGF2);
 }
 else
 {
  STimeCodeNTSC *sTimeCodeNTSC_Ptr=&uTimeCode_Ptr->sTimeCodeNTSC;
  fprintf(file,"%sTimeCode.Frames=%c%c\r\n",prefix.c_str(),sTimeCodeNTSC_Ptr->TensOfFrames+'0',sTimeCodeNTSC_Ptr->UnitsOfFrames+'0');
  fprintf(file,"%sTimeCode.Second=%c%c\r\n",prefix.c_str(),sTimeCodeNTSC_Ptr->TensOfSecond+'0',sTimeCodeNTSC_Ptr->UnitsOfSecond+'0');
  fprintf(file,"%sTimeCode.Minutes=%c%c\r\n",prefix.c_str(),sTimeCodeNTSC_Ptr->TensOfMinutes+'0',sTimeCodeNTSC_Ptr->UnitsOfMinutes+'0');
  fprintf(file,"%sTimeCode.Hours=%c%c\r\n",prefix.c_str(),sTimeCodeNTSC_Ptr->TensOfHours+'0',sTimeCodeNTSC_Ptr->UnitsOfHours+'0');
  fprintf(file,"%sTimeCode.CF=%i\r\n",prefix.c_str(),sTimeCodeNTSC_Ptr->CF);
  fprintf(file,"%sTimeCode.PC=%i\r\n",prefix.c_str(),sTimeCodeNTSC_Ptr->PC);
  fprintf(file,"%sTimeCode.BGF0=%i\r\n",prefix.c_str(),sTimeCodeNTSC_Ptr->BGF0);
  fprintf(file,"%sTimeCode.BGF1=%i\r\n",prefix.c_str(),sTimeCodeNTSC_Ptr->BGF1);
  fprintf(file,"%sTimeCode.BGF2=%i\r\n",prefix.c_str(),sTimeCodeNTSC_Ptr->BGF2);
 } 
}


