[System Dlls]
SELECTED=Yes
FILENEED=STANDARD
HTTPLOCATION=
STATUS=
UNINSTALLABLE=Yes
TARGET=<WINSYSDIR>
FTPLOCATION=
VISIBLE=Yes
DESCRIPTION=
DISPLAYTEXT=
IMAGE=
DEFSELECTION=Yes
filegroup0=System Dlls
COMMENT=
INCLUDEINBUILD=Yes
INSTALLATION=NEWERVERSION\NEWERDATE
COMPRESSIFSEPARATE=No
MISC=
ENCRYPT=No
DISK=ANYDISK
TARGETDIRCDROM=
PASSWORD=
TARGETHIDDEN=Windows Operating System\Windows System Folder

[Components]
component0=System Dlls
component1=Program Dlls
component2=Program Files
component3=Shared DLLs
component4=Example Files

[TopComponents]
component0=System Dlls
component1=Program Dlls
component2=Example Files
component3=Program Files
component4=Shared DLLs

[Program Dlls]
SELECTED=Yes
FILENEED=CRITICAL
HTTPLOCATION=
STATUS=
UNINSTALLABLE=Yes
TARGET=<WINSYSDIR>
FTPLOCATION=
VISIBLE=Yes
DESCRIPTION=
DISPLAYTEXT=
IMAGE=
DEFSELECTION=Yes
filegroup0=Program DLLs
requiredby0=Shared DLLs
COMMENT=
INCLUDEINBUILD=Yes
INSTALLATION=ALWAYSOVERWRITE
COMPRESSIFSEPARATE=No
MISC=
ENCRYPT=No
DISK=ANYDISK
TARGETDIRCDROM=
PASSWORD=
TARGETHIDDEN=Windows Operating System\Windows System Folder

[SetupType]
setuptype0=Everything
setuptype1=Typical

[Program Files]
SELECTED=Yes
FILENEED=STANDARD
HTTPLOCATION=
STATUS=
UNINSTALLABLE=Yes
TARGET=<TARGETDIR>
FTPLOCATION=
VISIBLE=Yes
DESCRIPTION=
DISPLAYTEXT=
IMAGE=
DEFSELECTION=Yes
filegroup0=Program Executable Files
COMMENT=
INCLUDEINBUILD=Yes
INSTALLATION=ALWAYSOVERWRITE
COMPRESSIFSEPARATE=No
MISC=
ENCRYPT=No
DISK=ANYDISK
TARGETDIRCDROM=
PASSWORD=
TARGETHIDDEN=General Application Destination

[Shared DLLs]
required0=Program Dlls
SELECTED=Yes
FILENEED=CRITICAL
HTTPLOCATION=
STATUS=
UNINSTALLABLE=Yes
TARGET=<WINSYSDIR>
FTPLOCATION=
VISIBLE=Yes
DESCRIPTION=
DISPLAYTEXT=
IMAGE=
DEFSELECTION=Yes
filegroup0=Shared DLLs
COMMENT=
INCLUDEINBUILD=Yes
INSTALLATION=ALWAYSOVERWRITE
COMPRESSIFSEPARATE=No
MISC=
ENCRYPT=No
DISK=ANYDISK
TARGETDIRCDROM=
PASSWORD=
TARGETHIDDEN=Windows Operating System\Windows System Folder

[SetupTypeItem-Everything]
Comment=
item0=System Dlls
item1=Program Files
item2=Shared DLLs
item3=Example Files
Descrip=As Typical + Microsoft System DLL's (these should normaly be on your system already)
DisplayText=Typical + System DLL's

[Info]
Type=CompDef
Version=1.00.000
Name=

[Example Files]
SELECTED=Yes
FILENEED=STANDARD
HTTPLOCATION=
STATUS=
UNINSTALLABLE=Yes
TARGET=<TARGETDIR>
FTPLOCATION=
VISIBLE=Yes
DESCRIPTION=
DISPLAYTEXT=
IMAGE=
DEFSELECTION=Yes
filegroup0=Help Files
COMMENT=
INCLUDEINBUILD=Yes
INSTALLATION=ALWAYSOVERWRITE
COMPRESSIFSEPARATE=No
MISC=
ENCRYPT=No
DISK=ANYDISK
TARGETDIRCDROM=
PASSWORD=
TARGETHIDDEN=General Application Destination

[SetupTypeItem-Typical]
Comment=Only install Hirst Files
item0=Program Files
item1=Shared DLLs
item2=Example Files
Descrip=Install all Hirst files
DisplayText=Typical

