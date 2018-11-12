//====================================================================================================
//подключаемые библиотеки
//====================================================================================================
#include "cbackup.h"
#include "craiifilein.h"
#include "craiifileout.h"

//====================================================================================================
//глобальные переменные
//====================================================================================================

//====================================================================================================
//конструктор класса
//====================================================================================================
CBackUp::CBackUp(void)
{ 
 ResetState();
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CBackUp::~CBackUp()
{
}

//====================================================================================================
//закрытые функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//перевернуть байты
//----------------------------------------------------------------------------------------------------
void CBackUp::ReverseByte(void *ptr,size_t size) const
{
 uint8_t *l_ptr=reinterpret_cast<uint8_t*>(ptr)+size-1;
 uint8_t *r_ptr=reinterpret_cast<uint8_t*>(ptr);
 for(size_t n=0;n<(size>>1);n++,r_ptr++,l_ptr--)
 {
  uint8_t b=*r_ptr;
  *r_ptr=*l_ptr;
  *l_ptr=b;
 }
}
//----------------------------------------------------------------------------------------------------
//алгоритм вычисления CRC
//----------------------------------------------------------------------------------------------------
uint32_t CBackUp::CreateCRC(uint32_t last_crc,const uint8_t* ptr,size_t length) const
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
//проверка заголовка
//----------------------------------------------------------------------------------------------------
void CBackUp::CheckHeader(uint8_t databuffer[MAX_FRAME_SIZE],bool &version_ok,bool &id_ok,bool &crc_ok,bool &header_size_ok)
{
 version_ok=false;
 id_ok=false;
 crc_ok=false;
 header_size_ok=false;
 SDVPackerHeader *sDVPackerHeader_Ptr=reinterpret_cast<SDVPackerHeader*>(databuffer); 
 SDVPackerHeader sDVPackerHeader=*sDVPackerHeader_Ptr;
 sDVPackerHeader_Ptr->CRC=0;//убираем CRC
 //так как автор dvbackup сделал порядок байт big-endian, перевернём в копии структуры байты многобайтных переменных
 ReverseByte(&sDVPackerHeader.HeaderSize,sizeof(sDVPackerHeader.HeaderSize));
 ReverseByte(&sDVPackerHeader.Offset,sizeof(sDVPackerHeader.Offset));
 ReverseByte(&sDVPackerHeader.CRC,sizeof(sDVPackerHeader.CRC));

 //проверяем crc
 uint32_t crc_in_package=sDVPackerHeader.CRC; 
 uint32_t crc=CreateCRC(0,NULL,0);
 uint32_t header_size=sDVPackerHeader.HeaderSize;
 uint32_t data_size=sDVPackerHeader.FrameSize[0];
 data_size<<=8;
 data_size|=sDVPackerHeader.FrameSize[1];
 data_size<<=8;
 data_size|=sDVPackerHeader.FrameSize[2];
 uint32_t size=header_size+data_size;
 if (size>144000) size=MAX_FRAME_SIZE;//мало ли что там считалось с искажениями?
 crc=CreateCRC(crc,databuffer,size);
 if (crc==crc_in_package) crc_ok=true;
 //проверяем идентификатор
 if (sDVPackerHeader.ID[0]=='D' && sDVPackerHeader.ID[1]=='V' && sDVPackerHeader.ID[2]=='-' && sDVPackerHeader.ID[3]=='B') id_ok=true;
 //проверяем версию программы
 if (sDVPackerHeader.Version==DVBACKUP_VERSION) version_ok=true;
 //проверяем размер заголовка (он должен быть не меньше минимального)
 if (sDVPackerHeader.HeaderSize>=HEADER_SIZE) header_size_ok=true;
}
//====================================================================================================
//открытые функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//обработка данных из кадра
//----------------------------------------------------------------------------------------------------
void CBackUp::Extract(uint8_t databuffer[MAX_FRAME_SIZE],const std::string &path,IExtractDataStream *iExtractDataStream_Ptr)
{ 
 //проверяем заголовок 
 bool version_ok;
 bool id_ok;
 bool crc_ok;
 bool header_size_ok;
 CheckHeader(databuffer,version_ok,id_ok,crc_ok,header_size_ok);
 
 if (crc_ok==true)//считаем данные кадра достоверными, а ошибки связанными с неверными версиями программы
 {
  if (id_ok==false)
  {
   iExtractDataStream_Ptr->AddAnswer("Неверный идентификатор заголовка!\n");
   return;	   
  }
  if (version_ok==false)
  {
   iExtractDataStream_Ptr->AddAnswer("Неверная версия программы!\n");
   return;	   
  }
  if (header_size_ok==false)
  {
   iExtractDataStream_Ptr->AddAnswer("Неверный размер заголовка!\n");
   return;	   
  }
 }

 //либо кадр исправен и все параметры корректны, либо кадр с искажениями
 SDVPackerHeader *sDVPackerHeader_Ptr=reinterpret_cast<SDVPackerHeader*>(databuffer);
 //так как автор dvbackup сделал порядок байт big-endian, перевернём в копии структуры байты многобайтных переменных
 ReverseByte(&sDVPackerHeader_Ptr->HeaderSize,sizeof(sDVPackerHeader_Ptr->HeaderSize));
 ReverseByte(&sDVPackerHeader_Ptr->Offset,sizeof(sDVPackerHeader_Ptr->Offset));
 ReverseByte(&sDVPackerHeader_Ptr->CRC,sizeof(sDVPackerHeader_Ptr->CRC));

 uint32_t data_size=sDVPackerHeader_Ptr->FrameSize[0];
 data_size<<=8;
 data_size|=sDVPackerHeader_Ptr->FrameSize[1];
 data_size<<=8;
 data_size|=sDVPackerHeader_Ptr->FrameSize[2];
 if (crc_ok==true)//обрабатываем как есть - все параметры корректны
 {
  uint32_t name_length=sDVPackerHeader_Ptr->HeaderSize-HEADER_SIZE;
  char *name=new char[name_length+1];
  memcpy(name,databuffer+HEADER_SIZE,name_length);
  name[name_length]=0;
  std::string file_name=name;
  delete[](name);

  if (FileInProcessing==false)//файл ещё не создавался
  {
   //начинаем новый файл
   iExtractDataStream_Ptr->Close();
   iExtractDataStream_Ptr->Create(path+"\\"+file_name);
   iExtractDataStream_Ptr->AddAnswer("Новый файл:"+file_name+"\n");
   CurrentFileName=file_name;
   FileInProcessing=true;
   CurrentHeaderSize=sDVPackerHeader_Ptr->HeaderSize;
   CurrentAddr=sDVPackerHeader_Ptr->Offset;
   CurrentAddr+=data_size;
   LastDataSize=data_size;
   iExtractDataStream_Ptr->Write(databuffer+CurrentHeaderSize,data_size);
   if (sDVPackerHeader_Ptr->Flag!=0)
   {
    FileInProcessing=false;
	iExtractDataStream_Ptr->Close();	
   }
   return;
  }
  else//идёт запись файла
  {
   if (file_name.compare(CurrentFileName)!=0)//поменялось имя файла
   {
    iExtractDataStream_Ptr->AddAnswer(CurrentFileName+" - файл считан не весь!\n");
    //начинаем новый файл
    iExtractDataStream_Ptr->Close();
    iExtractDataStream_Ptr->Create(path+"\\"+file_name);
    iExtractDataStream_Ptr->AddAnswer("Новый файл:"+file_name+"\n");
	CurrentFileName=file_name;
    FileInProcessing=true;
    CurrentHeaderSize=sDVPackerHeader_Ptr->HeaderSize;
    CurrentAddr=sDVPackerHeader_Ptr->Offset;
    iExtractDataStream_Ptr->Write(databuffer+CurrentHeaderSize,data_size);
    CurrentAddr+=data_size;
	LastDataSize=data_size;
	if (sDVPackerHeader_Ptr->Flag!=0)//файл закончился
	{
     FileInProcessing=false;
	 iExtractDataStream_Ptr->Close();
	}
	return;
   }
   if (sDVPackerHeader_Ptr->Offset!=CurrentAddr)
   {
    char p_addr[255];
    char w_addr[255];
    sprintf(p_addr,"%lld",sDVPackerHeader_Ptr->Offset);
    sprintf(w_addr,"%lld",CurrentAddr);
	std::string answer=CurrentFileName;
	answer+="- ошибка адреса! Считан:";
	answer+=p_addr;
	answer+=" Нужен:";
	answer+=w_addr;
	answer+="\n";
    iExtractDataStream_Ptr->AddAnswer(answer);
   }
   if (sDVPackerHeader_Ptr->Offset>CurrentAddr)//требуется заполнение пропущенных адресов
   {
    iExtractDataStream_Ptr->AddAnswer("Выполняю заполнение недостающих данных нулём\n");
    uint8_t zero=0;
    for(;CurrentAddr<=sDVPackerHeader_Ptr->Offset;CurrentAddr++) iExtractDataStream_Ptr->Write(&zero,1);
   }
   iExtractDataStream_Ptr->Write(databuffer+CurrentHeaderSize,data_size);
   CurrentAddr+=data_size;
   LastDataSize=data_size;
   if (sDVPackerHeader_Ptr->Flag!=0)//файл закончился
   {
    FileInProcessing=false;
 	iExtractDataStream_Ptr->Close();
   }
  }
}
 else//данные некорректны
 {
  if (FileInProcessing==false) return;//файл ещё не создавался - пропускаем данные
  iExtractDataStream_Ptr->AddAnswer(CurrentFileName+" - CRC блока неверный!\n");
  //ситаем, что идёт продолжение записываемого файла (что может быть совершенно неверно!)
  uint64_t addr=sDVPackerHeader_Ptr->Offset;
  CurrentAddr+=LastDataSize;
  iExtractDataStream_Ptr->Write(databuffer+CurrentHeaderSize,LastDataSize);
 }
}

//----------------------------------------------------------------------------------------------------
//создать заголовок
//----------------------------------------------------------------------------------------------------
void CBackUp::CreateHeader(uint8_t databuffer[MAX_FRAME_SIZE],uint64_t CurrentAddress,uint32_t data_size,uint32_t header_size,bool eof,const char* backup_title) const
{ 
 SDVPackerHeader *sDVPackerHeader_Ptr=reinterpret_cast<SDVPackerHeader*>(databuffer);
 sDVPackerHeader_Ptr->ID[0]='D';
 sDVPackerHeader_Ptr->ID[1]='V';
 sDVPackerHeader_Ptr->ID[2]='-';
 sDVPackerHeader_Ptr->ID[3]='B';
 sDVPackerHeader_Ptr->HeaderSize=header_size;
 sDVPackerHeader_Ptr->Version=DVBACKUP_VERSION;
 sDVPackerHeader_Ptr->Offset=CurrentAddress;
 sDVPackerHeader_Ptr->FrameSize[0]=(data_size>>16)&0xff;
 sDVPackerHeader_Ptr->FrameSize[1]=(data_size>>8)&0xff;
 sDVPackerHeader_Ptr->FrameSize[2]=(data_size>>0)&0xff;
 sDVPackerHeader_Ptr->Flag=0;
 if (eof==true) sDVPackerHeader_Ptr->Flag=1;
 sDVPackerHeader_Ptr->CRC=0;
 if (backup_title!=NULL) memcpy(databuffer+sizeof(SDVPackerHeader),backup_title,strlen(backup_title));//завершающего нуля нет!
 //переставляем байты для big-endian
 ReverseByte(&sDVPackerHeader_Ptr->HeaderSize,sizeof(sDVPackerHeader_Ptr->HeaderSize));
 ReverseByte(&sDVPackerHeader_Ptr->Offset,sizeof(sDVPackerHeader_Ptr->Offset));
 //считаем CRC
 uint32_t crc=CBackUp::CreateCRC(0L,NULL,0);
 crc=CreateCRC(crc,databuffer,header_size+data_size);
 sDVPackerHeader_Ptr->CRC=crc;
 //переставляем байты для big-endian
 ReverseByte(&sDVPackerHeader_Ptr->CRC,sizeof(sDVPackerHeader_Ptr->CRC));
}
//----------------------------------------------------------------------------------------------------
//получить размер заголовка
//----------------------------------------------------------------------------------------------------
long CBackUp::GetHeaderSize(const char* backup_title) const
{
 if (backup_title==NULL) return(HEADER_SIZE);
 return((HEADER_SIZE)+strlen(backup_title)); 
}
//----------------------------------------------------------------------------------------------------
//сбросить состояние в исходное
void CBackUp::ResetState(void)
{
 FileInProcessing=false;
 CurrentFileName="";
 CurrentHeaderSize=0;
 CurrentAddr=0;
 LastDataSize=0;
}