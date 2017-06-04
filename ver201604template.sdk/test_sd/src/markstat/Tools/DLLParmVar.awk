# read auto definition files parameters, variables
# read supplemental information files (semicolon seperated) for parameters, variables, enumerations
# produce DLL Def files for parameters, variables


#------------------------------------------------------------------------------
# initialize
BEGIN {
    StartTime = systime()
    IGNORECASE   = 0
    FS           = "[ ]*;[ ]*"
    BracketLevel = 0
    nerrors      = 0

    # setup crossreference table for tre data types
    TreDataType["bool"]          = "BOOL"
    TreDataType["int"]           = "DINT"
    TreDataType["long"]          = "DINT"
    TreDataType["long int"]      = "DINT"
    TreDataType["int32_t"]       = "DINT"
    TreDataType["unsigned"]      = "UDINT"
    TreDataType["unsigned int"]  = "UDINT"
    TreDataType["unsigned long"] = "UDINT"
    TreDataType["uint32_t"]      = "UDINT"
    TreDataType["WORD"]          = "DWORD"
    TreDataType["float"]         = "REAL"
    TreDataType["void"]          = "ANY"
    TreDataType["complex"]       = "DWORD"

    # setup crossreference table for default values
    Default["bool"]              = "false"
    Default["int"]               = "0"
    Default["long"]              = "0"
    Default["long int"]          = "0"
    Default["int32_t"]           = "0"
    Default["unsigned"]          = "0"
    Default["unsigned int"]      = "0"
    Default["unsigned long"]     = "0"
    Default["uint32_t"]          = "0"
    Default["WORD"]              = "0"
    Default["float"]             = "0.0"

    # setup crossreference table for default pointer
    DefaultPtr["bool"]          = "UnusedBool"
    DefaultPtr["int"]           = "UnusedInt"
    DefaultPtr["long"]          = "UnusedInt"
    DefaultPtr["long int"]      = "UnusedInt"
    DefaultPtr["int32_t"]       = "UnusedInt"
    DefaultPtr["unsigned"]      = "UnusedUInt"
    DefaultPtr["unsigned int"]  = "UnusedUInt"
    DefaultPtr["unsigned long"] = "UnusedUInt"
    DefaultPtr["uint32_t"]      = "UnusedUInt"
    DefaultPtr["WORD"]          = "UnusedWord"
    DefaultPtr["float"]         = "UnusedFloat"
    DefaultPtr["void"]          = "UnusedUInt"

    if (OutDir == "")  OutDir = "."
    ParmDefFile    = OutDir"\\DLLParameter.def"
    LadrFile       = OutDir"\\VarLadr.h"

    VarLadr        = 0

    if (nerrors != 0) exit nerrors
}


#------------------------------------------------------------------------------
# watching for beginning of input files
{
    if (FNR == 1)
    {
#        print "... " FILENAME                                                   > "/dev/stderr"
        BracketLevel = 0
        FieldIndexValid = 0
    }
}


#------------------------------------------------------------------------------
# ignore comment lines
/^[ \t]*\/\//  {
    next
}


#------------------------------------------------------------------------------
# ignore blank lines
/^[ \t]*$/  {
    next
}


#------------------------------------------------------------------------------
# keep track of bracket level
/[{}]/  {
    BracketLevel += BracketLevelAdjust($0)
}


