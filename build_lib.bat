cl -nologo -Iinclude -MT -EHsc -O2 -c model_loader.cpp
lib -nologo model_loader.obj lib\assimp-vc140-mt.lib -out:lib\model_loader.lib
del model_loader.obj