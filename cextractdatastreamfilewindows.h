#ifndef C_EXTRACT_DATA_STREAM_FILE_WINDOWS_H
#define C_EXTRACT_DATA_STREAM_DILE_WINDOWS_H

//====================================================================================================
//����� ������ � �������
//====================================================================================================

//====================================================================================================
//������������ ����������
//====================================================================================================
#include <stdint.h>
#include <string>
#include "craiifilein.h"
#include "craiifileout.h"
#include "iextractdatastream.h"
#include "craiiccriticalsection.h"

//====================================================================================================
//����� ������ � �������
//====================================================================================================
class CExtractDataStreamFileWindows:public IExtractDataStream
{
 private:
  //-���������� ������----------------------------------------------------------------------------------
  CRAIIFileOut cRAIIFileOut;
  //���������� ����������
  struct SProtected
  {
   std::string Answer;//�����
   bool Break;//��������� �� ���������� ������
   CCriticalSection cCriticalSection;//����������� ������ ��� ������� � ���������
  } sProtected;  
public:
  //-����������� ������---------------------------------------------------------------------------------
  CExtractDataStreamFileWindows(void);
  //-���������� ������----------------------------------------------------------------------------------
  ~CExtractDataStreamFileWindows();
 public:
  //-�������� ������� ������----------------------------------------------------------------------------
  bool Create(const std::string &file_name);//������� ����
  void Close(void);//������� ����
  bool Write(uint8_t *buffer_ptr,size_t size);//�������� ������ � ����
  void AddAnswer(const std::string &answer);//�������� � ������ ������
  void GetAnswer(std::string &answer);//�������� �����
  void ClearAnswer(void);//�������� �����
  void GetAndClearAnswer(std::string &answer);//�������� ����� � �������� ���
  bool IsBreak(void);//����� �� ��������� ���������
  void SetBreak(bool state);//������, ��������� �� ���������� ���������
 private:
  //-�������� ������� ������----------------------------------------------------------------------------  
};

#endif
