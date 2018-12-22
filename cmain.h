#ifndef C_MAIN_H
#define C_MAIN_H

//====================================================================================================
//�������� ����� ���������
//====================================================================================================

//====================================================================================================
//������������ ����������
//====================================================================================================
#include <memory>
#include <string>

#include "cthreadmain.h"
#include "craiiccriticalsection.h"
#include "cdialog_insert.h"
#include "cdialog_extract.h"

//====================================================================================================
//�������� ����� ���������
//====================================================================================================
class CMain
{
 public:
  //����� ������
  enum MODE
  {
   MODE_WAIT,//����� ��������
   MODE_INSERT_TO_DV,//��������� ����� ������ ������ � dv-���������
   MODE_EXTRACT_DV,//��������� ����� ���������� ������ �� dv-�����
   MODE_VERIFY_DV//��������� ����� �������� ������ � dv-�����
  };
 public:
  struct SMode
  {
   MODE Mode;//����� ������
   CDVTime cDVTime;//����� �������� �����
   CDialog_Insert::SInsertSettings sInsertSettings;//��������� ������ ������
   CDialog_Extract::SExtractSettings sExtractSettings;//��������� ���������� ������
  };
 private:
  //-���������� ������----------------------------------------------------------------------------------
  std::unique_ptr<CThreadMain> cThreadMain_Ptr;//��������� �� ����� ������
  //��������� ���������� ����������
  struct SProtected
  {
   SMode sMode;//����� ������
   bool IsProcessing;//������������ �� ���������
   CCriticalSection cCriticalSection;//����������� ������ ��� ������� � ���������
  } sProtected;
 public:
  //-����������� ������---------------------------------------------------------------------------------
  CMain(void);
  //-���������� ������----------------------------------------------------------------------------------
  ~CMain();
 public:
  //-�������� ������� ������----------------------------------------------------------------------------
  void GetAndClearAnswer(std::string &answer);//�������� � �������� �����
  void InsertData(CDVTime &cDVTime,const CDialog_Insert::SInsertSettings &sInsertSettings);//������� ������ � dv-����
  void ExtractData(const CDialog_Extract::SExtractSettings &sExtractSettings);//������� ������ �� dv-�����
  void VerifyData(const CDialog_Extract::SExtractSettings &sExtractSettings);//��������� ������ � dv-�����
  void GetMode(SMode &sMode);//�������� ����� ������
  void ClearMode(void);//�������� ����� ������ (���������� ����� ��������)
  void GetAndClearMode(SMode &sMode);//�������� ����� ������ � �������� ���
  void SetProcessingState(bool state);//������, ������������ �� ���������
  bool IsProcessing(void);//��������, ������������ �� ���������
  void Break(void);//�������� ���������
 private:
  //-�������� ������� ������----------------------------------------------------------------------------  
};

#endif