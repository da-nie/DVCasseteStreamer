#ifndef I_OUTPUT_DV_STREAM_H
#define I_OUTPUT_DV_STREAM_H

//====================================================================================================
//������������ ����� ������ ������ ������ DV
//====================================================================================================

//====================================================================================================
//������������ ����������
//====================================================================================================
#include <stdint.h>
#include <string>

//====================================================================================================
//������������ ����� ������ ������ ������ DV
//====================================================================================================
class IOutputDVStream
{
 private:
  //-���������� ������----------------------------------------------------------------------------------
public:
  //-����������� ������---------------------------------------------------------------------------------
  //-���������� ������----------------------------------------------------------------------------------
  virtual ~IOutputDVStream() {};
 public:
  //-�������� ������� ������----------------------------------------------------------------------------
  virtual bool AddFrame(uint8_t *frame_buffer,size_t size)=0;//�������� ����
  virtual bool Create(const std::string &file_name)=0;//������� ����
  virtual void Close(void)=0;//������� ����
  virtual void AddAnswer(const std::string &answer)=0;//�������� � ������ ������
  virtual void GetAnswer(std::string &answer)=0;//�������� �����
  virtual void ClearAnswer(void)=0;//�������� �����
  virtual void GetAndClearAnswer(std::string &answer)=0;//�������� ����� � �������� ���
  virtual bool IsBreak(void)=0;//����� �� ��������� ���������
  virtual void SetBreak(bool state)=0;//������, ��������� �� ���������� ���������
};

#endif
