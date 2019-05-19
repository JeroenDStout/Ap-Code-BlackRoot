@echo off

set NLM=^


set NL=^^^%NLM%%NLM%^%NLM%%NLM%

FOR /F "tokens=* USEBACKQ" %%F IN (`git rev-parse --abbrev-ref HEAD`) DO (
SET branch=%%F
)

FOR /F "tokens=* USEBACKQ" %%F IN (`git describe --tags`) DO (
SET version=%%F
)

FOR /F "tokens=* USEBACKQ" %%F IN (`git log -1 --pretty^=format:%%ai`) DO (
SET time=%%F
)

set /p licence=<licence_name.txt

cd ../
IF not exist "-genc" (mkdir "-genc")
cd "-genc/"

echo %NL% %~1%NL%  Licence: %licence%%NL%  Version: %version%%NL%  Branch:  %branch%%NL%  Time:    %time%%NL%

echo #define REPO_VERSION_DATA { "%~1", "%licence%", "%version%", "%branch%", "%time%", BR_BUILD_TOOL } >def_repo_version.h
echo { "repo" : { "name" : "%~1", "version" : "%version%", "branch" : "%branch%", "time" : "%time%" } } >repo_version.json