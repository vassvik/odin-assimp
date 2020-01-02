@echo off

cl /nologo /I..\include /MT /EHsc /O2 /c ..\src\meshloader_lib.cpp
lib /nologo meshloader_lib.obj ..\lib\assimp-vc141-mt.lib -out:..\lib\mesh_loader.lib
del meshloader_lib.obj

