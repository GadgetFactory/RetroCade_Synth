mkdir 250K\xst\projnav.tmp
mkdir 500K\xst\projnav.tmp
mkdir LX9\xst\projnav.tmp
mkdir DUO_LX9\xst\projnav.tmp
cd DUO_LX9
start Papilio_DUO_LX9.cmd
cd LX9
start Papilio_Pro.cmd
REM clean.bat
REM mkdir xst\projnav.tmp
cd ..\500K
start Papilio_One_500K.cmd
REM clean.bat
REM mkdir xst\projnav.tmp
cd ..\250K
start Papilio_One_250K.cmd
pause
clean.bat