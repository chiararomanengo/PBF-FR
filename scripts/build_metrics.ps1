# Save current directory
Push-Location

# Get the directory where this script is located
$scriptDir = $PSScriptRoot

# Define paths
$sourceDir = Join-Path $scriptDir "/../src/metrics"
$buildDir = Join-Path $scriptDir "/../build/metrics"

$binDir = Join-Path $scriptDir "/../bin"

# Create the build and install directories if they don't exist
if (-not (Test-Path $buildDir)) {
    New-Item -ItemType Directory -Path $buildDir | Out-Null
}
if (-not (Test-Path $binDir)) {
    New-Item -ItemType Directory -Path $binDir | Out-Null
}

# Change to the build directory
Set-Location $buildDir

# Run CMake configuration with install prefix
cmake -DCMAKE_RUNTIME_OUTPUT_DIRECTORY="$binDir" `
      -DCMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE="$binDir" `
       $sourceDir

# Build the project
cmake --build . --config Release

# Return to the original directory
Pop-Location