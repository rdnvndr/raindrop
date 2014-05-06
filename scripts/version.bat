@echo off

for /f "tokens=*" %%i in ('git describe --tags --always') do (
   set ver=%%i
)
set test=-
if exist version.h (
  for /F "TOKENS=2,3*" %%i in (version.h) do ( 
    set test=%%j
  ) 
)
for /f "tokens=*" %%i in ('git show -s --format^=format:%%ci %ver%') do (
    set date_ver=%%i
  )
if not %test%=="%ver%" (  
  echo #ifndef VERSION_H > version.h
  echo #define VERSION_H >> version.h
  echo #define VER_REV "%datever% (%ver%)" >> version.h
  echo #endif >> version.h
)