#------------------------------------------------------------------------------
# look for parameter definitions
# PARM_ENTRY( Class, OrigClass, BaseName, PublishedName, C_Name, Uniquifier, DataType, FinalValue )
/PARM_ENTRY/ {
    match($0, /^[ \t]*\/\*[ \t]*[[:digit:]]+[ \t]*\*\//)
    Ladr = substr($0, RSTART, RLENGTH)
    gsub(/[ \t\/\*]*/, "", Ladr)
    if (int(Ladr) > int(ParmNumMax)) ParmNumMax = Ladr

    match($0, /PARM_ENTRY[ \t]*\([ \t]*/)
    ParseLine = substr($0, RSTART+RLENGTH)
    match(ParseLine, /[ \t]*\)[^\)]*$/)
    ParseLine = substr(ParseLine, 1, RSTART-1)
    FldCnt = split(ParseLine, FldArray, /[ ]*,[ ]*/)

    # check for adequate field definitions
    if (FldCnt < 8)
    {
        printf "ERROR: inadequate field definitions  [%s:%d]\n", FILENAME, FNR   > "/dev/stderr"
        ++nerrors
        next
    }

    Class      = FldArray[1]
    OrigClass  = FldArray[2]
    BaseName   = FldArray[3]
    PubName    = FldArray[4]
    C_Name     = FldArray[5]
    Uniquifier = FldArray[6]
    DataType   = FldArray[7]
    FinalVal   = FldArray[8]
    gsub(/ *" */, "", Uniquifier)
    gsub(/ *\*/, "", DataType)
    if (match(FldArray[7], /\*/))
    {
        Pointer    = "yes"
        DefaultVal = DefaultPtr[DataType]

    }
    else
    {
        Pointer    = ""
        DefaultVal = Default[DataType]
    }

    #skip DummyParm
    if (PubName == "DummyParm")
        next

    # check for known data type
    if (!(DataType in TreDataType))
    {
        printf "ERROR: DataType %s for %s is not known in TreDataType  [%s:%d]\n", DataType, PubName, FILENAME, FNR   > "/dev/stderr"
        ++nerrors
        next
    }

    # check for known pointer data type
    if ((Pointer != "") && !(DataType in DefaultPtr))
    {
        printf "ERROR: Pointer DataType %s for %s is not known in DefaultPtr  [%s:%d]\n", DataType, PubName, FILENAME, FNR   > "/dev/stderr"
        ++nerrors
        next
    }

    # check for known data type
    if ((Pointer == "") && !(DataType in Default))
    {
        printf "ERROR: DataType %s for %s is not known in Default  [%s:%d]\n", DataType, PubName, FILENAME, FNR   > "/dev/stderr"
        ++nerrors
        next
    }

    # check for duplicate parameter entries
    if (PubName in ParmList)
    {
        printf "ERROR: Duplicate parameter entry for %s  [%s:%d]\n", PubName, FILENAME, FNR   > "/dev/stderr"
        ++nerrors
        next
    }

    # add parameter to list
    ParmList[PubName]        = sprintf("%s%c%s", OrigClass, SUBSEP, BaseName)
    if (!((OrigClass SUBSEP BaseName) in ParmInfo))
        ParmInfo[OrigClass,BaseName] = ""
    if ((Class != "") && (Uniquifier != ""))
        ParmUniquifier[PubName] = Uniquifier
    ParmCName[PubName] = C_Name

    # update parameter information
    # P_Class, P_BaseName, P_Description, P_DataType, P_Pointer, P_DefaultVal, P_Enum, P_Unit, P_HighLim, P_LowLim, P_FinalVal, P_ResetReq, P_CalcReq)
    UpdateParmInfo(OrigClass, BaseName, "", DataType, Pointer, DefaultVal, "", "", "", "", FinalVal)
}


#------------------------------------------------------------------------------
# look for variable definitions
# VAR_ENTRY( Class, OrigClass, BaseName, PublishedName, C_Name, Uniquifier, DataType, LadrDataType, ReadOnly )
/VAR_ENTRY/ {
    match($0, /^[ \t]*\/\*[ \t]*[[:digit:]]+[ \t]*\*\//)
    Ladr = substr($0, RSTART, RLENGTH)
    gsub(/[ \t\/\*]*/, "", Ladr)

    match($0, /VAR_ENTRY[ \t]*\([ \t]*/)
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
    DataType   = FldArray[7]
    gsub(/ *" */, "", Uniquifier)
    gsub(/ *\*/, "", DataType)

    #skip DummyVar
    if (PubName == "DummyVar")
    {
        VarLadr++
        next
    }

    # check for known data type
    if (!(DataType in TreDataType))
    {
        printf "ERROR: DataType %s for %s is not known in TreDataType  [%s:%d]\n", DataType, PubName, FILENAME, FNR   > "/dev/stderr"
        ++nerrors
        next
    }

    # check for duplicate variable entries
    if (PubName in VarList)
    {
        printf "ERROR: Duplicate variable entry for %s  [%s:%d]\n", PubName, FILENAME, FNR   > "/dev/stderr"
        ++nerrors
        next
    }

    # add variable to list
    VarList[PubName]        = sprintf("%s%c%s", OrigClass, SUBSEP, BaseName)
    if (!((OrigClass SUBSEP BaseName) in VarInfo))
        VarInfo[OrigClass,BaseName] = ""
    if ((Class != "") && (Uniquifier != ""))
        VarUniquifier[PubName] = Uniquifier
    VarLadrList[PubName]    = VarLadr++

    # update variable information
    # V_Class, V_BaseName, V_Description, V_DataType, V_Enum, V_Unit, V_Precision)
    UpdateVarInfo(OrigClass, BaseName, "", DataType)
}


#------------------------------------------------------------------------------
# look for parameter supplemental information
(match(tolower(FILENAME), /parm\.ssv/) != 0)  {
    if (FNR == 1)
    {
        SetFieldIdx()
        if (FldClass && FldBaseName && FldDesc && FldEnum && FldUnit && FldDflt && FldHLim && FldLLim && FldReset)
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

        # P_Class, P_BaseName, P_Description, P_DataType, P_Pointer, P_DefaultVal, P_Enum, P_Unit, P_HighLim, P_LowLim, P_FinalVal, P_ResetReq, P_CalcReq)
        UpdateParmInfo($(FldClass), $(FldBaseName), $(FldDesc), "", "", $(FldDflt), $(FldEnum), $(FldUnit), $(FldHLim), $(FldLLim), "", $(FldReset), "")

    }
}


#------------------------------------------------------------------------------
# look for variable supplemental information
(match(tolower(FILENAME), /var\.ssv/) != 0)  {
    if (FNR == 1)
    {
        SetFieldIdx()
        if (FldClass && FldBaseName && FldDesc && FldEnum && FldUnit && FldPrecision)
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

        # V_Class, V_BaseName, V_Description, V_DataType, V_Enum, V_Unit, V_Precision)
        UpdateVarInfo($(FldClass), $(FldBaseName), $(FldDesc), "", $(FldEnum), $(FldUnit), $(FldPrecision))

    }
}


#------------------------------------------------------------------------------
# look for enumeration
(match(tolower(FILENAME), /enum\.ssv/) != 0)  {
    if (FNR == 1)
    {
        EnumName    = ""
        EnumInfoCnt = 0

        next
    }

    # ignore lines with no field information, all fields empty
    if (match($0, /^[ \t;]*$/))
        next

    # new enumeration
    if ($1 != "")
    {
        EnumName = $1

        # replace previous enumeration
        if (EnumName in EnumList)
        {
            delete EnumIdent[EnumName]
            delete EnumDesc[EnumName]
        }

        TmpStr = $2
        for (Idx=3; (Idx <= NF) && ($Idx != ""); ++Idx)
            TmpStr = TmpStr "; " $Idx

        EnumList[EnumName] = TmpStr
        if (split(EnumList[EnumName], FldArray, /[ ]*;[ ]*/) == 0)
        {
            printf "ERROR: Enumeration %s has NO entries\n", EnumName           > "/dev/stderr"
            ++nerrors
        }
        EnumInfoCnt        = 1
    }
    # add eneumeration identifier
    else if (EnumInfoCnt == 1)
    {
        TmpStr = $2
        for (Idx=3; (Idx <= NF) && ($Idx != ""); ++Idx)
            TmpStr = TmpStr "; " $Idx

        EnumIdent[EnumName] = TmpStr
        if (split(EnumList[EnumName], FldArray, /[ ]*;[ ]*/) != split(EnumIdent[EnumName], FldArray, /[ ]*;[ ]*/))
        {
            printf "ERROR: Enumeration %s needs a one to one relationship between value and identifier\n", EnumName   > "/dev/stderr"
            ++nerrors
        }
        ++EnumInfoCnt
    }
    # add enumeration descriptions
    else if (EnumInfoCnt == 2)
    {
        TmpStr = $2
        for (Idx=3; (Idx <= NF); ++Idx)
            TmpStr = TmpStr "; " $Idx

        EnumDesc[EnumName] = TmpStr
        ++EnumInfoCnt
    }
}


#------------------------------------------------------------------------------
# everything else
{

    next
}


#------------------------------------------------------------------------------
# All done
END {

    CheckForErrors()

    PrintParmFile()
    PrintLadrFile()
#    PrintEnumSummary()


    if (nerrors != 0) exit nerrors
}


# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
# =============================================================================
# test for bracket level adjustment
function BracketLevelAdjust(TestLine)  {
    BracketAdjust = 0

    if (match(TestLine, /[{}]/))
    {
        # take care of comments
        if (match(TestLine, "//"))
            ParseLine = substr(TestLine, 1, RSTART-1)
        else
            ParseLine = TestLine

        TempStr = ParseLine
        while (match(TempStr, "{") > 0)
        {
            ++BracketAdjust
            TempStr = substr(TempStr, RSTART+1)
        }

        TempStr = ParseLine
        while (match(TempStr, "}") > 0)
        {
            --BracketAdjust
            TempStr = substr(TempStr, RSTART+1)
        }
    }

    return BracketAdjust
}


# =============================================================================
# update parameter supplemental information
function UpdateParmInfo(P_Class, P_BaseName, P_Description, P_DataType, P_Pointer, P_DefaultVal, P_Enum, P_Unit, P_HighLim, P_LowLim, P_FinalVal, P_ResetReq, P_CalcReq) {

    if (!((P_Class SUBSEP P_BaseName) in ParmInfo))
        return

    FldCnt = split(ParmInfo[P_Class,P_BaseName], FldArray, /[ ]*;[ ]*/)
    if (P_Description == "")  P_Description = FldArray[1]
    if (P_DataType == "")     P_DataType = FldArray[2]
    if (P_Pointer == "")      P_Pointer = FldArray[3]
    if (P_DefaultVal == "")   P_DefaultVal = FldArray[4]
    if (P_Enum == "")         P_Enum = FldArray[5]
    if (P_Unit == "")         P_Unit = FldArray[6]
    if (P_HighLim == "")      P_HighLim = FldArray[7]
    if (P_LowLim == "")       P_LowLim = FldArray[8]
    if (P_FinalVal == "")     P_FinalVal = FldArray[9]
    if (P_ResetReq == "")     P_ResetReq = FldArray[10]
    if (P_CalcReq == "")      P_CalcReq = FldArray[11]

    ParmInfo[P_Class,P_BaseName] = sprintf("%s; %s; %s; %s; %s; %s; %s; %s; %s; %s; %s",
                                   P_Description, P_DataType, P_Pointer, P_DefaultVal, P_Enum, P_Unit, P_HighLim, P_LowLim, P_FinalVal, P_ResetReq, P_CalcReq)
}


# =============================================================================
# update variable supplemental information
function UpdateVarInfo(V_Class, V_BaseName, V_Description, V_DataType, V_Enum, V_Unit, V_Precision) {

    if (!((V_Class SUBSEP V_BaseName) in VarInfo))
        return

    FldCnt = split(VarInfo[V_Class,V_BaseName], FldArray, /[ ]*;[ ]*/)
    if (V_Description == "")  V_Description = FldArray[1]
    if (V_DataType == "")     V_DataType = FldArray[2]
    if (V_Enum == "")         V_Enum = FldArray[3]
    if (V_Unit == "")         V_Unit = FldArray[4]
    if (V_Precision == "")    V_Precision = FldArray[5]

    VarInfo[V_Class,V_BaseName] = sprintf("%s; %s; %s; %s; %s", V_Description, V_DataType, V_Enum, V_Unit, V_Precision)
}


# =============================================================================
# determine field indexes
function SetFieldIdx() {
    FldClass     = 0
    FldBaseName  = 0
    FldDesc      = 0
    FldEnum      = 0
    FldUnit      = 0
    FldPrecision = 0
    FldDflt      = 0
    FldHLim      = 0
    FldLLim      = 0
    FldReset     = 0
    FldCalc      = 0

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
            case "enumeration":
                FldEnum = FldIdx
                break
            case "unit":
                FldUnit = FldIdx
                break
            case "precision":
                FldPrecision = FldIdx
                break
            case "defaultvalue":
                FldDflt = FldIdx
                break
            case "highlimit":
                FldHLim = FldIdx
                break
            case "lowlimit":
                FldLLim = FldIdx
                break
            case "resetrequired":
                FldReset = FldIdx
                break
            case "calcrequired":
                FldCalc = FldIdx
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
    for (TmpName in ParmList)
    {
        # check for short names that are too long
        if (length(TmpName) > 20)
        {
            printf "ERROR: Parameter name %s is too long\n", TmpName                > "/dev/stderr"
            ++nerrors
        }

        # P_Description, P_DataType, P_Pointer, P_DefaultVal, P_Enum, P_Unit, P_HighLim, P_LowLim, P_FinalVal, P_ResetReq, P_CalcReq
        FldCnt = split(ParmInfo[ParmList[TmpName]], FldArray, /[ ]*;[ ]*/)

        # check for available Uniquifier for descriptions using one
        if ((index(ParmList[TmpName], SUBSEP) > 1) && match(FldArray[1], /%R/) && !(TmpName in ParmUniquifier))
        {
            printf "ERROR: Uniquifier not defined for %s, from %s\n", TmpName, substr(ParmList[TmpName], 1, index(ParmList[TmpName], SUBSEP)-1)  > "/dev/stderr"
            printf "       or common class parameter can NOT have uniquifier\n"                                                                  > "/dev/stderr"
            ++nerrors
        }

        # check Default value for missed override
        if (match(FldArray[4], /\?/))
        {
            printf "ERROR: Override required on Default value for parameter %s\n", TmpName  > "/dev/stderr"
            ++nerrors
        }

        # check for Units that are too long
        if (length(FldArray[6]) > 6)
        {
            printf "ERROR: Units are too long for parameter %s\n", TmpName                  > "/dev/stderr"
            ++nerrors
        }

        # check High, Low limit for missed override
        if (match(FldArray[7], /\?/) || match(FldArray[8], /\?/))
        {
            printf "ERROR: Override required on Limits for parameter %s\n", TmpName         > "/dev/stderr"
            ++nerrors
        }

        # allow only 'Yes' and 'empty cell' for Reset Required flag
        if ( (tolower(FldArray[10]) != "yes") && (FldArray[10] != "") )
        {
            printf "ERROR: Reset Required flag requires (Yes) or (empty cell) for parameter %s\n", TmpName   > "/dev/stderr"
            ++nerrors
        }

        # allow only 'Yes' and 'empty cell' for Calc Required flag
        if ( (tolower(FldArray[11]) != "yes") && (FldArray[11] != "") )
        {
            printf "ERROR: Calc Required flag requires (Yes) or (empty cell) for parameter %s\n", TmpName   > "/dev/stderr"
            ++nerrors
        }
    }

    for (TmpName in VarList)
    {
        # check for short names that are too long
        if (length(TmpName) > 20)
        {
            printf "ERROR: Variable name %s is too long\n", TmpName                 > "/dev/stderr"
            ++nerrors
        }

        # V_Description, V_DataType, V_Enum, V_Unit, V_Precision
        FldCnt = split(VarInfo[VarList[TmpName]], FldArray, /[ ]*;[ ]*/)

        # check for available Uniquifier for descriptions using one
        if ((index(VarList[TmpName], SUBSEP) > 1) && match(FldArray[1], /%R/) && !(TmpName in VarUniquifier))
        {
            printf "ERROR: Uniquifier not defined for %s, from %s\n", TmpName, substr(VarList[TmpName], 1, index(VarList[TmpName], SUBSEP)-1)  > "/dev/stderr"
            printf "       or common class variable can NOT have uniquifier\n"                                                                 > "/dev/stderr"
            ++nerrors
        }

        # check for Units that are too long
        if (length(FldArray[4]) > 6)
        {
            printf "ERROR: Units are too long for variable %s\n", TmpName           > "/dev/stderr"
            ++nerrors
        }

        # Precision is only allowed for floats
        if ( (FldArray[5] != "") && (FldArray[2] != "float") )
        {
            printf "ERROR: Precision is Only allowed for float variables %s\n", TmpName       > "/dev/stderr"
            ++nerrors
        }

        # Precision is not allowed for enumerated variables
        if ( (FldArray[5] != "") && (FldArray[3] != "") )
        {
            printf "ERROR: Precision is Not allowed for enumerated variables %s\n", TmpName   > "/dev/stderr"
            ++nerrors
        }

        # allow only 0 to 8 and 'empty cell' for Precision
        if ( ((FldArray[5] < 0) || (FldArray[5] > 8)) && (FldArray[5] != "") )
        {
            printf "ERROR: Valid Precision values are 0 to 8 or (empty cell) for variable %s\n", TmpName   > "/dev/stderr"
            ++nerrors
        }
    }
}


# =============================================================================
# write parameter def file
function PrintParmFile() {

    if (ParmDefFile == "")
    {
        printf "ERROR  ParmDefFile not defined\n"                                       > "/dev/stderr"
        ++nerrors
        return
    }

    for (TmpName in ParmList)
    {

        # P_Description, P_DataType, P_Pointer, P_DefaultVal, P_Enum, P_Unit, P_HighLim, P_LowLim, P_FinalVal, P_ResetReq, P_CalcReq
        FldCnt = split(ParmInfo[ParmList[TmpName]], FldArray, /[ ]*;[ ]*/)
        TmpDescription = FldArray[1]
        TmpDataType    = FldArray[2]
        if (FldArray[3] == "")
            TmpPointer = ""
        else
            TmpPointer = "p"
        TmpDefaultVal  = FldArray[4]
        TmpEnum        = FldArray[5]
        TmpUnit        = FldArray[6]
        TmpHighLim     = FldArray[7]
        TmpLowLim      = FldArray[8]
        TmpFinalVal    = FldArray[9]
        TmpResetReq    = FldArray[10]
        TmpCalcReq     = "yes"  #FldArray[11]

        gsub(/ */, "", TmpDefaultVal)

        if (TmpFinalVal != "")                              TmpDefaultVal = "0"
        else if ( (TmpPointer == "") && (TmpEnum != "") )   TmpDefaultVal = FindEnum(TmpEnum, TmpDefaultVal)

        if (match(TmpDefaultVal, /[<>]+/))                  TmpDefaultVal = "0"

        if (TmpDefaultVal == "")   printf "Blank Dflt: %s  Enum:%s\n", TmpName, TmpEnum      > "/dev/stderr"

        if ((TmpPointer != "") && (match(TmpDataType, /WORD/)))
            TmpDataType = "unsigned"

        if (TmpName in ParmUniquifier)
        {
            TmpUniquifier = ParmUniquifier[TmpName]
            gsub(/&/, "\\\\&", TmpUniquifier)
            gsub(/%R/, TmpUniquifier, TmpDescription)
        }

        printf "PARM_DLL_ENTRY("                                                        > ParmDefFile
        printf "%s",   TmpName                                                          > ParmDefFile
        printf ", %s", ParmCName[TmpName]                                               > ParmDefFile
        printf ", %s%s", TmpPointer, TreDataType[TmpDataType]                           > ParmDefFile
        printf ", %s", TmpDefaultVal                                                    > ParmDefFile
        printf ")\n"                                                                    > ParmDefFile

    }
}


# =============================================================================
# write logical address to symbol definitions file
function PrintLadrFile() {

    if (LadrFile == "")
    {
        printf "ERROR  LadrFile not defined\n"                                          > "/dev/stderr"
        ++nerrors
        return
    }

        FldCnt = split(LadrFile, FldArray, /[\\\/]/)
        LadrFileDefine = toupper(FldArray[FldCnt])
        gsub(/["\\\/ ]*/, "", LadrFileDefine)
        gsub(/\./, "_", LadrFileDefine)
        printf "#ifndef  %s\n", LadrFileDefine                                  > LadrFile
        printf "#define  %s\n", LadrFileDefine                                  > LadrFile
        printf "/////////////\n\n"                                              > LadrFile

        for (TmpName in VarLadrList)
        {
            printf "#define   LADR_%s  %d\n", TmpName, VarLadrList[TmpName]     > LadrFile
        }
#        for (Idx in TmpLadrList) {
#            printf "#define   LADR_%s  %d\n", TmpLadrList[Idx], Idx             > LadrFile
#        }

        printf "\n/////////////\n"                                              > LadrFile
        printf "#endif  // %s\n", LadrFileDefine                                > LadrFile
}


# =============================================================================
function FindEnum(EnumName, EnumEntry) {
    Value = "0"

    if (!(EnumName in EnumList))
    {
        printf "ERROR: enumeration %s is NOT defined\n", EnumName                       > "/dev/stderr"
        ++nerrors
        return  Value
    }

    vFldCnt = split(EnumList[EnumName], vFldArray, /[ ]*;[ ]*/)
    iFldCnt = split(EnumIdent[EnumName], iFldArray, /[ ]*;[ ]*/)

    for (Idx=0; Idx <= vFldCnt; ++Idx)
    {
        if ( ( (iFldArray[Idx] != "") && (iFldArray[Idx] == EnumEntry) )  ||
             ( (vFldArray[Idx] != "") && (vFldArray[Idx] == EnumEntry) ) )
        {
            Value = vFldArray[Idx]
        }
    }

    return Value
}


# =============================================================================
function PrintEnumSummary() {
    for (TmpName in EnumList)
    {
        printf "%s: %s\n", TmpName, EnumList[TmpName]                                   > "/dev/stderr"
        printf "   %s\n", EnumIdent[TmpName]                                            > "/dev/stderr"
        printf "   %s\n", EnumDesc[TmpName]                                             > "/dev/stderr"
    }
}

