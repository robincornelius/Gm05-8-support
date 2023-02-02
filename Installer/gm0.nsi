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
!define VERSION "18122020"

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

Section /o "Microsoft x86 redistributables"
SetOutPath "$TEMP"
 File ..\redist\VC_redist.x86.exe          
 DetailPrint "Installing VC++ 2019 32 bit runtime"         
 ExecWait "$TEMP\VC_redist.x86.exe /q"         
 DetailPrint "Cleaning up"         
 Delete $TEMP\VC_redist.x86.exe      
SectionEnd


Section /o "Microsoft x64 redistributables"
SetOutPath "$TEMP"
 File ..\redist\VC_redist.x64.exe        
 DetailPrint "Installing VC++ 2019 64 bit runtime"         
 ExecWait "$TEMP\VC_redist.x64.exe /q"         
 DetailPrint "Cleaning up"         
 Delete $TEMP\VC_redist.x64.exe   
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

 DetailPrint "Uninstalling previous version"   
      
 Delete $SYSDIR\gm0.dll	
 RMDir /r "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter"

 DetailPrint "Uninstall complete"      
  

 SetOutPath "$INSTDIR\bin32"
 file "..\Release\x86\gm0.dll"
 file "..\Release\x86\gm0.lib"
 file "..\Release\x86\consoleexample.exe"
 file "..\apps\gm-control-32.xls"

 SetOutPath "$INSTDIR\common"
 file "..\apps\*.vi"
 file "..\apps\*.py"

   
 SetOutPath "$INSTDIR\bin64"
 file "..\Release\x64\gm0.dll"
 file "..\Release\x64\gm0.lib"
 file "..\Release\x64\consoleexample.exe"
 file "..\apps\gm-control-64.xls"

 SetOutPath "$INSTDIR\bin32"
 file "..\Release\x86\gm0-sharp.dll"
 file "..\Release\x86\gm0_sharp_console_test.exe"

 SetOutPath "$INSTDIR\bin64"
 file "..\Release\x64\gm0-sharp.dll"
 file "..\Release\x64\gm0_sharp_console_test.exe"

 
 SetOutPath "$INSTDIR\include"
 file "..\lib\gm0.h"

 SetOutPath "$INSTDIR\source"
 file /r /x .vs /x CVS /x apps /x Installer /x *.pch /x *.ncb /x *.dll /x Debug /x Release /x obj /x docs /x *.*sdf /x *.git /x x64 /x Backup* /x ipch /x m4 "..\"

 SetOutPath "$INSTDIR\"
 file /r /x CVS "..\docs"

 SetShellVarContext current    ;  All users
 CreateDirectory "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\"
 
 CreateDirectory "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\32 Bit\"
 CreateDirectory "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\64 Bit\"
 
 CreateDirectory "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\Common\"
 
 
 
 CreateShortCut "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\32 Bit\consoleexample32bit.lnk" "$INSTDIR\bin32\consoleexample.exe" "" "$INSTDIR\bin32\consoleexample.exe" 0
 CreateShortCut "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\32 Bit\dotnetconsoleexample32bit.lnk" "$INSTDIR\bin32\gm0_sharp_console_test.exe" "" "$INSTDIR\bin32\gm0_sharp_console_test.exe" 0
 
 CreateShortCut "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\64 Bit\consoleexample64bit.lnk" "$INSTDIR\bin64\consoleexample.exe" "" "$INSTDIR\bin64\consoleexample.exe" 0
 CreateShortCut "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\64 Bit\dotnetconsoleexample.lnk" "$INSTDIR\bin64\gm0_sharp_console_test.exe" "" "$INSTDIR\bin64\gm0_sharp_console_test.exe" 0
 
 CreateShortCut "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\GM05 & GM08 Programming Manual.lnk" "$INSTDIR\docs\GM05&08PROGRAMMING MANUAL.pdf" "" "$INSTDIR\docs\GM05&08PROGRAMMING MANUAL.pdf" 0
 
 CreateShortCut "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\32 Bit\Excel Example 32.lnk" "$INSTDIR\bin32\gm-control-32.xls" "" "$INSTDIR\bin32\gm-control-32.xls" 0
 CreateShortCut "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\64 Bit\Excel Example 64.lnk" "$INSTDIR\bin64\gm-control-64.xls" "" "$INSTDIR\bin64\gm-control-32.xls" 0
 
 CreateShortCut "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\Common\Labview Example.lnk" "$INSTDIR\common\simplegm0-1-6.vi" "" "$INSTDIR\common\simplegm0-1-6.vi" 0
 CreateShortCut "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\Common\Python Example.lnk" "$INSTDIR\common\gaussmeterexample.py" "" "$INSTDIR\common\gaussmeterexample.py" 0
 
 
 CreateShortCut "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\Common\Headers.lnk" "$INSTDIR\include" "" "$INSTDIR\include" 0
 CreateShortCut "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\32 Bit\Lib32.lnk" "$INSTDIR\bin32" "" "$INSTDIR\bin32" 0
 CreateShortCut "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\64 Bit\Lib64.lnk" "$INSTDIR\bin64" "" "$INSTDIR\bin64" 0
 CreateShortCut "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\Common\Source.lnk" "$INSTDIR\source" "" "$INSTDIR\source" 0
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

