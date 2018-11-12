#ifndef C_DIALOG_MAIN_H
#define C_DIALOG_MAIN_H

#include "stdafx.h"
#include <memory>
#include <stdint.h>

#include "cmain.h"

class CDialog_Main:public CDialog
{
 protected:
  //-Переменные класса----------------------------------------------------------------------------------
  static const uint32_t TIMER_PERIOD_MS=100;//период таймера
  static const uint32_t ID_TIMER_MAIN=1000;//идентификатор таймера

  std::unique_ptr<CMain> cMain_Ptr;//указатель на основной класс программы
 public:
  //-Конструктор класса---------------------------------------------------------------------------------
  CDialog_Main(LPCTSTR lpszTemplateName,CWnd* pParentWnd);
  //-Деструктор класса----------------------------------------------------------------------------------
  ~CDialog_Main();
  //-Функции обработки сообщений класса-----------------------------------------------------------------
  DECLARE_MESSAGE_MAP()
  afx_msg void OnOK(void);
  afx_msg void OnCancel(void);
  afx_msg BOOL OnInitDialog(void);
  afx_msg void OnDestroy(void);
  afx_msg void OnTimer(UINT nIDEvent);//событие таймера
  afx_msg void OnButton_Insert(void);//добавить файлы в dv-видеофайл
  afx_msg void OnButton_Extract(void);//извлечь файлы из dv-видеофайла
  afx_msg void OnButton_ClearLog(void);//очистить статистику
  afx_msg void OnButton_Break(void);//прервать обработку
 public:
  //-Открытые функции класса----------------------------------------------------------------------------
 private:
  //-Закрытые функции класса----------------------------------------------------------------------------
  afx_msg bool SelectDirectory(std::string &path,const std::string &caption);//выбор каталога
  
};

#endif