#ifndef C_GRAPHICS_H
#define C_GRAPHICS_H
//====================================================================================================
//����� ��������� � ������ �����������
//====================================================================================================

//====================================================================================================
//������������ ����������
//====================================================================================================
#include <string.h>
#include <stdint.h>

//====================================================================================================
//����������������
//====================================================================================================
#define FONT_WIDTH  8
#define FONT_HEIGHT 8

//====================================================================================================
//����� ��������� � ������ �����������
//====================================================================================================
class CGraphics
{
 protected:
  //-���������� ������-------------------------------------------------------  
  uint8_t *VideoPtr;//��������� �� ���������� (RGBA)
  uint32_t ScreenWidth;//������ �����������
  uint32_t ScreenHeight;//������ �����������
  uint32_t LineSize;//������ ������ �����������
 public:
  //-����������� ������------------------------------------------------------
  CGraphics(void);
  //-���������� ������-------------------------------------------------------
  ~CGraphics();
  //-������� ������----------------------------------------------------------
  void Init(uint8_t*video_ptr,uint32_t screen_width,uint32_t screen_height);//�������������
  void PutSymbol(uint32_t x,uint32_t y,char symbol,uint32_t color);//����� ������� � �������
  void PutString(uint32_t x,uint32_t y,const char *string,uint32_t color);//����� ������� � �������
  void SolidFill(uint32_t x,uint32_t y,uint32_t width,uint32_t height,uint32_t color);//��������� �������������
  void PutSymbolInversion(uint32_t x,uint32_t y,char symbol);//����� ������� � ������� � ��������� �����
  void PutStringInversion(uint32_t x,uint32_t y,const char *string);//����� ������� � ������� � ��������� �����
  void DrawPointInversion(uint32_t x,uint32_t y);//��������� ����� � ������� � ��������� �����

};

#endif