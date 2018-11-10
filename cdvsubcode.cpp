//====================================================================================================
//подключаемые библиотеки
//====================================================================================================
#include "cdvsubcode.h"

#include <string.h>

//====================================================================================================
//конструктор класса
//====================================================================================================
CDVSubCode::CDVSubCode(void)
{ 
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CDVSubCode::~CDVSubCode()
{
}

//====================================================================================================
//закрытые функции класса
//====================================================================================================

//====================================================================================================
//открытые функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//задать секцию субкодов
//----------------------------------------------------------------------------------------------------
void CDVSubCode::Set(SSubCode *sSubCode_Ptr,uint8_t dif_sequence,uint32_t frame,const CDVTime &cDVTime_CurrentTime,const CDVTime &cDVTime_VideoTime,bool is_pal)
{
 //секция SubCode содержит 2 DIF-блока
 uint8_t syb=0;//номер syb-блока
 for(uint8_t n=0;n<2;n++)
 {
  //заполняем каждый блок DIF
  cDVDIFID.SetSubCodeSectionID(&sSubCode_Ptr->sSubCodeDIF[n].sDIFID,dif_sequence,n);

  uint8_t half=5;//середина DIF-последовательности
  if (is_pal==true) half=6;
  uint8_t fr=1;//FR=1 для первой половины последовательности
  if (dif_sequence>=half) fr=0;
  for(uint8_t m=0;m<6;m++,syb++)
  {
   if (syb==0 || syb==6) 
   {
	sSubCode_Ptr->sSubCodeDIF[n].sSSYB[m].uSSYBID.sSSYBID_0_6.Syb=syb;
	sSubCode_Ptr->sSubCodeDIF[n].sSSYB[m].uSSYBID.sSSYBID_0_6.FR=fr;
	sSubCode_Ptr->sSubCodeDIF[n].sSSYB[m].uSSYBID.sSSYBID_0_6.Arb1=0;
	sSubCode_Ptr->sSubCodeDIF[n].sSSYB[m].uSSYBID.sSSYBID_0_6.Arb2=0;
	sSubCode_Ptr->sSubCodeDIF[n].sSSYB[m].uSSYBID.sSSYBID_0_6.AP3=0;
   }
   if ((syb>=1 && syb<=5) || (syb>=7 && syb<=10))
   {
	sSubCode_Ptr->sSubCodeDIF[n].sSSYB[m].uSSYBID.sSSYBID_1to5_7to10.Syb=syb;
	sSubCode_Ptr->sSubCodeDIF[n].sSSYB[m].uSSYBID.sSSYBID_1to5_7to10.FR=fr;
	sSubCode_Ptr->sSubCodeDIF[n].sSSYB[m].uSSYBID.sSSYBID_1to5_7to10.Arb1=0;
	sSubCode_Ptr->sSubCodeDIF[n].sSSYB[m].uSSYBID.sSSYBID_1to5_7to10.Arb2=0;
	sSubCode_Ptr->sSubCodeDIF[n].sSSYB[m].uSSYBID.sSSYBID_1to5_7to10.Res=0x07;
   }
   if (syb==11)
   {
	sSubCode_Ptr->sSubCodeDIF[n].sSSYB[m].uSSYBID.sSSYBID_11.Syb=syb;
	sSubCode_Ptr->sSubCodeDIF[n].sSSYB[m].uSSYBID.sSSYBID_11.FR=fr;
	sSubCode_Ptr->sSubCodeDIF[n].sSSYB[m].uSSYBID.sSSYBID_11.Arb1=0;
	sSubCode_Ptr->sSubCodeDIF[n].sSSYB[m].uSSYBID.sSSYBID_11.Arb2=0;
	sSubCode_Ptr->sSubCodeDIF[n].sSSYB[m].uSSYBID.sSSYBID_11.APT=0;
   }
   sSubCode_Ptr->sSubCodeDIF[n].sSSYB[m].FFByte=0xFF;
  }
  //в каждом блоке заполняем резервные байты
  memset(sSubCode_Ptr->sSubCodeDIF[n].ResArray,0xFF,29);
 }
 //заполняем dif-блоки   
 memset(sSubCode_Ptr->sSubCodeDIF[0].sSSYB[0].Data,0xFF,5);//0-зарезервировано
 memset(sSubCode_Ptr->sSubCodeDIF[0].sSSYB[1].Data,0xFF,5);//1-зарезервировано
 memset(sSubCode_Ptr->sSubCodeDIF[0].sSSYB[2].Data,0xFF,5);//2-зарезервировано
 //задаём временной код
 CTimeCode::UTimeCode *uTimeCode_Ptr;
 uTimeCode_Ptr=reinterpret_cast<CTimeCode::UTimeCode*>(sSubCode_Ptr->sSubCodeDIF[0].sSSYB[3].Data);
 cTimeCode.Set(uTimeCode_Ptr,cDVTime_VideoTime,frame,is_pal);
 uTimeCode_Ptr=reinterpret_cast<CTimeCode::UTimeCode*>(sSubCode_Ptr->sSubCodeDIF[0].sSSYB[4].Data);
 cTimeCode.Set(uTimeCode_Ptr,cDVTime_VideoTime,frame,is_pal);
 uTimeCode_Ptr=reinterpret_cast<CTimeCode::UTimeCode*>(sSubCode_Ptr->sSubCodeDIF[0].sSSYB[5].Data);
 cTimeCode.Set(uTimeCode_Ptr,cDVTime_VideoTime,frame,is_pal);
 uTimeCode_Ptr=reinterpret_cast<CTimeCode::UTimeCode*>(sSubCode_Ptr->sSubCodeDIF[1].sSSYB[3].Data);
 cTimeCode.Set(uTimeCode_Ptr,cDVTime_VideoTime,frame,is_pal);
 uTimeCode_Ptr=reinterpret_cast<CTimeCode::UTimeCode*>(sSubCode_Ptr->sSubCodeDIF[1].sSSYB[4].Data);
 cTimeCode.Set(uTimeCode_Ptr,cDVTime_VideoTime,frame,is_pal);
 uTimeCode_Ptr=reinterpret_cast<CTimeCode::UTimeCode*>(sSubCode_Ptr->sSubCodeDIF[1].sSSYB[5].Data);
 cTimeCode.Set(uTimeCode_Ptr,cDVTime_VideoTime,frame,is_pal);

 memset(sSubCode_Ptr->sSubCodeDIF[1].sSSYB[0].Data,0xFF,5);//6-зарезервировано
 memset(sSubCode_Ptr->sSubCodeDIF[1].sSSYB[1].Data,0xFF,5);//7-зарезервировано
 memset(sSubCode_Ptr->sSubCodeDIF[1].sSSYB[2].Data,0xFF,5);//8-зарезервировано
}
//----------------------------------------------------------------------------------------------------
//вывести в файл параметры секции
//----------------------------------------------------------------------------------------------------
void CDVSubCode::OutputToFile(FILE *file,const std::string &prefix,bool is_pal,SSubCode *sSubCode_Ptr)
{
 if (file==NULL) return;
 uint8_t syb=0;
 for(uint8_t n=0;n<2;n++)
 {
  CDVDIFID::SDIFID *sDIFID_Ptr=&sSubCode_Ptr->sSubCodeDIF[n].sDIFID;
  cDVDIFID.OutputToFile(file,prefix,sDIFID_Ptr);
  for(uint8_t m=0;m<6;m++,syb++)
  {
   fprintf(file,"%s--------------------\r\n",prefix.c_str());
   fprintf(file,"%sSyb:%i\r\n",prefix.c_str(),syb);
   fprintf(file,"%s--------------------\r\n",prefix.c_str());
   fprintf(file,"%sSubCode.Syb:0x%02x\r\n",prefix.c_str(),sSubCode_Ptr->sSubCodeDIF[n].sSSYB[m].uSSYBID.sSSYBID_0_6.Syb);
   fprintf(file,"%sSubCode.FR:0x%02x\r\n",prefix.c_str(),sSubCode_Ptr->sSubCodeDIF[n].sSSYB[m].uSSYBID.sSSYBID_0_6.FR);
   fprintf(file,"%sSubCode.Arb1:0x%02x\r\n",prefix.c_str(),sSubCode_Ptr->sSubCodeDIF[n].sSSYB[m].uSSYBID.sSSYBID_0_6.Arb1);
   fprintf(file,"%sSubCode.Arb2:0x%02x\r\n",prefix.c_str(),sSubCode_Ptr->sSubCodeDIF[n].sSSYB[m].uSSYBID.sSSYBID_0_6.Arb2);
   fprintf(file,"%sSubCode.AP3:0x%02x\r\n",prefix.c_str(),sSubCode_Ptr->sSubCodeDIF[n].sSSYB[m].uSSYBID.sSSYBID_0_6.AP3);   
   uint8_t id=sSubCode_Ptr->sSubCodeDIF[n].sSSYB[m].Data[0];
   if (id==CTimeCode::TIME_CODE_ID)
   {
    fprintf(file,"%sSubCode.Time Code:\r\n",prefix.c_str());   
    CTimeCode::UTimeCode *uTimeCode_Ptr;
    uTimeCode_Ptr=reinterpret_cast<CTimeCode::UTimeCode*>(sSubCode_Ptr->sSubCodeDIF[n].sSSYB[m].Data);
    cTimeCode.OutputToFile(file,prefix+"\t",is_pal,uTimeCode_Ptr);
    continue;
   }
   if (id==0xff) continue;
   //неизвестный код
   fprintf(file,"%sSubCode.Undefined ID! Data: ",prefix.c_str());
   for(uint8_t b=0;b<5;b++) fprintf(file,"0x%02x ",sSubCode_Ptr->sSubCodeDIF[n].sSSYB[m].Data[b]);
   fprintf(file,"\r\n");
  }
 }
 fprintf(file,"\r\n");
}
//----------------------------------------------------------------------------------------------------
//получить размер секции в байтах
//----------------------------------------------------------------------------------------------------
uint32_t CDVSubCode::GetSectionSize(void)
{
 return(sizeof(SSubCode));
}



