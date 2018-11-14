#ifndef C_DV_DIF_ID_H
#define C_DV_DIF_ID_H

//====================================================================================================
//����� ������ � ID DIF-�����
//====================================================================================================

//====================================================================================================
//������������ ����������
//====================================================================================================

#include <stdint.h>
#include <stdio.h>
#include <string>

//====================================================================================================
//����� ������ � ID DIF-�����
//====================================================================================================
class CDVDIFID
{
 public:
  //-������������---------------------------------------------------------------------------------------
  //���� ������
  enum SCT_VALUE
  {
   SCT_HEADER=0,//���������
   SCT_SUBCODE=1,//���-����
   SCT_VAUX=2,//������ VAUX
   SCT_AUDIO=3,//������ �����
   SCT_VIDEO=4//������ �����
  };
  //-���������------------------------------------------------------------------------------------------
  //���� ������ ����� ��������� ����������� �� �������� � ��������! � ������������ ��� ��������� ��������!
  #pragma pack(1)
  //������������� DIF-�����
  struct SDIFID
  {
   //���� 0
   uint8_t Arb:4;//������������ ������
   uint8_t Res1:1;//��������������� (������ ��� ���� 1)
   uint8_t SCT:3;//��� ������
   //���� 1
   uint8_t Res2:3;//��������������� (������ ��� ���� 1)
   uint8_t FSC:1;//��� 50 ����: 0-������ �����, 1-������; ��� 25 ���� ������ 0
   uint8_t Dseq:4;//������������� DIF-����� � ������ ������
   //���� 2
   uint8_t DBN;//����� DIF-�����
  };
  #pragma pack()
 private:
  //-����������-----------------------------------------------------------------------------------------
  static const int32_t PAL_FRAME_RATE=25;//������� ������ � ������ PAL
  static const int32_t NTSC_FRAME_RATE=30;//������� ������ � ������ NTSC
public:
  //-����������� ������---------------------------------------------------------------------------------
  CDVDIFID(void);
  //-���������� ������----------------------------------------------------------------------------------
  ~CDVDIFID();
 public:
  //-�������� ������� ������----------------------------------------------------------------------------  
  void SetHeaderSectionID(SDIFID *sDIFID_Ptr,uint8_t dif_sequence,uint8_t diff_block_number);//������ ID ������ ���������
  void SetSubCodeSectionID(SDIFID *sDIFID_Ptr,uint8_t dif_sequence,uint8_t diff_block_number);//������ ID ������ ���-�����
  void SetVAUXSectionID(SDIFID *sDIFID_Ptr,uint8_t dif_sequence,uint8_t diff_block_number);//������ ID ������ vaux
  void SetVideoSectionID(SDIFID *sDIFID_Ptr,uint8_t dif_sequence,uint8_t diff_block_number);//������ ID ������ �����
  void SetAudioSectionID(SDIFID *sDIFID_Ptr,uint8_t dif_sequence,uint8_t diff_block_number);//������ ID ������ �����
  void OutputToFile(FILE *file,const std::string &prefix,SDIFID *sDIFID_Ptr);//������� � ���� ��������� ���������
 private:
  //-�������� ������� ������----------------------------------------------------------------------------  
  void SetID(SDIFID *sDIFID_Ptr,SCT_VALUE sct,uint8_t dif_sequence,uint8_t diff_block_number);//������ ID ���������
};

#endif
