param($version)

Remove-Item -Recurse -Force Out | Out-Null

New-Item -Path 'Out\cpp\include' -ItemType 'directory' | Out-Null
Copy-Item -Recurse -Path 'Exp\Inc\*' -Destination 'Out\cpp\include' | Out-Null
New-Item -Path 'Out\cpp\lib\debug' -ItemType 'directory' | Out-Null
New-Item -Path 'Out\cpp\lib\release' -ItemType 'directory' | Out-Null
Copy-Item 'x64\Rebug\D14UIKit.dll' -Destination 'Out\cpp\lib\debug' | Out-Null
Copy-Item 'x64\Release\D14UIKit.lib' -Destination 'Out\cpp\lib' | Out-Null
Copy-Item 'x64\Release\D14UIKit.dll' -Destination 'Out\cpp\lib\release' | Out-Null
New-Item -Path 'Out\cpp\demo' -ItemType 'directory' | Out-Null
Copy-Item -Recurse -Path 'Test\*.cpp' -Destination 'Out\cpp\demo' | Out-Null
Copy-Item 'Test\test.png' -Destination 'Out\cpp\demo' | Out-Null
Compress-Archive -Force -Path 'Out\cpp' -DestinationPath "Out\d14uikit_cpp_$version.zip" | Out-Null

Write-Output "C++ wrapper packaged to Out\d14uikit_cpp_$version.zip"

New-Item -Path 'Out\python' -ItemType 'directory' | Out-Null
Copy-Item 'x64\RPyBind\D14UIKit.pyd' -Destination 'Out\python' | Out-Null
Set-Location 'Out\python' | Out-Null
&'stubgen' '-m' 'D14UIKit' '-o' '.' | Out-Null
Set-Location '..\..' | Out-Null # fucking mypy stubgen, --search-path is a bullshit
New-Item -Path 'Out\python\demo' -ItemType 'directory' | Out-Null
Copy-Item -Recurse -Path 'Test\PyBind\*.py' -Destination 'Out\python\demo' | Out-Null
Copy-Item 'Test\test.png' -Destination 'Out\python\demo' | Out-Null
Compress-Archive -Force -Path 'Out\python' -DestinationPath "Out\d14uikit_python_$version.zip" | Out-Null

Write-Output "Python wrapper packaged to Out\d14uikit_python_$version.zip"
