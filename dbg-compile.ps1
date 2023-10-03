$cfiles = [System.Collections.ArrayList]::new()
$unrecognized = [System.Collections.ArrayList]::new()


foreach ($arg in $args) {
    if ($arg -clike "*.c") {
        [void]$cfiles.Add($arg.ToString().Replace(".\", ""))
    }
    else {
        [void]$unrecognized.Add($arg)
    }
}

if ($unrecognized.Count -ne 0) {
    Write-Error "Incompatible files passed for compilation: ${unrecognized}"
    Exit 1
}

$cflags = @( 
    "/arch:AVX512",
    "/diagnostics:caret",
    "/DDEBUG",
    "/D_DEBUG",
    "/F0x10485100",
    "/favor:INTEL64",
    "/fp:strict",
    "/fpcvt:IA",
    "/Gw",
    "/MP",
    "/Od",
    "/Qpar",
    "/std:c17",
    "/TC",
    "/Wall",
    "/Zc:preprocessor",
    "/link /DEBUG:FULL"
)

Write-Host "cl.exe ${cfiles} ${cflags}" -ForegroundColor Cyan
cl.exe $cfiles $cflags    

foreach($file in $cfiles){
    Remove-Item $file.Replace(".c", ".obj") -Force
}
