#ifndef C_LISTVIEW_MAIN_H
#define C_LISTVIEW_MAIN_H

//====================================================================================================
//��������
//====================================================================================================

//����� ������������� ������

//====================================================================================================
//������������ ����������
//====================================================================================================

#include "stdafx.h"
#include "cdocument_main.h"

//====================================================================================================
//����� �������������
//====================================================================================================

class CListView_Main:public CListView
{
 protected: 
  //-����������-----------------------------------------------------------------------------------------
 public:
  //-����������� ������---------------------------------------------------------------------------------
  CListView_Main();
  //-���������� ������----------------------------------------------------------------------------------
  ~CListView_Main();
 public:
  //-�������� ������� ������----------------------------------------------------------------------------  
  afx_msg void OnInitialUpdate(void);//�������� ����
  //-������� ��������� ��������� ������-----------------------------------------------------------------
  afx_msg void OnSize(UINT nType,int cx,int cy);//��������� �������� ����
  afx_msg void OnUpdate(CView *pSender,LPARAM lHint,CObject *pHint);//�������� ���
  DECLARE_MESSAGE_MAP()
 public:
  //-�������� ������� ������----------------------------------------------------------------------------  
  void UpdateList(void);//�������� ������
 protected:
  //-�������� ������� ������----------------------------------------------------------------------------  
  CDocument_Main* GetDocument(void);//�������� ��������
  //-������---------------------------------------------------------------------------------------------
  DECLARE_DYNCREATE(CListView_Main)
  
};
#endif