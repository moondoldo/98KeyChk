@echo off
setlocal

:: ---------------------------------------
:: 同じフォルダにあるこの「.bat」と同名の「.ps1」ファイルをpowershellコマンドとして実行
::
:: [%~dp0]はこのバッチファイルのディレクトリを取得
:: [%~n0]はこのバッチファイルのファイル名を拡張子無しで取得
:: [%ps1filename%]を使う場合、スペース入りファイルパスを想定し["]で囲う("%ps1filename%")
:: ---------------------------------------

set "ps1filename=%~dp0%~n0.ps1"

:: ファイル存在チェック
if not exist "%ps1filename%" (
    echo ファイルが存在しません
    echo %ps1filename%
    pause
    exit /b 1
)

powershell -ExecutionPolicy Unrestricted -File "%ps1filename%"
