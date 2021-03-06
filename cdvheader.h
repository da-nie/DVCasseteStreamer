#ifndef C_DV_HEADER_H
#define C_DV_HEADER_H

//====================================================================================================
//����� ������ � ������� header ��� dv-�������
//====================================================================================================

//====================================================================================================
//������������ ����������
//====================================================================================================

#include <stdint.h>
#include "cdvdifid.h"

//====================================================================================================
//����� ������ � ������� header ��� dv-�������
//====================================================================================================
class CDVHeader
{
 public:
  //-������������---------------------------------------------------------------------------------------
  //�������� ���� DFS
  enum DFS_VALUE
  {
   DFS_NTSC=0,
   DFS_PAL=1
  };
  //-���������------------------------------------------------------------------------------------------
  //���� ������ ����� ��������� ����������� �� �������� � ��������! � ������������ ��� ��������� ��������!
  #pragma pack(1) 
  //������ DIF-����� � ������ ���������
  struct SDIFHeaderData
  {
   //���� 3
   uint8_t Res1:6;//��������������� (��� ���� � 1)
   uint8_t Zero:1;
   uint8_t DFS:1;//���� ������������������ ������ (0- NTSC,1-PAL)
   //���� 4
   uint8_t APT:3;//������������� ����� ��� ��������� �����������, ��� ���� ���������� ������ ��� ���� � 1
   uint8_t Res2:5;//��������������� (��� ���� � 1)
   //���� 5
   uint8_t AP1:3;//������������� ����� ��� ��������� �����������, ��� ���� ���������� ������ ��� ���� � 1
   uint8_t ResTF1:4;//��������������� (��� ���� � 1)
   uint8_t TF1:1;//���� �������� ����� DIF-������ (0-������ ����)
   //���� 6
   uint8_t AP2:3;//������������� ����� ��� ��������� �����������, ��� ���� ���������� ������ ��� ���� � 1
   uint8_t ResTF2:4;//��������������� (��� ���� � 1)
   uint8_t TF2:1;//���� �������� VAUX � ����� DIF-������ (0-������ ����)
   //���� 7
   uint8_t AP3:3;
   uint8_t ResTF3:4;//��������������� (��� ���� � 1)
   uint8_t TF3:1;//���� �������� ���-������� DIF-������ (0-������ ����)
   //����� 8..79
   uint8_t ResArray[80-8];//��������������� (��� ���� � 1)
  };
  struct SHeader
  {
   CDVDIFID::SDIFID sDIFID;
   SDIFHeaderData sDIFHeaderData;
  };
  #pragma pack()
 private:
  //-����������-----------------------------------------------------------------------------------------  
  CDVDIFID cDVDIFID; 
 public:
  //-����������� ������---------------------------------------------------------------------------------
  CDVHeader(void);
  //-���������� ������----------------------------------------------------------------------------------
  ~CDVHeader();
 public:
  //-�������� ������� ������----------------------------------------------------------------------------
  void Set(SHeader *sHeader_Ptr,uint8_t dif_sequence,bool is_pal);//������ ������ ���������
  void OutputToFile(FILE *file,const std::string &prefix,SHeader *sHeader_Ptr);//������� � ���� ��������� ������
  static uint32_t GetSectionSize(void);//�������� ������ ������ � ������
  bool IsPAL(SHeader *sHeader_Ptr,bool &is_pal);//������, �������� �� ��������� ���������� ������ PAL
  //-�������� ������� ������----------------------------------------------------------------------------
};

#endif
