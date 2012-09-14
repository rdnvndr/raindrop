@echo off
@depends -c -f:1 -u:1 -pa:1 -pf:0 -pe:0 -pb -sm:2 -sf:4 -oc:result.csv %1

set fileAddr=%~d1%~p1
if not exist %fileAddr%imageformats mkdir %fileAddr%imageformats
if not exist %fileAddr%accessible mkdir %fileAddr%accessible
if not exist %fileAddr%bearer mkdir %fileAddr%bearer
if not exist %fileAddr%codecs mkdir %fileAddr%codecs
if not exist %fileAddr%designer mkdir %fileAddr%designer
if not exist %fileAddr%graphicssystems mkdir %fileAddr%graphicssystems
if not exist %fileAddr%iconengines mkdir %fileAddr%iconengines
if not exist %fileAddr%imageformats mkdir %fileAddr%imageformats
if not exist %fileAddr%phonon_backend mkdir %fileAddr%phonon_backend
if not exist %fileAddr%qmltooling mkdir %fileAddr%qmltooling
if not exist %fileAddr%sqldrivers mkdir %fileAddr%sqldrivers

@setlocal EnableDelayedExpansion

set str=
for /f "tokens=1 delims=," %%i in (result.csv) do (
  set wdir=%%i
  set isdir=%%i
  set bindir=%%i
  set str=%%i
  set str=!str:%QTDIR%=!
  set wdir=!wdir:%windir%=!
  set bindir=!bindir:%fileAddr%=!
  set isdir=!isdir:\=!
  if %%i neq !isdir! if %%i == !bindir! if %%i == !wdir! (
    if %%i neq !str! (
      set str=!str:\bin\=!
      set str=!str:\plugins\=!
      echo Copy file: %%i
      copy /Y %%i !fileAddr!!str!
    ) else (
      echo Copy file: %%i
      copy /Y %%i !fileAddr!
    ) 
  )
)

for /f "tokens=2 delims=," %%i in (result.csv) do (
   set wdir=%%i
   set isdir=%%i
   set bindir=%%i
   set str=%%i
   set str=!str:%QTDIR%=!
   set wdir=!wdir:%windir%=!
   set bindir=!bindir:%fileAddr%=!
   set isdir=!isdir:\=!
   if %%i neq !isdir! if %%i == !bindir! if %%i == !wdir! (
     if %%i neq !str! (
       set str=!str:\bin\=!
       set str=!str:\plugins\=!
       echo Copy file: %%i
       copy /Y %%i !fileAddr!!str!
     ) else (
       echo Copy file: %%i
       copy /Y %%i !fileAddr!
     )
   )
)

del result.csv