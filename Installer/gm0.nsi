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

Section "System Wide Driver"

; Testing clause to Overwrite Existing Version - if exists
	IfFileExists "$SYSDIR\gm0.dll" 0 DoInstall
	MessageBox MB_YESNO "Overwrite Existing SYSTEM gm0.dll" IDYES DoInstall
	Abort "Quitting the install process"
DoInstall:

 SetOutPath "$SYSDIR"
 file "..\lib\Release\gm0.dll"
SectionEnd

; The stuff to install
Section "Examples, Source & Documentation" ;No components page, name is not important

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
IfFileExists "$INSTDIR" 0 DoInstall
	MessageBox MB_YESNO "Overwrite Existing Docs,Source and Examples" IDYES DoInstall
	Abort "Quitting the install process"
DoInstall:

 SetOutPath "$INSTDIR\lib"
 file "..\lib\Release\gm0.dll"
 file "..\lib\Release\gm0.lib"
 file "..\gm0-sharp\gm0-sharp\bin\Release\gm0-sharp.dll"

 SetOutPath "$INSTDIR\include"
 file "..\lib\gm0.h"

 SetOutPath "$INSTDIR\programs"
 file "..\lib\Release\gm0.dll"
 file "..\lib\Release\gm0.lib"
 file "..\consoleexample\Release\consoleexample.exe"
 file "..\gm0-sharp\gm0-sharp\bin\Release\gm0-sharp.dll"
 file "..\gm0-sharp\gm0_sharp_console_test\bin\Release\gm0_sharp_console_test.exe"
 file /r /x CVS "..\apps\"

 SetOutPath "$INSTDIR\source"
 file /r /x CVS /x apps /x Installer /x *.pch /x *.ncb /x *.dll /x Debug /x Release /x obj /x docs "..\"

 SetOutPath "$INSTDIR\"
 file /r /x CVS "..\docs"

 SetShellVarContext current    ;  All users
 CreateDirectory "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\"
 CreateShortCut "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\consoleexample.lnk" "$INSTDIR\programs\consoleexample.exe" "" "$INSTDIR\programsconsoleexample.exe" 0
 CreateShortCut "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\dotnetconsoleexample.lnk" "$INSTDIR\programs\gm0_sharp_console_test.exe" "" "$INSTDIR\programs\gm0_sharp_console_test.exe" 0
 CreateShortCut "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\GM05 & GM08 Programming Manual.lnk" "$INSTDIR\docs\GM05&08PROGRAMMING MANUAL.pdf" "" "$INSTDIR\docs\GM05&08PROGRAMMING MANUAL.pdf" 0
 CreateShortCut "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\Excel Example.lnk" "$INSTDIR\programs\gm05-control.xls" "" "$INSTDIR\programs\gm05-control.xls" 0
 CreateShortCut "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\Labview Example.lnk" "$INSTDIR\programs\simplegm0-1-6.vi" "" "$INSTDIRprograms\simplegm0-1-6.vi" 0
 CreateShortCut "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\Headers.lnk" "$INSTDIR\include" "" "$INSTDIR\include" 0
 CreateShortCut "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\Lib.lnk" "$INSTDIR\lib" "" "$INSTDIR\lib" 0
 CreateShortCut "$SMPROGRAMS\Hirst Magnetic Instruments Ltd\Gaussmeter\Source.lnk" "$INSTDIR\source" "" "$INSTDIR\source" 0
 SetShellVarContext current    ; reset to current user


SectionEnd ; end the section
