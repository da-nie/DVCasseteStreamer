#ifndef C_THREAD_MAIN_H
#define C_THREAD_MAIN_H

//====================================================================================================
//��������
//====================================================================================================

//����� �������� ������

//====================================================================================================
//������������ ����������
//====================================================================================================
#include "stdafx.h"
#include <stdint.h>

#include "cdvcontrol.h"
#include "coutputdvstreamfile.h"
#include "cextractdatastreamfilewindows.h"
#include "cextractdatastreamverify.h"

class CMain;//����������� ���������� ������ CMain

//====================================================================================================
//����� �������� ������
//====================================================================================================

class CThreadMain
{ 
  //-������������� ������� ������-----------------------------------------------------------------------
 friend UINT ThreadMain(LPVOID pParam);//������� ������
 protected:
  //-���������� ������----------------------------------------------------------------------------------
  CEvent cEvent_Exit;//���������� �� ����� �� ������
  CWinThread *cWinThread_Thread;//����� ���������
  CMain *cMain_Ptr;//��������� �� ����� ��������

  COutputDVStreamFile cOutputDVStreamFile;//����� �������� dv-�����
  CExtractDataStreamFileWindows cExtractDataStreamFileWindows;//����� �������� ������ �� dv-�����
  CExtractDataStreamVerify cExtractDataStreamVerify;//����� �������� ������ � dv-�����
  CDVControl cDVControl;//����� ������ � dv-��������
 public:  
  //-����������� ������---------------------------------------------------------------------------------
  CThreadMain(CMain *cMain_PtrSet);
  //-���������� ������----------------------------------------------------------------------------------
  ~CThreadMain();
 public:
  //-�������� ������� ������----------------------------------------------------------------------------
  void Start(void);//��������� �����
  void Stop(void);//���������� �����
  void Break(void);//���������� ������ � DV-������
  void GetAnswer(std::string &answer);//�������� �����
  void ClearAnswer(void);//�������� �����
  void GetAndClearAnswer(std::string &answer);//�������� ����� � �������� ���
 protected:
  //-�������� ������� ������----------------------------------------------------------------------------  
  void Execute(void);//�������� ���� ������
  bool IsExit(void);//��������, ��������� �� ����� �� ������
  bool IsBreak(void);//��������, ��������� �� ���������� ������ � ������ dv
  void InsertToDV(const std::string &path,const std::string &save_path,IOutputDVStream *iOutputDVStream_Ptr,CDVControl *cDVControl_Ptr,CDVTime &cDVTime,uint32_t local_prefix,uint32_t &prefix,const std::string &output_file_name,const std::string &logo_file_name);//������� ����� � DV-���������
};

#endif