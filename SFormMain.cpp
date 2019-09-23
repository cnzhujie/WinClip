//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SFormMain.h"
#include "Clipbrd.hpp"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "trayicon"
#pragma resource "*.dfm"
TFormMain *FormMain;
//---------------------------------------------------------------------------
__fastcall TFormMain::TFormMain(TComponent* Owner)
        : TForm(Owner)
{
      AnsiString exePath=ExtractFileDir(Application->ExeName);
      dataPath = exePath+"\\history.bin";
      pClipHistory = new ClipHistory(10240);
      pClipHistory->LoadFromFile(dataPath);
      lastContent = pClipHistory->getHeadContent();
}
//---------------------------------------------------------------------------
void TFormMain::Log(int index, AnsiString str)
{
    if(index>=statusBar->Panels->Count)
    {
        return;
    }
    statusBar->Panels->Items[index]->Text = str;
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::tmrClipWatcherTimer(TObject *Sender)
{
        try
        {
            AnsiString content = Clipboard()->AsText;
            if(content != lastContent)
            {
                lastContent = content;
                Log(SB_CLIP_DATA, content);
                pClipHistory->Add(content);
                ReloadClipHistoryList();
            }
        }
        catch(...)
        {}
        try
        {
            HWND currentForeground = GetForegroundWindow();
            if(currentForeground == this->Handle || currentForeground == edtSearch->Handle || currentForeground == lvClipHistory->Handle
                    || currentForeground == mmoPreview->Handle || currentForeground == mmoTime->Handle || currentForeground == statusBar->Handle)
            {
                return;
            }

            // 获取前置窗口进程名和标题
            lastWindowExeName = GetProcessNameByHandle(currentForeground);
            char chs[1000];
            GetWindowText(currentForeground, chs, 1000);
            lastWindowTitle = chs;

            Log(SB_CLIP_WIN, lastWindowExeName + " => " + lastWindowTitle);

            lastForeground = currentForeground;
        }catch(...) {

        }

}
AnsiString TFormMain::GetProcessNameByHandle(HWND nlHandle)
{
    //得到该进程的进程id  
    DWORD ldwProID;  
    GetWindowThreadProcessId(nlHandle,&ldwProID);  
    if(0==ldwProID)  
        return "";  
    HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);  
    if(handle==(HANDLE)-1)   
    {  
        return "";  
    }  
    PROCESSENTRY32 procinfo;  
    procinfo.dwSize = sizeof(PROCESSENTRY32);  
    BOOL more=::Process32First(handle,&procinfo);  
    while(more)  
    {  
        if(procinfo.th32ProcessID==ldwProID)  
        {  
            AnsiString loStrRet=procinfo.szExeFile;  
            CloseHandle(handle);  
            return loStrRet;  
        }  
        more=Process32Next(handle,&procinfo);   
    }  
    CloseHandle(handle);  
    return "";  
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::FormShow(TObject *Sender)
{
      edtSearch->SetFocus();
      ReloadClipHistoryList();

      // 注册热键
      //enter
      RegisterHotKey(this->Handle,   HOTKEY_RETURN,   NULL,   VK_RETURN);
      //up
      RegisterHotKey(this->Handle,   HOTKEY_UP,   NULL,   VK_UP);
      //down
      RegisterHotKey(this->Handle,   HOTKEY_DOWN,   NULL,   VK_DOWN);
      //esc
      RegisterHotKey(this->Handle,   HOTKEY_ESC,   NULL,   VK_ESCAPE);
}      
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormHide(TObject *Sender)
{
      // 注销热键 
      UnregisterHotKey(Handle,   HOTKEY_RETURN);
      UnregisterHotKey(Handle,   HOTKEY_UP);
      UnregisterHotKey(Handle,   HOTKEY_DOWN);
      UnregisterHotKey(Handle,   HOTKEY_ESC);
}
//---------------------------------------------------------------------------

void TFormMain::ReloadClipHistoryList()
{
     if(!this->Visible)
     {
         return;
     }
     list<ClipHistoryItem*> result = pClipHistory->Search(edtSearch->Text, 500);
     //Log(SB_CLIP_DATA, IntToStr(result.size()));
     lvClipHistory->Clear();
     list<ClipHistoryItem*>::iterator it;
     for(it=result.begin();it!=result.end();it++)
     {
//         Application->HandleMessage();
         ClipHistoryItem* pItem = *it;
         TListItem* pListItem = lvClipHistory->Items->Add();
         pListItem->Caption = "";
         pListItem->SubItems->Add(pItem->content.Length() > 100 ? pItem->content.SubString(1, 100) : pItem->content);
         pListItem->Data = (void *)pItem;
     }
     if(lvClipHistory->Items->Count > 0)
     {
         lvClipHistory->Items->Item[0]->Selected = True;
     }
}
//---------------------------------------------------------------------------
AnsiString TFormMain::FormatTime(long time)
{
    struct tm *temp_time = localtime(&time);
    AnsiString s = IntToStr(temp_time->tm_year + 1900);

    s += "-";
    if(temp_time->tm_mon + 1 < 10)
    {
         s += "0";
    }
    s += IntToStr(temp_time->tm_mon + 1);

    s += "-";
    s += (temp_time->tm_mday<10?"0":"");
    s += IntToStr(temp_time->tm_mday);
    s += " ";
    s += (temp_time->tm_hour<10?"0":"");
    s += IntToStr(temp_time->tm_hour);
    s += ":";
    s += (temp_time->tm_min<10?"0":"");
    s += IntToStr(temp_time->tm_min);
    s += ":";
    s += (temp_time->tm_sec<10?"0":"");
    s += IntToStr(temp_time->tm_sec);
    return s;
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::lvClipHistorySelectItem(TObject *Sender,
      TListItem *Item, bool Selected)
{
      if(Selected)
      {
          for(int i=0;i<lvClipHistory->Items->Count;i++)
          {
                lvClipHistory->Items->Item[i]->Caption = IntToStr(i);
          }
          Item->Caption = "->";
          ClipHistoryItem* pItem = (ClipHistoryItem*)(Item->Data);
          mmoTime->Text = FormatTime(pItem->time);
          mmoPreview->Text = pItem->content.Length() > 200 ? (pItem->content.SubString(1, 200) + "...") : pItem->content;
      }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::edtSearchChange(TObject *Sender)
{
      ReloadClipHistoryList();  
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::FormCloseQuery(TObject *Sender, bool &CanClose)
{
       this->Hide();
       CanClose = false;
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::trayRestore(TObject *Sender)
{
       this->Show(); 
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::N1Click(TObject *Sender)
{
       this->Show(); 
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::N3Click(TObject *Sender)
{
       pClipHistory->SaveToFile(dataPath);
       Application->Terminate(); 
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::tmrSaveTimer(TObject *Sender)
{
       pClipHistory->SaveToFile(dataPath);
}
//---------------------------------------------------------------------------
void TFormMain::OnSpecialKey(WORD Key)
{
      if(Key != VK_UP && Key != VK_DOWN && Key != VK_RETURN && Key != VK_ESCAPE && Key != VK_DELETE)
      {
          edtSearch->SetFocus();
          return;
      }
      if(Key == VK_ESCAPE)
      {
           this->Hide();
      }
      if(lvClipHistory->Items->Count == 0)
      {
          return;
      }

      if(lvClipHistory->Selected == NULL)
      {
           lvClipHistory->Items->Item[0]->Selected = True;
      }

      if(Key == VK_UP)
      {
           SetSelectedInClipboardHistoryList(lvClipHistory->Selected ? (lvClipHistory->Selected->Index-1) : 0);
      }
      else if(Key == VK_DOWN)
      {
           SetSelectedInClipboardHistoryList(lvClipHistory->Selected ? (lvClipHistory->Selected->Index+1) : 0);
      }
      else if(Key == VK_RETURN)
      {
           PasteAndHide();
      }  
      else if(Key == VK_DELETE)
      {
           N4Click(NULL);
      }
}
void TFormMain::OnCharKey(WORD Key, TShiftState Shift)
{                             
      // TODO
}
void TFormMain::PasteAndHide()
{
     if(!lvClipHistory->Selected)
     {
          return;
     }
     ClipHistoryItem* pItem = (ClipHistoryItem*)(lvClipHistory->Selected->Data);

     Clipboard()->AsText = pItem->content;
     this->Hide();
     if(lastForeground != NULL)
     {
            SetForegroundWindow(lastForeground);

            if(lastWindowExeName == "mintty.exe" || lastWindowExeName == "Xshell.exe")
            {
                // 模拟Shift+Ins
                INPUT input[4];
                memset(input, 0, sizeof(input));
                //设置模拟键盘输入
                input[0].type = input[1].type = input[2].type = input[3].type = INPUT_KEYBOARD;
                input[0].ki.wVk  = input[2].ki.wVk = VK_SHIFT;
                input[1].ki.wVk  = input[3].ki.wVk = VK_INSERT;
                // 释放按键，这非常重要
                input[2].ki.dwFlags = input[3].ki.dwFlags = KEYEVENTF_KEYUP;
                SendInput(4,input,sizeof(INPUT));
            }
            else
            {
                // 模拟Ctrl+V
                INPUT input[4];
                memset(input, 0, sizeof(input));
                //设置模拟键盘输入
                input[0].type = input[1].type = input[2].type = input[3].type = INPUT_KEYBOARD;
                input[0].ki.wVk  = input[2].ki.wVk = VK_CONTROL;
                input[1].ki.wVk  = input[3].ki.wVk = 86;
                // 释放按键，这非常重要
                input[2].ki.dwFlags = input[3].ki.dwFlags = KEYEVENTF_KEYUP;
                SendInput(4,input,sizeof(INPUT));
            }
     }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::edtSearchKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
      OnSpecialKey(Key);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::mmoPreviewKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
     OnSpecialKey(Key);
     OnCharKey(Key, Shift);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::lvClipHistoryKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
     OnSpecialKey(Key);
     OnCharKey(Key, Shift);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
     OnSpecialKey(Key);
     OnCharKey(Key, Shift);
}
//---------------------------------------------------------------------------



void __fastcall TFormMain::FormCreate(TObject *Sender)
{
      // alt+shift+v
      RegisterHotKey(this->Handle,   HOTKEY_CTRL_ALT_V,   MOD_ALT|MOD_SHIFT,   0x56);

      AutoRunCheck(true);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormDestroy(TObject *Sender)
{                                           
      UnregisterHotKey(Handle,   HOTKEY_CTRL_ALT_V);
      UnregisterHotKey(Handle,   HOTKEY_RETURN);
      UnregisterHotKey(Handle,   HOTKEY_UP);
      UnregisterHotKey(Handle,   HOTKEY_DOWN);
      UnregisterHotKey(Handle,   HOTKEY_ESC);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::WMHotKey(TMessage   &Msg)
{
      if(Msg.WParam == 0x1000)
      {
            // 当前鼠标位置
            POINT pt;
            GetCursorPos(&pt);

            //获取可用桌面大小  
            RECT rect;
            SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
            //ShowMessage(IntToStr(rect.left)+"=>"+IntToStr(rect.right)+", "+IntToStr(rect.top)+"=>"+IntToStr(rect.bottom));
            if(pt.x + this->Width > rect.right)
            {
                 pt.x = rect.right - this->Width;
            }
            if(pt.y + this->Height > rect.bottom)
            {
                 pt.y = rect.bottom - this->Height;
            }

            this->Left = pt.x;
            this->Top = pt.y;
            this->Hide();
            this->Show();
      }
      else if(Msg.WParam == HOTKEY_RETURN)
      {
            if(this->Visible)
            {
                  PasteAndHide();
            }
      }
      else if(Msg.WParam == HOTKEY_UP)
      {
            if(this->Visible)
            {
                  SetSelectedInClipboardHistoryList(lvClipHistory->Selected ? (lvClipHistory->Selected->Index-1) : 0);
            }
      }
      else if(Msg.WParam == HOTKEY_DOWN)
      {
            if(this->Visible)
            {
                  SetSelectedInClipboardHistoryList(lvClipHistory->Selected ? (lvClipHistory->Selected->Index+1) : 0);
            }
      }
      else if(Msg.WParam == HOTKEY_ESC)
      {
            this->Close();
      }
}
//---------------------------------------------------------------------------
void TFormMain::SetSelectedInClipboardHistoryList(int index)
{
     if(index < 0 || index >= lvClipHistory->Items->Count)
     {
         return;
     }
     lvClipHistory->Items->Item[index]->Selected = True;
     
     ListView_EnsureVisible(lvClipHistory->Handle, index, true);
     ListView_SetItemState(lvClipHistory->Handle, index, LVIS_SELECTED, LVIS_SELECTED);
     ::SetFocus(lvClipHistory->Handle);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N4Click(TObject *Sender)
{
      if(lvClipHistory->Selected == NULL)
      {
          return;
      }
      int idx = lvClipHistory->Selected->Index;
      pClipHistory->Remove(lvClipHistory->Selected->SubItems->Strings[0]);
      lvClipHistory->Selected->Delete();

      if(idx >= lvClipHistory->Items->Count)
      {
            idx = lvClipHistory->Items->Count - 1;
      }
      SetSelectedInClipboardHistoryList(idx);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::lvClipHistoryDblClick(TObject *Sender)
{
       PasteAndHide(); 
}

//---------------------------------------------------------------------------

int __fastcall TFormMain::AutoRunCheck(bool value)
{

        int ret = 0;

        TRegistry * Reg = new TRegistry;
        AnsiString keyval="\""+Application->ExeName+"\"";

        AnsiString key1="Software\\Microsoft\\Windows\\CurrentVersion\\Run\\";

        Reg->RootKey=HKEY_CURRENT_USER;

        if( !Reg->OpenKey(key1,false))
        {
                //Show msg
        }
        else
        {
             if(value)
             {
                  AnsiString curvalstr = Reg->ReadString("CLIP.EXE");
                  if(curvalstr != keyval)
                  {
                       Reg->WriteString("CLIP.EXE",keyval);
                  }
                  Reg->CloseKey();
                  ret = 1;
             }
             else
             {
                  if(Reg->DeleteValue("CLIP.EXE"))
                  {
                       ret = 1;
                  }
             }

        }

        delete Reg;
        return ret;
}
//---------------------------------------------------------------------------

