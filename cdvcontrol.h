#ifndef C_DV_CONTROL_H
#define C_DV_CONTROL_H

//====================================================================================================
//класс работы с RAW DV
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
#include "cdvsubcode.h"
#include "cdvvaux.h"
#include "cdvaudio.h"
#include "cdvvideo.h"

//====================================================================================================
//структуры
//====================================================================================================

//====================================================================================================
//класс работы с RAW DV
//====================================================================================================
class CDVControl
{
 private:
  #pragma pack(1)
  //макроблок
  struct SDVMacroBlock
  {
   int32_t Row;//высота суперблока
   int32_t Col;//ширина суперблока
   int32_t WithinSuperBlock;//положение в макроблоке
   int32_t XLeftTop;//позиция левого верхнего угла
   int32_t YLeftTop;
   int16_t DC[6];//коэффициенты косинусного преобразования
  };
  //сегмент видео
  struct SDVVideoSegment
  {
   int32_t Row;
   int32_t Col;
   SDVMacroBlock sDVMacroBlock[5];//макроблоки
   bool IsPAL;//режим кодирования PAL
  };
  #pragma pack()
  //-Переменные класса----------------------------------------------------------------------------------
  static const int32_t DV_WIDTH=(720/8);//количество блоков коэффициентов по ширине
  static const int32_t DV_PAL_HEIGHT=(576/8);//количество блоков коэффициентов по высоте для режима PAL
  static const int32_t DV_NTSC_HEIGHT=(480/8);//количество блоков коэффициентов по высоте для режима NTSC
  uint8_t BackGroundImage[DV_WIDTH*DV_PAL_HEIGHT*3];//фоновое изображение

  static const uint32_t FILE_NAME_COLOR=0xFFFFFF;//цвет имени файла
  static const uint32_t PROGRESS_BAR_COLOR=0xFFFFFF;//цвет линейки прогресса

  static const uint32_t DIF_BLOCK_SIZE=80;//размер DIF-Блока
  static const uint32_t PAL_FRAME_SIZE=144000;//размер кадра PAL
  static const uint32_t NTSC_FRAME_SIZE=120000;//размер кадра NTSC
  static const uint32_t MAX_FRAME_SIZE=PAL_FRAME_SIZE;//максимальный размер кадра

  int32_t FrameCounter;//текущий кадр
  CDVTime cDVTime_Video;//длительность видео
public:
  //-Конструктор класса---------------------------------------------------------------------------------
  CDVControl(void);
  //-Деструктор класса----------------------------------------------------------------------------------
  ~CDVControl();
 public:
  //-Открытые функции класса----------------------------------------------------------------------------
  bool LoadBackgroundImage(const std::string &file_name,bool is_pal);//загрузить фоновое изображение
  bool CreateDV(IOutputDVStream *iDataStream_Ptr,const std::string &file_name,const std::string &title_file_name,bool is_pal,uint32_t prefix,CDVTime &cDVTime_Current);//создать DV-файл с данными
  bool ExtractDV(const std::string &dv_file_name,const  std::string &path,IExtractDataStream *iExtractDataStream_Ptr);//вынуть файлы из DV-файла
  void NewRecord(void);//начать отсчёт кадров и блоков сначала
  void OutputToFile(FILE *file,uint8_t *frame_buffer,bool is_pal);//вывести в файл данные секций dv кадра
 private:
  //-Закрытые функции класса----------------------------------------------------------------------------  
  double Round(double value,uint32_t digit);//округление до заданного количества цифр после запятой
  int32_t RoundToInt(double value);//округление до ближайшего целого
  void DV_Place411MacroBlock(SDVMacroBlock *sDVMacroBlock_Ptr);//установка 411 макроблока
  void DV_Place420MacroBlock(SDVMacroBlock *sDVMacroBlock_Ptr);//установка 420 макроблока
  void ConvertToYUV(uint8_t* img_rgb,int32_t height,int16_t* img_y,int16_t* img_cr,int16_t* img_cb);//конвертация в палитру YUV
  void BuildCoeff(int16_t* img_y,int16_t* img_cr,int16_t* img_cb,SDVMacroBlock *sDVMacroBlock_Ptr,bool is_pal);//построение блока коэффициентов
  void ProcessVideoSegment(int16_t* img_y,int16_t* img_cr,int16_t* img_cb,SDVVideoSegment *sDVVideoSegment_Ptr,uint8_t*vsbuffer);//обработка видеосегмента
  void Encode(int16_t* img_y,int16_t* img_cr,int16_t* img_cb,bool is_pal,uint8_t* target,SDVVideoSegment &sDVVideoSegment);//кодирование
  void CreateInfoBlocks(uint8_t* target,int32_t frame,bool is_pal,CDVTime &cDVTime_Video,CDVTime &cDVTime_Current,bool enable_audio);//создание информационных блоков
  void EncodePicture(uint8_t* readbuf,bool is_pal,uint8_t* target,SDVVideoSegment &sDVVideoSegment);//кодирование изображения
  void AddInfoBlocks(bool is_pal,uint8_t* target,CDVTime &cDVTime_Video,CDVTime &cDVTime_Current,bool enable_audio,int32_t &frame_counter);//добавление информационных блоков
  int32_t GetChunkSize(bool isPAL);//получить размер фрагмента
  void InsertData(uint8_t* src,bool is_pal,uint8_t* target);//вставить данные
  void ExtractData(uint8_t* src,uint8_t* target);//извлечь данные
};

#endif
