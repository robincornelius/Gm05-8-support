VERSION 5.00
Object = "{427A7646-A9FD-4CB6-8F90-5ABC8B67866A}#1.0#0"; "GMCON~XX.DLL"
Begin VB.Form Form1 
   Caption         =   "Gaussmeter Control"
   ClientHeight    =   2025
   ClientLeft      =   165
   ClientTop       =   735
   ClientWidth     =   6765
   LinkTopic       =   "Form1"
   ScaleHeight     =   2025
   ScaleWidth      =   6765
   StartUpPosition =   3  'Windows Default
   Begin VB.Frame Frame1 
      Caption         =   "Range"
      Height          =   1215
      Left            =   3960
      TabIndex        =   1
      Top             =   0
      Width           =   2655
      Begin VB.OptionButton Range 
         Caption         =   "2999mT"
         Height          =   255
         Index           =   3
         Left            =   1320
         TabIndex        =   6
         Top             =   480
         Width           =   1215
      End
      Begin VB.OptionButton Range 
         Caption         =   "299.9mT"
         Height          =   255
         Index           =   2
         Left            =   1320
         TabIndex        =   5
         Top             =   240
         Width           =   1215
      End
      Begin VB.CommandButton Command1 
         Caption         =   "Auto"
         Height          =   255
         Left            =   840
         TabIndex        =   4
         Top             =   840
         Width           =   855
      End
      Begin VB.OptionButton Range 
         Caption         =   "29.99mT"
         Height          =   255
         Index           =   1
         Left            =   120
         TabIndex        =   3
         Top             =   480
         Width           =   1215
      End
      Begin VB.OptionButton Range 
         Caption         =   "2.999mT"
         Height          =   255
         Index           =   0
         Left            =   120
         TabIndex        =   2
         Top             =   240
         Width           =   1215
      End
   End
   Begin VB.TextBox Display 
      BeginProperty Font 
         Name            =   "Terminal"
         Size            =   13.5
         Charset         =   255
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   120
      TabIndex        =   0
      Text            =   "Text1"
      Top             =   360
      Width           =   3735
   End
   Begin GMCONTROL2LibCtl.GM0Control2 GM0Control21 
      Left            =   2400
      OleObjectBlob   =   "Form1.frx":0000
      Top             =   1440
   End
   Begin VB.Label probeser 
      Height          =   255
      Left            =   120
      TabIndex        =   8
      Top             =   1440
      Width           =   1815
   End
   Begin VB.Label meterser 
      Height          =   255
      Left            =   120
      TabIndex        =   7
      Top             =   960
      Width           =   1815
   End
   Begin VB.Menu SystemTop 
      Caption         =   "System"
      Begin VB.Menu Reconnect 
         Caption         =   "Reconnect"
         Index           =   1
      End
      Begin VB.Menu cptop 
         Caption         =   "ComPort"
         Begin VB.Menu cp 
            Caption         =   "1"
            Index           =   1
         End
         Begin VB.Menu cp 
            Caption         =   "2"
            Index           =   2
         End
         Begin VB.Menu cp 
            Caption         =   "3"
            Index           =   3
         End
         Begin VB.Menu cp 
            Caption         =   "4"
            Index           =   4
         End
      End
   End
   Begin VB.Menu Unitstop 
      Caption         =   "Units"
      Index           =   0
      Begin VB.Menu units 
         Caption         =   "Tesla"
         Index           =   1
      End
      Begin VB.Menu units 
         Caption         =   "Gauss"
         Index           =   2
      End
      Begin VB.Menu units 
         Caption         =   "kA/m"
         Index           =   3
      End
      Begin VB.Menu units 
         Caption         =   "Orsted"
         Index           =   4
      End
   End
   Begin VB.Menu modetop 
      Caption         =   "Mode"
      Begin VB.Menu mode 
         Caption         =   "DC"
         Index           =   0
      End
      Begin VB.Menu mode 
         Caption         =   "DC Peak"
         Index           =   1
      End
      Begin VB.Menu mode 
         Caption         =   "AC"
         Index           =   2
      End
      Begin VB.Menu mode 
         Caption         =   "AC Max"
         Index           =   3
      End
      Begin VB.Menu mode 
         Caption         =   "AC RMS"
         Index           =   4
      End
   End
   Begin VB.Menu utilities 
      Caption         =   "Utilities"
      Begin VB.Menu Null 
         Caption         =   "Null"
      End
      Begin VB.Menu AZ 
         Caption         =   "AutoZero"
      End
      Begin VB.Menu SetTime 
         Caption         =   "Set GM05 Time"
      End
      Begin VB.Menu GetTime 
         Caption         =   "Get GM05 Time"
      End
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim connected As Boolean
Dim comport As Integer
Dim modestr(5)
Dim unitsstr(4, 4)
Dim rangestr(4, 4)
Dim rangestr2(4, 4)

