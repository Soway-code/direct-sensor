﻿param([String]$debugfile = "");

# This powershell file has been generated by the IAR Embedded Workbench
# C - SPY Debugger, as an aid to preparing a command line for running
# the cspybat command line utility using the appropriate settings.
#
# Note that this file is generated every time a new debug session
# is initialized, so you may want to move or rename the file before
# making changes.
#
# You can launch cspybat by typing Powershell.exe -File followed by the name of this batch file, followed
# by the name of the debug file (usually an ELF / DWARF or UBROF file).
#
# Read about available command line parameters in the C - SPY Debugging
# Guide. Hints about additional command line parameters that may be
# useful in specific cases :
#   --download_only   Downloads a code image without starting a debug
#                     session afterwards.
#   --silent          Omits the sign - on message.
#   --timeout         Limits the maximum allowed execution time.
#


if ($debugfile -eq "")
{
<<<<<<< HEAD
& "C:\Program Files (x86)\IAR Systems\Embedded Workbench 8.1\common\bin\cspybat" -f "F:\信为产品\05 - 正反转\DirectionSensor_rotation_1.2_正反转_L031(F3)  调试第4版 修改累计运行时间 20200410  V1.0.3\DirectionSensor_rotation_1.2_正反转_L031(F3)\Project\settings\UpgradeL0.Debug.general.xcl" --backend -f "F:\信为产品\05 - 正反转\DirectionSensor_rotation_1.2_正反转_L031(F3)  调试第4版 修改累计运行时间 20200410  V1.0.3\DirectionSensor_rotation_1.2_正反转_L031(F3)\Project\settings\UpgradeL0.Debug.driver.xcl" 
}
else
{
& "C:\Program Files (x86)\IAR Systems\Embedded Workbench 8.1\common\bin\cspybat" -f "F:\信为产品\05 - 正反转\DirectionSensor_rotation_1.2_正反转_L031(F3)  调试第4版 修改累计运行时间 20200410  V1.0.3\DirectionSensor_rotation_1.2_正反转_L031(F3)\Project\settings\UpgradeL0.Debug.general.xcl" --debug_file=$debugfile --backend -f "F:\信为产品\05 - 正反转\DirectionSensor_rotation_1.2_正反转_L031(F3)  调试第4版 修改累计运行时间 20200410  V1.0.3\DirectionSensor_rotation_1.2_正反转_L031(F3)\Project\settings\UpgradeL0.Debug.driver.xcl" 
=======
& "D:\Embedded Workbench 8.1\common\bin\cspybat" -f "E:\project_repo\direct-sensor\Project\settings\UpgradeL0.Debug.general.xcl" --backend -f "E:\project_repo\direct-sensor\Project\settings\UpgradeL0.Debug.driver.xcl" 
}
else
{
& "D:\Embedded Workbench 8.1\common\bin\cspybat" -f "E:\project_repo\direct-sensor\Project\settings\UpgradeL0.Debug.general.xcl" --debug_file=$debugfile --backend -f "E:\project_repo\direct-sensor\Project\settings\UpgradeL0.Debug.driver.xcl" 
>>>>>>> develop
}
