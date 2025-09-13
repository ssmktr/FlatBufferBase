.\flatc.exe -c protocol.fbs
.\flatc --csharp protocol.fbs
if errorlevel 1 pause

REM =============== C++ Protocl ===============
xcopy /y protocol_generated.h "../Server"
xcopy /y protocol_generated.h "../ConsoleClient"
xcopy /y protocol_generated.h "../MFCServer"
xcopy /y protocol_generated.h "../MFCClient"

del protocol_generated.h


REM =============== C# Protocl ===============
ren fbs Protocol
xcopy /y /e /i Protocol "../CSharpClientClient/Protocol"

rmdir /s /q Protocol