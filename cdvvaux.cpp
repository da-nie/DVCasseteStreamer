//====================================================================================================
//подключаемые библиотеки
//====================================================================================================
#include "cdvvaux.h"

#include <string.h>

//====================================================================================================
//конструктор класса
//====================================================================================================
CDVVAUX::CDVVAUX(void)
{ 
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CDVVAUX::~CDVVAUX()
{
}

//====================================================================================================
//закрытые функции класса
//====================================================================================================

//====================================================================================================
//открытые функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//задать секцию VAUX
//----------------------------------------------------------------------------------------------------
void CDVVAUX::Set(SVAUX *sVAUX_Ptr,uint8_t dif_sequence,const CDVTime &cDVTime_Current,bool is_pal)
{
 //секци€ VAUX содержит 3 DIF-блока
 uint8_t block=0;
 for(uint8_t n=0;n<3;n++)
 {
  //заполн€ем каждый блок DIF
  cDVDIFID.SetVAUXSectionID(&sVAUX_Ptr->sVAUXDIF[n].sDIFID,dif_sequence,n);

  sVAUX_Ptr->sVAUXDIF[n].ReservedArray[0]=0xff;
  sVAUX_Ptr->sVAUXDIF[n].ReservedArray[1]=0xff;
  //заполним все блоки данных 0xff (блок не используетс€)
  for(uint8_t m=0;m<15;m++,block++)
  {
   UVAUXPayload *uVAUXPayload_Ptr=&sVAUX_Ptr->sVAUXDIF[n].uVAUXPayload[m];
   memset(uVAUXPayload_Ptr->Data,0xff,5);
   //заполн€ем блоки
   bool set_vs=false;//нужно ли задавать блок как vs
   bool set_vsc=false;//нужно ли задавать блок как vsc
   bool set_dateymd=false;//нужно ли задавать блок как год-мес€ц-число
   bool set_datehms=false;//нужно ли задавать блок как час-минута-секунда
   if ((dif_sequence&1)==0)//чЄтный ds
   {
    if (block==39) set_vs=true;//заполн€ем vs
    if (block==40) set_vsc=true;//заполн€ем vsc
	if (block==41) set_dateymd=true;//заполн€ем год-мес€ц-число
	if (block==42) set_datehms=true;//заполн€ем час-минута-секунда	 
   }
   else//нечЄтный ds
   { 
    if (block==0) set_vs=true;//заполн€ем vs
    if (block==1) set_vsc=true;//заполн€ем vsc
	if (block==2) set_dateymd=true;//заполн€ем год-мес€ц-число
	if (block==3) set_datehms=true;//заполн€ем час-минута-секунда	 
   }
   if (set_vs==true)//задаЄм блок vs
   {
    uVAUXPayload_Ptr->sVAUXVS.ID=VAUX_ID_VS;
    uVAUXPayload_Ptr->sVAUXVS.Res1=0xff;
    uVAUXPayload_Ptr->sVAUXVS.Res2=0x0f;
    uVAUXPayload_Ptr->sVAUXVS.Res3=0x03;
    uVAUXPayload_Ptr->sVAUXVS.CLF=CLF_PAL_7_8_FIELD;
    uVAUXPayload_Ptr->sVAUXVS.EN=EN_CLF_IS_INVALID;
    uVAUXPayload_Ptr->sVAUXVS.BW=BW_COLOR;
    uVAUXPayload_Ptr->sVAUXVS.STYPE=STYPE_4_1_1_COMPRESSION;
    uVAUXPayload_Ptr->sVAUXVS.F50_60=F50;
    if (is_pal==false) uVAUXPayload_Ptr->sVAUXVS.F50_60=F60;
    uVAUXPayload_Ptr->sVAUXVS.VISC=VISC_NO_INFO;
   }
   if (set_vsc==true)//задаЄм блок vsc
   {
    uVAUXPayload_Ptr->sVAUXVSC.ID=VAUX_ID_VSC;
    uVAUXPayload_Ptr->sVAUXVSC.Res1=0x3f;//все биты 1
    uVAUXPayload_Ptr->sVAUXVSC.Res2=1;
    uVAUXPayload_Ptr->sVAUXVSC.Res3=0x03;
    uVAUXPayload_Ptr->sVAUXVSC.Res4=0x03;
    uVAUXPayload_Ptr->sVAUXVSC.Res5=0xff;
    uVAUXPayload_Ptr->sVAUXVSC.CGMS=CGMS_COPY_FREE;
    uVAUXPayload_Ptr->sVAUXVSC.DISP=DISP_4_3;
    uVAUXPayload_Ptr->sVAUXVSC.Zero1=0;
    uVAUXPayload_Ptr->sVAUXVSC.Zero2=0;
    uVAUXPayload_Ptr->sVAUXVSC.IL=IL_INTERLACE;
    uVAUXPayload_Ptr->sVAUXVSC.FC=FC_DIFFERENT_PREVIOUS_FRAME;
    uVAUXPayload_Ptr->sVAUXVSC.FS=FS_FIELD_1;
    if (dif_sequence>=6) uVAUXPayload_Ptr->sVAUXVSC.FS=FS_FIELD_2;
    uVAUXPayload_Ptr->sVAUXVSC.FF=FF_NO_TWICE;
   }
   if (set_dateymd==true)//задаЄм блок год-мес€ц-число
   {
    CDateYMDCode::SDateYMDCode *sDateYMDCode_Ptr;
    sDateYMDCode_Ptr=reinterpret_cast<CDateYMDCode::SDateYMDCode*>(uVAUXPayload_Ptr->Data);    
	cDateYMDCode.SetVideo(sDateYMDCode_Ptr,cDVTime_Current);
   }
   if (set_datehms==true)//задаЄм блок час-минута-секунда
   {
    CDateHMSCode::SDateHMSCode *sDateHMSCode_Ptr;
    sDateHMSCode_Ptr=reinterpret_cast<CDateHMSCode::SDateHMSCode*>(uVAUXPayload_Ptr->Data);    
	cDateHMSCode.SetVideo(sDateHMSCode_Ptr,cDVTime_Current);
   }
  }
 }
}
//----------------------------------------------------------------------------------------------------
//вывести в файл параметры секции
//----------------------------------------------------------------------------------------------------
void CDVVAUX::OutputToFile(FILE *file,const std::string &prefix,SVAUX *sVAUX_Ptr)
{
 if (file==NULL) return;
 uint8_t block=0;
 for(uint8_t n=0;n<3;n++)
 {
  CDVDIFID::SDIFID *sDIFID_Ptr=&sVAUX_Ptr->sVAUXDIF[n].sDIFID;
  cDVDIFID.OutputToFile(file,prefix,sDIFID_Ptr);
  for(uint8_t m=0;m<15;m++,block++)
  {
   fprintf(file,"%s--------------------\r\n",prefix.c_str());
   fprintf(file,"%sBlock:%i\r\n",prefix.c_str(),block);
   fprintf(file,"%s--------------------\r\n",prefix.c_str());
   UVAUXPayload *uVAUXPayload_Ptr=&sVAUX_Ptr->sVAUXDIF[n].uVAUXPayload[m];
   uint8_t id=uVAUXPayload_Ptr->Data[0];
   if (id==VAUX_ID_VS)
   {   
    fprintf(file,"%sVAUX.VS Payload:\r\n",prefix.c_str());
    fprintf(file,"%s\tVAUX.CLF:%i\r\n",prefix.c_str(),uVAUXPayload_Ptr->sVAUXVS.CLF);
    fprintf(file,"%s\tVAUX.EN:%i\r\n",prefix.c_str(),uVAUXPayload_Ptr->sVAUXVS.EN);
    fprintf(file,"%s\tVAUX.BW:%i\r\n",prefix.c_str(),uVAUXPayload_Ptr->sVAUXVS.BW);
    fprintf(file,"%s\tVAUX.STYPE:%i\r\n",prefix.c_str(),uVAUXPayload_Ptr->sVAUXVS.STYPE);
    fprintf(file,"%s\tVAUX.F50_60:%i\r\n",prefix.c_str(),uVAUXPayload_Ptr->sVAUXVS.F50_60);
    fprintf(file,"%s\tVAUX.VISC:%i\r\n",prefix.c_str(),uVAUXPayload_Ptr->sVAUXVS.VISC);
    continue;
   }
   if (id==VAUX_ID_VSC)
   {
    fprintf(file,"%s\tVAUX.VSC Payload:\r\n",prefix.c_str());
    fprintf(file,"%s\tVAUX.CGMS:%i\r\n",prefix.c_str(),uVAUXPayload_Ptr->sVAUXVSC.CGMS);
    fprintf(file,"%s\tVAUX.DISP:%i\r\n",prefix.c_str(),uVAUXPayload_Ptr->sVAUXVSC.DISP);
    fprintf(file,"%s\tVAUX.IL:%i\r\n",prefix.c_str(),uVAUXPayload_Ptr->sVAUXVSC.IL);
    fprintf(file,"%s\tVAUX.FC:%i\r\n",prefix.c_str(),uVAUXPayload_Ptr->sVAUXVSC.FC);
    fprintf(file,"%s\tVAUX.FS:%i\r\n",prefix.c_str(),uVAUXPayload_Ptr->sVAUXVSC.FS);
    fprintf(file,"%s\tVAUX.FF:%i\r\n",prefix.c_str(),uVAUXPayload_Ptr->sVAUXVSC.FF);
    continue;
   }
   if (id==CDateYMDCode::DATE_YMD_CODE_VIDEO_ID)
   {
    fprintf(file,"%sVAUX.Date Year-Month-Day Code:\r\n",prefix.c_str());   
    CDateYMDCode::SDateYMDCode *sDateYMDCode_Ptr;
    sDateYMDCode_Ptr=reinterpret_cast<CDateYMDCode::SDateYMDCode*>(uVAUXPayload_Ptr->Data);
    cDateYMDCode.OutputToFile(file,prefix+"\t",sDateYMDCode_Ptr);
	continue;
   }
   if (id==CDateHMSCode::DATE_HMS_CODE_VIDEO_ID)
   {
    fprintf(file,"%sVAUX.Date Hour-Min-Sec Code:\r\n",prefix.c_str());   
    CDateHMSCode::SDateHMSCode *sDateHMSCode_Ptr;
    sDateHMSCode_Ptr=reinterpret_cast<CDateHMSCode::SDateHMSCode*>(uVAUXPayload_Ptr->Data);
    cDateHMSCode.OutputToFile(file,prefix+"\t",sDateHMSCode_Ptr);
    continue;
   }
   if (id==0xff) continue;
   //неизвестный код
   fprintf(file,"%sVAUX.Undefined ID! Data: ",prefix.c_str());
   for(uint8_t b=0;b<5;b++) fprintf(file,"0x%02x ",uVAUXPayload_Ptr->Data[b]);
   fprintf(file,"\r\n");   
  }
 } 
 fprintf(file,"\r\n");
}
//----------------------------------------------------------------------------------------------------
//получить размер секции в байтах
//----------------------------------------------------------------------------------------------------
uint32_t CDVVAUX::GetSectionSize(void)
{
 return(sizeof(SVAUX));
}
