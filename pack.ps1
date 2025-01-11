param($version)

if (Test-Path -Path 'Out\cpp-x86') { Remove-Item -Recurse -Force 'Out\cpp-x86' | Out-Null }
if (Test-Path -Path 'Out\cpp-x64') { Remove-Item -Recurse -Force 'Out\cpp-x64' | Out-Null }
if (Test-Path -Path 'Out\python-x86') { Remove-Item -Recurse -Force 'Out\python-x86' | Out-Null }
if (Test-Path -Path 'Out\python-x64') { Remove-Item -Recurse -Force 'Out\python-x64' | Out-Null }

###########
# cpp-x86 #
###########
# include
New-Item -Path 'Out\cpp-x86\include' -ItemType 'directory' | Out-Null
Copy-Item -Recurse -Path 'Exp\Inc\*' -Destination 'Out\cpp-x86\include' | Out-Null
# library
New-Item -Path 'Out\cpp-x86\lib\debug' -ItemType 'directory' | Out-Null
New-Item -Path 'Out\cpp-x86\lib\release' -ItemType 'directory' | Out-Null
Copy-Item 'Rebug\D14UIKit.dll' -Destination 'Out\cpp-x86\lib\debug' | Out-Null
Copy-Item 'Release\D14UIKit.lib' -Destination 'Out\cpp-x86\lib' | Out-Null
Copy-Item 'Release\D14UIKit.dll' -Destination 'Out\cpp-x86\lib\release' | Out-Null
# example
New-Item -Path 'Out\cpp-x86\demo' -ItemType 'directory' | Out-Null
Copy-Item -Recurse -Path 'Test\*.cpp' -Destination 'Out\cpp-x86\demo' | Out-Null
Copy-Item -Recurse -Path 'Test\images' -Destination 'Out\cpp-x86\demo\images' | Out-Null
# archive
Compress-Archive -Force -Path 'Out\cpp-x86\*' -DestinationPath "Out\d14uikit_cpp_$version-x86.zip" | Out-Null

###########
# cpp-x64 #
###########
# include
New-Item -Path 'Out\cpp-x64\include' -ItemType 'directory' | Out-Null
Copy-Item -Recurse -Path 'Exp\Inc\*' -Destination 'Out\cpp-x64\include' | Out-Null
# library
New-Item -Path 'Out\cpp-x64\lib\debug' -ItemType 'directory' | Out-Null
New-Item -Path 'Out\cpp-x64\lib\release' -ItemType 'directory' | Out-Null
Copy-Item 'x64\Rebug\D14UIKit.dll' -Destination 'Out\cpp-x64\lib\debug' | Out-Null
Copy-Item 'x64\Release\D14UIKit.lib' -Destination 'Out\cpp-x64\lib' | Out-Null
Copy-Item 'x64\Release\D14UIKit.dll' -Destination 'Out\cpp-x64\lib\release' | Out-Null
# example
New-Item -Path 'Out\cpp-x64\demo' -ItemType 'directory' | Out-Null
Copy-Item -Recurse -Path 'Test\*.cpp' -Destination 'Out\cpp-x64\demo' | Out-Null
Copy-Item -Recurse -Path 'Test\images' -Destination 'Out\cpp-x64\demo\images' | Out-Null
# archive
Compress-Archive -Force -Path 'Out\cpp-x64\*' -DestinationPath "Out\d14uikit_cpp_$version-x64.zip" | Out-Null

##############
# python-x86 #
##############
# library
New-Item -Path 'Out\python-x86' -ItemType 'directory' | Out-Null
Copy-Item 'RPyBind\D14UIKit.pyd' -Destination 'Out\python-x86' | Out-Null
Copy-Item 'RPyBind\D14UIKit.pyi' -Destination 'Out\python-x86' | Out-Null
# example
New-Item -Path 'Out\python-x86\demo' -ItemType 'directory' | Out-Null
Copy-Item -Recurse -Path 'Test\PyBind\*.py' -Destination 'Out\python-x86\demo' | Out-Null
Copy-Item -Recurse -Path 'Test\images' -Destination 'Out\python-x86\demo\images' | Out-Null
# archive
Compress-Archive -Force -Path 'Out\python-x86\*' -DestinationPath "Out\d14uikit_python_$version-x86.zip" | Out-Null

##############
# python-x64 #
##############
# library
New-Item -Path 'Out\python-x64' -ItemType 'directory' | Out-Null
Copy-Item 'x64\RPyBind\D14UIKit.pyd' -Destination 'Out\python-x64' | Out-Null
Copy-Item 'x64\RPyBind\D14UIKit.pyi' -Destination 'Out\python-x64' | Out-Null
# example
New-Item -Path 'Out\python-x64\demo' -ItemType 'directory' | Out-Null
Copy-Item -Recurse -Path 'Test\PyBind\*.py' -Destination 'Out\python-x64\demo' | Out-Null
Copy-Item -Recurse -Path 'Test\images' -Destination 'Out\python-x64\demo\images' | Out-Null
# archive
Compress-Archive -Force -Path 'Out\python-x64\*' -DestinationPath "Out\d14uikit_python_$version-x64.zip" | Out-Null
