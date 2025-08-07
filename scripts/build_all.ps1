# Save current directory
Push-Location

# Get the directory where this script is located
$scriptDir = $PSScriptRoot
$thisScript = $MyInvocation.MyCommand.Definition

# Find all build_*.ps1 scripts, excluding this one
$scripts = Get-ChildItem -Path $scriptDir -Filter "build_*.ps1" -File |
    Where-Object { $_.FullName -ne $thisScript } |
    Sort-Object Name

# Run each script
foreach ($script in $scripts) {
    Write-Host " Running $($script.Name)..."
    & "$($script.FullName)"
    if ($LASTEXITCODE -ne 0) {
        Write-Host "$($script.Name) failed with exit code $LASTEXITCODE" -ForegroundColor Red
        Pop-Location
        exit $LASTEXITCODE
    }
    Write-Host "Completed $($script.Name)" -ForegroundColor Green
}

# Return to the original directory
Pop-Location