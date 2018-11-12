#ifndef C_EXTRACT_DATA_STREAM_FILE_WINDOWS_H
#define C_EXTRACT_DATA_STREAM_DILE_WINDOWS_H

//====================================================================================================
//класс работы с файлами
//====================================================================================================

//====================================================================================================
//подключаемые библиотеки
//====================================================================================================
#include <stdint.h>
#include <string>
#include "craiifilein.h"
#include "craiifileout.h"
#include "iextractdatastream.h"
#include "craiiccriticalsection.h"

//====================================================================================================
//класс работы с файлами
//====================================================================================================
class CExtractDataStreamFileWindows:public IExtractDataStream
{
 private:
  //-Переменные класса----------------------------------------------------------------------------------
  CRAIIFileOut cRAIIFileOut;
  //защищённые переменные
  struct SProtected
  {
   std::string Answer;//ответ
   bool Break;//требуется ли завершение работы
   CCriticalSection cCriticalSection;//критическая секция для доступа к элементам
  } sProtected;  
public:
  //-Конструктор класса---------------------------------------------------------------------------------
  CExtractDataStreamFileWindows(void);
  //-Деструктор класса----------------------------------------------------------------------------------
  ~CExtractDataStreamFileWindows();
 public:
  //-Открытые функции класса----------------------------------------------------------------------------
  bool Create(const std::string &file_name);//создать файл
  void Close(void);//закрыть файл
  bool Write(uint8_t *buffer_ptr,size_t size);//записать данные в файл
  void AddAnswer(const std::string &answer);//добавить к ответу строку
  void GetAnswer(std::string &answer);//получить ответ
  void ClearAnswer(void);//очистить ответ
  void GetAndClearAnswer(std::string &answer);//получить ответ и очистить его
  bool IsBreak(void);//нужно ли завершать обработку
  void SetBreak(bool state);//задать, требуется ли завершение обработки
 private:
  //-Закрытые функции класса----------------------------------------------------------------------------  
};

#endif
