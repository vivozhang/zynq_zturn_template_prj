# the NumFile is not be needed since information is if Diagnostic.def

# read auto definition file for diagnostics
# (jtb) read class instance file - showing class instance prefix and suffix
# read supplemental information files (semicolon seperated) for diagnostics
# (jtb) read supplemental prefixes information file (Prefixes.ssv)
# produce attribute definitions file for diagnostics
# produce DLL Def file for diagnostics
# (ras&jtb) modify from VITO to CatEnum (in progess)

#------------------------------------------------------------------------------
# initialize
BEGIN {
    StartTime = systime()
    IGNORECASE   = 0
    FS           = "[ ]*;[ ]*"
    nerrors      = 0
    BaseDiagNum  = 2400
    DiagNumMax   = 0

    NONCLASSCATNUMSTART = 1
    CLASSCATNUMSTART    = 1000
    CLASS_SIZE          = 10
    NonClassCatNum      = NONCLASSCATNUMSTART
    ClassCatNum         = CLASSCATNUMSTART

    # set filenames with passed directory info
    if (OutDir == "")  OutDir = "."
    DiagAttribFile = OutDir"\\DiagAttrib.def"
    DiagDefFile    = OutDir"\\DLLDiagnostic.def"

    if (nerrors != 0) exit nerrors
}


