@ECHO OFF

CD ..\build_scripts
CHCP 65001 > NUL
cmake -P 5_build-docs_html.cmake
CHCP 866 > NUL
