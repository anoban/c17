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

$binary_name = $cfiles[0].Replace(".c", "-icx.exe")

$cflags = @( 
    "/debug:none",
    "/diagnostics:caret",
    "/D__STDC_WANT_SECURE_LIB",
    "/DNDEBUG",
    "/D_NDEBUG",
    "/EHa",
    "/F0x10485100",
    "-fcf-protection=full", # Enables shadow stack protection and endbranch (EB) generation
    "-flto=full", # lld-link will be used as linker instead of link.exe
    "/fp:fast",
    "/guard:cf",
    "/Gd", # Makes __cdecl the default calling convention
    "/GF",
    "/GR", # Enables C++ Runtime Type Information
    "/Gy", # Places each function in its own COMDAT section
    "/Gw", # Places each data item in its own COMDAT section
    "/O3",
    "/Oi",
    "/Ot",
    "/QaxCORE-AVX512,COMMON-AVX512",
    "/Qbranches-within-32B-boundaries",
    "/Qfma",
    "/Qfp-speculation:fast",
    "/Qftz", # Flushes denormal results to zero
    "/Qintrinsic-promote", # Enables functions containing calls to intrinsics that require a specific CPU feature to have their target
    # architecture automatically promoted to allow the required feature.
    "/Qipo", #  automatically sets option -fuse-ld=lld
    "/Qm64",
    "/Qopt-assume-no-loop-carried-dep=2",
    "/Qopt-dynamic-align-",
    "/Qopt-mem-layout-trans:2",
    "/Qopt-multiple-gather-scatter-by-shuffles",
    "/Qopt-streaming-stores:always",
    "/Qpc80",
    "/Qsox",
    "/Qstd:c2x",
    "/Qvec",
    "/Qvec-peel-loops",
    "/Qvec-threshold:0",
    "/Qunroll:1000",
    "/Qvec-with-mask",
    "/Qvec-remainder-loops",
    "/Qxx86-64-v4", # Generates code for Intel® Advanced Vector Extensions 512
    "/TP",
    "/tune:core-avx2",
    "/Wabi",
    "/Wall",
    "/Wcomment",
    "/Wdeprecated",
    "/wd4710", # not inlined
    "/wd4711", # selected for inlining
    "/wd4820", # struct padding
    "/Wformat",
    "/Wformat-security",
    "/Wmain",
    "/Wpointer-arith",
    "/Wreorder",
    "/Wreturn-type",
    "/Wshadow",
    "/Wsign-compare",
    "/Wstrict-aliasing",
    "/Wstrict-prototypes",
    "/Wtrigraphs",
    "/Wuninitialized",
    "/Wunknown-pragmas",
    "/Wunused-function",
    "/Wunused-variable",
    "/Wwrite-strings",
    # "/Zpx" # Specifies alignment for structures on byte boundaries (x)
    # /Zp1 will make the struct packed.
    "/Fe:${binary_name}"
)

Write-Host "icx.exe ${cfiles} ${cflags}" -ForegroundColor Cyan
icx.exe $cfiles $cflags    
Clear-Host

