//====================================================================================================
//������������ ����������
//====================================================================================================
#include "cdvdifid.h"

//====================================================================================================
//����������� ������
//====================================================================================================
CDVDIFID::CDVDIFID(void)
{ 
}
//====================================================================================================
//���������� ������
//====================================================================================================
CDVDIFID::~CDVDIFID()
{
}

//====================================================================================================
//�������� ������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//������ ID ���������
//----------------------------------------------------------------------------------------------------
void CDVDIFID::SetID(SDIFID *sDIFID_Ptr,SCT_VALUE sct,uint8_t dif_sequence,uint8_t diff_block_number)
{
 sDIFID_Ptr->SCT=sct;
 sDIFID_Ptr->Res1=1;
 sDIFID_Ptr->Arb=0x0F;//��� �������
 
 sDIFID_Ptr->Dseq=dif_sequence;
 sDIFID_Ptr->FSC=0;
 sDIFID_Ptr->Res2=0x07;//��� �������

 sDIFID_Ptr->DBN=diff_block_number;
}
//====================================================================================================
//�������� ������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//������ ID ������ ���������
//----------------------------------------------------------------------------------------------------
void CDVDIFID::SetHeaderSectionID(SDIFID *sDIFID_Ptr,uint8_t dif_sequence,uint8_t diff_block_number)
{
 SetID(sDIFID_Ptr,SCT_HEADER,dif_sequence,diff_block_number);
}
//----------------------------------------------------------------------------------------------------
//������ ID ������ ���-�����
//----------------------------------------------------------------------------------------------------
void CDVDIFID::SetSubCodeSectionID(SDIFID *sDIFID_Ptr,uint8_t dif_sequence,uint8_t diff_block_number)
{
 SetID(sDIFID_Ptr,SCT_SUBCODE,dif_sequence,diff_block_number);
}
//----------------------------------------------------------------------------------------------------
//������ ID ������ vaux
//----------------------------------------------------------------------------------------------------
void CDVDIFID::SetVAUXSectionID(SDIFID *sDIFID_Ptr,uint8_t dif_sequence,uint8_t diff_block_number)
{
 SetID(sDIFID_Ptr,SCT_VAUX,dif_sequence,diff_block_number);
}
//----------------------------------------------------------------------------------------------------
//������ ID ������ �����
//----------------------------------------------------------------------------------------------------
void CDVDIFID::SetVideoSectionID(SDIFID *sDIFID_Ptr,uint8_t dif_sequence,uint8_t diff_block_number)
{
 SetID(sDIFID_Ptr,SCT_VIDEO,dif_sequence,diff_block_number);
}
//----------------------------------------------------------------------------------------------------
//������ ID ������ �����
//----------------------------------------------------------------------------------------------------
void CDVDIFID::SetAudioSectionID(SDIFID *sDIFID_Ptr,uint8_t dif_sequence,uint8_t diff_block_number)
{
 SetID(sDIFID_Ptr,SCT_AUDIO,dif_sequence,diff_block_number);
}
//----------------------------------------------------------------------------------------------------
//������� � ���� ��������� ���������
//----------------------------------------------------------------------------------------------------
void CDVDIFID::OutputToFile(FILE *file,const std::string &prefix,SDIFID *sDIFID_Ptr)
{
 if (file==NULL) return;
 std::string name="Section Unknow";
 if (sDIFID_Ptr->SCT==SCT_HEADER) name="Section HEADER";
 if (sDIFID_Ptr->SCT==SCT_SUBCODE) name="Section SUBCODE";
 if (sDIFID_Ptr->SCT==SCT_VAUX) name="Section VAUX";
 if (sDIFID_Ptr->SCT==SCT_AUDIO) name="Section AUDIO";
 if (sDIFID_Ptr->SCT==SCT_VIDEO) name="Section VIDEO";
 fprintf(file,"%sDIFF_ID.SCT=0x%02x [%s]\r\n",prefix.c_str(),static_cast<uint8_t>(sDIFID_Ptr->SCT),name.c_str());
 fprintf(file,"%sDIFF_ID.FSC=%i\r\n",prefix.c_str(),sDIFID_Ptr->FSC);
 fprintf(file,"%sDIFF_ID.Dseq=%i\r\n",prefix.c_str(),sDIFID_Ptr->Dseq);
 fprintf(file,"%sDIFF_ID.DBN=%i\r\n",prefix.c_str(),sDIFID_Ptr->DBN);
}

