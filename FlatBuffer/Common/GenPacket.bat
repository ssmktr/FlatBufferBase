.\flatc.exe -c protocol.fbs
if errorlevel 1 pause

xcopy /y protocol_generated.h "../Server"
xcopy /y protocol_generated.h "../ConsoleClient"

del protocol_generated.h