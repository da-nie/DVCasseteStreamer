#ifndef LIB_TGA_H
#define LIB_TGA_H

#include <windows.h>
#include <stdio.h>
#include <stdint.h>

//��������� TGA-�����
#pragma pack(1)
struct STGAHeader
{
 int8_t identsize;//������ ���� ID ��������� (0)
 int8_t colorMapType;//���� �� �������:0-���,1-����
 int8_t imageType;//��� ��������:0-���,1-��������� �����,2-RGB,3-������� ������, (3-� ��� - RLE- �����������)
 uint16_t colorMapStart;//������ ����� ������
 uint16_t colorMapLength;//���������� ������ � �����
 uint8_t colorMapBits;//����������� �������
 uint16_t xstart;//��������� ���������� �����������
 uint16_t ystart;                 
 uint16_t width;//������ ����������� �� X
 uint16_t height;//������ ����������� �� Y
 int8_t bits;//��������� ��� �� ������� (8,16,24,32)
 int8_t descriptor;//���������� ������������
};
#pragma pack()

uint8_t *LoadTGAFromFile(const char *file_name,uint32_t &width,uint32_t &height);
uint8_t *LoadTGAFromResource(HMODULE hModule,uint32_t id,uint32_t &width,uint32_t &height);
bool SaveTGA(const char *file_name,uint32_t width,uint32_t height,uint8_t *image);

#endif