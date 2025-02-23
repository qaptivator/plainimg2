#define VersionFileHandle FileOpen("VERSION.txt")
#define AppVersionFromTxt FileRead(VersionFileHandle)
#expr FileClose(VersionFileHandle)
#undef VersionFileHandle

[Setup]
AppName=plainIMG
AppVersion={#AppVersionFromTxt}
DefaultDirName={localappdata}\plainIMG
DefaultGroupName=plainIMG
OutputDir=build\release\installer
OutputBaseFilename=plainIMG_installer
Compression=lzma2
SolidCompression=yes
ChangesAssociations=yes

[Files]
Source: "build\release\static\plainIMG_static.exe"; DestDir: "{app}"; DestName: "plainIMG.exe"; Flags: ignoreversion

[Tasks]
Name: "plainimgAsDefaultAssoc"; Description: "Set plainIMG as the default image viewer"; GroupDescription: "Additional features:"; Flags: unchecked
Name: "addPlainimgToDesktop"; Description: "Add plainIMG to the desktop"; GroupDescription: "Additional features:"; Flags: unchecked
Name: "addPlainimgToStart"; Description: "Add plainIMG to the start menu"; GroupDescription: "Additional features:"

[Icons]
Name: "{group}\plainIMG"; Filename: "{app}\plainIMG.exe"; Tasks: addPlainimgToStart
Name: "{userdesktop}\plainIMG"; Filename: "{app}\plainIMG.exe"; Tasks: addPlainimgToDesktop

[Registry]
Root: HKCR; Subkey: "plainIMG"; ValueType: string; ValueName: ""; ValueData: "plainIMG Image Viewer"; Flags: uninsdeletekey
Root: HKCR; Subkey: "plainIMG\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: """{app}\plainIMG.exe"",0"; Flags: uninsdeletekey
Root: HKCR; Subkey: "plainIMG\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\plainIMG.exe"" ""%1"""; Flags: uninsdeletekey

Root: HKCR; Subkey: ".jpg"; ValueType: string; ValueName: ""; ValueData: "plainIMG"; Flags: uninsdeletevalue; Tasks: plainimgAsDefaultAssoc
Root: HKCR; Subkey: ".jpeg"; ValueType: string; ValueName: ""; ValueData: "plainIMG"; Flags: uninsdeletevalue; Tasks: plainimgAsDefaultAssoc
Root: HKCR; Subkey: ".png"; ValueType: string; ValueName: ""; ValueData: "plainIMG"; Flags: uninsdeletevalue; Tasks: plainimgAsDefaultAssoc
Root: HKCR; Subkey: ".bmp"; ValueType: string; ValueName: ""; ValueData: "plainIMG"; Flags: uninsdeletevalue; Tasks: plainimgAsDefaultAssoc
; Root: HKCR; Subkey: ".tiff"; ValueType: string; ValueName: ""; ValueData: "plainIMG"; Flags: uninsdeletevalue; Tasks: plainimgAsDefaultAssoc

; Root: HKCR; Subkey: "*\shell\Open with plainIMG"; ValueType: string; ValueName: ""; ValueData: "Open with plainIMG"; Flags: uninsdeletekey
; Root: HKCR; Subkey: "*\shell\Open with plainIMG\command"; ValueType: string; ValueName: ""; ValueData: """{app}\plainIMG.exe"" ""%1"""
; Root: HKCR; Subkey: "plainIMG\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\plainIMG.exe"" ""%1"""; Tasks: plainimgAsDefaultAssoc
; Root: HKCR; Subkey: "plainIMG\shell\open\command"; ValueType: string; ValueName: ""; ValueData: ExpandConstant('"{app}\plainIMG.exe" "%1"'); Flags: uninsdeletekey; Tasks: plainimgAsDefaultAssoc

[Run]
Filename: "{app}\plainIMG.exe"; Description: "Launch plainIMG"; Flags: nowait postinstall unchecked