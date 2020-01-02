@echo off

if not exist "..\include" mkdir ..\include
if not exist "..\lib" mkdir ..\lib

git clone https://github.com/assimp/assimp.git

pushd assimp
	mkdir build_release

	pushd build_release
		cmake -G "NMake Makefiles"^
		 -DASSIMP_NO_EXPORT=OFF^
		 -DASSIMP_INSTALL_PDB=OFF^
		 -DASSIMP_BUILD_ASSIMP_TOOLS=OFF^
		 -DASSIMP_BUILD_TESTS=OFF^
		 -DASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT=OFF^
		 -DASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT=OFF^
		 -DASSIMP_BUILD_FBX_IMPORTER=ON^
		 -DASSIMP_BUILD_OBJ_IMPORTER=ON^
		 -DCMAKE_BUILD_TYPE=Release^
		 .. 
		nmake
		xcopy /E /Y include ..\..\..\include\
		copy code\*.lib ..\..\..\lib\
		copy code\*.dll ..\..\..\
	popd

	xcopy /E /Y include ..\..\include\
popd

rmdir /Q /S assimp