#------------------------------------------------------------------------------
# watching for beginning of input files
{
    if (FNR==1) {
        print "Processing " FILENAME                                                   > "/dev/stderr"
        ++FileCnt
        FieldIndexValid = 0
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
# look for diagnostic definitions
# DIAG_ENTRY( Class, OrigClass, BaseName, PublishedName, C_Name, Uniquifier, Bridge )
/DIAG_ENTRY/ {
    match($0, /^[ \t]*\/\*[ \t]*[[:digit:]]+[ \t]*\*\//)
    Dnum = substr($0, RSTART, RLENGTH)
    gsub(/[ \t\/\*]*/, "", Dnum)

    match($0, /DIAG_ENTRY[ \t]*\([ \t]*/)
    ParseLine = substr($0, RSTART+RLENGTH)
    match(ParseLine, /[ \t]*\)[^\)]*$/)
    ParseLine = substr(ParseLine, 1, RSTART-1)
    FldCnt = split(ParseLine, FldArray, /[ ]*,[ ]*/)

    # check for adequate field definitions
    if (FldCnt < 7)
    {
        printf "ERROR: inadequate field definitions  [%s:%d]\n", FILENAME, FNR   > "/dev/stderr"
        ++nerrors
        next
    }

    Class      = FldArray[1]
    OrigClass  = FldArray[2]
    BaseName   = FldArray[3]
    PubName    = FldArray[4]
    Uniquifier = FldArray[6]
    Bridge     = FldArray[7]
    gsub(/ *" */, "", Uniquifier)

    #skip DummyDiag
    if (PubName == "DummyDiag")
        next

    # check for duplicate diagnostic entries
    if (PubName in DiagList)
    {
        printf "ERROR: Duplicate diagnostic entry for %s  [%s:%d]\n", PubName, FILENAME, FNR   > "/dev/stderr"
        ++nerrors
        next
    }

    if (Dnum < BaseDiagNum)
    {
        printf "ERROR: invalid diagnostic number %d  [%s:%d]\n", Dnum, FILENAME, FNR     > "/dev/stderr"
        ++nerrors
    }

    # add diagnostic to list
    DiagList[PubName]            = sprintf("%s%c%s", OrigClass, SUBSEP, BaseName)
    DiagInfo[OrigClass,BaseName] = ""
    DiagNameList[PubName]        = Dnum
    DiagNumList[Dnum]            = PubName
    if (Bridge != "")            BridgeOrideList[PubName] = Bridge
    if (DiagNumMax < Dnum)       DiagNumMax = Dnum
    if ((Class != "") && (Uniquifier != ""))
        DiagUniquifier[PubName] = Uniquifier

}


#------------------------------------------------------------------------------
# look for diagnostic supplemental information
(match(tolower(FILENAME), /diag\.ssv/) != 0)  {
    if (FNR == 1)
    {
        SetFieldIdx()
        if (FldClass && FldBaseName && FldDesc && FldInit && FldType && FldLocked && FldBridge1 && FldBridge2 && FldCatDesc && FldCatNum)
        {
            FieldIndexValid = 1
        }
        else
        {
            FieldIndexValid = 0
            printf "ERROR: invalid column header, required field missing  [%s:%d]\n", FILENAME, FNR   > "/dev/stderr"
            ++nerrors
        }
        next
    }

    if (FieldIndexValid)
    {
        if ($(FldBaseName) == "")
            next

        # D_Class, D_BaseName, D_Desc, D_Init, D_Type, D_Locked, D_Bridge1, D_Bridge2, D_CatDesc, D_CatNum
                UpdateDiagInfo($(FldClass), $(FldBaseName), $(FldDesc), $(FldInit), $(FldType), $(FldLocked), $(FldBridge1), $(FldBridge2), $(FldCatDesc), $(FldCatNum))

    }
}


#------------------------------------------------------------------------------
# look for prefix information 
(match(tolower(FILENAME), /prefixes\.ssv/) != 0) {
	DiagOffset[tolower($1)] = $2
}


#------------------------------------------------------------------------------
# everything else
{

    next
}


#------------------------------------------------------------------------------
# All done
END {

    printf "Creating   " DiagAttribFile  "\n"                                   > "/dev/stderr"
    CheckForErrors()

    if (nerrors == 0)
    {
        PrintAttribFile()
        PrintDiagFile()
    }

    if (nerrors != 0) exit nerrors
}


# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

# =============================================================================
# update diagnostic supplemental information
function UpdateDiagInfo(D_Class, D_BaseName, D_Desc, D_Init, D_Type, D_Locked, D_Bridge1, D_Bridge2, D_CatDesc, D_CatNum) {

    if (!((D_Class SUBSEP D_BaseName) in DiagInfo))
        return

    FldCnt = split(DiagInfo[D_Class,D_BaseName], FldArray, /[ ]*;[ ]*/)
    if (D_Desc == "")     D_Desc    = FldArray[1]
    if (D_Init == "")     D_Init    = FldArray[2]
    if (D_Type == "")     D_Type    = FldArray[3]
    if (D_Locked == "")   D_Locked  = FldArray[4]
    if (D_Bridge1 == "")  D_Bridge1 = FldArray[5]
    if (D_Bridge2 == "")  D_Bridge2 = FldArray[6]
    if (D_CatDesc == "")  D_CatDesc = FldArray[7]
    if (D_CatNum == "")   D_CatNum  = FldArray[8]
    if ( (D_CatNum > NonClassCatNum) && (D_CatNum < CLASSCATNUMSTART) ) 
    {
        NonClassCatNum = D_CatNum
    }
    if (D_CatNum > ClassCatNum)
    {
        ClassCatNum = (D_CatNum - (D_CatNum%CLASS_SIZE) + (CLASS_SIZE-1))
    }
    DiagInfo[D_Class,D_BaseName] = sprintf("%s; %s; %s; %s; %s; %s; %s; %s",
                                   D_Desc, D_Init, D_Type, D_Locked, D_Bridge1, D_Bridge2, D_CatDesc, D_CatNum)
}


# =============================================================================
# determine field indexes
function SetFieldIdx(FldIdx) {
    FldClass     = 0
    FldBaseName  = 0
    FldDesc      = 0
    FldInit      = 0
    FldType      = 0
    FldLocked    = 0
    FldBridge1   = 0
    FldBridge2   = 0
    FldCatDesc   = 0
    FldCatNum    = 0

    for (FldIdx=1; FldIdx<=NF; ++FldIdx)
    {
        switch (tolower($(FldIdx)))
        {
            case "class":
                FldClass = FldIdx
                break
            case "name":
                FldBaseName = FldIdx
                break
            case "description":
                FldDesc = FldIdx
                break
            case "init":
                FldInit = FldIdx
                break
            case "type":
                FldType = FldIdx
                break
            case "locked":
                FldLocked = FldIdx
                break
            case "bridge 1":
                FldBridge1 = FldIdx
                break
            case "bridge 2":
                FldBridge2 = FldIdx
                break
            case "categories":
                FldCatDesc = FldIdx
                break
            case "enumerations":
                FldCatNum = FldIdx
                break
            case "":
                break
            default:
                #break
        }
    }
}


# =============================================================================
function CheckForErrors() {
    for (TmpName in DiagList)
    {
        # D_Desc, D_Init, D_Type, D_Locked, D_Bridge1, D_Bridge2, D_CatDesc, D_CatNum)
        FldCnt = split(DiagInfo[DiagList[TmpName]], FldArray, /[ ]*;[ ]*/)

        # check for missing discription
        if (FldArray[1] == "")
        {
            printf "ERROR: Description missing for diagnostic %s\n", TmpName                           > "/dev/stderr"
            ++nerrors
        }

        # check for available Uniquifier for descriptions using one
        if ((index(DiagList[TmpName], SUBSEP) > 1) && match(FldArray[1], /%R/) && !(TmpName in DiagUniquifier))
        {
            printf "ERROR: Uniquifier not defined for %s, from %s\n", TmpName, substr(DiagList[TmpName], 1, index(DiagList[TmpName], SUBSEP)-1)  > "/dev/stderr"
            ++nerrors
        }

        # allow only 'Yes' and 'empty cell' for Init flag
        if ( (tolower(FldArray[2]) != "yes") && (FldArray[2] != "") )
        {
            printf "ERROR: Init flag requires 'Yes' or 'empty cell' for diagnostic %s\n", TmpName      > "/dev/stderr"
            ++nerrors
        }

        # allow only 'Trip', 'Alarm', 'SOE' for Type
        if ( (tolower(FldArray[3]) != "trip") && (tolower(FldArray[3]) != "alarm")  && (tolower(FldArray[3]) != "soe") )
        {
            printf "ERROR: Type requires 'Trip', 'Alarm', 'SOE' for diagnostic %s\n", TmpName      > "/dev/stderr"
            ++nerrors
        }

        # allow only Type 'Trip' with an enumeration > 0
        if ( (tolower(FldArray[3]) == "trip") && (FldArray[8] == 0) )
        {
            printf "ERROR: Type 'Trip' requires an integer enumeration > 0 for diagnostic %s.\n", TmpName    > "/dev/stderr"
            ++nerrors
        }

        # allow only non-Type 'Trip' with a zero enumeration
        if ( (tolower(FldArray[3]) != "trip") && (FldArray[8] > 0) )
        {
            printf "ERROR: All 'Alarms' and 'SOEs' must have zero enumeration for diagnostic %s.\n", TmpName > "/dev/stderr"
            ++nerrors
        }

        # allow only 'Yes' and 'empty cell' for Locked flag
        if ( (tolower(FldArray[4]) != "yes") && (FldArray[4] != "") )
        {
            printf "ERROR: Locked flag requires 'Yes' or 'empty cell' for diagnostic %s\n", TmpName    > "/dev/stderr"
            ++nerrors
        }

        # check Bridge flags for missed override
        if (match(FldArray[5], /\?/) || match(FldArray[6], /\?/))
        {
            if (!(TmpName in BridgeOrideList))
            {
                printf "ERROR: Class Override required, on Bridge selection, for diagnostic %s\n", TmpName   > "/dev/stderr"
                printf "       or common diagnostic can NOT be overriden\n"                                  > "/dev/stderr"
                ++nerrors
            }
        }
        else
        {
            # allow only 'Yes' and 'empty cell' for Bridge 1 flag
            if ( (tolower(FldArray[5]) != "yes") && (FldArray[5] != "") )
            {
                printf "ERROR: Bridge 1 flag requires 'Yes' or 'empty cell' for diagnostic %s\n", TmpName  > "/dev/stderr"
                ++nerrors
            }

            # allow only 'Yes' and 'empty cell' for Bridge 2 flag
            if ( (tolower(FldArray[6]) != "yes") && (FldArray[6] != "") )
            {
                printf "ERROR: Bridge 2 flag requires 'Yes' or 'empty cell' for diagnostic %s\n", TmpName  > "/dev/stderr"
                ++nerrors
            }
        }

        if (FldArray[8] < 0)
        {
            printf "ERROR: Category Enumeration must be >= 0 for diagnostic %s.\n", TmpName                  > "/dev/stderr"
            ++nerrors
        }
        if (match(FldArray[8],/[[:alpha:]]/) > 0)
        {
            printf "ERROR: Category Enumeration must be an integer for diagnostic %s.\n", TmpName            > "/dev/stderr"
            ++nerrors
        }
    }

    #Validate non-class enumerations do not overlap with class enumerations
    if (NonClassCatNum >= CLASSCATNUMSTART)
    {
        printf "ERROR:  Non-class enumeration maximum must < 1000"                                           > "/dev/stderr"
        ++nerrors
    }

    if (ClassCatNum < CLASSCATNUMSTART)
    {
        printf "ERROR:  Class enumeration maximum must >= 1000"                                              > "/dev/stderr"
        ++nerrors
    }
}


# =============================================================================
# write diagnostic attribute file
function PrintAttribFile() {

    if (DiagAttribFile == "")
    {
        printf "ERROR  DiagAttribFile not defined\n"                            > "/dev/stderr"
        ++nerrors
        return
    }

    printf "////////////////////////////// \n"                                  > DiagAttribFile
    printf "#ifdef  DIAG_DEFINITIONS       \n"                                  > DiagAttribFile
    printf "////////////////////////////// \n"                                  > DiagAttribFile
    printf "\n"                                                                 > DiagAttribFile
    printf "#define ATTRIB_INIT      0x00010000 \n"                             > DiagAttribFile
    printf "#define ATTRIB_TRIP      0x00020000 \n"                             > DiagAttribFile
    printf "#define ATTRIB_SOE       0x00040000 \n"                             > DiagAttribFile
    printf "#define ATTRIB_LOCKED    0x00080000 \n"                             > DiagAttribFile
    printf "#define ATTRIB_BRIDGE1   0x00100000 \n"                             > DiagAttribFile
    printf "#define ATTRIB_BRIDGE2   0x00200000 \n"                             > DiagAttribFile
    for (i = NONCLASSCATNUMSTART; i <= NonClassCatNum; i++)
        printf "#define ATTRIB_ENUM%04s  0x%08X \n", i, i                       > DiagAttribFile
    for (i = CLASSCATNUMSTART; i <= ClassCatNum; i++)
        printf "#define ATTRIB_ENUM%04s  0x%08X \n", i, i                       > DiagAttribFile
    printf "\n"                                                                 > DiagAttribFile
    printf "////////////////////////////// \n"                                  > DiagAttribFile
    printf "#endif  // DIAG_DEFINITIONS    \n"                                  > DiagAttribFile
    printf "#ifdef  DIAG_ATTRIBUTES        \n"                                  > DiagAttribFile
    printf "////////////////////////////// \n"                                  > DiagAttribFile
    printf "\n"                                                                 > DiagAttribFile

    for (Idx = BaseDiagNum; Idx <= DiagNumMax; ++Idx)
    {
        if (!(Idx in DiagNumList))
        {
            printf "/* %4d  %-20s */  0,\n", Idx, ""                            > DiagAttribFile
            continue
        }

        TmpName = DiagNumList[Idx]

        # D_Desc, D_Init, D_Type, D_Locked, D_Bridge1, D_Bridge2, D_CatDesc, D_CatNum
        FldCnt = split(DiagInfo[DiagList[TmpName]], FldArray, /[ ]*;[ ]*/)

        TmpDesc     = FldArray[1]
        TmpInit     = FldArray[2]
        TmpType     = FldArray[3]
        TmpLocked   = FldArray[4]
        TmpBridge1  = FldArray[5]
        TmpBridge2  = FldArray[6]
        TmpCatDesc  = FldArray[7]
        TmpCatNum   = FldArray[8]

        # override bridge settings
        if (TmpName in BridgeOrideList)
        {
            if (match(tolower(BridgeOrideList[TmpName]), /bridge_*(1|1&2)/))
                TmpBridge1 = "yes"
            else
                TmpBridge1 = ""

            if (match(tolower(BridgeOrideList[TmpName]), /bridge_*(2|1&2)/))
                TmpBridge2 = "yes"
            else
                TmpBridge2 = ""
        }


        printf "/* %4d  %-20s */  0", Idx, TmpName                              > DiagAttribFile
        if (TmpInit != "")     printf " | ATTRIB_INIT"                          > DiagAttribFile
        if (tolower(TmpType) == "trip") printf " | ATTRIB_TRIP"                 > DiagAttribFile
        if (tolower(TmpType) == "soe")  printf " | ATTRIB_SOE"                  > DiagAttribFile
        if (TmpLocked != "")   printf " | ATTRIB_LOCKED"                        > DiagAttribFile
        if (TmpBridge1 != "")  printf " | ATTRIB_BRIDGE1"                       > DiagAttribFile
        if (TmpBridge2 != "")  printf " | ATTRIB_BRIDGE2"                       > DiagAttribFile
        TmpOffset = 0
        if ((TmpCatNum >= CLASSCATNUMSTART) && (TmpCatNum%CLASS_SIZE == 0) && (tolower(TmpType) == "trip")) 
        {
            if (match(TmpName, "_") > 0) 
            {
                TmpPrefix = tolower(substr(TmpName, 1, RSTART))
                if (TmpPrefix in DiagOffset)
                {
                    TmpOffset = DiagOffset[TmpPrefix]
                }
            }
        }
        TmpCatNum +=TmpOffset
        if (TmpCatNum > 0)    printf " | ATTRIB_ENUM%04s", TmpCatNum                > DiagAttribFile		
        printf ",\n"                                                            > DiagAttribFile
    }
    printf "\n"                                                                 > DiagAttribFile
    printf "////////////////////////////// \n"                                  > DiagAttribFile
    printf "#endif  // DIAG_ATTRIBUTES     \n"                                  > DiagAttribFile
    printf "////////////////////////////// \n"                                  > DiagAttribFile


}


# =============================================================================
# write diagnostic def file
function PrintDiagFile() {

    if (DiagDefFile == "")
    {
        printf "ERROR  DiagDefFile not defined\n"                               > "/dev/stderr"
        ++nerrors
        return
    }

    for (Idx = BaseDiagNum; Idx <= DiagNumMax; ++Idx)
    {
        if (Idx in DiagNumList)
        {
            TmpName = DiagNumList[Idx]

            # D_Desc, D_Init, D_Type, D_Locked, D_Bridge1, D_Bridge2, D_CatDesc, D_CatNum
            FldCnt = split(DiagInfo[DiagList[TmpName]], FldArray, /[ ]*;[ ]*/)

            TmpDesc     = FldArray[1]
            TmpInit     = FldArray[2]
            TmpType     = FldArray[3]
            TmpLocked   = FldArray[4]
            TmpBridge1  = FldArray[5]
            TmpBridge2  = FldArray[6]
            TmpCatDesc  = FldArray[7]
            TmpCatNum   = FldArray[8]

            if (TmpName in DiagUniquifier)
            {
                TmpUniquifier = DiagUniquifier[TmpName]
                gsub(/&/, "\\\\&", TmpUniquifier)
                gsub(/%R/, TmpUniquifier, TmpDescription)
            }
        }
        else
        {
            TmpName        = ""
            TmpDescription = ""
        }

        printf "/*%4d*/ ", Idx                                                  > DiagDefFile
        printf "DIAG_DLL_ENTRY("                                                > DiagDefFile
        printf "\"%s\"",   TmpName                                              > DiagDefFile
        printf ", \"%s\"", TmpDescription                                       > DiagDefFile
        if (tolower(TmpType) == "alarm")      printf ", \"ALARM\""              > DiagDefFile
        else if (tolower(TmpType) == "trip")  printf ", \"TRIP\""               > DiagDefFile
        else if (tolower(TmpType) == "soe")   printf ", \"SOE\""                > DiagDefFile
        else                                  printf ", \"\""                   > DiagDefFile
        printf ")\n"                                                            > DiagDefFile

    }

}
