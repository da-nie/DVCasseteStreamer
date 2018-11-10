//====================================================================================================
//подключаемые библиотеки
//====================================================================================================
#include "cdvcontrol.h"
#include "tga.h"
#include "craiifilein.h"
#include "craiifileout.h"
#include "cgraphics.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

//====================================================================================================
//глобальные переменные
//====================================================================================================
static const int32_t DV_SuperMapVerical[5]={2,6,8,0,4};
static const int32_t DV_SuperMapHorizontal[5]={2,1,3,0,4};
static const int32_t DV_ParseBitStart[6]={4,18,32,46,60,70};
static const int32_t DV_ParseBitEnd[6]={18,32,46,60,70,80};

//====================================================================================================
//конструктор класса
//====================================================================================================
CDVControl::CDVControl(void)
{ 
 uint8_t *ptr=BackGroundImage;
 for(uint32_t y=0;y<DV_PAL_HEIGHT;y++)
 {
  for(uint32_t x=0;x<DV_WIDTH;x++)
  {
   *ptr=0;
   ptr++;
   *ptr=0;
   ptr++;
   *ptr=0;
   ptr++;
  }
 }

 NewRecord();
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CDVControl::~CDVControl()
{

}

//====================================================================================================
//закрытые функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//округление до заданного количества цифр после запятой
//----------------------------------------------------------------------------------------------------
double CDVControl::Round(double value,uint32_t digit)
{
 double d=1;
 for(uint32_t n=0;n<digit;n++) d*=10;
 if (value>=0) return(floor(value*d+0.5)/d);
 return(ceil(value*d-0.5)/d);
}
//----------------------------------------------------------------------------------------------------
//округление до ближайшего целого
//----------------------------------------------------------------------------------------------------
int32_t CDVControl::RoundToInt(double value)
{
 return(static_cast<int32_t>(Round(value,0)));
}
//----------------------------------------------------------------------------------------------------
//алгоритм вычисления CRC
//----------------------------------------------------------------------------------------------------
uint32_t CDVControl::CreateCRC(uint32_t last_crc,const uint8_t* ptr,size_t length)
{
 //используется алгоритм Adler32
 if (ptr==NULL) return(1UL);
 uint32_t sum1=last_crc;
 uint32_t sum2=0;
 sum2=(sum1>>16)&0xffff;
 sum1&=0xffff;
 while(length>0)
 {
  sum1=(sum1+(*ptr))%65521;
  sum2=(sum2+sum1)%65521;
  length--;
  ptr++;
 }
 return((sum2<<16)+sum1);
}
//----------------------------------------------------------------------------------------------------
//преобразовать в short
//----------------------------------------------------------------------------------------------------
uint16_t CDVControl::ReadShort(uint8_t* src)
{
return (src[0]<<8)|src[1];
}
//----------------------------------------------------------------------------------------------------
//преобразовать в long
//----------------------------------------------------------------------------------------------------
uint32_t CDVControl::ReadLong(uint8_t* src)
{
return (src[0]<<24)|(src[1]<<16)|(src[2]<<8)|src[3];
}
//----------------------------------------------------------------------------------------------------
//преобразовать в longlong
//----------------------------------------------------------------------------------------------------
uint64_t CDVControl::ReadLongLong(uint8_t* src)
{
 uint64_t ret=0;
 ret|=((uint64_t)src[0]<<56);
 ret|=((uint64_t)src[1]<<48);
 ret|=((uint64_t)src[2]<<40);
 ret|=((uint64_t)src[3]<<32);
 ret|=((uint64_t)src[4]<<24);
 ret|=((uint64_t)src[5]<<16);
 ret|=((uint64_t)src[6]<<8);
 ret|=((uint64_t)src[7]);
 return(ret);
}

//----------------------------------------------------------------------------------------------------
//создать short
//----------------------------------------------------------------------------------------------------
void CDVControl::WriteShort(uint8_t* dst,uint16_t val)
{
 dst[0]=static_cast<uint8_t>(val>>8);
 dst[1]=static_cast<uint8_t>(val);
}
//----------------------------------------------------------------------------------------------------
//создать long
//----------------------------------------------------------------------------------------------------
void CDVControl::WriteLong(uint8_t* dst,uint32_t val)
{
 dst[0]=static_cast<uint8_t>(val>>24);
 dst[1]=static_cast<uint8_t>(val>>16);
 dst[2]=static_cast<uint8_t>(val>>8);
 dst[3]=static_cast<uint8_t>(val);
}
//----------------------------------------------------------------------------------------------------
//создать long long
//----------------------------------------------------------------------------------------------------
void CDVControl::WriteLongLong(uint8_t* dst,uint64_t val)
{
 dst[0]=static_cast<uint8_t>(val>>56);
 dst[1]=static_cast<uint8_t>(val>>48);
 dst[2]=static_cast<uint8_t>(val>>40);
 dst[3]=static_cast<uint8_t>(val>>32);
 dst[4]=static_cast<uint8_t>(val>>24);
 dst[5]=static_cast<uint8_t>(val>>16);
 dst[6]=static_cast<uint8_t>(val>>8);
 dst[7]=static_cast<uint8_t>(val);
}

//----------------------------------------------------------------------------------------------------
//установка 411 макроблока
//----------------------------------------------------------------------------------------------------
void CDVControl::DV_Place411MacroBlock(SDVMacroBlock *sDVMacroBlock_Ptr) 
{
 int32_t mb_num;
 int32_t mb_num_mod_6,mb_num_div_6;
 int32_t mb_row;
 int32_t mb_col;
 static const uint32_t column_offset[]={0,4,9,13,18};

 mb_num=((sDVMacroBlock_Ptr->Col%2)==1)?sDVMacroBlock_Ptr->WithinSuperBlock+3:sDVMacroBlock_Ptr->WithinSuperBlock;
 mb_num_mod_6=mb_num%6;
 mb_num_div_6=mb_num/6;

 mb_row=((mb_num_div_6%2)==0)?mb_num_mod_6:(5-mb_num_mod_6);
 mb_col=(mb_num_div_6+column_offset[sDVMacroBlock_Ptr->Col])*4;
 if (mb_col<(22*4)) mb_row+=(sDVMacroBlock_Ptr->Row*6);
 else mb_row=mb_row*2+sDVMacroBlock_Ptr->Row*6;
 sDVMacroBlock_Ptr->XLeftTop=mb_col;
 sDVMacroBlock_Ptr->YLeftTop=mb_row;
}
//----------------------------------------------------------------------------------------------------
//установка 420 макроблока
//----------------------------------------------------------------------------------------------------
void CDVControl::DV_Place420MacroBlock(SDVMacroBlock *sDVMacroBlock_Ptr) 
{
 int32_t mb_num;
 int32_t mb_num_mod_3,mb_num_div_3;
 int32_t mb_row;
 int32_t mb_col;
 static const int32_t column_offset[]={0,9,18,27,36};
 mb_num=sDVMacroBlock_Ptr->WithinSuperBlock;
 mb_num_mod_3=mb_num%3;
 mb_num_div_3=mb_num/3;
 mb_row=((mb_num_div_3%2)==0)?mb_num_mod_3:(2-mb_num_mod_3);
 mb_col=mb_num_div_3+column_offset[sDVMacroBlock_Ptr->Col];
 mb_row+=(sDVMacroBlock_Ptr->Row*3);
 sDVMacroBlock_Ptr->XLeftTop=mb_col*2;
 sDVMacroBlock_Ptr->YLeftTop=mb_row*2;
}
//----------------------------------------------------------------------------------------------------
//конвертация в палитру YUV
//----------------------------------------------------------------------------------------------------
void CDVControl::ConvertToYUV(uint8_t* img_rgb,int32_t height,int16_t* img_y,int16_t* img_cr,int16_t* img_cb)
{
 int32_t x,y;

 static const double cm[3][3]=
 {
  {0.299*219.0/255.0,0.587*219.0/255.0,0.114*219.0/255.0},
  {0.5*224.0/255.0,-0.419*224.0/255.0,-0.081*224.0/255.0},
  {-0.169*224.0/255.0,-0.331*224.0/255.0,0.5*224.0/255.0}
 };

 double tmp_cr[DV_PAL_HEIGHT][DV_WIDTH];
 double tmp_cb[DV_PAL_HEIGHT][DV_WIDTH];

 for(y=0;y<height;y++) 
 {
  for(x=0;x<DV_WIDTH;x++) 
  {
   double cy;
   double cr;
   double cb;
   double val;
   int32_t r=img_rgb[(y*DV_WIDTH+x)*3+0];
   int32_t g=img_rgb[(y*DV_WIDTH+x)*3+1];
   int32_t b=img_rgb[(y*DV_WIDTH+x)*3+2];
   cy=(cm[0][0]*r)+(cm[0][1]*g)+(cm[0][2]*b);
   cr=(cm[1][0]*r)+(cm[1][1]*g)+(cm[1][2]*b);
   cb=(cm[2][0]*r)+(cm[2][1]*g)+(cm[2][2]*b);

   val=cy-128+16;

   img_y[y*DV_WIDTH+x]=RoundToInt(static_cast<float>(val*2));
   tmp_cr[y][x]=cr;
   tmp_cb[y][x]=cb;
  }
 }

 for(y=0;y<height;y++) 
 {
  for(x=0;x<DV_WIDTH/2;x++)
  {
   img_cr[y*DV_WIDTH/2+x]=RoundToInt(static_cast<float>((tmp_cr[y][2*x]+tmp_cr[y][2*x+1])/2.0));
   img_cb[y*DV_WIDTH/2+x]=RoundToInt(static_cast<float>((tmp_cb[y][2*x]+tmp_cb[y][2*x+1])/2.0));
  }
 }
}
//----------------------------------------------------------------------------------------------------
//построение блока коэффициентов
//----------------------------------------------------------------------------------------------------
void CDVControl::BuildCoeff(int16_t* img_y,int16_t* img_cr,int16_t* img_cb,SDVMacroBlock *sDVMacroBlock_Ptr,bool is_pal)
{
 int32_t y=sDVMacroBlock_Ptr->YLeftTop;
 int32_t x=sDVMacroBlock_Ptr->XLeftTop;

 if (is_pal==true || sDVMacroBlock_Ptr->XLeftTop==DV_WIDTH-2)
 {
  sDVMacroBlock_Ptr->DC[0]=img_y[y*DV_WIDTH+x+0];
  sDVMacroBlock_Ptr->DC[1]=img_y[y*DV_WIDTH+x+1];
  sDVMacroBlock_Ptr->DC[2]=img_y[(y+1)*DV_WIDTH+x+0];
  sDVMacroBlock_Ptr->DC[3]=img_y[(y+1)*DV_WIDTH+x+1];
  sDVMacroBlock_Ptr->DC[4]=(img_cr[y*DV_WIDTH/2+x/2]+img_cr[(y+1)*DV_WIDTH/2+x/2])>>1;
  sDVMacroBlock_Ptr->DC[5]=(img_cb[y*DV_WIDTH/2+x/2]+img_cb[(y+1)*DV_WIDTH/2+x/2])>>1;
 } 
 else 
 {
  sDVMacroBlock_Ptr->DC[0]=img_y[y*DV_WIDTH+x+0];
  sDVMacroBlock_Ptr->DC[1]=img_y[y*DV_WIDTH+x+1];
  sDVMacroBlock_Ptr->DC[2]=img_y[y*DV_WIDTH+x+2];
  sDVMacroBlock_Ptr->DC[3]=img_y[y*DV_WIDTH+x+3];
  sDVMacroBlock_Ptr->DC[4]=(img_cr[y*DV_WIDTH/2+x/2]+img_cr[y*DV_WIDTH/2+x/2+1])>>1;
  sDVMacroBlock_Ptr->DC[5]=(img_cb[y*DV_WIDTH/2+x/2]+img_cb[y*DV_WIDTH/2+x/2+1])>>1;
 }
}
//----------------------------------------------------------------------------------------------------
//кодирование
//----------------------------------------------------------------------------------------------------
void CDVControl::Encode(int16_t* img_y,int16_t* img_cr,int16_t* img_cb,bool is_pal,uint8_t* target,SDVVideoSegment &sDVVideoSegment)
{
 int32_t numDIFseq;
 int32_t ds;
 int32_t v;
 uint32_t dif;
 uint32_t offset;

 memset(target,0,144000);

 dif=0;
 offset=dif*DIF_BLOCK_SIZE;
 if (is_pal==true) target[offset+3]|=0x80;
 numDIFseq=is_pal?12:10;

 for(ds=0;ds<numDIFseq;ds++) 
 { 
  dif+=6;
  for(v=0;v<27;v++) 
  {
   if (!(v%3)) dif++;
   offset=dif*DIF_BLOCK_SIZE;

   sDVVideoSegment.Row=ds;
   sDVVideoSegment.Col=v;
   sDVVideoSegment.IsPAL=is_pal;

   ProcessVideoSegment(img_y,img_cr,img_cb,&sDVVideoSegment,target+offset);
   dif+=5;
  } 
 } 
}

//----------------------------------------------------------------------------------------------------
//обработка видеосегмента
//----------------------------------------------------------------------------------------------------
void CDVControl::ProcessVideoSegment(int16_t* img_y,int16_t* img_cr,int16_t* img_cb,SDVVideoSegment *sDVVideoSegment_Ptr,uint8_t*vsbuffer)
{
 SDVMacroBlock *sDVMacroBlock_Ptr=sDVVideoSegment_Ptr->sDVMacroBlock;
 for(int32_t m=0;m<5;m++,sDVMacroBlock_Ptr++) 
 {
  sDVMacroBlock_Ptr->Row=(sDVVideoSegment_Ptr->Row+DV_SuperMapVerical[m])%(sDVVideoSegment_Ptr->IsPAL?12:10);
  sDVMacroBlock_Ptr->Col=DV_SuperMapHorizontal[m];
  sDVMacroBlock_Ptr->WithinSuperBlock=sDVVideoSegment_Ptr->Col;

  if (sDVVideoSegment_Ptr->IsPAL==true) DV_Place420MacroBlock(sDVMacroBlock_Ptr);
                                   else DV_Place411MacroBlock(sDVMacroBlock_Ptr);
 
  BuildCoeff(img_y,img_cr,img_cb,sDVMacroBlock_Ptr,sDVVideoSegment_Ptr->IsPAL);

  for(uint32_t b=0;b<6;b++) 
  {
   int32_t ofs=(DIF_BLOCK_SIZE*m)+DV_ParseBitStart[b];
   vsbuffer[ofs]=(sDVMacroBlock_Ptr->DC[b]>>1)&0xff;
   vsbuffer[ofs+1]=((sDVMacroBlock_Ptr->DC[b]<<7)&0xff)|0x6;
  }
 }
}

//----------------------------------------------------------------------------------------------------
//создание информационных блоков
//----------------------------------------------------------------------------------------------------
void CDVControl::CreateInfoBlocks(uint8_t* target,int32_t frame,bool is_pal,CDVTime &cDVTime_Video,CDVTime &cDVTime_Current,bool enable_audio)
{ 
 uint8_t* frame_buf=target;
 uint8_t max_dif_seq=10;
 uint8_t frame_rate=CDVTime::NTSC_FRAME_RATE;
 if (is_pal==true)
 {
  max_dif_seq=12;
  frame_rate=CDVTime::PAL_FRAME_RATE;
 }
 if (frame%frame_rate==0)
 {
  cDVTime_Video.AddSecond();
  cDVTime_Current.AddSecond();
 }

 CDVHeader cDVHeader;
 CDVSubCode cDVSubCode;
 CDVVAUX cDVVAUX;
 CDVAudio cDVAudio;
 CDVVideo cDVVideo;
 for(uint8_t dif_seq=0;dif_seq<max_dif_seq;dif_seq++)
 { 
  CDVHeader::SHeader *sHeader_Ptr=reinterpret_cast<CDVHeader::SHeader*>(target);
  cDVHeader.Set(sHeader_Ptr,dif_seq,is_pal);
  target+=cDVHeader.GetSectionSize();
  CDVSubCode::SSubCode *sSubCode_Ptr=reinterpret_cast<CDVSubCode::SSubCode*>(target);
  cDVSubCode.Set(sSubCode_Ptr,dif_seq,frame,cDVTime_Current,cDVTime_Video,is_pal);
  target+=cDVSubCode.GetSectionSize();
  CDVVAUX::SVAUX *sVAUX_Ptr=reinterpret_cast<CDVVAUX::SVAUX*>(target);
  cDVVAUX.Set(sVAUX_Ptr,dif_seq,cDVTime_Current,is_pal);
  target+=cDVVAUX.GetSectionSize();
  //заполняем заголовки секции video
  uint8_t *ptr=target;
  uint8_t block=0;
  for(uint8_t n=0;n<9;n++)//9 секций по 16 блоков (0-аудио)
  {
   ptr+=cDVAudio.GetSectionSize();//пропускаем dif-блок audio
   for(uint8_t m=1;m<16;m++,ptr+=cDVVideo.GetSectionSize(),block++)
   {
    CDVVideo::SVideo *sVideo_Ptr=reinterpret_cast<CDVVideo::SVideo*>(ptr);
    cDVVideo.Set(sVideo_Ptr,dif_seq,block);
   }
  }
  //заполняем заголовки секции audio 
  ptr=target;
  for(uint8_t m=0;m<9;m++,ptr+=cDVAudio.GetSectionSize()+cDVVideo.GetSectionSize()*15) 
  {
   CDVAudio::SAudio *sAudio_Ptr=reinterpret_cast<CDVAudio::SAudio*>(ptr);
   cDVAudio.Set(sAudio_Ptr,dif_seq,m,cDVTime_Current,is_pal);
  }
  target+=9*(cDVAudio.GetSectionSize()*1+cDVVideo.GetSectionSize()*15);
 }
}

//----------------------------------------------------------------------------------------------------
//кодирование изображения
//----------------------------------------------------------------------------------------------------
void CDVControl::EncodePicture(uint8_t* readbuf,bool is_pal,uint8_t* target,SDVVideoSegment &sDVVideoSegment)
{	
 int16_t img_y[DV_PAL_HEIGHT*DV_WIDTH];
 int16_t img_cr[DV_PAL_HEIGHT*DV_WIDTH/2];
 int16_t img_cb[DV_PAL_HEIGHT*DV_WIDTH/2];
 ConvertToYUV(readbuf,is_pal?72:60,img_y,img_cr,img_cb);
 Encode(img_y,img_cr,img_cb,is_pal,target,sDVVideoSegment);
}
//----------------------------------------------------------------------------------------------------
//добавление информационных блоков
//----------------------------------------------------------------------------------------------------
void CDVControl::AddInfoBlocks(bool is_pal,uint8_t* target,CDVTime &cDVTime_Video,CDVTime &cDVTime_Current,bool enable_audio,int32_t &frame_counter)
{
 CreateInfoBlocks(target,frame_counter,is_pal,cDVTime_Video,cDVTime_Current,enable_audio);
 frame_counter++;
}
//----------------------------------------------------------------------------------------------------
//получить размер фрагмента
//----------------------------------------------------------------------------------------------------
int32_t CDVControl::GetChunkSize(bool is_pal)
{
 uint32_t numbytes=9*((80-3-5)+(12*4+2*8)*15);
 return(numbytes*(is_pal?12:10));
}

//----------------------------------------------------------------------------------------------------
//вставить данные
//----------------------------------------------------------------------------------------------------
void CDVControl::InsertData(uint8_t* src,bool is_pal,uint8_t* target)
{
 int32_t numDIFseq;
 int32_t ds,i,j,b;

 numDIFseq=is_pal?12:10;

 for(ds=0;ds<numDIFseq;ds++) 
 { 
  target+=6*DIF_BLOCK_SIZE;
  for(i=0;i<9;i++) 
  {
   //first use audio 
   memcpy(target+3+5,src,DIF_BLOCK_SIZE-3-5);
   target+=DIF_BLOCK_SIZE;
   src+=DIF_BLOCK_SIZE-3-5;
   for(j=1;j<16;j++) 
   {
	//j=0: audio blocks 
    for(b=0;b<4;b++) 
	{
     memcpy(target+DV_ParseBitStart[b]+2,src,12);
     src+=12;
    }
    for(b=4;b<6;b++)
	{
     memcpy(target+DV_ParseBitStart[b]+2,src,8);
     src+=8;
    }
    target+=DIF_BLOCK_SIZE;
   }
  }
 }
}
//----------------------------------------------------------------------------------------------------
//извлечь данные
//----------------------------------------------------------------------------------------------------
void CDVControl::ExtractData(uint8_t* src,uint8_t* target)
{
 int32_t numDIFseq;
 int32_t ds,i,j,b;
 int32_t IsPAL=src[3]&0x80;

 numDIFseq=IsPAL?12:10;

 for(ds=0;ds<numDIFseq;ds++) 
 { 
  src+=6*DIF_BLOCK_SIZE;
  for(i=0;i<9;i++) 
  {
   //first use audio 
   memcpy(target,src+3+5,DIF_BLOCK_SIZE-3-5);
   src+=DIF_BLOCK_SIZE;
   target+=DIF_BLOCK_SIZE-3-5;
   for(j=1;j<16;j++) 
   {
    //j=0: audio blocks 
    for(b=0;b<4;b++) 
    {
     memcpy(target,src+DV_ParseBitStart[b]+2,12);
     target+=12;
    }
    for(b=4;b<6;b++) 
    {
     memcpy(target,src+DV_ParseBitStart[b]+2,8);
     target+=8;
    }
    src+=DIF_BLOCK_SIZE;
   }
  }
 }
}

//----------------------------------------------------------------------------------------------------
//проверка данных
//----------------------------------------------------------------------------------------------------
bool CDVControl::VerifyData(uint8_t* databuffer,uint64_t &next_addr,uint16_t &old_header_size,std::string &backup_title,bool &end_data,std::string &answer,bool &new_file)
{
 end_data=false;
 new_file=false;
 answer="";

 uint64_t addr;
 uint32_t got;
 uint16_t header_size;
 int32_t flag_byte;
 uint32_t cksum=ReadLong(databuffer+19);
 uint32_t calc_cksum=CreateCRC(0L,NULL,0);
 
 WriteLong(databuffer+19,0);

 if (databuffer[0]!='D' || databuffer[1]!='V' || databuffer[2]!='-' || databuffer[3]!='B') 
 {
  answer+="Неверный идентификатор заголовка!\n";
  return(false);
 }

 header_size=ReadShort(databuffer+4);
 if (databuffer[6]!=DVBACKUP_VERSION) 
 {
  answer+="Неподдерживаемая версия программы!\n";
  return(false);
 }
 if (header_size<HEADER_SIZE) 
 {
  answer+="Неверный размер заголовка!\n";
  return(false);
 }

 got=ReadLong(databuffer+15);
 flag_byte=got>>24;
 got&=0x00ffffff;
 addr=ReadLongLong(databuffer+7);

 if (next_addr==0)
 {
  char *backup_title_str=new char[header_size-HEADER_SIZE+1]; 
  memcpy(backup_title_str,databuffer+23,header_size-HEADER_SIZE);
  backup_title_str[header_size-HEADER_SIZE]=0;
  backup_title=backup_title_str;
  delete[](backup_title_str);
  old_header_size=header_size;
  if (got==0) answer+="Найден префикс!\n";
  else
  {
   answer+="Найден новый файл: ";
   answer+=backup_title;
   answer+="\n";
   new_file=true;
  }  
 }
 else 
 {
  if (header_size!=old_header_size ||  memcmp(backup_title.c_str(),databuffer+23,header_size-HEADER_SIZE)!=0) 
  {
   answer+="Изменилось имя файла! Новое имя: ";
   answer+=backup_title;
   answer+="\n";

   char *backup_title_str=new char[header_size-HEADER_SIZE+1]; 
   memcpy(backup_title_str,databuffer+23,header_size-HEADER_SIZE);
   backup_title_str[header_size-HEADER_SIZE]=0;
   backup_title=backup_title_str;
   delete[](backup_title_str);
   old_header_size=header_size;
   new_file=true;
   next_addr=0;
  }
  char str[255];
  sprintf(str,"%lld",addr);
  answer+="Считан адрес:";
  answer+=str;
  answer+=" файла "+backup_title;
  answer+="\n";
 }

 calc_cksum=CreateCRC(calc_cksum,databuffer,header_size+got);
 if (cksum!=calc_cksum)
 {
  answer+="Ошибка контрольной суммы блока!\n";
  next_addr=addr+got;
  return(false);
 }

 if (addr!=next_addr) 
 {
  char str[255];
  answer+="Сбой адресов блоков! Принято:";
  sprintf(str,"%lld",addr);
  answer+=str;
  sprintf(str,"%lld",next_addr);
  answer+=" Нужен:";
  answer+=str;
  answer+="\n";
  return(false);
 }

 next_addr=addr+got;

 if (flag_byte&0x01) 
 {
  answer+="Файл ";
  answer+=backup_title;
  answer+=" считан весь.\n";
  next_addr=0;
  end_data=true;
 }
 return(true);
}
//----------------------------------------------------------------------------------------------------
//запись извлечённых данных
//----------------------------------------------------------------------------------------------------
void CDVControl::WriteExtractedData(uint8_t* databuffer,bool do_recover,IExtractDataStream *iExtractDataStream_Ptr,uint16_t &last_header_size,uint32_t &last_got)
{
 uint16_t header_size=ReadShort(databuffer+4);
 uint32_t got=ReadLong(databuffer+15)&0x00ffffff;

 if (do_recover==true)
 {
  if (last_header_size!=0 && last_got!=0)
  {
   header_size=last_header_size;
   got=last_got;
  }
 }
 iExtractDataStream_Ptr->Write(databuffer+header_size,got);

 last_header_size=header_size;
 last_got=got;
}
//----------------------------------------------------------------------------------------------------
//проверка конца данных
//----------------------------------------------------------------------------------------------------
bool CDVControl::VerifyEOF(uint64_t next_addr)
{
 if (next_addr!=0) return(false);
 return(true);
}

//----------------------------------------------------------------------------------------------------
//получить размер заголовка
//----------------------------------------------------------------------------------------------------
long CDVControl::GetHeaderSize(const char* backup_title)
{
 if (!backup_title) return HEADER_SIZE;
 return(HEADER_SIZE+strlen(backup_title)); 
}
//----------------------------------------------------------------------------------------------------
//создать заголовок
//----------------------------------------------------------------------------------------------------
void CDVControl::BuildHeader(uint8_t* databuffer,uint64_t current_address,uint32_t got,uint32_t header_size,int32_t eof,const char* backup_title)
{
 uint32_t cksum=CreateCRC(0L,NULL,0);

 databuffer[0]='D';
 databuffer[1]='V';
 databuffer[2]='-';
 databuffer[3]='B';

 WriteShort(databuffer+4,header_size);
 databuffer[6]=DVBACKUP_VERSION;
 WriteLongLong(databuffer+7,current_address);
 WriteLong(databuffer+15,got|(eof?0x01000000:0));
 WriteLong(databuffer+19,0);
 if (backup_title) memcpy(databuffer+23,backup_title,strlen(backup_title));
 WriteLong(databuffer+19,CreateCRC(cksum,databuffer,header_size+got));
}


//====================================================================================================
//открытые функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//загрузить фоновое изображение
//----------------------------------------------------------------------------------------------------
bool CDVControl::LoadBackgroundImage(const std::string &file_name,bool is_pal)
{
 uint32_t width;
 uint32_t height;
 uint8_t *image=LoadTGAFromFile(file_name.c_str(),width,height);
 if (image==NULL) return(false);
 if (width!=DV_WIDTH || (height!=DV_PAL_HEIGHT && is_pal==true) || (height!=DV_NTSC_HEIGHT && is_pal==false))
 {
  delete[](image);
  return(false);
 }
 uint8_t *ptr=BackGroundImage;
 uint8_t *i_ptr=image; 
 for(uint32_t y=0;y<height;y++)
 {
  for(uint32_t x=0;x<width;x++)
  {
   uint8_t b=*i_ptr;i_ptr++;
   uint8_t g=*i_ptr;i_ptr++;
   uint8_t r=*i_ptr;i_ptr++;
   i_ptr++;

   *ptr=r;ptr++;
   *ptr=g;ptr++;
   *ptr=b;ptr++;   
  }
 }
 delete[](image);
 return(true);
}
//----------------------------------------------------------------------------------------------------
//создать DV-файл с данными
//----------------------------------------------------------------------------------------------------
bool CDVControl::CreateDV(IOutputDVStream *iOutputDVStream_Ptr,const std::string &file_name,const std::string &title_file_name,bool is_pal,uint32_t prefix,CDVTime &cDVTime_Current)
{ 
 iOutputDVStream_Ptr->AddAnswer("Добавляю файл:"+title_file_name+"\n");

 uint8_t framebuffer[144000];
 uint8_t databuffer[144000];
 uint32_t chunk_size=GetChunkSize(is_pal);
 uint32_t header_size=GetHeaderSize(title_file_name.c_str());

 uint32_t height=DV_PAL_HEIGHT;
 uint32_t frame_size=144000;
 if (is_pal==false)
 {
  height=DV_NTSC_HEIGHT;
  frame_size=120000;
 }
 CRAIIFileIn cRAIIFileIn(file_name.c_str(),std::ios_base::in|std::ios_base::binary);

 if (cRAIIFileIn.IsOpened()==false) return(false);

 //узнаем размер входного файла
 cRAIIFileIn.GetHandle().seekg(0,std::ios_base::end);
 uint64_t file_size=cRAIIFileIn.GetHandle().tellg();
 cRAIIFileIn.GetHandle().seekg(0,std::ios_base::beg);

 //сохраняем данные
 uint8_t image[DV_WIDTH*DV_PAL_HEIGHT*3];//фоновое изображение с учётом текста
 CGraphics cGraphics;
 cGraphics.Init(image,DV_WIDTH,height);

 std::string name=title_file_name;//строка для скроллинга имени файла
 name+=" ";
 name+=title_file_name;
 name+=" ";
 size_t name_length=title_file_name.length();
 size_t name_pixel_length=(name_length+1)*FONT_WIDTH; 
 //вычислим скорость скроллинга имени файла
 int32_t symbol_in_width=static_cast<int32_t>(DV_WIDTH/FONT_WIDTH);//количество символов по ширине
 int32_t data_in_frame=chunk_size-header_size;//количество данных в кадре
 int32_t symbol_off_screen=(title_file_name.length()-symbol_in_width);
 double scroll_speed=0;//скорость прокрутки названия файла
 if (symbol_off_screen>0)//не все символы имени помещаются на экране
 {
  //определяем, сколько по времени кадров будет выводиться файл
  int32_t frame=file_size/data_in_frame;
  frame+=prefix;
  if (frame>0) scroll_speed=static_cast<double>(symbol_off_screen*FONT_WIDTH)/static_cast<double>(frame);  
 }	 
 double left_offset=0;

 SDVVideoSegment sDVVideoSegment;
 uint64_t current_address=0;
 while (true)
 {
  if (iOutputDVStream_Ptr->IsExit()==true)
  {
   iOutputDVStream_Ptr->AddAnswer("Отменено пользователем!\n");
   return(false);
  }
  memcpy(image,BackGroundImage,DV_WIDTH*DV_PAL_HEIGHT*3);
  //выводим имя файла, проплывающее по экрану
  uint32_t name_offset=static_cast<uint32_t>(left_offset);
  size_t x=0;
  for(size_t n=0;n<name_length;n++,x+=FONT_WIDTH)
  {
   size_t offset=static_cast<size_t>(name_offset/FONT_WIDTH);
   size_t offset_x=static_cast<size_t>(name_offset%FONT_WIDTH);
   char s=name.c_str()[n+offset];
   cGraphics.PutSymbol(x-offset_x,0,s,FILE_NAME_COLOR);
  }
  left_offset+=scroll_speed;
  if (left_offset>=name_pixel_length) left_offset-=name_pixel_length;
  //выводим проценты
  double percent_size=0;
  if (file_size>0) percent_size=((DV_WIDTH-2)*current_address)/file_size;
  cGraphics.SolidFill(1,height-4,percent_size,3,PROGRESS_BAR_COLOR);

  EncodePicture(image,is_pal,framebuffer,sDVVideoSegment);

  if (prefix>0)//формируем префиксный блок, если он нужен (это почти как пилот-тон у спектрума)
  {
   BuildHeader(databuffer,0,0,header_size,0,title_file_name.c_str());
   AddInfoBlocks(is_pal,framebuffer,cDVTime_Video,cDVTime_Current,false,FrameCounter);
   InsertData(databuffer,is_pal,framebuffer);   
   iOutputDVStream_Ptr->AddFrame(framebuffer,frame_size);
   prefix--;
   continue;
  }
    
  uint32_t got=data_in_frame;
  if (cRAIIFileIn.GetHandle().read(reinterpret_cast<char*>(databuffer+header_size),sizeof(uint8_t)*got).fail()==true)
  {
   got=cRAIIFileIn.GetHandle().gcount();//узнаём, сколько байт было считано
   if (got==0) break;//конец файла
  }	  
  int32_t eof=(got<data_in_frame);
  BuildHeader(databuffer,current_address,got,header_size,eof,title_file_name.c_str());
  AddInfoBlocks(is_pal,framebuffer,cDVTime_Video,cDVTime_Current,false,FrameCounter);
  InsertData(databuffer,is_pal,framebuffer);
  iOutputDVStream_Ptr->AddFrame(framebuffer,frame_size);  
  current_address+=got;
 }
 return(true);
}
//----------------------------------------------------------------------------------------------------
//вынуть файлы из DV-файла
//----------------------------------------------------------------------------------------------------
bool CDVControl::ExtractDV(const std::string &dv_file_name,const  std::string &path,IExtractDataStream *iExtractDataStream_Ptr)
{
 uint8_t framebuffer[144000];
 uint8_t databuffer[144000];

 CRAIIFileIn cRAIIFileIn(dv_file_name.c_str(),std::ios_base::in|std::ios_base::binary); 

 uint64_t next_addr=0;
 std::string answer;
 uint64_t counter=0;
 bool is_processing=false;
 bool done=false;
 while(done==false)
 {
  if (iExtractDataStream_Ptr->IsExit()==true) break;
  uint16_t last_header_size=0;
  uint32_t last_got=0;
  uint16_t old_header_size=0;
  std::string backup_title;  
  bool recover=false;
  while (true) 
  {
   if (iExtractDataStream_Ptr->IsExit()==true)
   {
	iExtractDataStream_Ptr->AddAnswer("Отменено пользователем!\n");
    break;
   }
   uint32_t got=0;
   if (cRAIIFileIn.GetHandle().read(reinterpret_cast<char*>(framebuffer),sizeof(uint8_t)*120000).fail()==true)
   {
    got=cRAIIFileIn.GetHandle().gcount();//узнаём, сколько байт было считано
    if (got==0)
	{
     done=true;
 	 break;
	}
   }  
   if ((framebuffer[3]&0x80)!=0)//режим PAL, нужно прочесть остаток данных
   {
    if (cRAIIFileIn.GetHandle().read(reinterpret_cast<char*>(framebuffer+120000),sizeof(uint8_t)*(144000-120000)).fail()==true)
    {
     if (cRAIIFileIn.GetHandle().gcount()==0)
	 {
      done=true;
	  break;
	 }
    }
    got+=cRAIIFileIn.GetHandle().gcount();
   }
   if (got>0)
   {   
    ExtractData(framebuffer,databuffer);//вынимаем данные из кадра
	counter++;

    bool end_data;   
    bool new_file;
    bool res=VerifyData(databuffer,next_addr,old_header_size,backup_title,end_data,answer,new_file);
    if (new_file==true)
    {
	 //iExtractDataStream_Ptr->AddAnswer("Новый файл:"+backup_title+"\n");
     is_processing=true;
	 recover=false;
     iExtractDataStream_Ptr->Close();
	 iExtractDataStream_Ptr->Create(path+"\\"+backup_title);
    }
	if (is_processing==true)//если файл уже пишется и произошла ошибка, то включаем режим восстановления
	{
	 if (res==false && recover==false)
	 {
	  iExtractDataStream_Ptr->AddAnswer("Файл:"+backup_title+"Ошибка в данных! Включён режим сохранения всех данных в файл!\n");
	  recover=true;
	 }
     if (res==true || recover==true) 
     {
      WriteExtractedData(databuffer,recover,iExtractDataStream_Ptr,last_header_size,last_got);
     }
	}
    if (answer.length()>0 && res==false && is_processing==true)//если есть ответ при негативном результате
    {
     iExtractDataStream_Ptr->AddAnswer(answer);
    }
    if (end_data==true) break;
   }
  }
  if (VerifyEOF(next_addr)==false && is_processing==true)
  {   
   iExtractDataStream_Ptr->AddAnswer("Файл считан не весь!\n");
  }
  //else iExtractDataStream_Ptr->AddAnswer("Считано.\n");
  is_processing=false;
 }
 return(true);
}

//----------------------------------------------------------------------------------------------------
//начать отсчёт кадров и блоков сначала
//----------------------------------------------------------------------------------------------------
void CDVControl::NewRecord(void)
{
 FrameCounter=0;
 cDVTime_Video.Set(0,0,0,0,0,0);
}
//----------------------------------------------------------------------------------------------------
//вывести в файл данные секций dv кадра
//----------------------------------------------------------------------------------------------------
void CDVControl::OutputToFile(FILE *file,uint8_t *frame_buffer,bool is_pal)
{
 if (file==NULL) return;
 int8_t numDIFseq;
 numDIFseq=is_pal?12:10;

 uint8_t *target=frame_buffer;

 CDVHeader cDVHeader;
 CDVSubCode cDVSubCode;
 CDVVAUX cDVVAUX;
 fprintf(file,"**************************************************\r\n");
 fprintf(file,"*NEW FRAME!                                      *\r\n");
 fprintf(file,"**************************************************\r\n");
 for(uint8_t ds=0;ds<numDIFseq;ds++)
 { 
  fprintf(file,"------------------------------\r\n");
  fprintf(file,"DIF sequence:%i\r\n",ds);
  fprintf(file,"------------------------------\r\n");
  CDVHeader::SHeader *sHeader_Ptr=reinterpret_cast<CDVHeader::SHeader*>(target);
  //cDVHeader.OutputToFile(file,"\t",sHeader_Ptr);
  target+=1*80;
  fprintf(file,"\r\n");
  CDVSubCode::SSubCode *sSubCode_Ptr=reinterpret_cast<CDVSubCode::SSubCode*>(target);
  //cDVSubCode.OutputToFile(file,"\t",is_pal,sSubCode_Ptr);
  target+=2*80;
  fprintf(file,"\r\n");
  CDVVAUX::SVAUX *sVAUX_Ptr=reinterpret_cast<CDVVAUX::SVAUX*>(target);
  //cDVVAUX.OutputToFile(file,"\t",sVAUX_Ptr);
  target+=3*80;
  //CreateVideoHeaders(target,frame,ds);
  uint8_t *ptr=target;
  CDVAudio cDVAudio;
  for(uint8_t m=0;m<9;m++,ptr+=16*DIF_BLOCK_SIZE) 
  {
   fprintf(file,"------------------------------\r\n");
   fprintf(file,"Block:%i\r\n",m);
   fprintf(file,"------------------------------\r\n");
   CDVAudio::SAudio *sAudio_Ptr=reinterpret_cast<CDVAudio::SAudio*>(ptr);
   cDVAudio.OutputToFile(file,"\t",sAudio_Ptr);
  }
  target+=144*80;
  fprintf(file,"\r\n");
 }
 fprintf(file,"\r\n\r\n");
}
