//---------------------------------------------------------------------------

#ifndef SFormMainH
#define SFormMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Registry.hpp>

#include <ctype.h>
#include "ClipHistory.h"
#include "trayicon.h"
#include <Menus.hpp>
#include <string>  
#include <windows.h>    
#include <Tlhelp32.h>

#define SB_CLIP_DATA 0  
#define SB_CLIP_WIN 1

#define HOTKEY_CTRL_ALT_V 0x1000 
#define HOTKEY_RETURN 0x1001
#define HOTKEY_UP 0x1002
#define HOTKEY_DOWN 0x1003      
#define HOTKEY_ESC 0x1004

//---------------------------------------------------------------------------
class TFormMain : public TForm
{
__published:	// IDE-managed Components
        TTimer *tmrClipWatcher;
        TStatusBar *statusBar;
        TPanel *pnlList;
        TPanel *pnlPreview;
        TListView *lvClipHistory;
        TPanel *pnlSearch;
        TMemo *mmoPreview;
        TTrayIcon *tray;
        TPopupMenu *pmTray;
        TMenuItem *N1;
        TMenuItem *N3;
        TTimer *tmrSave;
        TMemo *mmoTime;
        TMemo *edtSearch;
        TPopupMenu *pmLvHistory;
        TMenuItem *N4;
        void __fastcall tmrClipWatcherTimer(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall lvClipHistorySelectItem(TObject *Sender,
          TListItem *Item, bool Selected);
        void __fastcall edtSearchChange(TObject *Sender);
        void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
        void __fastcall trayRestore(TObject *Sender);
        void __fastcall N1Click(TObject *Sender);
        void __fastcall N3Click(TObject *Sender);
        void __fastcall tmrSaveTimer(TObject *Sender);
        void __fastcall edtSearchKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall mmoPreviewKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall lvClipHistoryKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall N4Click(TObject *Sender);
        void __fastcall lvClipHistoryDblClick(TObject *Sender);
        void __fastcall FormHide(TObject *Sender);
private:	// User declarations
        AnsiString lastContent;
        ClipHistory* pClipHistory;
        AnsiString dataPath;
        HWND lastForeground;
        AnsiString lastWindowTitle;
        AnsiString lastWindowExeName;

public:		// User declarations
        __fastcall TFormMain(TComponent* Owner);

        void Log(int index, AnsiString str);
        void ReloadClipHistoryList();
        AnsiString FormatTime(long time);

        void OnSpecialKey(WORD key);
        void OnCharKey(WORD key, TShiftState Shift);

        void PasteAndHide();

        void SetSelectedInClipboardHistoryList(int index);

        // 热键处理
        void   __fastcall   WMHotKey(TMessage   &Msg);
        BEGIN_MESSAGE_MAP   
            //处理WM_HOTKEY消息，注册热键之后，按下热键就会向指定窗口发出该消息
            VCL_MESSAGE_HANDLER(WM_HOTKEY,   TMessage,   WMHotKey);   
        END_MESSAGE_MAP(TForm);

        AnsiString GetProcessNameByHandle(HWND nlHandle);

        int __fastcall AutoRunCheck(bool value);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;
//---------------------------------------------------------------------------
#endif
