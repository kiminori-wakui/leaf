set MAKE=gmake
cd /d %~dp0
cd debug
%MAKE%
rem �������ʂ̔���
if %errorlevel% neq 0 (
  echo [BATCHLOG]ABNORMAL END
  pause
  exit -1
)
cd ..
cd memory
%MAKE%
rem �������ʂ̔���
if %errorlevel% neq 0 (
  echo [BATCHLOG]ABNORMAL END
  pause
  exit -1
)
cd ..
cd share
%MAKE%
rem �������ʂ̔���
if %errorlevel% neq 0 (
  echo [BATCHLOG]ABNORMAL END
  pause
  exit -1
)
cd ..
cd compiler
%MAKE%
rem �������ʂ̔���
if %errorlevel% neq 0 (
  echo [BATCHLOG]ABNORMAL END
  pause
  exit -1
)
cd ..
cd interpreter
%MAKE%
rem �������ʂ̔���
if %errorlevel% neq 0 (
  echo [BATCHLOG]ABNORMAL END
  pause
  exit -1
)
cd ..
%MAKE%
rem �������ʂ̔���
if %errorlevel% neq 0 (
  echo [BATCHLOG]ABNORMAL END
  pause
  exit -1
)
