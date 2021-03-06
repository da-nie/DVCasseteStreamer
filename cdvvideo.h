#ifndef C_DV_VIDEO_H
#define C_DV_VIDEO_H

//====================================================================================================
//����� ������ � ������� video ��� dv-�������
//====================================================================================================

//====================================================================================================
//������������ ����������
//====================================================================================================

#include <stdint.h>

#include "cdvdifid.h"

//====================================================================================================
//����� ������ � ������� video ��� dv-�������
//====================================================================================================
class CDVVideo
{
 public:
  //-������������---------------------------------------------------------------------------------------
  //-���������------------------------------------------------------------------------------------------
  //���� ������ ����� ��������� ����������� �� �������� � ��������! � ������������ ��� ��������� ��������!
  #pragma pack(1)
  //������ �����
  struct SVideo
  {
   CDVDIFID::SDIFID sDIFID;//������������� DIF-�����
   uint8_t VideoData[80-sizeof(CDVDIFID::SDIFID)];//������ �����
  };
  #pragma pack()
 private:
  //-����������-----------------------------------------------------------------------------------------  
  CDVDIFID cDVDIFID;
 public:
  //-����������� ������---------------------------------------------------------------------------------
  CDVVideo(void);
  //-���������� ������----------------------------------------------------------------------------------
  ~CDVVideo();
 public:
  //-�������� ������� ������----------------------------------------------------------------------------
  void Set(SVideo *sVideo_Ptr,uint8_t dif_sequence,uint8_t block_number);//������ ������ Video
  void OutputToFile(FILE *file,const std::string &prefix,SVideo *sVideo_Ptr);//������� � ���� ��������� ������
  static uint32_t GetSectionSize(void);//�������� ������ ������ � ������
  //-�������� ������� ������----------------------------------------------------------------------------
};

#endif
