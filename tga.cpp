#include "tga.h"

uint8_t *LoadTGAFromFile(const char *file_name,uint32_t &width,uint32_t &height)
{
 //������� ������� �����������
 STGAHeader sTGAHeader;
 FILE *file;
 file=fopen(file_name,"rb");//��������� ����
 if (file==NULL) return(NULL);//������

 //������ ���������
 if (fread(&sTGAHeader,sizeof(STGAHeader),1,file)<=0)//������ - ���� ������
 {
  fclose(file);
  return(NULL);
 } 
 //��������� �� ����������� ������
 if (sTGAHeader.imageType&8)
 {
  fclose(file);
  return(NULL);//RLE �� ������������
 }
 if ((sTGAHeader.imageType&7)==0 || (sTGAHeader.imageType&7)==3)
 {
  fclose(file);
  return(NULL);//�������� ������ � ���������� ����������� �� ������������
 }
 //����� ��������� �����������
 width=sTGAHeader.width;
 height=sTGAHeader.height;
 int32_t LineLength=sTGAHeader.width*4;
 //�������� ������ ��� �����������
 int32_t image_length=sTGAHeader.width*sTGAHeader.height*sTGAHeader.bits/8;
 uint8_t *i_buffer=new uint8_t[image_length];
 //��������� �����������
 fseek(file,sizeof(struct STGAHeader)+sTGAHeader.colorMapStart+sTGAHeader.colorMapLength*sTGAHeader.colorMapBits/8,SEEK_SET);
 if (fread(i_buffer,image_length,1,file)<=0)
 {
  fclose(file);
  delete[](i_buffer);
  return(NULL);
 }
 //� ������ ����������� ������
 if (sTGAHeader.bits==24)//BGR - ������������ ��� ������� ����
 {

  fclose(file);
  uint8_t *out_image=new uint8_t[sTGAHeader.width*sTGAHeader.height*4];
  int32_t y,x;
  if (sTGAHeader.descriptor==32)//������ ������
  {
   uint8_t *oi_ptr=out_image;
   uint8_t *i_ptr=i_buffer;
   for(y=0;y<sTGAHeader.height;y++,i_ptr+=sTGAHeader.width*3)
   {
    uint8_t *i_ptrc=i_ptr;
    for(x=0;x<sTGAHeader.width;x++)
    {
     uint8_t b=*(i_ptrc);i_ptrc++;
     uint8_t g=*(i_ptrc);i_ptrc++;
     uint8_t r=*(i_ptrc);i_ptrc++;
     uint8_t a=1;
     *oi_ptr=b;oi_ptr++;
     *oi_ptr=g;oi_ptr++;
     *oi_ptr=r;oi_ptr++;
     *oi_ptr=a;oi_ptr++;
    }
   }
  }
  if (sTGAHeader.descriptor==8)//�������� ������
  {
   uint8_t *oi_ptr=out_image;
   uint8_t *i_ptr=i_buffer+sTGAHeader.width*sTGAHeader.height*3-1;
   for(y=sTGAHeader.height-1;y>=0;y--,i_ptr-=sTGAHeader.width*3)
   {
    uint8_t *i_ptrc=i_ptr;
    for(x=0;x<sTGAHeader.width;x++)
    {
     uint8_t b=*(i_ptrc);i_ptrc++;
     uint8_t g=*(i_ptrc);i_ptrc++;
     uint8_t r=*(i_ptrc);i_ptrc++;
     uint8_t a=1;
     *oi_ptr=b;oi_ptr++;
     *oi_ptr=g;oi_ptr++;
     *oi_ptr=r;oi_ptr++;
     *oi_ptr=a;oi_ptr++;
    }
   }
  }
  delete[](i_buffer);
  return(out_image);
 }

 //� ������ ����������� ������
 if (sTGAHeader.bits==32)//BGR - ������������ ��� ������� ����
 {

  fclose(file);
  uint8_t *out_image=new uint8_t[sTGAHeader.width*sTGAHeader.height*4];
  int32_t y,x;
  if (sTGAHeader.descriptor==32)//������ ������
  {
   uint8_t *oi_ptr=out_image;
   uint8_t *i_ptr=i_buffer;
   for(y=0;y<sTGAHeader.height;y++,i_ptr+=sTGAHeader.width*4)
   {
    uint8_t *i_ptrc=i_ptr;
    for(x=0;x<sTGAHeader.width;x++)
    {
     uint8_t b=*(i_ptrc);i_ptrc++;
     uint8_t g=*(i_ptrc);i_ptrc++;
     uint8_t r=*(i_ptrc);i_ptrc++;
     uint8_t a=*(i_ptrc);i_ptrc++;
     *oi_ptr=b;oi_ptr++;
     *oi_ptr=g;oi_ptr++;
     *oi_ptr=r;oi_ptr++;
     *oi_ptr=a;oi_ptr++;
    }
   }
  }
  if (sTGAHeader.descriptor==8)//�������� ������
  {
   uint8_t *oi_ptr=out_image;
   uint8_t *i_ptr=i_buffer+sTGAHeader.width*sTGAHeader.height*3-1;
   for(y=sTGAHeader.height-1;y>=0;y--,i_ptr-=sTGAHeader.width*4)
   {
    uint8_t *i_ptrc=i_ptr;
    for(x=0;x<sTGAHeader.width;x++)
    {
     uint8_t b=*(i_ptrc);i_ptrc++;
     uint8_t g=*(i_ptrc);i_ptrc++;
     uint8_t r=*(i_ptrc);i_ptrc++;
     uint8_t a=*(i_ptrc);i_ptrc++;
     *oi_ptr=b;oi_ptr++;
     *oi_ptr=g;oi_ptr++;
     *oi_ptr=r;oi_ptr++;
     *oi_ptr=a;oi_ptr++;
    }
   }
  }
  delete[](i_buffer);
  return(out_image);
 }

 if (sTGAHeader.colorMapType==1 && sTGAHeader.colorMapBits/8==3)//���� ������� �� 24 ����
 {
  fseek(file,sTGAHeader.colorMapStart+sizeof(struct STGAHeader),SEEK_SET);
  //������ �������
  uint8_t *color_map=new uint8_t[sTGAHeader.colorMapLength*3];
  if (fread(color_map,sTGAHeader.colorMapLength*3,1,file)<=0)
  {
   fclose(file);
   delete[](color_map);
   delete[](i_buffer);
   return(NULL);
  }
  //��� ����������� �������� ������
  uint8_t *out_image=new uint8_t[sTGAHeader.width*sTGAHeader.height*4];
  int32_t y,x;
  if (sTGAHeader.descriptor==32)//������ ������
  {
   uint8_t *oi_ptr=out_image;
   uint8_t *i_ptr=i_buffer;
   for(y=0;y<sTGAHeader.height;y++,i_ptr+=sTGAHeader.width)
   {
    uint8_t *i_ptrc=i_ptr;
    for(x=0;x<sTGAHeader.width;x++,i_ptrc++)
    {
     int32_t index=(*i_ptrc)*3;
     uint8_t b=color_map[index];
     uint8_t g=color_map[index+1];
     uint8_t r=color_map[index+2];
     uint8_t a=1;
     *oi_ptr=b;oi_ptr++;
     *oi_ptr=g;oi_ptr++;
     *oi_ptr=r;oi_ptr++;
     *oi_ptr=a;oi_ptr++;
    }
   }
  }
  if (sTGAHeader.descriptor==8)//������ �����������
  {
   uint8_t *oi_ptr=out_image;
   uint8_t *i_ptr=i_buffer+sTGAHeader.width*(sTGAHeader.height-1);
   for(y=sTGAHeader.height-1;y>=0;y--,i_ptr-=sTGAHeader.width)
   {
    uint8_t *i_ptrc=i_ptr;
    for(x=0;x<sTGAHeader.width;x++,i_ptrc++)
    {
     int32_t index=(*i_ptrc)*3;
     uint8_t b=color_map[index];
     uint8_t g=color_map[index+1];
     uint8_t r=color_map[index+2];
     uint8_t a=1;
     *oi_ptr=b;oi_ptr++;
     *oi_ptr=g;oi_ptr++;
     *oi_ptr=r;oi_ptr++;
     *oi_ptr=a;oi_ptr++;
    }
   }
  }
  fclose(file);
  delete[](i_buffer);
  delete[](color_map);
  return(out_image);  
 }
  //���� ������ �� ������������
 fclose(file);
 delete[](i_buffer);
 return(NULL);
}
uint8_t *LoadTGAFromResource(HMODULE hModule,uint32_t id,int32_t &width,uint32_t &height)
{
 HRSRC hRSRC=FindResource(hModule,(LPSTR)id,RT_RCDATA);
 if (hRSRC==NULL) return(NULL);
 HGLOBAL hGlobal=LoadResource(hModule,hRSRC);
 if (hGlobal==NULL) return(NULL);
 uint8_t *Data=(uint8_t*)LockResource(hGlobal);
 int32_t DataSize=SizeofResource(hModule,hRSRC);
 //� ������ ��������� ������
 if (DataSize<sizeof(STGAHeader))//���� ��������
 {
  GlobalUnlock(hGlobal);
  return(NULL);
 }
 STGAHeader *sTGAHeader;
 //������ ���������
 sTGAHeader=(STGAHeader*)Data;
 //��������� �� ����������� ������
 if (sTGAHeader->imageType&8)
 {
  GlobalUnlock(hGlobal);
  return(NULL);//RLE �� ������������
 }
 if ((sTGAHeader->imageType&7)==0 || (sTGAHeader->imageType&7)==3)
 {
  GlobalUnlock(hGlobal);
  return(NULL);//�������� ������ � ���������� ����������� �� ������������
 }
 //����� ��������� �����������
 width=sTGAHeader->width;
 height=sTGAHeader->height;
 int32_t LineLength=sTGAHeader->width*4;
 //�������� ������ ��� �����������
 int32_t image_length=sTGAHeader->width*sTGAHeader->height*sTGAHeader->bits/8;
 uint8_t *i_buffer=new uint8_t[image_length];
 //��������� �����������
 int32_t offset=sizeof(STGAHeader)+sTGAHeader->colorMapStart+sTGAHeader->colorMapLength*sTGAHeader->colorMapBits/8;
 if (offset+image_length>DataSize)//���� ��������
 {
  GlobalUnlock(hGlobal);
  delete[](i_buffer);
  return(NULL);
 }
 memcpy(i_buffer,Data+offset,image_length);//�������� ������
 //� ������ ����������� ������
 if (sTGAHeader->bits==24)//BGR - ������������ ��� ������� ����
 {
  GlobalUnlock(hGlobal);
  uint8_t *out_image=new uint8_t[sTGAHeader->width*sTGAHeader->height*4];
  int32_t y,x;
  if (sTGAHeader->descriptor==32)//������ ������
  {
   uint8_t *oi_ptr=out_image;
   uint8_t *i_ptr=i_buffer;
   for(y=0;y<sTGAHeader->height;y++,i_ptr+=sTGAHeader->width*3)
   {
    uint8_t *i_ptrc=i_ptr;
    for(x=0;x<sTGAHeader->width;x++)
    {
     uint8_t b=*(i_ptrc);i_ptrc++;
     uint8_t g=*(i_ptrc);i_ptrc++;
     uint8_t r=*(i_ptrc);i_ptrc++;
     uint8_t a=1;
     *oi_ptr=b;oi_ptr++;
     *oi_ptr=g;oi_ptr++;
     *oi_ptr=r;oi_ptr++;
     *oi_ptr=a;oi_ptr++;
    }
   }
  }
  if (sTGAHeader->descriptor==8)//�������� ������
  {
   uint8_t *oi_ptr=out_image;
   uint8_t *i_ptr=i_buffer+sTGAHeader->width*sTGAHeader->height*3-1;
   for(y=sTGAHeader->height-1;y>=0;y--,i_ptr-=sTGAHeader->width*3)
   {
    uint8_t *i_ptrc=i_ptr;
    for(x=0;x<sTGAHeader->width;x++)
    {
     uint8_t b=*(i_ptrc);i_ptrc++;
     uint8_t g=*(i_ptrc);i_ptrc++;
     uint8_t r=*(i_ptrc);i_ptrc++;
     uint8_t a=1;
     *oi_ptr=b;oi_ptr++;
     *oi_ptr=g;oi_ptr++;
     *oi_ptr=r;oi_ptr++;
     *oi_ptr=a;oi_ptr++;
    }
   }
  }
  delete[](i_buffer);
  return(out_image);  
 }

 if (sTGAHeader->bits==32)//BGR - ������������ ��� ������� ����
 {
  GlobalUnlock(hGlobal);
  uint8_t *out_image=new uint8_t[sTGAHeader->width*sTGAHeader->height*4];
  int32_t y,x;
  if (sTGAHeader->descriptor==32)//������ ������
  {
   uint8_t *oi_ptr=out_image;
   uint8_t *i_ptr=i_buffer;
   for(y=0;y<sTGAHeader->height;y++,i_ptr+=sTGAHeader->width*4)
   {
    uint8_t *i_ptrc=i_ptr;
    for(x=0;x<sTGAHeader->width;x++)
    {
     uint8_t b=*(i_ptrc);i_ptrc++;
     uint8_t g=*(i_ptrc);i_ptrc++;
     uint8_t r=*(i_ptrc);i_ptrc++;
     uint8_t a=*(i_ptrc);i_ptrc++;
     *oi_ptr=b;oi_ptr++;
     *oi_ptr=g;oi_ptr++;
     *oi_ptr=r;oi_ptr++;
     *oi_ptr=a;oi_ptr++;
    }
   }
  }
  if (sTGAHeader->descriptor==8)//�������� ������
  {
   uint8_t *oi_ptr=out_image;
   uint8_t *i_ptr=i_buffer+sTGAHeader->width*sTGAHeader->height*3-1;
   for(y=sTGAHeader->height-1;y>=0;y--,i_ptr-=sTGAHeader->width*4)
   {
    uint8_t *i_ptrc=i_ptr;
    for(x=0;x<sTGAHeader->width;x++)
    {
     uint8_t b=*(i_ptrc);i_ptrc++;
     uint8_t g=*(i_ptrc);i_ptrc++;
     uint8_t r=*(i_ptrc);i_ptrc++;
     uint8_t a=*(i_ptrc);i_ptrc++;
     *oi_ptr=b;oi_ptr++;
     *oi_ptr=g;oi_ptr++;
     *oi_ptr=r;oi_ptr++;
     *oi_ptr=a;oi_ptr++;
    }
   }
  }
  delete[](i_buffer);
  return(out_image);  
 }


 if (sTGAHeader->colorMapType==1 && sTGAHeader->colorMapBits/8==3)//���� ������� �� 24 ����
 {
  int32_t offset=sTGAHeader->colorMapStart+sizeof(STGAHeader);
  if (offset+sTGAHeader->colorMapLength*3>DataSize)
  {
   GlobalUnlock(hGlobal);
   delete[](i_buffer);
   return(NULL);
  }
  uint8_t *color_map=Data+offset;   
  //��� ����������� �������� ������
  uint8_t *out_image=new uint8_t[sTGAHeader->width*sTGAHeader->height*4];
  int32_t y,x;
  if (sTGAHeader->descriptor==32)//������ ������
  {
   uint8_t *oi_ptr=out_image;
   uint8_t *i_ptr=i_buffer;
   for(y=0;y<sTGAHeader->height;y++,i_ptr+=sTGAHeader->width)
   {
    uint8_t *i_ptrc=i_ptr;
    for(x=0;x<sTGAHeader->width;x++,i_ptrc++)
    {
     int32_t index=(*i_ptrc)*3;
     uint8_t b=color_map[index];
     uint8_t g=color_map[index+1];
     uint8_t r=color_map[index+2];
     uint8_t a=1;
     *oi_ptr=b;oi_ptr++;
     *oi_ptr=g;oi_ptr++;
     *oi_ptr=r;oi_ptr++;
     *oi_ptr=a;oi_ptr++;
    }
   }
  }
  if (sTGAHeader->descriptor==8)//������ �����������
  {
   uint8_t *oi_ptr=out_image;
   uint8_t *i_ptr=i_buffer+sTGAHeader->width*(sTGAHeader->height-1);
   for(y=sTGAHeader->height-1;y>=0;y--,i_ptr-=sTGAHeader->width)
   {
    uint8_t *i_ptrc=i_ptr;
    for(x=0;x<sTGAHeader->width;x++,i_ptrc++)
    {
     int32_t index=(*i_ptrc)*3;
     uint8_t b=color_map[index];
     uint8_t g=color_map[index+1];
     uint8_t r=color_map[index+2];
     uint8_t a=1;
     *oi_ptr=b;oi_ptr++;
     *oi_ptr=g;oi_ptr++;
     *oi_ptr=r;oi_ptr++;
     *oi_ptr=a;oi_ptr++;
    }
   }
  }
  GlobalUnlock(hGlobal);
  delete[](i_buffer);
  delete[](color_map);
  return(out_image);  
 }
 //���� ������ �� ������������
 delete[](i_buffer);
 GlobalUnlock(hGlobal);
 return(NULL); 
}

bool SaveTGA(const char *file_name,uint32_t width,uint32_t height,uint8_t *Image)
{
 struct STGAHeader sTGAHeader;
 sTGAHeader.identsize=0;
 sTGAHeader.colorMapType=0;
 sTGAHeader.imageType=2;
 sTGAHeader.colorMapStart=0;
 sTGAHeader.colorMapLength=0;
 sTGAHeader.colorMapBits=24;
 sTGAHeader.xstart=0;
 sTGAHeader.ystart=0;
 sTGAHeader.width=static_cast<uint16_t>(width);
 sTGAHeader.height=static_cast<uint16_t>(height);
 sTGAHeader.bits=32;
 sTGAHeader.descriptor=32;
 FILE *file=fopen(file_name,"wb");
 if (file==NULL) return(false);//������ �������� �����
 //��������� ���������
 if (fwrite(&sTGAHeader,1,sizeof(struct STGAHeader),file)<sizeof(struct STGAHeader))//������ ������
 {
  fclose(file);
  return(false);
 }
 //��������� � ����
 if (fwrite(Image,sizeof(uint32_t),width*height,file)<(uint32_t)(width*height))//������ ������
 {
  fclose(file);
  return(false);
 }
 //��������� ����
 fclose(file);
 return(true);
}
