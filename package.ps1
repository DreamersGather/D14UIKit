param($version)

Remove-Item -Recurse -Force Wrap | Out-Null

New-Item -Path 'Wrap\cpp\include' -ItemType 'directory' | Out-Null
Copy-Item -Recurse -Path 'Exp\Inc\*' -Destination 'Wrap\cpp\include' | Out-Null
New-Item -Path 'Wrap\cpp\lib\debug' -ItemType 'directory' | Out-Null
New-Item -Path 'Wrap\cpp\lib\release' -ItemType 'directory' | Out-Null
Copy-Item 'x64\Rebug\D14UIKit.dll' -Destination 'Wrap\cpp\lib\debug' | Out-Null
Copy-Item 'x64\Release\D14UIKit.lib' -Destination 'Wrap\cpp\lib' | Out-Null
Copy-Item 'x64\Release\D14UIKit.dll' -Destination 'Wrap\cpp\lib\release' | Out-Null
Compress-Archive -Force -Path 'Wrap\cpp' -DestinationPath "Wrap\d14uikit_cpp_$version.zip" | Out-Null

Write-Output "C++ wrapper packaged to Wrap\d14uikit_cpp_$version.zip"

New-Item -Path 'Wrap\python' -ItemType 'directory' | Out-Null
Copy-Item 'x64\RPyBind\D14UIKit.pyd' -Destination 'Wrap\python' | Out-Null
Set-Location 'Wrap\python' | Out-Null
&'stubgen' '-m' 'D14UIKit' '-o' '.' | Out-Null
Set-Location '..\..' | Out-Null # fucking mypy stubgen, --search-path is a bullshit
Compress-Archive -Force -Path 'Wrap\python' -DestinationPath "Wrap\d14uikit_python_$version.zip" | Out-Null

Write-Output "Python wrapper packaged to Wrap\d14uikit_python_$version.zip"
