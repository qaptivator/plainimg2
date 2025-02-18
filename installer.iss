[Setup]
AppName=plainIMG
AppVersion=1.0.0
DefaultDirName={localappdata}\plainIMG
DefaultGroupName=plainIMG
OutputDir=./release
OutputBaseFilename=plainIMG_installer
Compression=lzma2
SolidCompression=yes

[Files]
Source: "dist\plainIMG.exe"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\plainIMG"; Filename: "{app}\plainIMG.exe"
Name: "{userdesktop}\plainIMG"; Filename: "{app}\plainIMG.exe"; Tasks: desktopicon

[Registry]
Root: HKCR; Subkey: "*\shell\Open with plainIMG"; ValueType: string; ValueName: ""; ValueData: "Open with plainIMG"; Flags: uninsdeletekey
Root: HKCR; Subkey: "*\shell\Open with plainIMG\command"; ValueType: string; ValueName: ""; ValueData: """{app}\plainIMG.exe"" ""%1"""

[Run]
Filename: "{app}\plainIMG.exe"; Description: "Launch plainIMG"; Flags: nowait postinstall