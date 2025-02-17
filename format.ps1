function D14UIKit-Run-SortXML
{
    param($XMLFilePath)
    Write-Output "Format $XMLFilePath"
    & ".\\sortxml.exe" "--overwrite" $XMLFilePath | Out-Null
}
$executablePath = Get-Command "csc.exe" -ErrorAction SilentlyContinue
if ($executablePath)
{
    # Compile sortxml
    & $executablePath "sortxml.cs" | Out-Null

    # Format D14UIKit
    # D14UIKit-Run-SortXML -XMLFilePath "D14UIKit.vcxproj"
    # D14UIKit-Run-SortXML -XMLFilePath "D14UIKit.vcxproj.filters"
    # D14UIKit-Run-SortXML -XMLFilePath "D14UIKit.vcxproj.user"

    # Format TestCpp
    # D14UIKit-Run-SortXML -XMLFilePath "Test\\TestCpp.vcxproj"
    # D14UIKit-Run-SortXML -XMLFilePath "Test\\TestCpp.vcxproj.filters"
    # D14UIKit-Run-SortXML -XMLFilePath "Test\\TestCpp.vcxproj.user"

    # Format TestPyBind
    # D14UIKit-Run-SortXML -XMLFilePath "Test\\PyBind\\TestPyBind.pyproj"
    # D14UIKit-Run-SortXML -XMLFilePath "Test\\PyBind\\TestPyBind.pyproj.user"
}
else # Keep everything unchanged.
{
    Write-Output "Warning: csc.exe not found, no changes have been made."
}
