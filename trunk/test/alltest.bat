luac.exe -s -o alltest.luc alltest.lua
ChunkSpy.lua alltest.luc alltest.lua -o alltest.lst
lua alltest.lua
uLuaPC.exe alltest.luc
pause