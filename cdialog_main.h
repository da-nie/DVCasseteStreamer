#ifndef C_DIALOG_MAIN_H
#define C_DIALOG_MAIN_H

#include "stdafx.h"
#include <memory>
#include <stdint.h>

#include "cmain.h"

class CDialog_Main:public CDialog
{
 protected:
  //-���������� ������----------------------------------------------------------------------------------
  static const uint32_t TIMER_PERIOD_MS=100;//������ �������
  static const uint32_t ID_TIMER_MAIN=1000;//������������� �������

  std::unique_ptr<CMain> cMain_Ptr;//��������� �� �������� ����� ���������
 public:
  //-����������� ������---------------------------------------------------------------------------------
  CDialog_Main(LPCTSTR lpszTemplateName,CWnd* pParentWnd);
  //-���������� ������----------------------------------------------------------------------------------
  ~CDialog_Main();
  //-������� ��������� ��������� ������-----------------------------------------------------------------
  DECLARE_MESSAGE_MAP()
  afx_msg void OnOK(void);
  afx_msg void OnCancel(void);
  afx_msg BOOL OnInitDialog(void);
  afx_msg void OnDestroy(void);
  afx_msg void OnTimer(UINT nIDEvent);//������� �������
  afx_msg void OnButton_Insert(void);//�������� ����� � dv-���������
  afx_msg void OnButton_Extract(void);//������� ����� �� dv-����������
  afx_msg void OnButton_ClearLog(void);//�������� ����������
  afx_msg void OnButton_Break(void);//�������� ���������
 public:
  //-�������� ������� ������----------------------------------------------------------------------------
 private:
  //-�������� ������� ������----------------------------------------------------------------------------
  afx_msg bool SelectDirectory(std::string &path,const std::string &caption);//����� ��������
  
};

#endif