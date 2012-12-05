  REM Gadget Factory
  REM Papilio One YM2149 YM File from Flash Example
 
 REM This example will let you play YM files from the SPI Flash using the ZPUino Soft Processor. 
 
 REM To learn more about this example visit its home page at the Papilio Audio Wiki:
 REM http://audio.gadgetfactory.net/index.php?n=Main.YM2149Flash
 
 REM To learn more about the Papilio and Gadget Factory products visit:
 REM http://www.GadgetFactory.net

 REM created 2012
 REM by Jack Gassett from Zetris code example
 REM http://www.gadgetfactory.net
 
 REM This example code is Creative Commons Attribution.
 
cd /D %TEMP%
copy %1 %TEMP%\audio.ym
"%~dp0bin\ymTool.exe" c audio.ym
"%~dp0bin\ymdump" audio.BIN
copy %TEMP%\audio.BIN.dat %1\..\"%~n1.ymd"
del %TEMP%\audio.BIN
del %TEMP%\audio.ym
del %TEMP%\audio.BIN.dat
REM pause