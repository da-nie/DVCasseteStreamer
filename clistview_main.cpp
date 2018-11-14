#include "clistview_main.h"

//====================================================================================================
//функции обработки сообщений класса
//====================================================================================================

BEGIN_MESSAGE_MAP(CListView_Main,CView)
 ON_WM_SIZE()
 ON_WM_PAINT()//без ON_WM_PAINT список вообще не отрисовывается!!! Хотя должен бы.
END_MESSAGE_MAP()

//====================================================================================================
//конструктор класса
//====================================================================================================
CListView_Main::CListView_Main() 
{ 
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CListView_Main::~CListView_Main() 
{ 
}

//====================================================================================================
//закрытые функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//обновить список
//----------------------------------------------------------------------------------------------------
void CListView_Main::UpdateList(void)
{
 CDocument_Main *cDocument_Main_Ptr=GetDocument();
 std::string answer;
 cDocument_Main_Ptr->GetAndClearAnswer(answer);

 if (answer.length()==0) return; 
 //выводим ответ
 const uint32_t MAX_LINE_SIZE=MAX_PATH;
 char line[MAX_LINE_SIZE+1];
 size_t line_size=0;
 size_t length=answer.length();
 for(size_t n=0;n<length;n++)
 {
  char s=answer.c_str()[n];
  if (s=='\n' || n==length-1 || line_size==MAX_LINE_SIZE)
  {
   line[line_size]=0;

   CListCtrl &cListCtrl=GetListCtrl();
   uint32_t index=cListCtrl.GetItemCount();
   LV_ITEM item;  
   item.mask=LVIF_TEXT;
   item.iItem=index;
   item.iSubItem=0;
   item.pszText=const_cast<LPSTR>((LPCSTR)line);
   item.iItem=cListCtrl.InsertItem(&item);
   
   line_size=0;
   if (s=='\n' || n==length-1) continue;
  }
  line[line_size]=s;
  line_size++;
 } 
}
//----------------------------------------------------------------------------------------------------
//получить документ
//----------------------------------------------------------------------------------------------------
CDocument_Main* CListView_Main::GetDocument(void)
{
 ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDocument)));// проверить есть ли класс документа
 return(reinterpret_cast<CDocument_Main*>(m_pDocument));//возвратить указатель на документ
}

//====================================================================================================
//открытые функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//создание окна
//----------------------------------------------------------------------------------------------------
afx_msg void CListView_Main::OnInitialUpdate(void)
{ 
 CListView::OnInitialUpdate();
 //добавим элементы

 RECT Rect;
 GetClientRect(&Rect);
 long width=Rect.right-Rect.left;
 CListCtrl &cListCtrl=GetListCtrl(); 
 cListCtrl.ModifyStyle(LVS_TYPEMASK,LVS_REPORT|WS_BORDER|WS_VISIBLE);
 cListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES);
 cListCtrl.InsertColumn(0,"Отчёт",LVCFMT_CENTER,width);
 cListCtrl.DeleteAllItems();
 UpdateList();
}

//====================================================================================================
//функции обработки сообщений класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//изменение размеров окна
//----------------------------------------------------------------------------------------------------
afx_msg void CListView_Main::OnSize(UINT nType,int cx,int cy)
{ 
 CListView::OnSize(nType,cx,cy); 
 
 //перемещаем колонки
 RECT Rect;
 GetClientRect(&Rect);
 long width=Rect.right-Rect.left;
 CListCtrl &cListCtrl=GetListCtrl();
 cListCtrl.SetColumnWidth(0,width); 
}

//----------------------------------------------------------------------------------------------------
//обновить вид
//----------------------------------------------------------------------------------------------------
afx_msg void CListView_Main::OnUpdate(CView *pSender,LPARAM lHint,CObject *pHint)
{
 UpdateList();
}

//====================================================================================================
//прочее
//====================================================================================================
IMPLEMENT_DYNCREATE(CListView_Main,CListView)
