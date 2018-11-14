//====================================================================================================
//подключаемые библиотеки
//====================================================================================================
#include "cdvcontrol.h"
#include "tga.h"
#include "craiifilein.h"
#include "craiifileout.h"
#include "cgraphics.h"
#include "cbackup.h"

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

 memset(target,0,MAX_FRAME_SIZE);

 dif=0;
 offset=dif*DIF_BLOCK_SIZE;
 if (is_pal==true) target[offset+3]|=0x80;
 numDIFseq=is_pal?PAL_DIF_SEQUENCE:NTSC_DIF_SEQUENCE;

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
 uint8_t max_dif_seq=NTSC_DIF_SEQUENCE;
 uint8_t frame_rate=CDVTime::NTSC_FRAME_RATE;
 if (is_pal==true)
 {
  max_dif_seq=PAL_DIF_SEQUENCE;
  frame_rate=CDVTime::PAL_FRAME_RATE;
 }
 if (frame%frame_rate==0)//прошла секунда
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
 ConvertToYUV(readbuf,is_pal?DV_PAL_HEIGHT:DV_NTSC_HEIGHT,img_y,img_cr,img_cb);
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
 uint32_t numbytes=9*((DIF_BLOCK_SIZE-3-5)+(12*4+2*8)*15);
 return(numbytes*(is_pal?PAL_DIF_SEQUENCE:NTSC_DIF_SEQUENCE));
}

//----------------------------------------------------------------------------------------------------
//вставить данные
//----------------------------------------------------------------------------------------------------
void CDVControl::InsertData(uint8_t* src,bool is_pal,uint8_t* target)
{
 int32_t numDIFseq;
 int32_t ds,i,j,b;

 numDIFseq=is_pal?PAL_DIF_SEQUENCE:NTSC_DIF_SEQUENCE;

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
 bool is_pal;

 CDVHeader cDVHeader;
 CDVHeader::SHeader *sHeader_Ptr=reinterpret_cast<CDVHeader::SHeader*>(src);
 cDVHeader.IsPAL(sHeader_Ptr,is_pal);

 numDIFseq=is_pal?PAL_DIF_SEQUENCE:NTSC_DIF_SEQUENCE;

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

 CBackUp cBackUp;

 uint8_t framebuffer[MAX_FRAME_SIZE];
 uint8_t databuffer[MAX_FRAME_SIZE];
 uint32_t chunk_size=GetChunkSize(is_pal);
 uint32_t header_size=cBackUp.GetHeaderSize(title_file_name.c_str());

 uint32_t height=DV_PAL_HEIGHT;
 uint32_t frame_size=PAL_FRAME_SIZE;
 if (is_pal==false)
 {
  height=DV_NTSC_HEIGHT;
  frame_size=NTSC_FRAME_SIZE;
 }
 CRAIIFileIn cRAIIFileIn(file_name.c_str(),std::ios_base::in|std::ios_base::binary);

 if (cRAIIFileIn.IsOpened()==false) return(false);

 //узнаем размер входного файла
 cRAIIFileIn.GetHandle().seekg(0,std::ios_base::end);
 uint64_t file_size=cRAIIFileIn.GetHandle().tellg();
 uint64_t remainder_size=file_size;
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
  if (iOutputDVStream_Ptr->IsBreak()==true)
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
  if (file_size<=chunk_size) percent_size=DV_WIDTH-2;//для файлов, не более размера блока в кадре, указываем сразу 100%
  cGraphics.SolidFill(1,height-4,percent_size,3,PROGRESS_BAR_COLOR);

  EncodePicture(image,is_pal,framebuffer,sDVVideoSegment);

  if (prefix>0)//формируем префиксный блок, если он нужен (это почти как пилот-тон у спектрума)
  {
   cBackUp.CreateHeader(databuffer,0,0,header_size,false,title_file_name.c_str());
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
  bool eof=false;
  remainder_size-=got;
  if (got<data_in_frame || remainder_size==0) eof=true;
  cBackUp.CreateHeader(databuffer,current_address,got,header_size,eof,title_file_name.c_str());
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
bool CDVControl::ExtractDV(const std::string &dv_file_name,const std::string &path,IExtractDataStream *iExtractDataStream_Ptr)
{
 uint8_t framebuffer[MAX_FRAME_SIZE];
 uint8_t databuffer[MAX_FRAME_SIZE];

 CRAIIFileIn cRAIIFileIn(dv_file_name.c_str(),std::ios_base::in|std::ios_base::binary); 
 if (cRAIIFileIn.IsOpened()==false)
 {
  iExtractDataStream_Ptr->AddAnswer("Не могу открыть dv-файл:"+dv_file_name+"\n");
  return(false);
 }
 CDVHeader cDVHeader;
 CBackUp cBackUp;
 cBackUp.ResetState();
 while(true)
 {
  if (iExtractDataStream_Ptr->IsBreak()==true) break;
  //читаем dv-файл и вынимаем данные
  if (cRAIIFileIn.GetHandle().read(reinterpret_cast<char*>(framebuffer),sizeof(uint8_t)*NTSC_FRAME_SIZE).fail()==true)
  {
   iExtractDataStream_Ptr->AddAnswer("Файл dv считан весь.\n");
   break;
  }
  CDVHeader::SHeader *sHeader_Ptr=reinterpret_cast<CDVHeader::SHeader*>(framebuffer);
  bool is_pal;
  if (cDVHeader.IsPAL(sHeader_Ptr,is_pal)==false)
  {
   iExtractDataStream_Ptr->AddAnswer("Ошибочная секция header файла dv!\n");
   break;
  }
  if (is_pal==true)//требуется дочитать кадр
  {
   if (cRAIIFileIn.GetHandle().read(reinterpret_cast<char*>(framebuffer+NTSC_FRAME_SIZE),sizeof(uint8_t)*(PAL_FRAME_SIZE-NTSC_FRAME_SIZE)).fail()==true)
   {
    iExtractDataStream_Ptr->AddAnswer("Файл dv считан весь.\n");
    break;
   }
  }
  //вынимаем данные из кадра
  ExtractData(framebuffer,databuffer);  
  //отправляем на обработку
  cBackUp.Extract(databuffer,path,iExtractDataStream_Ptr);
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
  cDVHeader.OutputToFile(file,"\t",sHeader_Ptr);
  target+=1*80;
  fprintf(file,"\r\n");
  CDVSubCode::SSubCode *sSubCode_Ptr=reinterpret_cast<CDVSubCode::SSubCode*>(target);
  cDVSubCode.OutputToFile(file,"\t",is_pal,sSubCode_Ptr);
  target+=2*80;
  fprintf(file,"\r\n");
  CDVVAUX::SVAUX *sVAUX_Ptr=reinterpret_cast<CDVVAUX::SVAUX*>(target);
  cDVVAUX.OutputToFile(file,"\t",sVAUX_Ptr);
  target+=3*80;  
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
