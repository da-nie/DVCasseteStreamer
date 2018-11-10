#ifndef C_DV_AUDIO_H
#define C_DV_AUDIO_H

//====================================================================================================
//класс работы с секцией audio для dv-формата
//====================================================================================================

//====================================================================================================
//подключаемые библиотеки
//====================================================================================================

#include <stdint.h>

#include "cdvdifid.h"
#include "cdvtime.h"
#include "cdateymdcode.h"
#include "cdatehmscode.h"

//====================================================================================================
//класс работы с секцией audio для dv-формата
//====================================================================================================
class CDVAudio
{
 public:
  //-Перечисления---------------------------------------------------------------------------------------
  //значения идентификатора
  enum AAUX_ID_VALUE
  {
   AAUX_ID_VS=0x50,
   AAUX_ID_VSC=0x51   
  };
  //значения аудиорежима
  enum AUDIOMODE_VALUE
  {
   AUDIOMODE_CH1_CH3=0x00,
   AUDIOMODE_CH2_CH4=0x01,
   AUDIOMODE_INVALID=0x0f   
  };
  //значения частоты кадров
  enum F50_60_VALUE
  {
   F50=0x01,
   F60=0x00
  };
  //значения количества отсчётов на кадр
  enum AFSIZE_VALUE
  {
   AFSIZE_1600_SAMPLES=0x14,//525/60
   AFSIZE_1602_SAMPLES=0x16,//525/60
   AFSIZE_1920_SAMPLES=0x18//625/50
  };
  //значения количества аудио блоков в кадре
  enum STYPE_VALUE
  {
   STYPE_2=0x00,
   STYPE_4=0x02
  };
  //значения количества каналов в аудио блоке
  enum CHN_VALUE
  {
   CHN_1=0x00
  };
  //значения частот семплирования
  enum SMP
  {
   SMP_48000=0x00
  };
  //значения разрядности данных
  enum QU_VALUE
  {
   QU_16=0x00
  };
  //значения разрешений на копирование
  enum CGMS_VALUE
  {
   CGMS_COPY_FREE=0x00
  };
  //значения блокировки
  enum LF_VALUE
  {
   LF_ENABLED=0x00,
   LF_RESERVER=0x01
  };
  //значения "акцента" для аудио блока
  enum EFC_VALUE
  {
   EFC_EMPHASSIS_OFF=0x00,
   EFC_EMPHASSIS_ON=0x01,
   EFC_EMPHASSIS_RESERVED=0x03
  };
  //значения начальной точки записи
  enum RECT_ST_VALUE
  {   
   REC_ST_OFF=0x00,
   REC_ST_ON=0x01
  };
  //значения конечной точки записи
  enum RECT_END_VALUE
  {   
   REC_END_OFF=0x00,
   REC_END_ON=0x01
  };
  //значения затухания звука в стартовой точки записи
  enum FADE_ST_VALUE
  {
   FADE_ST_OFF=0x00,
   FADE_ST_ON=0x01
  };
  //значения затухания звука в конечной точки записи
  enum FADE_END_VALUE
  {
   FADE_END_OFF=0x00,
   FADE_END_ON=0x01   
  };
  //значения направления
  enum DRF_VALUE
  {
   DRF_REVERSE=0x00,
   DRF_FORWARD=0x01
  };
  //-Структуры------------------------------------------------------------------------------------------
  //биты внутри байта структуры заполняются от младшего к старшему! В документации они приведены наоборот!
  #pragma pack(1)
  //данные AS (audio source pack)
  struct SAAUX_AS
  {
   //байт 0
   uint8_t ID;//идентификатор
   //байт 1
   uint8_t AFSIZE:6;//количество отсчётов на кадр
   uint8_t Res1:1;//зарезервировано (все биты 1)
   uint8_t LF:1;//флаг режима блокировки (0-режим блокировки включён, 1- зарезервировано)
   //байт 2
   uint8_t AudioMode:4;
   uint8_t Res2:1;//зарезервировано (все биты 1)
   uint8_t CHN:2;//количество каналов в блоке 
   uint8_t Zero:1;//ноль
   //байт 3
   uint8_t STYPE:5;//количество аудио блоков в кадре
   uint8_t F50_60:1;//частота кадров
   uint8_t Res3:2;//зарезервировано (все биты 1)
   //байт 4
   uint8_t QU:3;
   uint8_t SMP:3;
   uint8_t Res4:2;//зарезервировано (все биты 1)
  };
  //данные ASC (audio source control pack)
  struct SAAUX_ASC
  {
   //байт 0
   uint8_t ID;//идентификатор
   //байт 1
   uint8_t EFC:2;
   uint8_t Res1:4;//зарезервировано (все биты 1)
   uint8_t CGMS:2;
   //байт 2
   uint8_t Res2:4;//зарезервировано (все биты 1)
   uint8_t FADE_END:1;
   uint8_t FADE_ST:1;
   uint8_t REC_END:1;
   uint8_t REC_ST:1;
   //байт 3
   uint8_t SPEED:7;
   uint8_t DRF:1;
   //байт 4
   uint8_t Res3;//зарезервировано (все биты 1)
  };
  //данные
  union UAAUXPayload
  {
   SAAUX_AS sAAUX_AS;//блок AS
   SAAUX_ASC sAAUX_ASC;//блок ASC
   uint8_t Data[5];
  };
  //данные DIF-блока
  struct SDIFAudioData
  {
   UAAUXPayload uAAUXPayload;//данные
   uint8_t AudioData[80-8];//данные звука
  };
  //данные аудио
  struct SAudio
  {
   CDVDIFID::SDIFID sDIFID;//идентификатор DIF-блока
   SDIFAudioData sDIFAudioData;//данные DIF-блока
  };
  #pragma pack()
 private:
  //-Переменные-----------------------------------------------------------------------------------------  
  CDateYMDCode cDateYMDCode;
  CDateHMSCode cDateHMSCode;
  CDVDIFID cDVDIFID;
 public:
  //-Конструктор класса---------------------------------------------------------------------------------
  CDVAudio(void);
  //-Деструктор класса----------------------------------------------------------------------------------
  ~CDVAudio();
 public:
  //-Открытые функции класса----------------------------------------------------------------------------
  void Set(SAudio *sAudio_Ptr,uint8_t dif_sequence,uint8_t block_number,const CDVTime &cDVTime_Current,bool is_pal);//задать секцию Audio
  void OutputToFile(FILE *file,const std::string &prefix,SAudio *sAudio_Ptr);//вывести в файл параметры секции
  static uint32_t GetSectionSize(void);//получить размер секции в байтах
  //-Закрытые функции класса----------------------------------------------------------------------------
};

#endif
