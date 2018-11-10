#ifndef LIB_TGA_H
#define LIB_TGA_H

#include <windows.h>
#include <stdio.h>
#include <stdint.h>

//заголовок TGA-файла
#pragma pack(1)
struct STGAHeader
{
 int8_t identsize;//размер поля ID заголовка (0)
 int8_t colorMapType;//если ли палитра:0-нет,1-есть
 int8_t imageType;//тип картинки:0-нет,1-индексные цвета,2-RGB,3-оттенки серого, (3-й бит - RLE- кодирование)
 uint16_t colorMapStart;//начало карты цветов
 uint16_t colorMapLength;//количество цветов в карте
 uint8_t colorMapBits;//размерность палитры
 uint16_t xstart;//начальные координаты изображения
 uint16_t ystart;                 
 uint16_t width;//размер изображения по X
 uint16_t height;//размер изображения по Y
 int8_t bits;//количесто бит на пиксель (8,16,24,32)
 int8_t descriptor;//дескриптор изрображения
};
#pragma pack()

uint8_t *LoadTGAFromFile(const char *file_name,uint32_t &width,uint32_t &height);
uint8_t *LoadTGAFromResource(HMODULE hModule,uint32_t id,uint32_t &width,uint32_t &height);
bool SaveTGA(const char *file_name,uint32_t width,uint32_t height,uint8_t *image);

#endif