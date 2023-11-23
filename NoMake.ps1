<#
.SYNOPSIS
Walmart version of make

.DESCRIPTION
Accepts a single parameter, which decides whether to compile, test or clean

.PARAMETER Action
The action to be performed by the script. Must be one of 'Build', 'Invoke-Tests', or 'Clear-Build'.
The correspending function will be called

.EXAMPLE
>  .\NoMake.ps1 -Action Build
# Compiles ./src into the ./build folder

.EXAMPLE
>  .\NoMake.ps1 -Action Invoke-Tests
# Builds and runs the unit tests

.EXAMPLE
>  .\NoMake.ps1 -Action Clear-Build
# Clears the programs' and tests' build folders
#>
Param(
        [Parameter(Mandatory=$true, HelpMessage="Action must be provided")]
        [ValidateSet('Build', 'Invoke-Tests', 'Clear-Build')]
        [string]$Action
     )

$CXX = "g++"
function NoMake {
    [CmdletBinding()]
    param()
    Set-Vars
    Set-PSDebug -Trace 1
    Invoke-Expression "$Action"
    Set-PSDebug -Trace 0
}

function Build {
    Build-Objects

    New-Item -Force -ItemType Directory $OUTPUTDIR
    foreach ($source in $SOURCES) {
        $target = Join-Path $OUTPUTDIR ($source.BaseName + $EXTENSION)
        Build-Program $source.FullName $target $OBJECTS
    }
}

function Build-Tests {
    Build-Objects

    New-Item -Force -ItemType Directory $TESTOBJECTDIR
    foreach ($testobectsource in $TESTLIBSOURCES) {
        $target = Join-Path $TESTOBJECTDIR ($testobectsource.BaseName + ".o")
        Build-Object $testobectsource.FullName $target
    }

    New-Item -Force -ItemType Directory $TESTOUTPUTDIR
    foreach ($testsource in $TESTSOURCES) {
        $target = Join-Path $TESTOUTPUTDIR ($testsource.BaseName + $EXTENSION)
        Build-Program $testsource.FullName $target $OBJECTS,$TESTOBJECTS
    }
}

function Build-Objects {
    New-Item -Force -ItemType Directory $OBJECTDIR
    foreach ($objectsource in $LIBSOURCES) {
        $target = Join-Path $OBJECTDIR ($objectsource.BaseName + ".o")
        Build-Object $objectsource.FullName $target
    }
}

function Invoke-Tests {
    Build-Tests
    foreach ($testprogram in $TESTTARGETS) {
        "$testprogram -sv high" | Invoke-Expression
    }
}

function Build-Object ($source, $target) {
    Write-Host "=== $source -> $target ==="
    "$CXX $CXXFLAGS -c $source -o $target" | Invoke-Expression
}

function Build-Program ($source, $target, $objects) {
    Write-Host "=== $source -> $target ==="
    "$CXX $CXXFLAGS $objects $source -o $target" | Invoke-Expression
}

function Clear-Build {
    Write-Host $OUTPUTDIR $TESTOUTPUTDIR
    Remove-Item -Recurse -Force -Path $OUTPUTDIR,$TESTOUTPUTDIR
}

function Set-Vars {
    $script:SOURCEDIR = "./src"
    $script:OUTPUTDIR = "./build"
    $script:OBJECTDIR = "$OUTPUTDIR/obj"
    $script:INCLUDEDIR = "./include"
    $script:LIBDIR = "./lib"

    $script:TESTSOURCEDIR = "./test"
    $script:TESTOUTPUTDIR = "./test/build"
    $script:TESTOBJECTDIR = "$TESTOUTPUTDIR/obj"
    $script:TESTLIBDIR = "./test/catch2"

    $script:WARNINGS = "all", "extra", "pedantic"
    $script:DEBUGFLAGS = "-g3", "-O0"
    $script:FFLAGS = "no-omit-frame-pointer"
    $script:STANDARD = "c++20"
    $script:EXTENSION = ".exe"

    $script:SOURCES = Get-ChildItem -Path $SOURCEDIR -Filter "*.cpp"
    $script:LIBSOURCES = Get-ChildItem -Path $LIBDIR -Filter "*.cpp"
    $script:OBJECTS = $LIBSOURCES | ForEach-Object { Join-Path $OBJECTDIR ($_.BaseName + ".o") }

    $script:TESTSOURCES = Get-ChildItem -Path $TESTSOURCEDIR -Filter "*.cpp"
    $script:TESTTARGETS = $TESTSOURCES | ForEach-Object { Join-Path $TESTOUTPUTDIR ($_.BaseName + $EXTENSION) }
    $script:TESTLIBSOURCES = Get-ChildItem -Path $TESTLIBDIR -Filter "*.cpp"
    $script:TESTOBJECTS = $TESTLIBSOURCES | ForEach-Object { Join-Path $TESTOBJECTDIR ($_.BaseName + ".o") }

    # TODO: write this in one line lol
    $script:CXXFLAGS += "-static-libstdc++ "
    $script:CXXFLAGS += "-I$INCLUDEDIR", "-I$TESTLIBDIR "
    $script:CXXFLAGS += $WARNINGS | ForEach-Object { "-W$_" }
    $script:CXXFLAGS += " "
    $script:CXXFLAGS += $FFLAGS | ForEach-Object { "-f$_" }
    $script:CXXFLAGS += " "
    $script:CXXFLAGS += $DEBUGFLAGS
    $script:CXXFLAGS += " "
    $script:CXXFLAGS += "-std=$STANDARD"
}

NoMake
