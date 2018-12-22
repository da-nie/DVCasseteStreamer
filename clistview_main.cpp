#include "clistview_main.h"

//====================================================================================================
//������� ��������� ��������� ������
//====================================================================================================

BEGIN_MESSAGE_MAP(CListView_Main,CView)
 ON_WM_SIZE()
 ON_WM_PAINT()//��� ON_WM_PAINT ������ ������ �� ��������������!!! ���� ������ ��.
END_MESSAGE_MAP()

//====================================================================================================
//����������� ������
//====================================================================================================
CListView_Main::CListView_Main() 
{ 
}
//====================================================================================================
//���������� ������
//====================================================================================================
CListView_Main::~CListView_Main() 
{ 
}

//====================================================================================================
//�������� ������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//�������� ������
//----------------------------------------------------------------------------------------------------
void CListView_Main::UpdateList(void)
{
 CDocument_Main *cDocument_Main_Ptr=GetDocument();
 std::string answer;
 cDocument_Main_Ptr->GetAndClearAnswer(answer);

 if (answer.length()==0) return; 
 //������� �����
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
//�������� ��������
//----------------------------------------------------------------------------------------------------
CDocument_Main* CListView_Main::GetDocument(void)
{
 ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDocument)));// ��������� ���� �� ����� ���������
 return(reinterpret_cast<CDocument_Main*>(m_pDocument));//���������� ��������� �� ��������
}

//====================================================================================================
//�������� ������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//�������� ����
//----------------------------------------------------------------------------------------------------
afx_msg void CListView_Main::OnInitialUpdate(void)
{ 
 CListView::OnInitialUpdate();
 //������� ��������

 RECT Rect;
 GetClientRect(&Rect);
 long width=Rect.right-Rect.left;
 CListCtrl &cListCtrl=GetListCtrl(); 
 cListCtrl.ModifyStyle(LVS_TYPEMASK,LVS_REPORT|WS_BORDER|WS_VISIBLE);
 cListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES);
 cListCtrl.InsertColumn(0,"�����",LVCFMT_CENTER,width);
 cListCtrl.DeleteAllItems();
 UpdateList();
}

//====================================================================================================
//������� ��������� ��������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//��������� �������� ����
//----------------------------------------------------------------------------------------------------
afx_msg void CListView_Main::OnSize(UINT nType,int cx,int cy)
{ 
 CListView::OnSize(nType,cx,cy); 
 
 //���������� �������
 RECT Rect;
 GetClientRect(&Rect);
 long width=Rect.right-Rect.left;
 CListCtrl &cListCtrl=GetListCtrl();
 cListCtrl.SetColumnWidth(0,width); 
}

//----------------------------------------------------------------------------------------------------
//�������� ���
//----------------------------------------------------------------------------------------------------
afx_msg void CListView_Main::OnUpdate(CView *pSender,LPARAM lHint,CObject *pHint)
{
 UpdateList();
}

//====================================================================================================
//������
//====================================================================================================
IMPLEMENT_DYNCREATE(CListView_Main,CListView)
