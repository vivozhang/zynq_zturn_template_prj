# read version and product information from MarkStat.mk
# product DLL version information file
#
# use:   gawk  -f DLLVersion.awk  -v OutDir=markstat\\Work   MarkStat.mk

#------------------------------------------------------------------------------
# initialize
BEGIN {

    IGNORECASE     = 0
    nerrors        = 0

    # set filenames with passed directory info
    if (OutDir == "")  OutDir = "."
    VersionFile    = OutDir"\\Version.inc"
    TargetBatFile  = OutDir"\\SetTarget.bat"
}


#------------------------------------------------------------------------------
/^[ \t]*PRODUCT_NAME[ \t=]+/  {
    if (!match($0, "^[ \t]*PRODUCT_NAME[ \t]*=[ \t]*"))
    {
        printf "ERROR: PRODUCT_NAME  (%s:%d)\n", FILENAME, NR                   > "/dev/stderr"
        ++nerrors
        next
    }
    ProductName = substr($0, RSTART+RLENGTH)
    sub(/[ \t]*$/, "", ProductName)
}


#------------------------------------------------------------------------------
/^[ \t]*VER_MAJOR[ \t=]+/  {
    if (!match($0, "^[ \t]*VER_MAJOR[ \t]*=[ \t]*"))
    {
        printf "ERROR: VER_MAJOR  (%s:%d)\n", FILENAME, NR                      > "/dev/stderr"
        ++nerrors
        next
    }
    VerMajor = substr($0, RSTART+RLENGTH)
    sub(/[ \t]*$/, "", VerMajor)
}


#------------------------------------------------------------------------------
/^[ \t]*VER_MINOR[ \t=]+/  {
    if (!match($0, "^[ \t]*VER_MINOR[ \t]*=[ \t]*"))
    {
        printf "ERROR: VER_MINOR  (%s:%d)\n", FILENAME, NR                      > "/dev/stderr"
        ++nerrors
        next
    }
    VerMinor = substr($0, RSTART+RLENGTH)
    sub(/[ \t]*$/, "", VerMinor)
}


#------------------------------------------------------------------------------
/^[ \t]*VER_PATCH[ \t=]+/  {
    if (!match($0, "^[ \t]*VER_PATCH[ \t]*=[ \t]*"))
    {
        printf "ERROR: VER_PATCH  (%s:%d)\n", FILENAME, NR                      > "/dev/stderr"
        ++nerrors
        next
    }
    VerPatch = substr($0, RSTART+RLENGTH)
    sub(/[ \t]*$/, "", VerPatch)
}


#------------------------------------------------------------------------------
/^[ \t]*STATUS[ \t=]+/  {
    if (!match($0, "^[ \t]*STATUS[ \t]*=[ \t]*"))
    {
        printf "ERROR: STATUS  (%s:%d)\n", FILENAME, NR                         > "/dev/stderr"
        ++nerrors
        next
    }
    VerStatus = substr($0, RSTART+RLENGTH)
    sub(/[ \t]*$/, "", VerStatus)
}


#------------------------------------------------------------------------------
# All done
END {

    if ( (VerMajor == "") || (VerMinor == "") || (VerPatch == "") || (VerStatus == "") )
    {
        printf "ERROR: Version %s.%s.%s%s\n", VerMajor, VerMinor, VerPatch, VerStatus    > "/dev/stderr"
        ++nerrors
    }
    if (ProductName == "")
    {
        printf "ERROR: missing product name\n"                                           > "/dev/stderr"
        ++nerrors
    }

    printf "#ifdef PRODUCTCODE\n"                                                        > VersionFile
    printf "const char ProductVersion[] = \"%s  V%s.%s.%s%s\";\n", ProductName, VerMajor, VerMinor, VerPatch, VerStatus  > VersionFile
    printf "#else  // resource file\n"                                                   > VersionFile
    printf "VALUE \"ProductName\",    \"%s\"\n", ProductName                             > VersionFile
    printf "VALUE \"ProductVersion\", \"V%s.%s.%s%s\"\n", VerMajor, VerMinor, VerPatch, VerStatus                        > VersionFile
    printf "#endif\n"                                                                    > VersionFile

    printf "set TargetName=%sDLL\n", ProductName                                         > TargetBatFile


    if (nerrors != 0) exit nerrors
}

