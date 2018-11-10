//====================================================================================================
//подключаемые библиотеки
//====================================================================================================
#include "cdvaudio.h"

#include <string.h>

//====================================================================================================
//конструктор класса
//====================================================================================================
CDVAudio::CDVAudio(void)
{ 
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CDVAudio::~CDVAudio()
{
}

//====================================================================================================
//закрытые функции класса
//====================================================================================================

//====================================================================================================
//открытые функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//задать секцию Video
//----------------------------------------------------------------------------------------------------
void CDVAudio::Set(SAudio *sAudio_Ptr,uint8_t dif_sequence,uint8_t block_number,const CDVTime &cDVTime_Current,bool is_pal)
{
 //заполняем каждый блок DIF
 memset(sAudio_Ptr,0xff,sizeof(SAudio));
 cDVDIFID.SetAudioSectionID(&sAudio_Ptr->sDIFID,dif_sequence,block_number);
 bool set_as=false;
 bool set_asc=false;
 bool set_dateymd=false;
 bool set_datehms=false;
 if ((dif_sequence%2)==0)//чётный элемент
 {
  if (block_number==3) set_as=true;
  if (block_number==4) set_asc=true;
  if (block_number==5) set_dateymd=true;
  if (block_number==6) set_datehms=true;
 }
 else//нечётный элемент
 {
  if (block_number==0) set_as=true;
  if (block_number==1) set_asc=true;
  if (block_number==2) set_dateymd=true;
  if (block_number==3) set_datehms=true;
 }
 UAAUXPayload *uAAUXPayload_Ptr=&sAudio_Ptr->sDIFAudioData.uAAUXPayload;

 if (set_as==true)
 {
  uAAUXPayload_Ptr->sAAUX_AS.ID=AAUX_ID_VS;
  uAAUXPayload_Ptr->sAAUX_AS.AFSIZE=AFSIZE_1602_SAMPLES;
  if (is_pal==true) uAAUXPayload_Ptr->sAAUX_AS.AFSIZE=AFSIZE_1920_SAMPLES;

  uAAUXPayload_Ptr->sAAUX_AS.AudioMode=AUDIOMODE_CH1_CH3;
  uAAUXPayload_Ptr->sAAUX_AS.CHN=CHN_1;
  
  uAAUXPayload_Ptr->sAAUX_AS.F50_60=F60;
  if (is_pal==true) uAAUXPayload_Ptr->sAAUX_AS.F50_60=F50;

  uAAUXPayload_Ptr->sAAUX_AS.LF=LF_RESERVER;
  uAAUXPayload_Ptr->sAAUX_AS.QU=QU_16;
  uAAUXPayload_Ptr->sAAUX_AS.SMP=SMP_48000;
  uAAUXPayload_Ptr->sAAUX_AS.STYPE=STYPE_2;
  uAAUXPayload_Ptr->sAAUX_AS.Zero=0;
  uAAUXPayload_Ptr->sAAUX_AS.Res1=0x01;
  uAAUXPayload_Ptr->sAAUX_AS.Res2=0x01;
  uAAUXPayload_Ptr->sAAUX_AS.Res3=0x03;
  uAAUXPayload_Ptr->sAAUX_AS.Res4=0x03;
 }
 if (set_asc==true)
 {
  uAAUXPayload_Ptr->sAAUX_ASC.ID=AAUX_ID_VSC;
  uAAUXPayload_Ptr->sAAUX_ASC.CGMS=CGMS_COPY_FREE;
  uAAUXPayload_Ptr->sAAUX_ASC.DRF=DRF_FORWARD;
  uAAUXPayload_Ptr->sAAUX_ASC.EFC=EFC_EMPHASSIS_RESERVED;
  uAAUXPayload_Ptr->sAAUX_ASC.FADE_END=FADE_END_OFF;
  uAAUXPayload_Ptr->sAAUX_ASC.FADE_ST=FADE_ST_OFF;
  uAAUXPayload_Ptr->sAAUX_ASC.REC_END=REC_END_ON;
  uAAUXPayload_Ptr->sAAUX_ASC.REC_ST=REC_ST_ON;
  uAAUXPayload_Ptr->sAAUX_ASC.SPEED=32;//в единицах 1/120
  uAAUXPayload_Ptr->sAAUX_ASC.Res1=0x0f;
  uAAUXPayload_Ptr->sAAUX_ASC.Res2=0x0f;
  uAAUXPayload_Ptr->sAAUX_ASC.Res3=0xff;
 }
 if (set_dateymd==true)
 {
  CDateYMDCode::SDateYMDCode *sDateYMDCode_Ptr;
  sDateYMDCode_Ptr=reinterpret_cast<CDateYMDCode::SDateYMDCode*>(uAAUXPayload_Ptr->Data);    
  cDateYMDCode.SetAudio(sDateYMDCode_Ptr,cDVTime_Current);
 }
 if (set_datehms==true)//задаём блок час-минута-секунда
 {
  CDateHMSCode::SDateHMSCode *sDateHMSCode_Ptr;
  sDateHMSCode_Ptr=reinterpret_cast<CDateHMSCode::SDateHMSCode*>(uAAUXPayload_Ptr->Data);    
  cDateHMSCode.SetAudio(sDateHMSCode_Ptr,cDVTime_Current);
 }
}
//----------------------------------------------------------------------------------------------------
//вывести в файл параметры секции
//----------------------------------------------------------------------------------------------------
void CDVAudio::OutputToFile(FILE *file,const std::string &prefix,SAudio *sAudio_Ptr)
{
 if (file==NULL) return;
 uint8_t block=0;
 CDVDIFID::SDIFID *sDIFID_Ptr=&sAudio_Ptr->sDIFID;
 cDVDIFID.OutputToFile(file,prefix,sDIFID_Ptr);
 UAAUXPayload *uAAUXPayload_Ptr=&sAudio_Ptr->sDIFAudioData.uAAUXPayload;
 uint8_t id=uAAUXPayload_Ptr->Data[0];
 if (id==AAUX_ID_VS)
 {
  fprintf(file,"%sAudio.VS Payload:\r\n",prefix.c_str());
  fprintf(file,"%s\tAAUX.AFSIZE:%i\r\n",prefix.c_str(),uAAUXPayload_Ptr->sAAUX_AS.AFSIZE);
  fprintf(file,"%s\tAAUX.LF:%i\r\n",prefix.c_str(),uAAUXPayload_Ptr->sAAUX_AS.LF);
  fprintf(file,"%s\tAAUX.AudioMode:%i\r\n",prefix.c_str(),uAAUXPayload_Ptr->sAAUX_AS.AudioMode);
  fprintf(file,"%s\tAAUX.CHN:%i\r\n",prefix.c_str(),uAAUXPayload_Ptr->sAAUX_AS.CHN);
  fprintf(file,"%s\tAAUX.STYPE:%i\r\n",prefix.c_str(),uAAUXPayload_Ptr->sAAUX_AS.STYPE);
  fprintf(file,"%s\tAAUX.F50_60:%i\r\n",prefix.c_str(),uAAUXPayload_Ptr->sAAUX_AS.F50_60);
  fprintf(file,"%s\tAAUX.QU:%i\r\n",prefix.c_str(),uAAUXPayload_Ptr->sAAUX_AS.QU);
  fprintf(file,"%s\tAAUX.SMP:%i\r\n",prefix.c_str(),uAAUXPayload_Ptr->sAAUX_AS.SMP);
  return;
 }
 if (id==AAUX_ID_VSC)
 {
  fprintf(file,"%s\tAAUX.VSC Payload:\r\n",prefix.c_str());  
  fprintf(file,"%s\tAAUX.EFC:%i\r\n",prefix.c_str(),uAAUXPayload_Ptr->sAAUX_ASC.EFC);
  fprintf(file,"%s\tAAUX.CGMS:%i\r\n",prefix.c_str(),uAAUXPayload_Ptr->sAAUX_ASC.CGMS);
  fprintf(file,"%s\tAAUX.FADE_END:%i\r\n",prefix.c_str(),uAAUXPayload_Ptr->sAAUX_ASC.FADE_END);
  fprintf(file,"%s\tAAUX.FADE_ST:%i\r\n",prefix.c_str(),uAAUXPayload_Ptr->sAAUX_ASC.FADE_ST);
  fprintf(file,"%s\tAAUX.REC_END:%i\r\n",prefix.c_str(),uAAUXPayload_Ptr->sAAUX_ASC.REC_END);
  fprintf(file,"%s\tAAUX.REC_ST:%i\r\n",prefix.c_str(),uAAUXPayload_Ptr->sAAUX_ASC.REC_ST);
  fprintf(file,"%s\tAAUX.SPEED:%i\r\n",prefix.c_str(),uAAUXPayload_Ptr->sAAUX_ASC.SPEED);
  fprintf(file,"%s\tAAUX.DRF:%i\r\n",prefix.c_str(),uAAUXPayload_Ptr->sAAUX_ASC.DRF);
  return;
 }
 if (id==CDateYMDCode::DATE_YMD_CODE_AUDIO_ID)
 {
  fprintf(file,"%sAAUX.Date Year-Month-Day Code:\r\n",prefix.c_str());   
  CDateYMDCode::SDateYMDCode *sDateYMDCode_Ptr;
  sDateYMDCode_Ptr=reinterpret_cast<CDateYMDCode::SDateYMDCode*>(uAAUXPayload_Ptr->Data);
  cDateYMDCode.OutputToFile(file,prefix+"\t",sDateYMDCode_Ptr);
  return;
 }
 if (id==CDateHMSCode::DATE_HMS_CODE_AUDIO_ID)
 {
  fprintf(file,"%sAAUX.Date Hour-Min-Sec Code:\r\n",prefix.c_str());   
  CDateHMSCode::SDateHMSCode *sDateHMSCode_Ptr;
  sDateHMSCode_Ptr=reinterpret_cast<CDateHMSCode::SDateHMSCode*>(uAAUXPayload_Ptr->Data);
  cDateHMSCode.OutputToFile(file,prefix+"\t",sDateHMSCode_Ptr);
  return;
 }
 if (id==0xff) return;
 //неизвестный код
 fprintf(file,"%sAudio.Undefined ID! Data: ",prefix.c_str());
 for(uint8_t b=0;b<5;b++) fprintf(file,"0x%02x ",uAAUXPayload_Ptr->Data[b]);
 fprintf(file,"\r\n");
 fprintf(file,"\r\n");
}
//----------------------------------------------------------------------------------------------------
//получить размер секции в байтах
//----------------------------------------------------------------------------------------------------
uint32_t CDVAudio::GetSectionSize(void)
{
 return(sizeof(SAudio));
}
