# May god forgive whoever invented powershell.

Param(
        [Parameter(Mandatory = $true)]
        [ValidateSet('Build', 'Build-Tests', 'Invoke-Tests', 'Clear-Build')]
        [string]$Action
     )

function NoMake {
    Set-Vars
    Show-Vars

    Set-PSDebug -Trace 1
    Invoke-Expression "$Action"
}

function Build {
    New-Item -Force -ItemType Directory $OBJECTDIR
    foreach ($objectsource in $LIBSOURCES) {
        $target = Join-Path $OBJECTDIR ($objectsource.BaseName + ".o")
        Build-Object $objectsource $target
    }

    New-Item -Force -ItemType Directory $OUTPUTDIR
    foreach ($source in $SOURCES) {
        $target = Join-Path $OUTPUTDIR ($source.BaseName + $EXTENSION)
        Build-Program $source $target $OBJECTS
    }
}

function Build-Tests {
    New-Item -Force -ItemType Directory $OBJECTDIR
    foreach ($objectsource in $LIBSOURCES) {
        $target = Join-Path $OBJECTDIR ($objectsource.BaseName + ".o")
        Build-Object $objectsource $target
    }

    New-Item -Force -ItemType Directory $TESTOBJECTDIR
    foreach ($testobectsource in $TESTLIBSOURCES) {
        $target = Join-Path $TESTOBJECTDIR ($testobectsource.BaseName + ".o")
        Build-Object $testobectsource $target
    }

    New-Item -Force -ItemType Directory $TESTOUTPUTDIR
    foreach ($testsource in $TESTSOURCES) {
        $target = Join-Path $TESTOUTPUTDIR ($testsource.BaseName + $EXTENSION)
        Build-Program $testsource $target $OBJECTS,$TESTOBJECTS
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
    $global:CXX = "g++"

    $global:SOURCEDIR = "./src"
    $global:OUTPUTDIR = "./build"
    $global:OBJECTDIR = "$OUTPUTDIR/obj"
    $global:INCLUDEDIR = "./include"
    $global:LIBDIR = "./lib"

    $global:TESTSOURCEDIR = "./test"
    $global:TESTOUTPUTDIR = "./test/build"
    $global:TESTOBJECTDIR = "$TESTOUTPUTDIR/obj"
    $global:TESTLIBDIR = "./test/catch2"

    $global:WARNINGS = "all", "extra", "pedantic"

    $global:DEBUGFLAGS = "-g3", "-O0"

    $global:FFLAGS = "no-omit-frame-pointer"
    $global:STANDARD = "c++20"

    $global:EXTENSION = ".exe"
    $global:CXXFLAGS += "-static-libstdc++"
    $global:CXXFLAGS += " "

    $global:SOURCES = Get-ChildItem -Path $SOURCEDIR -Filter "*.cpp"
    $global:DEFAULTTARGETS = $SOURCES | ForEach-Object { Join-Path $OUTPUTDIR ($_.BaseName + $EXTENSION) }
    $global:LIBSOURCES = Get-ChildItem -Path $LIBDIR -Filter "*.cpp"
    $global:LIBHEADERS = Get-ChildItem -Path $INCLUDEDIR -Filter "*.hpp"
    $global:OBJECTS = $LIBSOURCES | ForEach-Object { Join-Path $OBJECTDIR ($_.BaseName + ".o") }

    $global:TESTSOURCES = Get-ChildItem -Path $TESTSOURCEDIR -Filter "*.cpp"
    $global:TESTTARGETS = $TESTSOURCES | ForEach-Object { Join-Path $TESTOUTPUTDIR ($_.BaseName + $EXTENSION) }
    $global:TESTLIBSOURCES = Get-ChildItem -Path $TESTLIBDIR -Filter "*.cpp"
    $global:TESTLIBHEADERS = Get-ChildItem -Path $TESTLIBDIR -Filter "*.hpp"
    $global:TESTOBJECTS = $TESTLIBSOURCES | ForEach-Object { Join-Path $TESTOBJECTDIR ($_.BaseName + ".o") }

    $global:CXXFLAGS += "-I$INCLUDEDIR", "-I$TESTLIBDIR"
    $global:CXXFLAGS += " "
    $global:CXXFLAGS += $WARNINGS | ForEach-Object { "-W$_" }
    $global:CXXFLAGS += " "
    $global:CXXFLAGS += $FFLAGS | ForEach-Object { "-f$_" }
    $global:CXXFLAGS += " "
    $global:CXXFLAGS += $DEBUGFLAGS
    $global:CXXFLAGS += " "
    $global:CXXFLAGS += "-std=$STANDARD"
    $global:CXXFLAGS += " "
}

function Show-Vars {
    Write-Host "CXXFLAGS: $CXXFLAGS"
    Write-Host "SOURCES: $SOURCES"
    Write-Host "DEFAULTTARGETS: $DEFAULTTARGETS"
    Write-Host "LIBSOURCES: $LIBSOURCES"
    Write-Host "LIBHEADERS: $LIBHEADERS"
    Write-Host "OBJECTS: $OBJECTS"
    Write-Host "TESTSOURCES: $TESTSOURCES"
    Write-Host "TESTTARGETS: $TESTTARGETS"
    Write-Host "TESTLIBSOURCES: $TESTLIBSOURCES"
    Write-Host "TESTLIBHEADERS: $TESTLIBHEADERS"
    Write-Host "TESTOBJECTS: $TESTOBJECTS"
    Write-Host "`n`n"
}

NoMake
