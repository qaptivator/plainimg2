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

[Files]
Source: "build\release\static\plainIMG_static.exe"; DestDir: "{app}"; DestName: "plainIMG.exe"; Flags: ignoreversion

[Icons]
Name: "{group}\plainIMG"; Filename: "{app}\plainIMG.exe"
Name: "{userdesktop}\plainIMG"; Filename: "{app}\plainIMG.exe"

[Registry]
Root: HKCR; Subkey: "*\shell\Open with plainIMG"; ValueType: string; ValueName: ""; ValueData: "Open with plainIMG"; Flags: uninsdeletekey
Root: HKCR; Subkey: "*\shell\Open with plainIMG\command"; ValueType: string; ValueName: ""; ValueData: """{app}\plainIMG.exe"" ""%1"""

[Run]
Filename: "{app}\plainIMG.exe"; Description: "Launch plainIMG"; Flags: nowait postinstall