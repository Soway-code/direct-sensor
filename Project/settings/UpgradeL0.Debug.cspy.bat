@REM This batch file has been generated by the IAR Embedded Workbench
@REM C-SPY Debugger, as an aid to preparing a command line for running
@REM the cspybat command line utility using the appropriate settings.
@REM
@REM Note that this file is generated every time a new debug session
@REM is initialized, so you may want to move or rename the file before
@REM making changes.
@REM
@REM You can launch cspybat by typing the name of this batch file followed
@REM by the name of the debug file (usually an ELF/DWARF or UBROF file).
@REM
@REM Read about available command line parameters in the C-SPY Debugging
@REM Guide. Hints about additional command line parameters that may be
@REM useful in specific cases:
@REM   --download_only   Downloads a code image without starting a debug
@REM                     session afterwards.
@REM   --silent          Omits the sign-on message.
@REM   --timeout         Limits the maximum allowed execution time.
@REM 


@echo off 

if not "%~1" == "" goto debugFile 

@echo on 

<<<<<<< HEAD
"C:\Program Files (x86)\IAR Systems\Embedded Workbench 8.1\common\bin\cspybat" -f "F:\信为产品\05 - 正反转\DirectionSensor_rotation_1.2_正反转_L031(F3)  调试第4版 修改累计运行时间 20200410  V1.0.3\DirectionSensor_rotation_1.2_正反转_L031(F3)\Project\settings\UpgradeL0.Debug.general.xcl" --backend -f "F:\信为产品\05 - 正反转\DirectionSensor_rotation_1.2_正反转_L031(F3)  调试第4版 修改累计运行时间 20200410  V1.0.3\DirectionSensor_rotation_1.2_正反转_L031(F3)\Project\settings\UpgradeL0.Debug.driver.xcl" 
=======
"D:\Embedded Workbench 8.1\common\bin\cspybat" -f "E:\project_repo\direct-sensor\Project\settings\UpgradeL0.Debug.general.xcl" --backend -f "E:\project_repo\direct-sensor\Project\settings\UpgradeL0.Debug.driver.xcl" 
>>>>>>> develop

@echo off 
goto end 

:debugFile 

@echo on 

<<<<<<< HEAD
"C:\Program Files (x86)\IAR Systems\Embedded Workbench 8.1\common\bin\cspybat" -f "F:\信为产品\05 - 正反转\DirectionSensor_rotation_1.2_正反转_L031(F3)  调试第4版 修改累计运行时间 20200410  V1.0.3\DirectionSensor_rotation_1.2_正反转_L031(F3)\Project\settings\UpgradeL0.Debug.general.xcl" "--debug_file=%~1" --backend -f "F:\信为产品\05 - 正反转\DirectionSensor_rotation_1.2_正反转_L031(F3)  调试第4版 修改累计运行时间 20200410  V1.0.3\DirectionSensor_rotation_1.2_正反转_L031(F3)\Project\settings\UpgradeL0.Debug.driver.xcl" 
=======
"D:\Embedded Workbench 8.1\common\bin\cspybat" -f "E:\project_repo\direct-sensor\Project\settings\UpgradeL0.Debug.general.xcl" "--debug_file=%~1" --backend -f "E:\project_repo\direct-sensor\Project\settings\UpgradeL0.Debug.driver.xcl" 
>>>>>>> develop

@echo off 
:end