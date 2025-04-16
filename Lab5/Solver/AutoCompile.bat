@echo off
chcp 65001 >nul
REM 设置项目路径
set PROJECT_DIR=%~dp0
set BUILD_DIR=%PROJECT_DIR%build

REM 检查是否安装了 CMake 和 MinGW
where cmake >nul 2>nul
if %errorlevel% neq 0 (
    echo [ERROR] CMake 未安装或未添加到 PATH 环境变量中。
    pause
    exit /b 1
)

where g++ >nul 2>nul
if %errorlevel% neq 0 (
    echo [ERROR] MinGW 的 g++ 未安装或未添加到 PATH 环境变量中。
    pause
    exit /b 1
)

where mingw32-make >nul 2>nul
if %errorlevel% neq 0 (
    echo [ERROR] MinGW 的 mingw32-make 未安装或未添加到 PATH 环境变量中。
    pause
    exit /b 1
)

REM 创建 build 文件夹
if not exist "%BUILD_DIR%" (
    mkdir "%BUILD_DIR%"
)

REM 进入 build 文件夹
cd /d "%BUILD_DIR%"

REM 运行 CMake 配置
echo [INFO] 正在运行 CMake 配置...
cmake -G "MinGW Makefiles" ..

if %errorlevel% neq 0 (
    echo [ERROR] CMake 配置失败。
    pause
    exit /b 1
)

REM 运行 make 构建
echo [INFO] 正在运行 make 构建...
mingw32-make

if %errorlevel% neq 0 (
    echo [ERROR] 构建失败。
    pause
    exit /b 1
)

REM 运行生成的可执行文件
echo [INFO] 正在运行程序...
LAB5.exe

if %errorlevel% neq 0 (
    echo [ERROR] 程序运行失败。
    pause
    exit /b 1
)

echo [INFO] 全部完成！
pause
exit /b 0