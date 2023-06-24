@echo off
REM Flags
REM ***********Compiler Switches*****************
REM -Zi => Produces Debug Information (.pdb files) 
REM  ^__(can be problematic)
REM -GR- => Disable runtime type info
REM -EHa- => Disable exception handling
REM -Oi => Enable compiler intrinsics
REM -Od => (for learning) No Optimisation and leave everything as is
REM -MT => Use the static library, package all the c 
REM runtime library into the executable
REM ********************************************
IF NOT EXIST w:\gamedev\playground\learn-opengl\build mkdir w:\gamedev\playground\learn-opengl\build
pushd w:\gamedev\playground\learn-opengl\build
set ProjectRoot=w:\gamedev\playground\learn-opengl
set IncludePath=%ProjectRoot%\include
set LibPath=%ProjectRoot%\lib
set CommonIncludeFlags=/I"%IncludePath%" /Iglfw
set CommonCompilerFlags=-MT -nologo -Gm- -GR- -EHa- -Od -Oi -WX -W2 -wd4201 -wd4100 -wd4189 -FC -Z7
set CommonLinkerFlags=%ProjectRoot%\lib\glfw3.lib opengl32.lib kernel32.lib user32.lib gdi32.lib winmm.lib shell32.lib
cl %CommonCompilerFlags% %CommonIncludeFlags% %ProjectRoot%\code\win32_main.cpp %ProjectRoot%\code\glad.c /link %CommonLinkerFlags% /NODEFAULTLIB:LIBCMT
popd