Sub DoConnect()

If (connected = True) Then
    GM0Control21.Close
    connected = False
    Display.Text = "Disconnected"
    Exit Sub
End If


GM0Control21.comport = comport
GM0Control21.Initalise
GM0Control21.AttemptConnect
Display.Text = "Connecting com " + Str(comport)

End Sub


Sub initalisesystem()

connected = False
comport = 1

cp.Item(1).Checked = False
cp.Item(2).Checked = False
cp.Item(3).Checked = False
cp.Item(4).Checked = False
cp.Item(comport).Checked = True


modestr(0) = " DC"
modestr(1) = " DC Pk"
modestr(2) = " AC"
modestr(3) = " AC Max"
modestr(4) = " AC RMS"

unitsstr(0, 3) = " mT"
unitsstr(0, 2) = " mT"
unitsstr(0, 1) = " mT"
unitsstr(0, 0) = " T"

unitsstr(2, 3) = " kA/m"
unitsstr(2, 2) = " kA/m"
unitsstr(2, 1) = " kA/m"
unitsstr(2, 0) = " kA/m"

unitsstr(1, 3) = " G"
unitsstr(1, 2) = " G"
unitsstr(1, 1) = " kG"
unitsstr(1, 0) = " kG"

unitsstr(3, 3) = " Oe"
unitsstr(3, 2) = " Oe"
unitsstr(3, 1) = " kOe"
unitsstr(3, 0) = " kOe"

rangestr(0, 3) = "0.000"
rangestr(0, 2) = "00.00"
rangestr(0, 1) = "000.0"
rangestr(0, 0) = "0.000"
'kam
rangestr(2, 3) = "0.000"
rangestr(2, 2) = "00.00"
rangestr(2, 1) = "000.0"
rangestr(2, 0) = "0000"
'g
rangestr(1, 3) = "00.00"
rangestr(1, 2) = "000.0"
rangestr(1, 1) = "0.000"
rangestr(1, 0) = "00.00"
'oe
rangestr(3, 3) = "00.00"
rangestr(3, 2) = "000.0"
rangestr(3, 1) = "0.000"
rangestr(3, 0) = "00.00"


rangestr2(0, 3) = "2.999"
rangestr2(0, 2) = "29.99"
rangestr2(0, 1) = "299.9"
rangestr2(0, 0) = "2.999"
'kam
rangestr2(2, 3) = "2.999"
rangestr2(2, 2) = "29.99"
rangestr2(2, 1) = "299.9"
rangestr2(2, 0) = "2999"
'g
rangestr2(1, 3) = "29.99"
rangestr2(1, 2) = "299.9"
rangestr2(1, 1) = "2.999"
rangestr2(1, 0) = "29.99"
'oe
rangestr2(3, 3) = "29.99"
rangestr2(3, 2) = "299.9"
rangestr2(3, 1) = "2.999"
rangestr2(3, 0) = "29.99"





End Sub


Private Sub Command1_Click()
GM0Control21.SetRange (4)
End Sub

Private Sub cp_Click(Index As Integer)
'GM0Control21.Close
disconnect
comport = Index
cp.Item(1).Checked = False
cp.Item(2).Checked = False
cp.Item(3).Checked = False
cp.Item(4).Checked = False
cp.Item(Index).Checked = True
End Sub

