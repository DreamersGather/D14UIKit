param($version)

function D14UIKit-Remove-Expired
{
    param($DistName)
    if (Test-Path -Path "Dist\\$DistName")
    {
        Remove-Item -Recurse -Force "Dist\\$DistName" | Out-Null
    }
}
D14UIKit-Remove-Expired -DistName "cpp-x86"
D14UIKit-Remove-Expired -DistName "cpp-x64"

D14UIKit-Remove-Expired -DistName "py310-x86"
D14UIKit-Remove-Expired -DistName "py311-x86"
D14UIKit-Remove-Expired -DistName "py312-x86"
D14UIKit-Remove-Expired -DistName "py313-x86"

D14UIKit-Remove-Expired -DistName "py310-x64"
D14UIKit-Remove-Expired -DistName "py311-x64"
D14UIKit-Remove-Expired -DistName "py312-x64"
D14UIKit-Remove-Expired -DistName "py313-x64"

function D14UIKit-Pack-Cpp
{
    param($ArchName)
    # include
    New-Item -Path "Dist\\cpp-$ArchName\\include" -ItemType "directory" | Out-Null
    Copy-Item -Recurse -Path "Exp\\Inc\\*" -Destination "Dist\\cpp-$ArchName\\include" | Out-Null
    # library
    New-Item -Path "Dist\\cpp-$ArchName\\lib\\debug" -ItemType "directory" | Out-Null
    New-Item -Path "Dist\\cpp-$ArchName\\lib\\release" -ItemType "directory" | Out-Null
    Copy-Item "Out\\$ArchName\\Rebug\\D14UIKit.dll" -Destination "Dist\\cpp-$ArchName\\lib\\debug" | Out-Null
    Copy-Item "Out\\$ArchName\\Release\\D14UIKit.lib" -Destination "Dist\\cpp-$ArchName\\lib" | Out-Null
    Copy-Item "Out\\$ArchName\\Release\\D14UIKit.dll" -Destination "Dist\\cpp-$ArchName\\lib\\release" | Out-Null
    # example
    New-Item -Path "Dist\\cpp-$ArchName\\demo" -ItemType "directory" | Out-Null
    Copy-Item -Recurse -Path "Test\\*.cpp" -Destination "Dist\\cpp-$ArchName\\demo" | Out-Null
    Copy-Item -Recurse -Path "Test\\images" -Destination "Dist\\cpp-$ArchName\\demo\\images" | Out-Null
    # archive
    Compress-Archive -Force -Path "Dist\\cpp-$ArchName\\*" -DestinationPath "Dist\\d14uikit_cpp_$version-$ArchName.zip" | Out-Null
}
D14UIKit-Pack-Cpp -ArchName "x86"
D14UIKit-Pack-Cpp -ArchName "x64"

# Attempt to generate a stub file using mypy's stubgen.exe.
$executablePath = Get-Command "stubgen.exe" -ErrorAction SilentlyContinue
if ($executablePath)
{
    Set-Location -Path "Test\\PyBind"
    # Choose any library (must match the mypy version on the host).
    Copy-Item "..\\..\\Out\\x64\\RPy310\\D14UIKit.pyd" -Destination "." | Out-Null
    Start-Process -Wait -FilePath $executablePath -ArgumentList "-m D14UIKit -o ." | Out-Null
    Set-Location -Path "..\\.." # back
}
else # If stubgen.exe is not found, generate a dummy stub file instead.
{
    New-Item -Path "Test\\PyBind\\D14UIKit.pyi" -ItemType "file" | Out-Null
}

function D14UIKit-Pack-Python
{
    param($PyVerNum, $ArchName)
    # library
    New-Item -Path "Dist\\py$PyVerNum-$ArchName" -ItemType "directory" | Out-Null
    Copy-Item "Out\\$ArchName\\RPy$PyVerNum\\D14UIKit.pyd" -Destination "Dist\\py$PyVerNum-$ArchName" | Out-Null
    Copy-Item "Test\\PyBind\\D14UIKit.pyi" -Destination "Dist\\py$PyVerNum-$ArchName" | Out-Null
    # example
    New-Item -Path "Dist\\py$PyVerNum-$ArchName\\demo" -ItemType "directory" | Out-Null
    Copy-Item -Recurse -Path "Test\\PyBind\\*.py" -Destination "Dist\\py$PyVerNum-$ArchName\\demo" | Out-Null
    Copy-Item -Recurse -Path "Test\\images" -Destination "Dist\\py$PyVerNum-$ArchName\\demo\\images" | Out-Null
    # archive
    Compress-Archive -Force -Path "Dist\\py$PyVerNum-$ArchName\\*" -DestinationPath "Dist\\d14uikit_py$($PyVerNum)_$version-$ArchName.zip" | Out-Null
}
D14UIKit-Pack-Python -PyVerNum "310" -ArchName "x86"
D14UIKit-Pack-Python -PyVerNum "311" -ArchName "x86"
D14UIKit-Pack-Python -PyVerNum "312" -ArchName "x86"
D14UIKit-Pack-Python -PyVerNum "313" -ArchName "x86"

D14UIKit-Pack-Python -PyVerNum "310" -ArchName "x64"
D14UIKit-Pack-Python -PyVerNum "311" -ArchName "x64"
D14UIKit-Pack-Python -PyVerNum "312" -ArchName "x64"
D14UIKit-Pack-Python -PyVerNum "313" -ArchName "x64"
