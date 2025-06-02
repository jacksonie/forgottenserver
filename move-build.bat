REM @echo off

if exist .\theforgottenserver-x64.exe (
	del .\theforgottenserver-x64.exe
)

if exist .\theforgottenserver-x64.pdb (
	del .\theforgottenserver-x64.pdb
)

if exist .\vc17\x64\release\theforgottenserver-x64.exe (
	copy .\vc17\x64\release\theforgottenserver-x64.exe .\theforgottenserver-x64.exe
)

if exist .\vc17\x64\release\theforgottenserver-x64.pdb (
	copy .\vc17\x64\release\theforgottenserver-x64.pdb .\theforgottenserver-x64.pdb
)