Private Sub Form_Load()
initalisesystem
DoConnect

End Sub

Private Sub Form_Terminate()
GM0Control21.Close
End Sub

Private Sub Form_Unload(Cancel As Integer)
GM0Control21.Close
End Sub

Private Sub GetTime_Click()
GM0Control21.GetTime
msg = Str(GM0Control21.Day) + "/" + Str(GM0Control21.Month) + "/" + Str(GM0Control21.Year) + " " + Str(GM0Control21.Hour) + ":" + Str(GM0Control21.Min) + ":" + Str(GM0Control21.sec)
MsgBox (msg)
End Sub

Private Sub GM0Control21_Connected()
connected = True
meterser.Caption = "GM Serial No    :" + Str(GM0Control21.gmserial)
probeser.Caption = "Probe Serial No :" + Str(GM0Control21.probeserial)

End Sub

Private Sub GM0Control21_NewData()
gmunits = GM0Control21.lastunits
gmrange = GM0Control21.lastrange
gmmode = GM0Control21.lastmode

Data = GM0Control21.lastvalue
Display.Text = Format(Data, "#" + rangestr(gmunits, gmrange)) + " " + unitsstr(gmunits, gmrange) + modestr(gmmode)


End Sub

Private Sub GM0Control21_NewMode()

gmmode = GM0Control21.lastmode
mode.Item(0).Checked = False
mode.Item(1).Checked = False
mode.Item(2).Checked = False
mode.Item(3).Checked = False
mode.Item(4).Checked = False
mode.Item(gmmode).Checked = True

End Sub

Private Sub GM0Control21_NewRange()
TheRange = GM0Control21.lastrange
If (TheRange = 0) Then
    Range(0).Value = True
    Range(1).Value = False
    Range(2).Value = False
    Range(3).Value = False
End If

If (TheRange = 1) Then
    Range(0).Value = False
    Range(1).Value = True
    Range(2).Value = False
    Range(3).Value = False

End If
If (TheRange = 2) Then
    Range(0).Value = False
    Range(1).Value = False
    Range(2).Value = True
    Range(3).Value = False

End If
If (TheRange = 3) Then
    Range(0).Value = False
    Range(1).Value = False
    Range(2).Value = False
    Range(3).Value = True

End If
End Sub

Private Sub GM0Control21_NewUnits()
gmunits = GM0Control21.lastunits

units.Item(1).Checked = False
units.Item(2).Checked = False
units.Item(3).Checked = False
units.Item(4).Checked = False
units.Item(gmunits + 1).Checked = True

Range(0).Caption = rangestr2(gmunits, 0) + unitsstr(gmunits, 0)
Range(1).Caption = rangestr2(gmunits, 1) + unitsstr(gmunits, 1)
Range(2).Caption = rangestr2(gmunits, 2) + unitsstr(gmunits, 2)
Range(3).Caption = rangestr2(gmunits, 3) + unitsstr(gmunits, 3)


End Sub

Private Sub GM0Control21_RequireSheild()
MsgBox ("Sheild probe and press OK")
GM0Control21.ProbeInSheild
End Sub

Private Sub mode_Click(Index As Integer)
GM0Control21.SetMode (Index)
End Sub

Private Sub Range_Click(Index As Integer)
GM0Control21.SetRange (Index)
End Sub

Private Sub Reconnect_Click(Index As Integer)
disconnect
DoConnect
End Sub

Sub disconnect()
GM0Control21.Close
Display.Text = "Disconnected"
DoEvents
connected = False
End Sub

Private Sub SetTime_Click()


GM0Control21.Year = Year(Now) - 2000
GM0Control21.Month = Month(Now)
GM0Control21.Day = Day(Now)
GM0Control21.Hour = Hour(Now)
GM0Control21.Min = Minute(Now)
GM0Control21.sec = Second(Now)
GM0Control21.SetTime
End Sub

Private Sub Units_Click(Index As Integer)
GM0Control21.SetUnits (Index - 1)
End Sub
