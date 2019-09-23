object FormMain: TFormMain
  Left = 704
  Top = 106
  Width = 530
  Height = 323
  BorderStyle = bsSizeToolWin
  Caption = 'Clip'
  Color = clBtnFace
  Font.Charset = GB2312_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = #24494#36719#38597#40657
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poScreenCenter
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnHide = FormHide
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 16
  object statusBar: TStatusBar
    Left = 0
    Top = 263
    Width = 514
    Height = 21
    Panels = <
      item
        Width = 250
      end
      item
        Width = 50
      end>
    SimplePanel = False
  end
  object pnlList: TPanel
    Left = 0
    Top = 0
    Width = 401
    Height = 263
    Align = alClient
    TabOrder = 1
    object lvClipHistory: TListView
      Left = 1
      Top = 25
      Width = 399
      Height = 237
      Align = alClient
      Columns = <
        item
          Width = 25
        end
        item
          Caption = #31896#36148#26495#20869#23481
          MinWidth = 200
          Width = 350
        end>
      GridLines = True
      HideSelection = False
      ReadOnly = True
      RowSelect = True
      PopupMenu = pmLvHistory
      TabOrder = 0
      ViewStyle = vsReport
      OnDblClick = lvClipHistoryDblClick
      OnKeyDown = lvClipHistoryKeyDown
      OnSelectItem = lvClipHistorySelectItem
    end
    object pnlSearch: TPanel
      Left = 1
      Top = 1
      Width = 399
      Height = 24
      Align = alTop
      TabOrder = 1
      object edtSearch: TMemo
        Left = 1
        Top = 1
        Width = 397
        Height = 22
        Align = alClient
        TabOrder = 0
        WantReturns = False
        OnChange = edtSearchChange
        OnKeyDown = edtSearchKeyDown
      end
    end
  end
  object pnlPreview: TPanel
    Left = 401
    Top = 0
    Width = 113
    Height = 263
    Align = alRight
    TabOrder = 2
    object mmoPreview: TMemo
      Left = 1
      Top = 1
      Width = 111
      Height = 236
      Align = alClient
      BorderStyle = bsNone
      Color = clMenu
      ReadOnly = True
      TabOrder = 0
      OnKeyDown = mmoPreviewKeyDown
    end
    object mmoTime: TMemo
      Left = 1
      Top = 237
      Width = 111
      Height = 25
      Align = alBottom
      BorderStyle = bsNone
      Color = clMenu
      ReadOnly = True
      TabOrder = 1
      OnKeyDown = mmoPreviewKeyDown
    end
  end
  object tmrClipWatcher: TTimer
    OnTimer = tmrClipWatcherTimer
    Left = 448
    Top = 48
  end
  object tray: TTrayIcon
    Visible = True
    PopupMenu = pmTray
    Hide = True
    RestoreOn = imDoubleClick
    PopupMenuOn = imRightClickUp
    OnRestore = trayRestore
    Left = 448
    Top = 96
  end
  object pmTray: TPopupMenu
    Left = 448
    Top = 136
    object N1: TMenuItem
      Caption = #26174#31034
      OnClick = N1Click
    end
    object N3: TMenuItem
      Caption = #36864#20986
      OnClick = N3Click
    end
  end
  object tmrSave: TTimer
    Interval = 60000
    OnTimer = tmrSaveTimer
    Left = 448
    Top = 16
  end
  object pmLvHistory: TPopupMenu
    Left = 448
    Top = 176
    object N4: TMenuItem
      Caption = #21024#38500
      OnClick = N4Click
    end
  end
end
