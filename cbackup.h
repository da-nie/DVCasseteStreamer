#ifndef C_BACKUP_H
#define C_BACKIP_H

//====================================================================================================
//����� ��������/���������� �����
//====================================================================================================

//====================================================================================================
//������������ ����������
//====================================================================================================

#include <stdint.h>
#include <stdio.h>
#include <string>
#include "ioutputdvstream.h"
#include "iextractdatastream.h"
#include "cdvheader.h"

//====================================================================================================
//���������
//====================================================================================================

//====================================================================================================
//����� ��������/���������� �����
//====================================================================================================
class CBackUp
{
 public:
  #pragma pack(1)
  //��������� (������� ����� � ����, ��� ����� � ��������� ������ ���� ������ ��� big-endian ��� ������������� � dvbackup!)
  struct SDVPackerHeader
  {
   uint8_t ID[4];//������������� DV-B
   uint16_t HeaderSize;//������ ���������
   uint8_t Version;//������
   uint64_t Offset;//�������� ����� � ������
   uint8_t Flag;//���� ����� ������
   uint8_t FrameSize[3];//��� ����� ������� ������ � �����
   uint32_t CRC;//����������� �����
  };
 #pragma pack() 
  static const int32_t DVBACKUP_VERSION=0x01;//������ ���������
  static const int32_t HEADER_SIZE=sizeof(SDVPackerHeader);//������ ���������
  static const int32_t MAX_FRAME_SIZE=144000;//������������ ������ �����
 private:
  bool FileInProcessing;//��� ������ �����
  std::string CurrentFileName;//��� �����
  uint32_t CurrentHeaderSize;//������� ������ ���������
  uint64_t CurrentAddr;//������� ������������ ����� �����
  uint32_t LastDataSize;//������ ���������� ������ ������

 public:
  //-����������� ������---------------------------------------------------------------------------------
  CBackUp(void);
  //-���������� ������----------------------------------------------------------------------------------
  ~CBackUp();
 public:
  //-�������� ������� ������----------------------------------------------------------------------------
  void Extract(uint8_t databuffer[MAX_FRAME_SIZE],const std::string &path,IExtractDataStream *iExtractDataStream_Ptr);//��������� ������ �� �����
  void CreateHeader(uint8_t databuffer[MAX_FRAME_SIZE],uint64_t current_address,uint32_t data_size,uint32_t header_size,bool eof,const char* backup_title) const;//������� ���������
  long GetHeaderSize(const char* backup_title) const;//�������� ������ ���������
  void ResetState(void);//�������� ��������� � ��������
 private:
  //-�������� ������� ������----------------------------------------------------------------------------  
  void ReverseByte(void *ptr,size_t size) const;//����������� �����
  uint32_t CreateCRC(uint32_t last_crc,const uint8_t* ptr,size_t length) const;//�������� ���������� CRC
  void CheckHeader(uint8_t databuffer[MAX_FRAME_SIZE],bool &version_ok,bool &id_ok,bool &crc_ok,bool &header_size_ok);//�������� ���������
};

#endif
