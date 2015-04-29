; example1.nsi
;
; This script is perhaps one of the simplest NSIs you can make. All of the
; optional settings are left to their default settings. The installer simply 
; prompts the user asking them where to install, and drops a copy of example1.nsi
; there. 

;--------------------------------

CRCCheck on

!define PRODUCT "gm0"
!define VENDOR "Hirst Magnetic Instruments Ltd"
!define VERSION "29042015"

Name "Hirst Gaussmeter drivers"
OutFile "gm0driver.exe"

InstallDir "$PROGRAMFILES\${VENDOR}\${PRODUCT}"

; License Information
;LicenseForceSelection radiobuttons checkbox
;LicenseText "This program is Licensed under the GNU General Public License (GPL)."
LicenseData "license.rtf"

; Request application privileges for Windows Vista
RequestExecutionLevel none

;--------------------------------

; Pages
Page license
Page components
Page directory
Page instfiles

;--------------------------------

Section "Microsoft x86 redistributables"
SetOutPath "$TEMP"
 File c:\code\vcredist_x86_2013.exe          
 DetailPrint "Installing VC++ 2013 32bit runtime"         
 ExecWait "$TEMP\vcredist_x86_2013.exe /q"         
 DetailPrint "Cleaning up"         
 Delete $TEMP\vcredist_x86_2013.exe     
SectionEnd


Section "Microsoft x64 redistributables"
SetOutPath "$TEMP"
 File c:\code\vcredist_x64_2013.exe          
 DetailPrint "Installing VC++ 2013 64 bit runtime"         
 ExecWait "$TEMP\vcredist_x64_2013.exe /q"         
 DetailPrint "Cleaning up"         
 Delete $TEMP\vcredist_x64_2013.exe     
SectionEnd

;Section "System Wide Driver"
;
; Testing clause to Overwrite Existing Version - if exists
;	IfFileExists "$SYSDIR\gm0.dll" 0 DoInstall
;	MessageBox MB_YESNO "Overwrite Existing SYSTEM gm0.dll" IDYES DoInstall
;	Abort "Quitting the install process"
;DoInstall:
;
;SetOutPath "$SYSDIR"
;file "..\lib\Release\gm0.dll"
;SectionEnd

; The stuff to install
Section "Examples, Source & Documentation" ;No components page, name is not important

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
IfFileExists "$INSTDIR" 0 DoInstall
	MessageBox MB_YESNO "Overwrite Existing Docs,Source and Examples" IDYES DoInstall
	Abort "Quitting the install process"
DoInstall:


 SetOutPath "$INSTDIR\bin32"
 file "..\Release\Win32\gm0.dll"
 file "..\Release\Win32\gm0.lib"
 file "..\Release\Win32\gm0-sharp.dll"
 file "..\Release\Win32\consoleexample.exe"
 file "..\Release\Win32\gm0-sharp.dll"
 file "..\Release\Win32\gm0_sharp_console_test.exe"
 file /r /x CVS "..\apps\"
   
  SetOutPath "$INSTDIR\bin64"
 file "..\Release\x64\gm0.dll"
 file "..\Release\x64\gm0.lib"
 file "..\Release\x64\gm0-sharp.dll"
 file "..\Release\x64\consoleexample.exe"
 file "..\Release\x64\gm0-sharp.dll"
 file "..\Release\x64\gm0_sharp_console_test.exe"
 file /r /x CVS "..\apps\"
   
 SetOutPath "$INSTDIR\include"
 file "..\lib\gm0.h"

 SetOutPath "$INSTDIR\source"
 file /r /x CVS /x apps /x Installer /x *.pch /x *.ncb /x *.dll /x Debug /x Release /x obj /x docs /x *.*sdf /x *.git /x x64 /x Backup* /x ipch /x m4 "..\"

 SetOutPath "$INSTDIR\"
 file /r /x CVS "..\docs"

 SetShellVarContext current    ;  All users
 CreateDirectory "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\"
 
 CreateShortCut "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\consoleexample32bit.lnk" "$INSTDIR\bin32\consoleexample.exe" "" "$INSTDIR\bin32\consoleexample.exe" 0
 CreateShortCut "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\dotnetconsoleexample32bit.lnk" "$INSTDIR\bin32\gm0_sharp_console_test.exe" "" "$INSTDIR\bin32\gm0_sharp_console_test.exe" 0
 
 CreateShortCut "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\consoleexample64bit.lnk" "$INSTDIR\bin64\consoleexample.exe" "" "$INSTDIR\bin64\consoleexample.exe" 0
 CreateShortCut "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\dotnetconsoleexample64bit.lnk" "$INSTDIR\bin64\gm0_sharp_console_test.exe" "" "$INSTDIR\bin64\gm0_sharp_console_test.exe" 0
 
 CreateShortCut "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\GM05 & GM08 Programming Manual.lnk" "$INSTDIR\docs\GM05&08PROGRAMMING MANUAL.pdf" "" "$INSTDIR\docs\GM05&08PROGRAMMING MANUAL.pdf" 0
 
 CreateShortCut "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\Excel Example.lnk" "$INSTDIR\bin32\gm05-control.xls" "" "$INSTDIR\bin32\gm05-control.xls" 0
 CreateShortCut "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\Labview Example.lnk" "$INSTDIR\bin32\simplegm0-1-6.vi" "" "$INSTDIR\bin32\simplegm0-1-6.vi" 0
 
 CreateShortCut "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\Headers.lnk" "$INSTDIR\include" "" "$INSTDIR\include" 0
 CreateShortCut "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\Lib.lnk" "$INSTDIR\lib" "" "$INSTDIR\lib" 0
 CreateShortCut "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\Source.lnk" "$INSTDIR\source" "" "$INSTDIR\source" 0
 CreateShortCut "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\Uninstall.lnk" "$INSTDIR\Uninstall.exe" "" "$INSTDIR\Uninstall.exe" 0
 
 SetShellVarContext current    ; reset to current user

 WriteUninstaller $INSTDIR\Uninstall.exe

SectionEnd ; end the section



Section "Uninstall"
  RMDir /r $INSTDIR
  RMDir /r "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter"
  Delete "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter"
  Delete /REBOOTOK $SYSDIR\gm0.dll
SectionEnd

