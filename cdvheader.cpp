//====================================================================================================
//подключаемые библиотеки
//====================================================================================================
#include "cdvheader.h"

#include <string.h>

//====================================================================================================
//конструктор класса
//====================================================================================================
CDVHeader::CDVHeader(void)
{ 
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CDVHeader::~CDVHeader()
{
}

//====================================================================================================
//закрытые функции класса
//====================================================================================================

//====================================================================================================
//открытые функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//задать секцию заголовка
//----------------------------------------------------------------------------------------------------
void CDVHeader::Set(SHeader *sHeader_Ptr,uint8_t dif_sequence,bool is_pal)
{
 //секция заголовка содержит один DIF-блок
 cDVDIFID.SetHeaderSectionID(&sHeader_Ptr->sDIFID,dif_sequence,0);
 
 memset(sHeader_Ptr->sDIFHeaderData.ResArray,0xFF,80-8);
 sHeader_Ptr->sDIFHeaderData.Res1=0x3F;//все биты в 1
 sHeader_Ptr->sDIFHeaderData.Res2=0x1F;//все биты в 1
 sHeader_Ptr->sDIFHeaderData.ResTF1=0x0F;//все биты в 1
 sHeader_Ptr->sDIFHeaderData.ResTF2=0x0F;//все биты в 1
 sHeader_Ptr->sDIFHeaderData.ResTF3=0x0F;//все биты в 1

 sHeader_Ptr->sDIFHeaderData.DFS=DFS_NTSC;
 if (is_pal==true) sHeader_Ptr->sDIFHeaderData.DFS=DFS_PAL;
 sHeader_Ptr->sDIFHeaderData.Zero=0;
 sHeader_Ptr->sDIFHeaderData.TF1=0;
 sHeader_Ptr->sDIFHeaderData.TF2=0;
 sHeader_Ptr->sDIFHeaderData.TF3=0;

 sHeader_Ptr->sDIFHeaderData.APT=0;
 sHeader_Ptr->sDIFHeaderData.AP1=0;
 sHeader_Ptr->sDIFHeaderData.AP2=0;
 sHeader_Ptr->sDIFHeaderData.AP3=0;
}
//----------------------------------------------------------------------------------------------------
//вывести в файл параметры секции
//----------------------------------------------------------------------------------------------------
void CDVHeader::OutputToFile(FILE *file,const std::string &prefix,SHeader *sHeader_Ptr)
{
 if (file==NULL) return;
 cDVDIFID.OutputToFile(file,prefix,&sHeader_Ptr->sDIFID);
 fprintf(file,"%sHeader.DFS=%i\r\n",prefix.c_str(),sHeader_Ptr->sDIFHeaderData.DFS);
 fprintf(file,"%sHeader.TF1=%i\r\n",prefix.c_str(),sHeader_Ptr->sDIFHeaderData.TF1);
 fprintf(file,"%sHeader.TF2=%i\r\n",prefix.c_str(),sHeader_Ptr->sDIFHeaderData.TF2);
 fprintf(file,"%sHeader.TF3=%i\r\n",prefix.c_str(),sHeader_Ptr->sDIFHeaderData.TF3);
 fprintf(file,"%sHeader.APT=%i\r\n",prefix.c_str(),sHeader_Ptr->sDIFHeaderData.APT);
 fprintf(file,"%sHeader.AP1=%i\r\n",prefix.c_str(),sHeader_Ptr->sDIFHeaderData.AP1);
 fprintf(file,"%sHeader.AP2=%i\r\n",prefix.c_str(),sHeader_Ptr->sDIFHeaderData.AP2);
 fprintf(file,"%sHeader.AP3=%i\r\n",prefix.c_str(),sHeader_Ptr->sDIFHeaderData.AP3);
 fprintf(file,"\r\n");
}
//----------------------------------------------------------------------------------------------------
//получить размер секции в байтах
//----------------------------------------------------------------------------------------------------
uint32_t CDVHeader::GetSectionSize(void)
{
 return(sizeof(SHeader));
}

