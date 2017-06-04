
# NumFile     - file keeping track of diagnostic symbol and number (DiagNum.txt)
# DiagDefFile - file to store diagnostic number table definitions (Diagnostic.def)


#------------------------------------------------------------------------------
# initialize
BEGIN {
    IGNORECASE   = 1
    nerrors      = 0
    BaseDiagNum  = 2400
    DiagNumMax   = 0

    if (NumFile == "") {
        print "error: NumFile is required"                                      > "/dev/stderr"
        exit 1
    }
    #if (DiagDefFile != "")
    #    printf "... saving diagnostic number table definitions in %s\n", DiagDefFile    > "/dev/stderr"

    LineNo     = 0
    while ((getline < NumFile) > 0) {
        ++LineNo

        # skip all lines without sufficient information
        if (NF < 2) continue

        # skip all lines that start with '#'
        if (match($0, /^[ \t]*#/)) continue

        # The cross ref file has a diagnostic number in the first field, and
        # the published name in the second field.  Verify that these are unique and
        # store them by name and number.
        if ($1 in DiagNumList) {
            printf "ERROR: duplicate diagnostic number %d  [%s:%d]\n", $1, NumFile, LineNo   > "/dev/stderr"
            ++nerrors
        }
        if ($2 in DiagNameList) {
            printf "ERROR: duplicate diagnostic name %s  [%s:%d]\n", $2, NumFile, LineNo     > "/dev/stderr"
            ++nerrors
        }
        if ($1 < BaseDiagNum) {
            printf "ERROR: invalid diagnostic number %d  [%s:%d]\n", $1, NumFile, LineNo     > "/dev/stderr"
            ++nerrors
        }
        if (0 && ($1 > DNUM_MAX)) {
            printf "ERROR: diagnostic number %d exceeds DNUM_MAX %d  [%s:%d]\n", $1, DNUM_MAX, NumFile, LineNo   > "/dev/stderr"
            ++nerrors
        }
        DiagNameList[$2] = $1
        DiagNumList[$1]  = $2

        if ($1 > DiagNumMax)  DiagNumMax = $1
    }
    close(NumFile)
    if (nerrors != 0) exit nerrors
}


#------------------------------------------------------------------------------
# watching for beginning of input files
{
    if (FNR==1) {
        print "... " FILENAME                                                   > "/dev/stderr"
        ++FileCnt
    }
}


#------------------------------------------------------------------------------
# ignore comment lines
/^\/\//  {
    next
}


#------------------------------------------------------------------------------
# ignore blank lines
/^[ \t]*$/  {
    next
}


#------------------------------------------------------------------------------
# add diagnostic number to definition file
# DIAG_ENTRY( Class, OrigClass, BaseName, PublishedName, C_Name, Uniquifier, Bridge )
/^[ \t]*DIAG_ENTRY[ \t]*\(/ {
    match($0, /^[ \t]*DIAG_ENTRY[ \t]*\([ \t]*/)
    ParseLine = substr($0, RSTART+RLENGTH)
    FldCnt = split(ParseLine, FldArray, /[ ]*,[ ]*/)
    DiagName = FldArray[4]

    # verify published name is unique
    if (DiagName in TmpNameList) {
        printf "ERROR: duplicate DIAG_ENTRY %s  [%s:%d]\n", DiagName, FILENAME, FNR   > "/dev/stderr"
        ++nerrors
        next
    }
    TmpNameList[DiagName] = 0

    # get number for diagnostic
    if (DiagName in DiagNameList) {
        DiagNum = DiagNameList[DiagName]
    } else {
        DiagNum = allocNum(DiagName)
    }

    # write to definition file
    if (DiagDefFile != "") {
        if (DiagNum in TmpNumList) {
            printf "ERROR: duplicate diagnostic number %d  [%s:%d]\n", DiagNum, FILENAME, FNR   > "/dev/stderr"
            ++nerrors
            next
        } else {
            printf "/*%4d*/ %s\n", DiagNum, $0                                 > DiagDefFile
            TmpNumList[DiagNum] = DiagName
        }
    }
}


#------------------------------------------------------------------------------
# All done
END {
    if (nerrors != 0) exit nerrors

    if (DiagDefFile != "") {
        # Fill in the spares
        for (Idx = BaseDiagNum; Idx <= DiagNumMax; ++Idx) {
            if (Idx in TmpNumList) continue
            printf "/*%4d*/ DIAG_ENTRY( , , DummyDiag, DummyDiag, DummyDiag, \"\", )\n", Idx            > DiagDefFile
        }
        # at least one line in the file
        printf "\n"                                                                           > DiagDefFile
    }

    # Update the cross reference file with the new diagnostic number
    AddNewLine = 1
    for (Idx in NewDiagList) {
        # Add a newline to the end of file just in case there's not one already there
        if (AddNewLine) {
            AddNewLine = 0
            printf "\n"                                                         >> NumFile
        }
        printf "%d  %s\n", Idx, NewDiagList[Idx]                                >> NumFile
    }

    if (nerrors != 0) exit nerrors

    if (CleanNumFile != "")
        CreateCleanNumFile()
}


# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
# =============================================================================
function allocNum (NewDiagName, tmpname, tmpnum) {
    # diagnostic already in NumFile, just an upper/lower case problem
    for (tmpname in DiagNameList) {
        if (tolower(NewDiagName) == tolower(tmpname)) {
            tmpnum = DiagNameList[tmpname]
            printf "Warning: U/L case diagnostic %s already exists as %s in %s\n", NewDiagName, tmpname, NumFile    > "/dev/stderr"
            ++nerrors
            return tmpnum
        }
    }

    # find first free diagnostic number
    for (tmpnum = BaseDiagNum; ; ++tmpnum) {
        if (tmpnum in DiagNumList) continue
        DiagNameList[NewDiagName] = tmpnum
        DiagNumList[tmpnum] = NewDiagName
        NewDiagList[tmpnum]  = NewDiagName
        if (tmpnum > DiagNumMax)  DiagNumMax = tmpnum
        return tmpnum
    }
}


# =============================================================================
# create a clean copy of NumFile, without the unused diagnostic
function CreateCleanNumFile() {
    printf "Creating a clean copy of %s in %s\n", NumFile, CleanNumFile         > "/dev/stderr"
    printf "\n"                                                                 > "/dev/stderr"
    printf "\n"                                                                 > CleanNumFile

    while ((getline TmpLine < NumFile) > 0) {
        # skip all blank lines and all lines that start with '#'
        if ( (match(TmpLine, "^[ \t]*$")) || (match(TmpLine, "^[ \t]*#")) )  {
            continue
        }
        # save only the diagnostic that are used
        FldCnt = split(TmpLine, FldArray, /[ \t,]+/)
        if (FldArray[2] in TmpNameList)  {
            print TmpLine                                                       >> CleanNumFile
        }
    }
    close(NumFile)
}
