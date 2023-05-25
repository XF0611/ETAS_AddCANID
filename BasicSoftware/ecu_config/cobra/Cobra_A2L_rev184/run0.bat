@echo off
if "%1" == "" (
set RTAA2L=C:\ETAS\RTA-CAR_9.2.1\RTA-RTE_7.5.3\Tools\RTA-A2L\bin\RTAA2L.exe
) else (
set RTAA2L="%1"
)

echo on
%RTAA2L% --template ETAS_BIP_Template.a2l --mcsd ..\..\..\src\rte\gen\Rte_McSupportData.arxml --out ETAS_BIP0.a2l
