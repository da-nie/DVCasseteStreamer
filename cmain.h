#ifndef C_MAIN_H
#define C_MAIN_H

//====================================================================================================
//основной класс программы
//====================================================================================================

//====================================================================================================
//подключаемые библиотеки
//====================================================================================================
#include <memory>
#include <string>

#include "cthreadmain.h"
#include "craiiccriticalsection.h"
#include "cdialog_insert.h"
#include "cdialog_extract.h"

//====================================================================================================
//основной класс программы
//====================================================================================================
class CMain
{
 public:
  //режим работы
  enum MODE
  {
   MODE_WAIT,//режим ожидания
   MODE_INSERT_TO_DV,//запустить режим сборки файлов в dv-видеофайл
   MODE_EXTRACT_DV,//запустить режим извлечения файлов из dv-файла
   MODE_VERIFY_DV//запустить режим проверки файлов в dv-файле
  };
 public:
  struct SMode
  {
   MODE Mode;//режим работы
   CDVTime cDVTime;//время создания файла
   CDialog_Insert::SInsertSettings sInsertSettings;//настройки сборки файлов
   CDialog_Extract::SExtractSettings sExtractSettings;//настройки извлечения файлов
  };
 private:
  //-Переменные класса----------------------------------------------------------------------------------
  std::unique_ptr<CThreadMain> cThreadMain_Ptr;//указатель на класс потока
  //структура защищённых переменных
  struct SProtected
  {
   SMode sMode;//режим работы
   bool IsProcessing;//производится ли обработка
   CCriticalSection cCriticalSection;//критическая секция для доступа к элементам
  } sProtected;
 public:
  //-Конструктор класса---------------------------------------------------------------------------------
  CMain(void);
  //-Деструктор класса----------------------------------------------------------------------------------
  ~CMain();
 public:
  //-Открытые функции класса----------------------------------------------------------------------------
  void GetAndClearAnswer(std::string &answer);//получить и очистить ответ
  void InsertData(CDVTime &cDVTime,const CDialog_Insert::SInsertSettings &sInsertSettings);//собрать данные в dv-файл
  void ExtractData(const CDialog_Extract::SExtractSettings &sExtractSettings);//извлечь данные из dv-файла
  void VerifyData(const CDialog_Extract::SExtractSettings &sExtractSettings);//проверить данные в dv-файле
  void GetMode(SMode &sMode);//получить режим работы
  void ClearMode(void);//очистить режим работы (установить режим ожидания)
  void GetAndClearMode(SMode &sMode);//получить режим работы и очистить его
  void SetProcessingState(bool state);//задать, производится ли обработка
  bool IsProcessing(void);//получить, производится ли обработка
  void Break(void);//прервать обработку
 private:
  //-Закрытые функции класса----------------------------------------------------------------------------  
};

#endif