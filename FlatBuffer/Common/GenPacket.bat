.\flatc.exe -c protocol.fbs
if errorlevel 1 pause

xcopy /y protocol_generated.h "../Server"

del protocol_generated.h