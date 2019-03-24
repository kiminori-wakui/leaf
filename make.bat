set MAKE=gmake
cd /d %~dp0
cd debug
%MAKE%
rem ˆ—Œ‹‰Ê‚Ì”»’è
if %errorlevel% neq 0 (
  echo [BATCHLOG]ABNORMAL END
  pause
  exit -1
)
cd ..
cd memory
%MAKE%
rem ˆ—Œ‹‰Ê‚Ì”»’è
if %errorlevel% neq 0 (
  echo [BATCHLOG]ABNORMAL END
  pause
  exit -1
)
cd ..
cd share
%MAKE%
rem ˆ—Œ‹‰Ê‚Ì”»’è
if %errorlevel% neq 0 (
  echo [BATCHLOG]ABNORMAL END
  pause
  exit -1
)
cd ..
cd compiler
%MAKE%
rem ˆ—Œ‹‰Ê‚Ì”»’è
if %errorlevel% neq 0 (
  echo [BATCHLOG]ABNORMAL END
  pause
  exit -1
)
cd ..
cd interpreter
%MAKE%
rem ˆ—Œ‹‰Ê‚Ì”»’è
if %errorlevel% neq 0 (
  echo [BATCHLOG]ABNORMAL END
  pause
  exit -1
)
cd ..
%MAKE%
rem ˆ—Œ‹‰Ê‚Ì”»’è
if %errorlevel% neq 0 (
  echo [BATCHLOG]ABNORMAL END
  pause
  exit -1
)
