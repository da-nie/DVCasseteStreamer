#ifndef C_DV_CONTROL_H
#define C_DV_CONTROL_H

//====================================================================================================
//����� ������ � RAW DV
//====================================================================================================

//====================================================================================================
//������������ ����������
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
//���������
//====================================================================================================

//====================================================================================================
//����� ������ � RAW DV
//====================================================================================================
class CDVControl
{
 private:
  #pragma pack(1)
  //���������
  struct SDVMacroBlock
  {
   int32_t Row;//������ ����������
   int32_t Col;//������ ����������
   int32_t WithinSuperBlock;//��������� � ����������
   int32_t XLeftTop;//������� ������ �������� ����
   int32_t YLeftTop;
   int16_t DC[6];//������������ ����������� ��������������
  };
  //������� �����
  struct SDVVideoSegment
  {
   int32_t Row;
   int32_t Col;
   SDVMacroBlock sDVMacroBlock[5];//����������
   bool IsPAL;//����� ����������� PAL
  };
  #pragma pack()
  //-���������� ������----------------------------------------------------------------------------------
  static const int32_t DV_WIDTH=(720/8);//���������� ������ ������������� �� ������
  static const int32_t DV_PAL_HEIGHT=(576/8);//���������� ������ ������������� �� ������ ��� ������ PAL
  static const int32_t DV_NTSC_HEIGHT=(480/8);//���������� ������ ������������� �� ������ ��� ������ NTSC
  uint8_t BackGroundImage[DV_WIDTH*DV_PAL_HEIGHT*3];//������� �����������

  static const uint32_t FILE_NAME_COLOR=0xFFFFFF;//���� ����� �����
  static const uint32_t PROGRESS_BAR_COLOR=0xFFFFFF;//���� ������� ���������

  static const uint32_t DIF_BLOCK_SIZE=80;//������ DIF-�����
  static const uint32_t PAL_FRAME_SIZE=144000;//������ ����� PAL
  static const uint32_t NTSC_FRAME_SIZE=120000;//������ ����� NTSC
  static const uint32_t MAX_FRAME_SIZE=PAL_FRAME_SIZE;//������������ ������ �����

  int32_t FrameCounter;//������� ����
  CDVTime cDVTime_Video;//������������ �����
public:
  //-����������� ������---------------------------------------------------------------------------------
  CDVControl(void);
  //-���������� ������----------------------------------------------------------------------------------
  ~CDVControl();
 public:
  //-�������� ������� ������----------------------------------------------------------------------------
  bool LoadBackgroundImage(const std::string &file_name,bool is_pal);//��������� ������� �����������
  bool CreateDV(IOutputDVStream *iDataStream_Ptr,const std::string &file_name,const std::string &title_file_name,bool is_pal,uint32_t prefix,CDVTime &cDVTime_Current);//������� DV-���� � �������
  bool ExtractDV(const std::string &dv_file_name,const  std::string &path,IExtractDataStream *iExtractDataStream_Ptr);//������ ����� �� DV-�����
  void NewRecord(void);//������ ������ ������ � ������ �������
  void OutputToFile(FILE *file,uint8_t *frame_buffer,bool is_pal);//������� � ���� ������ ������ dv �����
 private:
  //-�������� ������� ������----------------------------------------------------------------------------  
  double Round(double value,uint32_t digit);//���������� �� ��������� ���������� ���� ����� �������
  int32_t RoundToInt(double value);//���������� �� ���������� ������
  void DV_Place411MacroBlock(SDVMacroBlock *sDVMacroBlock_Ptr);//��������� 411 ����������
  void DV_Place420MacroBlock(SDVMacroBlock *sDVMacroBlock_Ptr);//��������� 420 ����������
  void ConvertToYUV(uint8_t* img_rgb,int32_t height,int16_t* img_y,int16_t* img_cr,int16_t* img_cb);//����������� � ������� YUV
  void BuildCoeff(int16_t* img_y,int16_t* img_cr,int16_t* img_cb,SDVMacroBlock *sDVMacroBlock_Ptr,bool is_pal);//���������� ����� �������������
  void ProcessVideoSegment(int16_t* img_y,int16_t* img_cr,int16_t* img_cb,SDVVideoSegment *sDVVideoSegment_Ptr,uint8_t*vsbuffer);//��������� �������������
  void Encode(int16_t* img_y,int16_t* img_cr,int16_t* img_cb,bool is_pal,uint8_t* target,SDVVideoSegment &sDVVideoSegment);//�����������
  void CreateInfoBlocks(uint8_t* target,int32_t frame,bool is_pal,CDVTime &cDVTime_Video,CDVTime &cDVTime_Current,bool enable_audio);//�������� �������������� ������
  void EncodePicture(uint8_t* readbuf,bool is_pal,uint8_t* target,SDVVideoSegment &sDVVideoSegment);//����������� �����������
  void AddInfoBlocks(bool is_pal,uint8_t* target,CDVTime &cDVTime_Video,CDVTime &cDVTime_Current,bool enable_audio,int32_t &frame_counter);//���������� �������������� ������
  int32_t GetChunkSize(bool isPAL);//�������� ������ ���������
  void InsertData(uint8_t* src,bool is_pal,uint8_t* target);//�������� ������
  void ExtractData(uint8_t* src,uint8_t* target);//������� ������
};

#endif
