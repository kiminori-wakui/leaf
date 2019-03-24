set MAKE=gmake
cd /d %~dp0
cd debug
%MAKE%
rem 処理結果の判定
if %errorlevel% neq 0 (
  echo [BATCHLOG]ABNORMAL END
  pause
  exit -1
)
cd ..
cd memory
%MAKE%
rem 処理結果の判定
if %errorlevel% neq 0 (
  echo [BATCHLOG]ABNORMAL END
  pause
  exit -1
)
cd ..
cd share
%MAKE%
rem 処理結果の判定
if %errorlevel% neq 0 (
  echo [BATCHLOG]ABNORMAL END
  pause
  exit -1
)
cd ..
cd compiler
%MAKE%
rem 処理結果の判定
if %errorlevel% neq 0 (
  echo [BATCHLOG]ABNORMAL END
  pause
  exit -1
)
cd ..
cd interpreter
%MAKE%
rem 処理結果の判定
if %errorlevel% neq 0 (
  echo [BATCHLOG]ABNORMAL END
  pause
  exit -1
)
cd ..
%MAKE%
rem 処理結果の判定
if %errorlevel% neq 0 (
  echo [BATCHLOG]ABNORMAL END
  pause
  exit -1
)
