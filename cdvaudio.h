#ifndef C_DV_AUDIO_H
#define C_DV_AUDIO_H

//====================================================================================================
//����� ������ � ������� audio ��� dv-�������
//====================================================================================================

//====================================================================================================
//������������ ����������
//====================================================================================================

#include <stdint.h>

#include "cdvdifid.h"
#include "cdvtime.h"
#include "cdateymdcode.h"
#include "cdatehmscode.h"

//====================================================================================================
//����� ������ � ������� audio ��� dv-�������
//====================================================================================================
class CDVAudio
{
 public:
  //-������������---------------------------------------------------------------------------------------
  //�������� ��������������
  enum AAUX_ID_VALUE
  {
   AAUX_ID_VS=0x50,
   AAUX_ID_VSC=0x51   
  };
  //�������� �����������
  enum AUDIOMODE_VALUE
  {
   AUDIOMODE_CH1_CH3=0x00,
   AUDIOMODE_CH2_CH4=0x01,
   AUDIOMODE_INVALID=0x0f   
  };
  //�������� ������� ������
  enum F50_60_VALUE
  {
   F50=0x01,
   F60=0x00
  };
  //�������� ���������� �������� �� ����
  enum AFSIZE_VALUE
  {
   AFSIZE_1600_SAMPLES=0x14,//525/60
   AFSIZE_1602_SAMPLES=0x16,//525/60
   AFSIZE_1920_SAMPLES=0x18//625/50
  };
  //�������� ���������� ����� ������ � �����
  enum STYPE_VALUE
  {
   STYPE_2=0x00,
   STYPE_4=0x02
  };
  //�������� ���������� ������� � ����� �����
  enum CHN_VALUE
  {
   CHN_1=0x00
  };
  //�������� ������ �������������
  enum SMP
  {
   SMP_48000=0x00
  };
  //�������� ����������� ������
  enum QU_VALUE
  {
   QU_16=0x00
  };
  //�������� ���������� �� �����������
  enum CGMS_VALUE
  {
   CGMS_COPY_FREE=0x00
  };
  //�������� ����������
  enum LF_VALUE
  {
   LF_ENABLED=0x00,
   LF_RESERVER=0x01
  };
  //�������� "�������" ��� ����� �����
  enum EFC_VALUE
  {
   EFC_EMPHASSIS_OFF=0x00,
   EFC_EMPHASSIS_ON=0x01,
   EFC_EMPHASSIS_RESERVED=0x03
  };
  //�������� ��������� ����� ������
  enum RECT_ST_VALUE
  {   
   REC_ST_OFF=0x00,
   REC_ST_ON=0x01
  };
  //�������� �������� ����� ������
  enum RECT_END_VALUE
  {   
   REC_END_OFF=0x00,
   REC_END_ON=0x01
  };
  //�������� ��������� ����� � ��������� ����� ������
  enum FADE_ST_VALUE
  {
   FADE_ST_OFF=0x00,
   FADE_ST_ON=0x01
  };
  //�������� ��������� ����� � �������� ����� ������
  enum FADE_END_VALUE
  {
   FADE_END_OFF=0x00,
   FADE_END_ON=0x01   
  };
  //�������� �����������
  enum DRF_VALUE
  {
   DRF_REVERSE=0x00,
   DRF_FORWARD=0x01
  };
  //-���������------------------------------------------------------------------------------------------
  //���� ������ ����� ��������� ����������� �� �������� � ��������! � ������������ ��� ��������� ��������!
  #pragma pack(1)
  //������ AS (audio source pack)
  struct SAAUX_AS
  {
   //���� 0
   uint8_t ID;//�������������
   //���� 1
   uint8_t AFSIZE:6;//���������� �������� �� ����
   uint8_t Res1:1;//��������������� (��� ���� 1)
   uint8_t LF:1;//���� ������ ���������� (0-����� ���������� �������, 1- ���������������)
   //���� 2
   uint8_t AudioMode:4;
   uint8_t Res2:1;//��������������� (��� ���� 1)
   uint8_t CHN:2;//���������� ������� � ����� 
   uint8_t Zero:1;//����
   //���� 3
   uint8_t STYPE:5;//���������� ����� ������ � �����
   uint8_t F50_60:1;//������� ������
   uint8_t Res3:2;//��������������� (��� ���� 1)
   //���� 4
   uint8_t QU:3;
   uint8_t SMP:3;
   uint8_t Res4:2;//��������������� (��� ���� 1)
  };
  //������ ASC (audio source control pack)
  struct SAAUX_ASC
  {
   //���� 0
   uint8_t ID;//�������������
   //���� 1
   uint8_t EFC:2;
   uint8_t Res1:4;//��������������� (��� ���� 1)
   uint8_t CGMS:2;
   //���� 2
   uint8_t Res2:4;//��������������� (��� ���� 1)
   uint8_t FADE_END:1;
   uint8_t FADE_ST:1;
   uint8_t REC_END:1;
   uint8_t REC_ST:1;
   //���� 3
   uint8_t SPEED:7;
   uint8_t DRF:1;
   //���� 4
   uint8_t Res3;//��������������� (��� ���� 1)
  };
  //������
  union UAAUXPayload
  {
   SAAUX_AS sAAUX_AS;//���� AS
   SAAUX_ASC sAAUX_ASC;//���� ASC
   uint8_t Data[5];
  };
  //������ DIF-�����
  struct SDIFAudioData
  {
   UAAUXPayload uAAUXPayload;//������
   uint8_t AudioData[80-8];//������ �����
  };
  //������ �����
  struct SAudio
  {
   CDVDIFID::SDIFID sDIFID;//������������� DIF-�����
   SDIFAudioData sDIFAudioData;//������ DIF-�����
  };
  #pragma pack()
 private:
  //-����������-----------------------------------------------------------------------------------------  
  CDateYMDCode cDateYMDCode;
  CDateHMSCode cDateHMSCode;
  CDVDIFID cDVDIFID;
 public:
  //-����������� ������---------------------------------------------------------------------------------
  CDVAudio(void);
  //-���������� ������----------------------------------------------------------------------------------
  ~CDVAudio();
 public:
  //-�������� ������� ������----------------------------------------------------------------------------
  void Set(SAudio *sAudio_Ptr,uint8_t dif_sequence,uint8_t block_number,const CDVTime &cDVTime_Current,bool is_pal);//������ ������ Audio
  void OutputToFile(FILE *file,const std::string &prefix,SAudio *sAudio_Ptr);//������� � ���� ��������� ������
  static uint32_t GetSectionSize(void);//�������� ������ ������ � ������
  //-�������� ������� ������----------------------------------------------------------------------------
};

#endif
