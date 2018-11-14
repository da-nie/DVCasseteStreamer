#ifndef C_THREAD_MAIN_H
#define C_THREAD_MAIN_H

//====================================================================================================
//описание
//====================================================================================================

//Класс главного потока

//====================================================================================================
//подключаемые библиотеки
//====================================================================================================
#include "stdafx.h"
#include <stdint.h>

#include "cdvcontrol.h"
#include "coutputdvstreamfile.h"
#include "cextractdatastreamfilewindows.h"
#include "cextractdatastreamverify.h"

class CMain;//опережающее объявление класса CMain

//====================================================================================================
//класс главного потока
//====================================================================================================

class CThreadMain
{ 
  //-Дружественные функции класса-----------------------------------------------------------------------
 friend UINT ThreadMain(LPVOID pParam);//функция потока
 protected:
  //-Переменные класса----------------------------------------------------------------------------------
  CEvent cEvent_Exit;//требование на выход из потока
  CWinThread *cWinThread_Thread;//поток обработки
  CMain *cMain_Ptr;//указатель на класс родителя

  COutputDVStreamFile cOutputDVStreamFile;//класс создания dv-файла
  CExtractDataStreamFileWindows cExtractDataStreamFileWindows;//класс экспорта данных из dv-файла
  CExtractDataStreamVerify cExtractDataStreamVerify;//класс проверки данных в dv-файле
  CDVControl cDVControl;//класс работы с dv-форматом
 public:  
  //-Конструктор класса---------------------------------------------------------------------------------
  CThreadMain(CMain *cMain_PtrSet);
  //-Деструктор класса----------------------------------------------------------------------------------
  ~CThreadMain();
 public:
  //-Открытые функции класса----------------------------------------------------------------------------
  void Start(void);//запустить поток
  void Stop(void);//остановить поток
  void Break(void);//остановить работу с DV-файлом
  void GetAnswer(std::string &answer);//получить ответ
  void ClearAnswer(void);//очистить ответ
  void GetAndClearAnswer(std::string &answer);//получить ответ и очистить его
 protected:
  //-Закрытые функции класса----------------------------------------------------------------------------  
  void Execute(void);//основной цикл класса
  bool IsExit(void);//получить, требуется ли выйти из потока
  bool IsBreak(void);//получить, требуется ли остановить работу с файлом dv
  void InsertToDV(const std::string &path,const std::string &save_path,IOutputDVStream *iOutputDVStream_Ptr,CDVControl *cDVControl_Ptr,CDVTime &cDVTime,uint32_t local_prefix,uint32_t &prefix,const std::string &output_file_name,const std::string &logo_file_name);//собрать файлы в DV-видеофайл
};

#endif