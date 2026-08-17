@echo off
setlocal

set ROOT=%~dp0
set BUILD_DIR=%ROOT%build
set CMAKE_BUILD_TYPE=Release

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

call rustup target add x86_64-pc-windows-gnu

cmake -S "%ROOT%" -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -B "%BUILD_DIR%"
cmake --build "%BUILD_DIR%" -j%NUMBER_OF_PROCESSORS%

set GOOS=windows
set GOARCH=amd64
set CGO_ENABLED=1
cd /d "%ROOT%src\net"
go build -ldflags="-s -w" -buildmode=c-archive -o "%BUILD_DIR%\libaura_net.a"

cd /d "%ROOT%src\core"
cargo build --release --target x86_64-pc-windows-gnu

if exist "%BUILD_DIR%\aura_core_native.dll" (
    echo [Aura] Windows Release build ready: %BUILD_DIR%\aura_core_native.dll
) else if exist "%BUILD_DIR%\Release\aura_core_native.dll" (
    echo [Aura] Windows Release build ready: %BUILD_DIR%\Release\aura_core_native.dll
) else (
    echo [Aura] Build generated no DLL in the expected path.
)

endlocal
