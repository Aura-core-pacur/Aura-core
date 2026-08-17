@echo off
setlocal

set ROOT=%~dp0
set BUILD_DIR=%ROOT%build
set JAVA_HOME=%JAVA_HOME%

if not exist "%BUILD_DIR%" (
    call "%ROOT%build_windows.bat"
)

if not exist "%BUILD_DIR%\aura_core_native.dll" if not exist "%BUILD_DIR%\Release\aura_core_native.dll" (
    echo [Aura] Native DLL not found. Build failed.
    exit /b 1
)

if not "%JAVA_HOME%"=="" (
    set PATH=%JAVA_HOME%\bin;%PATH%
)

javac -d "%BUILD_DIR%\java" "%ROOT%src\java\com\auracore\*.java"
java -Djava.library.path=%BUILD_DIR% -cp "%BUILD_DIR%\java" com.auracore.Main

endlocal
