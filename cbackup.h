#ifndef C_BACKUP_H
#define C_BACKIP_H

//====================================================================================================
//класс упаковки/распаковки файла
//====================================================================================================

//====================================================================================================
//подключаемые библиотеки
//====================================================================================================

#include <stdint.h>
#include <stdio.h>
#include <string>
#include "ioutputdvstream.h"
#include "iextractdatastream.h"
#include "cdvheader.h"

//====================================================================================================
//структуры
//====================================================================================================

//====================================================================================================
//класс упаковки/распаковки файла
//====================================================================================================
class CBackUp
{
 public:
  #pragma pack(1)
  //заголовок (следует иметь в виду, что байты в заголовке должны быть заданы как big-endian для совместимости с dvbackup!)
  struct SDVPackerHeader
  {
   uint8_t ID[4];//идентификатор DV-B
   uint16_t HeaderSize;//размер заголовка
   uint8_t Version;//версия
   uint64_t Offset;//смещение блока в байтах
   uint8_t Flag;//флаг конца данных
   uint8_t FrameSize[3];//три байта размера данных в кадре
   uint32_t CRC;//контрольная сумма
  };
 #pragma pack() 
  static const int32_t DVBACKUP_VERSION=0x01;//версия программы
  static const int32_t HEADER_SIZE=sizeof(SDVPackerHeader);//размер заголовка
  static const int32_t MAX_FRAME_SIZE=144000;//максимальный размер кадра
 private:
  bool FileInProcessing;//идёт запись файла
  std::string CurrentFileName;//имя файла
  uint32_t CurrentHeaderSize;//текущий размер заголовка
  uint64_t CurrentAddr;//текущий записываемый адрес файла
  uint32_t LastDataSize;//размер последнего пакета данных

 public:
  //-Конструктор класса---------------------------------------------------------------------------------
  CBackUp(void);
  //-Деструктор класса----------------------------------------------------------------------------------
  ~CBackUp();
 public:
  //-Открытые функции класса----------------------------------------------------------------------------
  void Extract(uint8_t databuffer[MAX_FRAME_SIZE],const std::string &path,IExtractDataStream *iExtractDataStream_Ptr);//обработка данных из кадра
  void CreateHeader(uint8_t databuffer[MAX_FRAME_SIZE],uint64_t current_address,uint32_t data_size,uint32_t header_size,bool eof,const char* backup_title) const;//создать заголовок
  long GetHeaderSize(const char* backup_title) const;//получить размер заголовка
  void ResetState(void);//сбросить состояние в исходное
 private:
  //-Закрытые функции класса----------------------------------------------------------------------------  
  void ReverseByte(void *ptr,size_t size) const;//перевернуть байты
  uint32_t CreateCRC(uint32_t last_crc,const uint8_t* ptr,size_t length) const;//алгоритм вычисления CRC
  void CheckHeader(uint8_t databuffer[MAX_FRAME_SIZE],bool &version_ok,bool &id_ok,bool &crc_ok,bool &header_size_ok);//проверка заголовка
};

#endif
