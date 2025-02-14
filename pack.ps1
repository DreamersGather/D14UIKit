param($version)

######################
# Clear Expired Dist #
######################

function D14UIKit-Clear-Expired
{
    param($DistName)
    if (Test-Path -Path "Dist\\$DistName")
    {
        Remove-Item -Recurse -Force "Dist\\$DistName" | Out-Null
    }
}
D14UIKit-Clear-Expired -DistName "cpp-x86"
D14UIKit-Clear-Expired -DistName "cpp-x64"

D14UIKit-Clear-Expired -DistName "python-x86"
D14UIKit-Clear-Expired -DistName "python-x64"

#####################
# Pack d14uikit_cpp #
#####################

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
    Copy-Item -Recurse -Path "Test\\images" -Destination "Dist\\cpp-$ArchName\\demo\\images" | Out-Null
    Copy-Item -Recurse -Path "Test\\sources" -Destination "Dist\\cpp-$ArchName\\demo\\sources" | Out-Null
    # archive
    Compress-Archive -Force -Path "Dist\\cpp-$ArchName\\*" -DestinationPath "Dist\\d14uikit_cpp_$version-$ArchName.zip" | Out-Null
}
D14UIKit-Pack-Cpp -ArchName "x86"
D14UIKit-Pack-Cpp -ArchName "x64"

########################
# Pack d14uikit_python #
########################

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

function D14UIKit-Copy-Python
{
    param($PyVerNum, $ArchName)
    # library
    New-Item -Path "Dist\\python-$ArchName\\py$PyVerNum-$ArchName" -ItemType "directory" | Out-Null
    Copy-Item "Out\\$ArchName\\RPy$PyVerNum\\D14UIKit.pyd" -Destination "Dist\\python-$ArchName\\py$PyVerNum-$ArchName" | Out-Null
    Copy-Item "Test\\PyBind\\D14UIKit.pyi" -Destination "Dist\\python-$ArchName\\py$PyVerNum-$ArchName" | Out-Null
    # example
    New-Item -Path "Dist\\python-$ArchName\\py$PyVerNum-$ArchName\\demo" -ItemType "directory" | Out-Null
    Copy-Item -Recurse -Path "Test\\PyBind\\images" -Destination "Dist\\python-$ArchName\\py$PyVerNum-$ArchName\\demo\\images" | Out-Null
    Copy-Item -Recurse -Path "Test\\PyBind\\scripts" -Destination "Dist\\python-$ArchName\\py$PyVerNum-$ArchName\\demo\\scripts" | Out-Null
}
function D14UIKit-Pack-Python
{
    param($ArchName)
    Compress-Archive -Force -Path "Dist\\python-$ArchName\\*" -DestinationPath "Dist\\d14uikit_python_$version-$ArchName.zip" | Out-Null
}
D14UIKit-Copy-Python -PyVerNum "310" -ArchName "x86"
D14UIKit-Copy-Python -PyVerNum "311" -ArchName "x86"
D14UIKit-Copy-Python -PyVerNum "312" -ArchName "x86"
D14UIKit-Copy-Python -PyVerNum "313" -ArchName "x86"

D14UIKit-Copy-Python -PyVerNum "310" -ArchName "x64"
D14UIKit-Copy-Python -PyVerNum "311" -ArchName "x64"
D14UIKit-Copy-Python -PyVerNum "312" -ArchName "x64"
D14UIKit-Copy-Python -PyVerNum "313" -ArchName "x64"

D14UIKit-Pack-Python -ArchName "x86"
D14UIKit-Pack-Python -ArchName "x64"
