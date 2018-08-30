call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64 

REM Increasing stack size for dev purposes

set CommonCompilerFlags=-F10485760 -MTd -nologo -fp:fast -Gm- -GR- -EHa- -Od -Oi -WX -W4 -wd4457 -wd4245 -wd4996 -wd4201 -wd4100 -wd4127 -wd4189 -wd4505 -wd4577 -wd4101 -wd4702 -wd4456 -wd4238 -wd4244 -wd4366 -wd4700 -wd4701 -wd4703 -wd4805 -wd4091 -wd4706 -DDIRECTX=1 -DOPENGL=0 -DIMGUI=1 -DWINDOWS=1 -DGAME_INTERNAL=1  -DDLL_NAME#"\"tile_battle.dll\"" -DDLL_TEMP_NAME#"\"tile_battle_temp.dll\"" -FC -Z7 

set CommonLinkerFlags=  -incremental:no -opt:ref Kernel32.lib user32.lib gdi32.lib winmm.lib Opengl32.lib Ws2_32.lib Ole32.lib Xinput9_1_0.lib libfbxsdk.lib

IF NOT EXIST build mkdir build
pushd build

del *.pdb > NUL 2> NUL

echo WAITING FOR PDB > lock.tmp

del lock.tmp

REM cl %CommonCompilerFlags%  ..\assetpacker.cpp -Fmassetpacker.map  /link -incremental:no -opt:ref -PDB:assetpacker.pdb %CommonLinkerFlags%
cl %CommonCompilerFlags%  yoyoyo.cpp -Fmyoyoyo.map /link -incremental:no -opt:ref -PDB:yoyoyo.pdb %CommonLinkerFlags%

popd
