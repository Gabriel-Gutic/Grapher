@ECHO OFF

PUSHD ..
CALL Scripts\premake\premake5.exe vs2022
POPD
PAUSE