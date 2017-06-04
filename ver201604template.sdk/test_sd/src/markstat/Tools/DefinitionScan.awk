# scan .cpp .h or .ppx files looking for
# parameter, variable, diagnostic, data passing definitions
# class instances


#------------------------------------------------------------------------------
# initialize
BEGIN {
    StartTime      = systime()
    IGNORECASE     = 0
    BracketLevel   = 0
    FileCnt        = 0
    nerrors        = 0

    # reset counters, indexes
    SrcFileName    = ""
    ClassIdx       = 0
    ClassName      = ""
    mClassName     = ""
    ParmIdx        = 0
    VarIdx         = 0
    DiagIdx        = 0
    PassIdx        = 0

    ClassCnt       = 0
    InstanceCnt    = 0

    # setup crossreference table for valid data types
    # ValidDataType["bool"]          = ""
    ValidDataType["int"]           = ""
    ValidDataType["long"]          = ""
    ValidDataType["long int"]      = ""
    ValidDataType["int32_t"]       = ""
    ValidDataType["unsigned"]      = ""
    ValidDataType["unsigned int"]  = ""
    ValidDataType["unsigned long"] = ""
    ValidDataType["uint32_t"]      = ""
    ValidDataType["WORD"]          = ""
    ValidDataType["float"]         = ""
    ValidDataType["void"]          = ""

    # setup crossreference table for logical address data types
    LadrDataType["bool"]          = "BLN"
    LadrDataType["int"]           = "INT"
    LadrDataType["long"]          = "INT"
    LadrDataType["long int"]      = "INT"
    LadrDataType["int32_t"]       = "INT"
    LadrDataType["unsigned"]      = "UINT"
    LadrDataType["unsigned int"]  = "UINT"
    LadrDataType["unsigned long"] = "UINT"
    LadrDataType["uint32_t"]      = "UINT"
    LadrDataType["WORD"]          = "MEM"
    LadrDataType["float"]         = "FPT"
    LadrDataType["complex"]       = "MEM"

    # setup recognized datapass transfer types in specific order
    PassXferType[1]               = "T1_T2"
    PassXferType[2]               = "T1A_T2"
    PassXferType[3]               = "T1_T2_F"
    PassXferType[4]               = "T1A_T2_F"
    PassXferType[5]               = "T2_T3"
    PassXferType[6]               = "T2_T3_F"
    PassXferType[7]               = "T1_T3"
    PassXferType[8]               = "T1A_T3"
    PassXferType[9]               = "T1_T3_F"
    PassXferType[10]              = "T1A_T3_F"
    PassXferType[11]              = "T3_T2"
    PassXferType[12]              = "T2_T1"
    PassXferType[13]              = "T3_T1"

    # set filenames with passed directory info
    if (OutDir == "")  OutDir = "."
    ParmDefFile    = OutDir"\\Parameter.def"
    ParmExternFile = OutDir"\\ParmExtern.h"
    VarDefFile     = OutDir"\\Variable.def"
    VarExternFile  = OutDir"\\VarExtern.h"
    DiagDefFile    = OutDir"\\Diagnostic.def"
    DiagExternFile = OutDir"\\DiagExtern.h"
    PassDefFile    = OutDir"\\DataPass.def"
    PassExternFile = OutDir"\\DataPassExtern.h"
    InstanceFile   = OutDir"\\ClassInstance.ssv"
}


#------------------------------------------------------------------------------
# watching for beginning of input files
{
    if (FNR==1)
    {
        ParseLine = substr($0, RSTART, RLENGTH)
        FldCnt = split(FILENAME, FldArray, /[\\\/]/)
        SrcFileName = FldArray[FldCnt]
        gsub(/["\\\/ ]*/, "", SrcFileName)
        #print "... " SrcFileName                                                 > "/dev/stderr"

        BracketLevel = 0
        ++FileCnt

        # reset flags at the beginning of every file
        ClassDef       = 0
        DupClassDef    = 0

        while (ClassIdx > 0)
        {
            printf "ERROR: class %s never closed\n", ClassName                   > "/dev/stderr"
            PopClass()
            ++nerrors
        }
    }
}


#------------------------------------------------------------------------------
# look for source file in a ppx extension
/^[ \t]*\#[(line) \t]+[[:digit:]]+[ \t]+\"[[:alnum:]_\\\/\:\. ]+\"/ && (match(tolower(FILENAME), /\.ppx/) != 0) {
    match($0, /".*"/)
    ParseLine = substr($0, RSTART, RLENGTH)
    FldCnt = split(ParseLine, FldArray, /[\\\/]/)
    SrcFileName = FldArray[FldCnt]
    gsub(/["\\\/ ]*/, "", SrcFileName)
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
# keep track of bracket level
/[{}]/  {
    BracketLevel += BracketLevelAdjust($0)
}


#------------------------------------------------------------------------------
# look for parameter definitions
/^[ \t]*(CREATE_PARM|CREATE_OUR_PARM)[ \t]*\([ \t]*[[:alnum:]_]+[ \t]*,[ \t]*[[:alnum:]_ ]+[\*]*[ \t]*\)/ && !DupClassDef {

    # find parameter name
    match($0, /(CREATE_PARM|CREATE_OUR_PARM)[ \t]*\([ \t]*/)
    ParseLine = substr($0, RSTART+RLENGTH)
    match(ParseLine, /^[ \t]*[[:alnum:]_]+[ \t]*,/)
    Name = substr(ParseLine, RSTART, RLENGTH)
    gsub(/[ \t,]*/, "", Name)

    #find parameter datatype
    ParseLine = substr(ParseLine, RSTART+RLENGTH)
    match(ParseLine, /^[ \t]*[[:alnum:]_ \*]+[ \t]*\)/)
    Type = substr(ParseLine, RSTART, RLENGTH)
    gsub(/^[ \t]*/, "", Type)
    gsub(/[ \t\)]*$/, "", Type)
    gsub(/\*/, " *", Type)
    gsub(/[ ]+/, " ", Type)

    # check for valid data type
    if (!DataTypeIsValid(Type))
    {
        printf "ERROR: invalid datatype %s for %s\n", Type, Name                > "/dev/stderr"
        ++nerrors
    }

    # determine scope
    if      ((ClassName == "") && match($0, /^[ \t]*CREATE_PARM/))      Scope = "GP"
    else if ((ClassName != "") && match($0, /^[ \t]*CREATE_OUR_PARM/))  Scope = "CP"
    else if ((ClassName != "") && match($0, /^[ \t]*CREATE_PARM/))      Scope = "IP"
    else
    {
        Scope = ""
        printf "ERROR: invalid parameter definition %s  [%s]\n", Name, SrcFileName   > "/dev/stderr"
        ++nerrors
    }

    ++ParmIdx
    ParmList[ClassName, ParmIdx] = sprintf("%s; %s; %s; %s", ClassName, Name, Type, Scope)
    ClassAutomation[ClassName] = 2

    next
}


#------------------------------------------------------------------------------
# look for variable definitions
/^[ \t]*(CREATE_PUBVAR|CREATE_OUR_PUBVAR)[ \t]*\([ \t]*[[:alnum:]_]+[ \t]*,[ \t]*[[:alnum:]_ ]+[ \t]*\)/ && !DupClassDef {

    # find variable name
    match($0, /(CREATE_PUBVAR|CREATE_OUR_PUBVAR)[ \t]*\([ \t]*/)
    ParseLine = substr($0, RSTART+RLENGTH)
    match(ParseLine, /^[ \t]*[[:alnum:]_]+[ \t]*,/)
    Name = substr(ParseLine, RSTART, RLENGTH)
    gsub(/[ \t,]*/, "", Name)

    #find variable datatype
    ParseLine = substr(ParseLine, RSTART+RLENGTH)
    match(ParseLine, /^[ \t]*[[:alnum:]_ ]+[ \t]*\)/)
    Type = substr(ParseLine, RSTART, RLENGTH)
    gsub(/^[ \t]*/, "", Type)
    gsub(/[ \t\)]*$/, "", Type)
    gsub(/[ ]+/, " ", Type)

    # check for valid data type
    if (!DataTypeIsValid(Type))
    {
        printf "ERROR: invalid datatype %s for %s\n", Type, Name                > "/dev/stderr"
        ++nerrors
    }

    # determine scope
    if      ((ClassName == "") && match($0, /^[ \t]*CREATE_PUBVAR/))       Scope = "GV"
    else if ((ClassName != "") && match($0, /^[ \t]*CREATE_OUR_PUBVAR/))   Scope = "CV"
    else if ((ClassName != "") && match($0, /^[ \t]*CREATE_PUBVAR/))       Scope = "IV"
    else
    {
        Scope = ""
        printf "ERROR: invalid variable definition %s  [%s]\n", Name, SrcFileName   > "/dev/stderr"
        ++nerrors
    }

    ++VarIdx
    VarList[ClassName, VarIdx] = sprintf("%s; %s; %s; %s", ClassName, Name, Type, Scope)
    ClassAutomation[ClassName] = 2

    if (GlobalVarList[ClassName, Name] == "")
        GlobalVarList[ClassName, Name] = Type

    next
}


#------------------------------------------------------------------------------
# look for complex variable definitions
/^[ \t]*(CREATE_COMPLEX_PUBVAR)[ \t]*\([ \t]*[[:alnum:]_]+[ \t]*,[ \t]*[[:alnum:]_ ]+[ \t]*\)/ && !DupClassDef {

    # find variable name
    match($0, /(CREATE_COMPLEX_PUBVAR)[ \t]*\([ \t]*/)
    ParseLine = substr($0, RSTART+RLENGTH)
    match(ParseLine, /^[ \t]*[[:alnum:]_]+[ \t]*,/)
    Name = substr(ParseLine, RSTART, RLENGTH)
    gsub(/[ \t,]*/, "", Name)

    #find variable datatype
    ParseLine = substr(ParseLine, RSTART+RLENGTH)
    match(ParseLine, /^[ \t]*[[:alnum:]_ ]+[ \t]*\)/)
    Type = substr(ParseLine, RSTART, RLENGTH)
    gsub(/^[ \t]*/, "", Type)
    gsub(/[ \t\)]*$/, "", Type)
    gsub(/[ ]+/, " ", Type)

    # determine scope
    if      ((ClassName == "") && match($0, /^[ \t]*CREATE_COMPLEX_PUBVAR/))       Scope = "GV"
    else if ((ClassName != "") && match($0, /^[ \t]*CREATE_COMPLEX_PUBVAR/))       Scope = "IV"
    else
    {
        Scope = ""
        printf "ERROR: invalid variable definition %s  [%s]\n", Name, SrcFileName   > "/dev/stderr"
        ++nerrors
    }

    ++VarIdx
    VarList[ClassName, VarIdx] = sprintf("%s; %s; %s; %s", ClassName, Name, "complex", Scope)
    ClassAutomation[ClassName] = 2

    if (GlobalVarList[ClassName, Name] == "")
        GlobalVarList[ClassName, Name] = Type

    next
}


#------------------------------------------------------------------------------
# look for diagnostic definitions
/^[ \t]*(CREATE_DIAG|CREATE_OUR_DIAG)[ \t]*\([ \t]*[[:alnum:]_]+[ \t]*\)/ && !DupClassDef {

    # find diagnostic name
    match($0, /(CREATE_DIAG|CREATE_OUR_DIAG)[ \t]*\([ \t]*/)
    ParseLine = substr($0, RSTART+RLENGTH)
    match(ParseLine, /^[ \t]*[[:alnum:]_]+[ \t]*\)/)
    Name = substr(ParseLine, RSTART, RLENGTH)
    gsub(/^[ \t]*/, "", Name)
    gsub(/[ \t\)]*$/, "", Name)

    # determine scope
    if      ((ClassName == "") && match($0, /^[ \t]*CREATE_DIAG/))      Scope = "GD"
    else if ((ClassName != "") && match($0, /^[ \t]*CREATE_OUR_DIAG/))  Scope = "CD"
    else if ((ClassName != "") && match($0, /^[ \t]*CREATE_DIAG/))      Scope = "ID"
    else
    {
        Scope = ""
        printf "ERROR: invalid diagnostic definition %s  [%s]\n", Name, SrcFileName   > "/dev/stderr"
        ++nerrors
    }

    ++DiagIdx
    DiagList[ClassName, DiagIdx] = sprintf("%s; %s; %s", ClassName, Name, Scope)
    ClassAutomation[ClassName] = 2

    next
}


#------------------------------------------------------------------------------
# look for data passing definitions
/^[ \t]*DATA_PASS[ \t]*\([ \t]*[[:alnum:]_\.\(\)\[\]]+[ \t]*,[ \t]*[[:alnum:]_\.\(\)\[\]]+[ \t]*,[ \t]*[[:alnum:]_]+[ \t]*,[ \t]*[[:alpha:]]+[ \t]*\)/ && !DupClassDef {

    # find source variable name
    match($0, /DATA_PASS[ \t]*\([ \t]*/)
    ParseLine = substr($0, RSTART+RLENGTH)
    match(ParseLine, /^[ \t]*[[:alnum:]_\.\(\)\[\]]+[ \t]*,/)
    SrcName = substr(ParseLine, RSTART, RLENGTH)
    gsub(/[ \t,]*/, "", SrcName)

    # find destination variable name
    ParseLine = substr(ParseLine, RSTART+RLENGTH)
    match(ParseLine, /^[ \t]*[[:alnum:]_\.\(\)\[\]]+[ \t]*,/)
    DstName = substr(ParseLine, RSTART, RLENGTH)
    gsub(/[ \t,]*/, "", DstName)

    # find transfer type
    ParseLine = substr(ParseLine, RSTART+RLENGTH)
    match(ParseLine, /^[ \t]*[[:alnum:]_]+[ \t]*,/)
    Type = substr(ParseLine, RSTART, RLENGTH)
    gsub(/[ \t,]*/, "", Type)

    # find filtered
    ParseLine = substr(ParseLine, RSTART+RLENGTH)
    match(ParseLine, /^[ \t]*[[:alnum:]_]+[ \t]*\)/)
    Filter = substr(ParseLine, RSTART, RLENGTH)
    gsub(/[ \t\)]*/, "", Filter)

    # substitution for OUT macro
    if (ClassName != "")
    {
        if (match(SrcName, /OUT *\(/))
        {
            sub(/OUT *\( */, "", SrcName)
            sub(/ *\) */, "", SrcName)
        }
        if (match(DstName, /OUT *\(/))
        {
            sub(/OUT *\( */, "", DstName)
            sub(/ *\) */, "", DstName)
        }
    }


    # check for valid transfer type
    if ( ((tolower(Filter) == "filtered") && PassTypeIsValid(Type "_F")) || ((tolower(Filter) != "filtered") && PassTypeIsValid(Type)) )
    {
        ++PassIdx
        PassList[ClassName, PassIdx] = sprintf("%s; %s; %s; %s; %s", ClassName, SrcName, DstName, Type, Filter)
        if (ClassAutomation[ClassName] < 1)
            ClassAutomation[ClassName] = 1
    }
    else
    {
        ParseLine = $0
        sub(/^ */, "", ParseLine)
        if (PassTypeIsValid(Type))
            printf "ERROR: invalid filtering on transfer type (%s) for %s  [%s]\n", Type, ParseLine, SrcFileName   > "/dev/stderr"
        else
            printf "ERROR: invalid datapass transfer type (%s) for %s  [%s]\n", Type, ParseLine, SrcFileName   > "/dev/stderr"
        ++nerrors
    }

    next
}


#------------------------------------------------------------------------------
# look for final values
/SET_FV[ \t]*\([ \t]*[[:alnum:]_]+[ \t]*,/ && !DupClassDef {

    # find final value name
    match($0, /SET_FV[ \t]*\([ \t]*/)
    ParseLine = substr($0, RSTART+RLENGTH)
    match(ParseLine, /^[ \t]*[[:alnum:]_]+[ \t]*,/)
    Name = substr(ParseLine, RSTART, RLENGTH)
    ParseLine = substr(ParseLine, RSTART+RLENGTH)
    gsub(/[ \t,]*/, "", Name)

    FV_List[mClassName, Name] = ""

    # look for physical pointers
    if (match(ParseLine, /[^\&]\&[ \t]*[[:alnum:]_]+/))
    {
        printf "ERROR: SET_FV for %s uses a physical pointer  [%s]\n", Name, SrcFileName   > "/dev/stderr"
        ++nerrors
    }

    next
}


#------------------------------------------------------------------------------
# look for class instance Uniquifier and Bridge override
/^[ \t]*INSTANCE_INFO[ \t]*\([ \t]*[[:alnum:]_]+[ \t]*,[ \t]*\"[^\"]*\"[ \t]*,[ \t]*[[:alnum:]_&]+[ \t]*,[ \t]*[[:alnum:]]+[ \t]*\)/ && !DupClassDef {

    # find class instance
    match($0, /INSTANCE_INFO[ \t]*\([ \t]*/)
    ParseLine = substr($0, RSTART+RLENGTH)
    match(ParseLine, /^[ \t]*[[:alnum:]_]+[ \t]*,/)
    cInstance = substr(ParseLine, RSTART, RLENGTH)
    gsub(/[ \t,]*/, "", cInstance)

    #find uniquifier
    ParseLine = substr(ParseLine, RSTART+RLENGTH)
    match(ParseLine, /\"[^\"]*\"/)
    Uniquifier = substr(ParseLine, RSTART, RLENGTH)
    gsub(/[ \t]*\"[ \t]*/, "\"", Uniquifier)
    match(ParseLine, /\"[^\"]*\"[ \t]*,/)

    # find bridge override
    ParseLine = substr(ParseLine, RSTART+RLENGTH)
    match(ParseLine, /^[ \t]*[[:alnum:]_\&]+[ \t]*,/)
    BridgeOride = substr(ParseLine, RSTART, RLENGTH)
    gsub(/[ \t,]*/, "", BridgeOride)

    # find Task 1
    ParseLine = substr(ParseLine, RSTART+RLENGTH)
    match(ParseLine, /\)/)
    Task1 = substr(ParseLine, 1, RSTART)
    gsub(/[ \t,\)]*/, "", Task1)

    # check for valid uniquifier
    if (match(Uniquifier, /[,;]/))
    {
        printf "ERROR: invalid character found in uniquifier (%s) in INSTANCE_INFO for %s  [%s]\n", Uniquifier, cInstance, SrcFileName   > "/dev/stderr"
        ++nerrors
    }

    # check for valid bridge override
    if (!match(tolower(BridgeOride), /^bridge_*(none|1|2|1&2)$/) && !match(tolower(BridgeOride), /^use_xls$/))
    {
        printf "ERROR: invalid bridge override (%s) in INSTANCE_INFO for %s  [%s]\n", BridgeOride, cInstance, SrcFileName   > "/dev/stderr"
        ++nerrors
    }
    if (tolower(BridgeOride) == "use_xls")
        BridgeOride = ""

    # check for valid Task 1 declaration
    if (!match(tolower(Task1), /^task1a*$/))
    {
        printf "ERROR: invalid Task 1 declaration (%s) in INSTANCE_INFO for %s  [%s]\n", Task1, cInstance, SrcFileName   > "/dev/stderr"
        ++nerrors
    }

    InstanceModifier[cInstance] = sprintf("%s; %s; %s", Uniquifier, BridgeOride, Task1)

    next
}


#------------------------------------------------------------------------------
# look for keywords that should already be handled
/(CREATE_PARM|CREATE_OUR_PARM|CREATE_PUBVAR|CREATE_OUR_PUBVAR|CREATE_COMPLEX_PUBVAR|CREATE_DIAG|CREATE_OUR_DIAG|DATA_PASS|SET_FV|INSTANCE_INFO)/ && !DupClassDef  {
    printf "ERROR: keyword NOT handled  [%s] %s\n", SrcFileName, $0   > "/dev/stderr"
    ++nerrors
}


#------------------------------------------------------------------------------
# look for class instance
/^[ \t]*[[:alnum:]_]+[ \t]+[[:alnum:]_]+([ \t]*\([^\(\)]*\))*([ \t]*,[ \t]*[[:alnum:]_]+([ \t]*\([^\(\)]*\))*)*[ \t]*;/ && ((BracketLevel == 0) || ($1 == "static") || (ClassName != "")) && !DupClassDef {

    sub(/(^| |\t)+volatile[ \t]+/, " ")
    sub(/(^| |\t)+static[ \t]+/, " ")
    sub(/(^| |\t)+const[ \t]+/, " ")
    sub(/[ \t]*\*[ \t]*/, " * ")

    if ( ($1 != "float") && ($1 != "double") && ($1 != "unsigned") && ($1 != "int") && ($1 != "short") && ($1 != "long") &&
         ($1 != "char") && ($1 != "bool") &&  ($1 != "int32_t") && ($1 != "uint32_t") && ($1 != "WORD") &&
         ($1 != "struct") && ($1 != "union") && ($1 != "enum") && ($1 != "extern") && ($1 != "class") &&
         ($1 != "delete") && ($1 != "return") )
    {

        #find class name
        match($0, /^[ \t]*[[:alnum:]_]+[ \t]+/)
        cName = substr($0, RSTART, RLENGTH)
        gsub(/[ \t]*/, "", cName)

        # find index in list
        Idx = 1
        while (InstanceList[cName, Idx] != "")
            ++Idx

        # find instance name
        ParseLine = substr($0, RSTART+RLENGTH)
        while (match(ParseLine, /^[ \t]*[[:alnum:]_]+([ \t]*\([^\(\)]*\))*[ \t]*(,|;)/))
        {
            Name = substr(ParseLine, RSTART, RLENGTH)
            gsub(/[ \t,;]*/, "", Name)
            gsub(/\(.*\)/, "", Name)
            ParseLine = substr(ParseLine, RSTART+RLENGTH)

            # determine prefix and suffix for instance
            TmpStr = cName
            gsub(/^c*/, "", TmpStr)
            if (match(Name, TmpStr))
            {
                Prefix = substr(Name, 1, RSTART-1)
                Suffix = substr(Name, RSTART+RLENGTH)
            }
            else if (ClassAutomation[cName] >= 2)
            {
                Prefix = ""
                Suffix = ""
                printf "ERROR: invalid naming for class instance %s  [%s]\n", Name, SrcFileName   > "/dev/stderr"
                ++nerrors
            }
            else
            {
                Prefix = ""
                Suffix = ""
            }

            InstanceList[cName, Idx] = sprintf("%s; %s; %s", Name, Prefix, Suffix)
            InstanceToClass[Name]    = cName
            ++InstanceCnt
            ++Idx
        }

        next
    }
}


#------------------------------------------------------------------------------
# look for class definitions
/^[ \t]*class[ \t]+[[:alnum:]_]+( *\: *(private|protected|public)? *[[:alnum:]_]+)?[ \t{]*$/, /^[ \t]*}[ \t]*;*[ \t]*$/ && (BracketLevel == ClassDefLevel) {

    # beginning of class definition
    if (match($0, /^[ \t]*class[ \t]+/) && !ClassDef)
    {
        # find class name
        ParseLine = substr($0, RSTART+RLENGTH)
        match(ParseLine, /[[:alnum:]_]+/)
        Name = substr(ParseLine, RSTART, RLENGTH)

        # find base class name
        cBaseName = ""
        ParseLine = substr(ParseLine, RSTART+RLENGTH)
        if (match(ParseLine, / *\: *(private|protected|public)? *[[:alnum:]_]+/) )
        {
            ParseLine = substr(ParseLine, RSTART, RLENGTH)
            match(ParseLine, / *\: *(private|protected|public)? */)
            cBaseName = substr(ParseLine, RSTART+RLENGTH)
            BaseClassList[Name] = cBaseName
        }

        ClassDef = 1

        # second definition of class
        if (Name in ClassList)
        {
            DupClassDef = 1
            NewClass("DupClassDef")
        }
        #first definition of class
        else
        {
            ++ClassCnt
            ClassList[Name] = SrcFileName
            ClassAutomation[Name] = 0
            NewClass(Name)
        }

        if (match($0, /{/))
            ClassDefLevel = BracketLevel-1
        else
            ClassDefLevel = BracketLevel
    }

    # end of class definition
    if (match($0, /^[ \t]*}/) && (BracketLevel <= ClassDefLevel))
    {
        ClassDef    = 0
        DupClassDef = 0
        PopClass()
    }

    next
}


#------------------------------------------------------------------------------
# look for class method
( (BracketLevel == 0) || ((BracketLevel == 1) && /\{/) ) &&  \
( (match(tolower(SrcFileName), /\.h/) == 0) || ( (match(tolower(SrcFileName), /\.h/) != 0) && /^[ \t]*(inline) / ) ) && \
/^[ \t]*(inline)?[ \t]*[[:alnum:]_]*[ \t]*[[:alnum:]_]+[ \t]*::[ \t]*[[:alnum:]_]+[ \t]*\(/   \
, (BracketLevel == 0) && /^[ \t]*}/ {

    # beginning of class definition
    if (match($0, /(^| )[[:alnum:]_]+[ \t]*::/) && (mClassName == ""))
    {
        # find class name
        Name = substr($0, RSTART, RLENGTH)
        sub(/^[ \t]*/, "", Name)
        sub(/[ \t]*::/, "", Name)

        if (ClassName != "")
        {
            printf "ERROR: method found inside class definition %s  [%s] %s\n", ClassName, SrcFileName, $0   > "/dev/stderr"
            ++nerrors
        }

        mClassName = Name
    }

    # end of class definition
    if (match($0, /^[ \t]*}/) && (BracketLevel == 0))
    {
        mClassName = ""
    }

    next
}


#------------------------------------------------------------------------------
# look for global unpublished variable
( (BracketLevel == 0) && match(tolower(SrcFileName), /\.cpp/) &&   \
  ( ($1 == "float")   || ($1 == "int")      || ($1 == "long") || ($1 == "unsigned") ||   \
    ($1 == "int32_t") || ($1 == "uint32_t") || ($1 == "WORD") || ($1 == "volatile")  )  )  {

    match($0, /^[ \t]*(volatile)?[ \t]*(float|int|long|long +int|unsigned|unsigned +int|unsigned +long|int32_t|uint32_t|WORD)/)
    DataType = substr($0, RSTART, RLENGTH)
    ParseLine = substr($0, RSTART+RLENGTH)
    gsub(/[ \t]+/,               " ", DataType)
    sub(/^[ \t]*/,               "",  DataType)
    sub(/[ \t]*$/,               "",  DataType)

    # remove function definition
    if (match(ParseLine, /^[ \t]*[[:alnum:]_]+([ \t]*\([^\(\)]*\)|[ \t]*::)+/))
    {
        # printf "toss: %s\n", $0                                                              > "/dev/stderr"
    }
    else if (!DataTypeIsValid(DataType))
    {
        printf "bad DataType %s\n", DataType                                                   > "/dev/stderr"
    }
    else
    {
        # printf "keep: %s\n", $0                                                              > "/dev/stderr"

        while (match(ParseLine, /^[ \t]*[[:alnum:]_]+[ \t]*(\[[[:alnum:]_\(\) ]*\])?[ \t]*(=[ \t]*[[:alnum:]_\.\+\-]+[ \t]*)?(,|;)/))
        {
            Name = substr(ParseLine, RSTART, RLENGTH)
            gsub(/[ \t,;]*/, "", Name)
            gsub(/=[[:alnum:]_\.\+\-]+/, "", Name)
            gsub(/\[[[:alnum:]_\(\) ]*\]/, "", Name)
            ParseLine = substr(ParseLine, RSTART+RLENGTH)

            if (GlobalVarList["", Name] == "")
                GlobalVarList["", Name] = DataType

            # printf "   %s  %s\n", DataType, Name                                            > "/dev/stderr"
        }

        next
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

    CheckForErrors_1()
    if (nerrors != 0) exit nerrors

    InheritClassInfo()

    PrintParmFile()
    PrintVarFile()
    PrintDiagFile()
    PrintPassFile()
    PrintInstanceFile()

    #PrintShortSummary()
    #PrintBaseClassSummary()
    #PrintInstanceSummary()
    #PrintFV_Summary()
    #PrintPassSummary()
    #PrintSummary()
    #PrintGlobalVar()

    CheckForErrors_2()
    if (nerrors != 0) exit nerrors
}


# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
# =============================================================================
# test for bracket level adjustment
function BracketLevelAdjust(TestLine, BracketAdjust, ParseLine, TempStr)  {
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
# save current class info and set indexes for starting new class
function NewClass(cName) {

    # save current class index information
    ++ClassIdx
    ClassInfoStack[ClassIdx] = sprintf("%s; %d; %d; %d; %d", ClassName, ParmIdx, VarIdx, DiagIdx, PassIdx)

    # reset indexes for new class
    ClassName      = cName
    ParmIdx        = 0
    VarIdx         = 0
    DiagIdx        = 0
    PassIdx        = 0

}

# =============================================================================
# restore indexes from previous class
function PopClass() {
    if (ClassIdx == 0)
    {
        printf "Class index already at global level [%s:%d]\n", FILENAME, FNR                   > "/dev/stderr"
    }
    else
    {
        FldCnt = split(ClassInfoStack[ClassIdx], FldArray, /[ ]*;[ ]*/)
        --ClassIdx
        ClassName      = FldArray[1]
        ParmIdx        = FldArray[2]
        VarIdx         = FldArray[3]
        DiagIdx        = FldArray[4]
        PassIdx        = FldArray[5]
    }
}

# =============================================================================
# check for valid data type
# forreturn1 and forreturn2 seem to fix a problem with running in XP ???
function DataTypeIsValid(DataType,   forreturn1, forreturn2) {

    sub(/^[ \t]*volatile[ \t]+/,    "", DataType)
    sub(/^[ \t]*const[ \t]+/,       "", DataType)
    sub(/^[ \t]*static[ \t]+/,      "", DataType)
    sub(/^[ \t]*reg[ \t]+/,         "", DataType)
    gsub(/ *\*/,                    "", DataType)

    if (DataType in ValidDataType)
        return 1

    return 0
}

# =============================================================================
# check for valid datapass type
# forreturn1 and forreturn2 seem to fix a problem with running in XP ???
function PassTypeIsValid(PassType,   i, forreturn1, forreturn2) {

    for (i=1; (PassXferType[i] != ""); ++i)
    {
        if (PassType == PassXferType[i])
            return 1
    }

    return 0
}


# =============================================================================
# find and return global variable datatype
function GetDataType(cName, vName) {

    sub(/\[ *[[:digit:]]+ *\]/, "", vName)
    DataType = GlobalVarList[cName, vName]

    sub(/^[ \t]*const[ \t]+/,       "", DataType)
    sub(/^[ \t]*static[ \t]+/,      "", DataType)
    sub(/^[ \t]*reg[ \t]+/,         "", DataType)

    return DataType
}


# =============================================================================
# inherit base class definitions
function InheritClassInfo(   Idx, TempClass) {

    # save current class indexes
    NewClass("Temp")

    for (cName in ClassList)
    {
        for (ParmIdx=1; (ParmList[cName, ParmIdx] != ""); ++ParmIdx)
            ;
        for (VarIdx=1; (VarList[cName, VarIdx] != ""); ++VarIdx)
            ;
        for (DiagIdx=1; (DiagList[cName, DiagIdx] != ""); ++DiagIdx)
            ;
        for (PassIdx=1; (PassList[cName, PassIdx] != ""); ++PassIdx)
            ;
        TempClass = cName

        # walk the base class list inheriting all information
        while ( (TempClass in BaseClassList) && !(TempClass in HandledBaseList) )
        {
            cBaseName = BaseClassList[TempClass]
            if (ClassAutomation[cName] < ClassAutomation[cBaseName])
                ClassAutomation[cName] = ClassAutomation[cBaseName]

            for (Idx=1; (ParmList[cBaseName, Idx] != ""); ++Idx)
            {
                ParmList[cName, ParmIdx] = ParmList[cBaseName, Idx]
                ++ParmIdx
            }
            for (Idx=1; (VarList[cBaseName, Idx] != ""); ++Idx)
            {
                VarList[cName, VarIdx] = VarList[cBaseName, Idx]
                ++VarIdx
            }
            for (Idx=1; (DiagList[cBaseName, Idx] != ""); ++Idx)
            {
                DiagList[cName, DiagIdx] = DiagList[cBaseName, Idx]
                ++DiagIdx
            }
            for (Idx=1; (PassList[cBaseName, Idx] != ""); ++Idx)
            {
                PassList[cName, PassIdx] = PassList[cBaseName, Idx]
                ++PassIdx
            }
            TempClass = cBaseName
        }
        HandledBaseList[cName] = 1
    }

    # restore class indexes
    PopClass()
}


# =============================================================================
function CheckForErrors_1(   pIdx, iIdx, cName, iName) {

    # check global data passing has no structures
    cName = ""
    for (pIdx=1; (PassList[cName, pIdx] != ""); ++pIdx)
    {
        FldCnt = split(PassList[cName, pIdx], FldArrayP, /[ ]*;[ ]*/)
        if (match(FldArrayP[2], /\./))
        {
            iName = substr(FldArrayP[2], 1, RSTART-1)
            if (!(iName in InstanceToClass) || !(InstanceToClass[iName] in ClassList))
            {
                printf "ERROR: DATA_PASS source (%s) is from unknown class\n", FldArrayP[2]         > "/dev/stderr"
                ++nerrors
            }
        }
        if (match(FldArrayP[3], /\./))
        {
            iName = substr(FldArrayP[3], 1, RSTART-1)
            if (!(iName in InstanceToClass) || !(InstanceToClass[iName] in ClassList))
            {
                printf "ERROR: DATA_PASS destination (%s) is to unknown class\n", FldArrayP[3]      > "/dev/stderr"
                ++nerrors
            }
        }
    }

    # check global data passing filters only float variables
    cName = ""
    for (pIdx=1; (PassList[cName, pIdx] != ""); ++pIdx)
    {
        FldCnt = split(PassList[cName, pIdx], FldArrayP, /[ ]*;[ ]*/)
        if (tolower(FldArrayP[5]) == "filtered")
        {
            if (match(FldArrayP[2], /\./))
            {
                iName = substr(FldArrayP[2], 1, RSTART-1)
                vName = substr(FldArrayP[2], RSTART+1)
                if (!match(GetDataType(InstanceToClass[iName], vName), /float/) && (GetDataType(InstanceToClass[iName], vName) != ""))
                {
                    printf "ERROR: DATA_PASS Filter applied to non-float instance variable %s\n", FldArrayP[2]      > "/dev/stderr"
                    ++nerrors
                }
            }
            else if (!match(GetDataType(cName, FldArrayP[2]), /float/) && (GetDataType(cName, FldArrayP[2]) != ""))
            {
                printf "ERROR: DATA_PASS Filter applied to non-float global variable %s\n", FldArrayP[2]      > "/dev/stderr"
                ++nerrors
            }
            if (match(FldArrayP[3], /\./))
            {
                iName = substr(FldArrayP[3], 1, RSTART-1)
                vName = substr(FldArrayP[3], RSTART+1)
                if (!match(GetDataType(InstanceToClass[iName], vName), /float/) && (GetDataType(InstanceToClass[iName], vName) != ""))
                {
                    printf "ERROR: DATA_PASS Filter applied to non-float instance variable %s\n", FldArrayP[3]      > "/dev/stderr"
                    ++nerrors
                }
            }
            else if (!match(GetDataType(cName, FldArrayP[3]), /float/) && (GetDataType(cName, FldArrayP[3]) != ""))
            {
                printf "ERROR: DATA_PASS Filter applied to non-float global variable %s\n", FldArrayP[3]      > "/dev/stderr"
                ++nerrors
            }
        }
    }

    # check class data passing filters only float variables
    for (cName in ClassList)
    {
        # skip classes with no data passing
        if (PassList[cName, 1] == "")
            continue
        for (pIdx=1; (PassList[cName, pIdx] != ""); ++pIdx)
        {
            FldCnt = split(PassList[cName, pIdx], FldArrayP, /[ ]*;[ ]*/)
            if (tolower(FldArrayP[5]) == "filtered")
            {
                if (!match(GetDataType(cName, FldArrayP[2]), /float/) && (GetDataType(cName, FldArrayP[2]) != ""))
                {
                    printf "ERROR: DATA_PASS Filter applied to non-float class variable %s.%s\n", cName, FldArrayP[2]      > "/dev/stderr"
                    ++nerrors
                }
                if (!match(GetDataType(cName, FldArrayP[3]), /float/) && (GetDataType(cName, FldArrayP[3]) != ""))
                {
                    printf "ERROR: DATA_PASS Filter applied to non-float class variable %s.%s\n", cName, FldArrayP[3]      > "/dev/stderr"
                    ++nerrors
                }
            }
        }
    }

    # confirm classes with task 1 data passing variables have a task 1 definition from INSTANCE_INFO
    for (cName in ClassList)
    {
        # look for data pass variables from task 1
        for (pIdx=1; (PassList[cName, pIdx] != ""); ++pIdx)
        {
            FldCnt = split(PassList[cName, pIdx], FldArrayP, /[ ]*;[ ]*/)
            if (match(FldArrayP[4], /^T1/))
            {
                # confirm that each instance of the class has specified a task 1 definition
                for (iIdx=1; (InstanceList[cName, iIdx] != ""); ++iIdx)
                {
                    FldCnt = split(InstanceList[cName, iIdx], FldArrayI, /[ ]*;[ ]*/)
                    if (!(FldArrayI[1] in InstanceModifier))
                    {
                        printf "ERROR: %s INSTANCE_INFO is required for data passing Task 1 class variables\n", FldArrayI[1]   > "/dev/stderr"
                        ++nerrors
                    }
                }
                # test the next class
                break
            }

        }
    }
}


# =============================================================================
function CheckForErrors_2(   IdxName) {

    # confirm all final values were found in the parameter list
    for (IdxName in FV_List)
    {
        if  (FV_List[IdxName] == "")
        {
            FldCnt = split(IdxName, FldArray, SUBSEP)
            if (FldArray[1] == "")
                printf "ERROR: global final value %s was NOT found in the parameter list\n", FldArray[2]  > "/dev/stderr"
            else
                printf "ERROR: class %s final value %s was NOT found in the parameter list\n", FldArray[1], FldArray[2]  > "/dev/stderr"
            ++nerrors
        }
    }
}


# =============================================================================
# print parameter definition files
# PARM_ENTRY( Class, OrigClass, BaseName, PublishedName, C_Name, Uniquifier, DataType, FinalValue )
function PrintParmFile(   pIdx, iIdx, ExternFileDefine, TmpFinalVal) {

    if ((ParmDefFile == "") || (ParmExternFile == ""))
        return

    # start define
    FldCnt = split(ParmExternFile, FldArray, /[\\\/]/)
    ExternFileDefine = toupper(FldArray[FldCnt])
    gsub(/["\\\/ ]*/, "", ExternFileDefine)
    gsub(/\./, "_", ExternFileDefine)
    printf "#ifndef  %s\n", ExternFileDefine                                        > ParmExternFile
    printf "#define  %s\n", ExternFileDefine                                        > ParmExternFile
    printf "/////////////\n\n"                                                      > ParmExternFile

    # print global parameters
    printf "// Global\n"                                                            > ParmDefFile
    printf "#include <stdint.h>\n"                                                  > ParmExternFile
    printf "#define WORD  uint32_t\n"                                               > ParmExternFile
    printf "\n"                                                                     > ParmExternFile
    printf "// Global\n"                                                            > ParmExternFile
    cName = ""
    for (pIdx=1; (ParmList[cName, pIdx] != ""); ++pIdx)
    {
        FldCnt = split(ParmList[cName, pIdx], FldArrayP, /[ ]*;[ ]*/)
        if ((cName SUBSEP FldArrayP[2]) in FV_List)
        {
            FV_List[cName, FldArrayP[2]] = "used"
            TmpFinalVal = "FV"
        }
        else
            TmpFinalVal = ""
        printf "extern %s PARM_%s;\n", FldArrayP[3], FldArrayP[2]                   > ParmExternFile
        printf "PARM_ENTRY(%s, %s, %s, %s, PARM_%s, \"\", %s, %s)\n",
               cName, FldArrayP[1], FldArrayP[2], FldArrayP[2], FldArrayP[2], FldArrayP[3], TmpFinalVal > ParmDefFile
    }

    # define
    printf "\n/////////////\n"                                                      > ParmExternFile
    printf "#endif  // %s\n", ExternFileDefine                                      > ParmExternFile
    printf "#ifdef  PRM_STATIC_INIT\n"                                              > ParmExternFile
    printf "/////////////\n"                                                        > ParmExternFile

    # print class parameters
    for (cName in ClassList)
    {
        for (pIdx=1; (ParmList[cName, pIdx] != ""); ++pIdx)
        {
            FldCnt = split(ParmList[cName, pIdx], FldArrayP, /[ ]*;[ ]*/)
            if ((cName SUBSEP FldArrayP[2]) in FV_List)
            {
                FV_List[cName, FldArrayP[2]] = "used"
                TmpFinalVal = "FV"
            }
            else
                TmpFinalVal = ""
            for (iIdx=1; (InstanceList[cName, iIdx] != ""); ++iIdx)
            {
                FldCnt = split(InstanceList[cName, iIdx], FldArrayI, /[ ]*;[ ]*/)
                if ((FldArrayI[1] in InstanceModifier) && (FldArrayP[4] != "CP"))
                {
                    FldCnt = split(InstanceModifier[FldArrayI[1]], FldArrayIm, /[ ]*;[ ]*/)
                    Uniquifier = FldArrayIm[1]
                }
                else
                    Uniquifier = "\"\""

                if ((pIdx == 1) && (iIdx == 1))
                {
                    printf "\n"                                                     > ParmDefFile
                    printf "// from file %s\n", ClassList[cName]                    > ParmDefFile
                    printf "\n"                                                     > ParmExternFile
                    printf "#include \"%s\"\n", ClassList[cName]                    > ParmExternFile
                }
                if (pIdx == 1)
                    printf "extern %s %s;\n", cName, FldArrayI[1]                   > ParmExternFile
                if ((FldArrayP[4] == "CP") && (iIdx == 1))
                    printf "%s %s::PARM_%s=0;\n", FldArrayP[3], cName, FldArrayP[2] > ParmExternFile

                if (FldArrayP[4] != "CP")
                    printf "PARM_ENTRY(%s, %s, %s, %s%s%s, %s.PARM_%s, %s, %s, %s)\n",
                           cName, FldArrayP[1], FldArrayP[2], FldArrayI[2], FldArrayP[2], FldArrayI[3], FldArrayI[1], FldArrayP[2], Uniquifier, FldArrayP[3], TmpFinalVal     > ParmDefFile
                else if (iIdx == 1)
                    printf "PARM_ENTRY(%s, %s, %s, %s, %s.PARM_%s, %s, %s, %s)\n",
                           cName, FldArrayP[1], FldArrayP[2], FldArrayP[2], FldArrayI[1], FldArrayP[2], Uniquifier, FldArrayP[3], TmpFinalVal > ParmDefFile
            }
        }
    }

    # close define
    printf "\n/////////////\n"                                                      > ParmExternFile
    printf "#endif  // PRM_STATIC_INIT\n"                                           > ParmExternFile
}


# =============================================================================
# print variable definition files
# VAR_ENTRY( Class, OrigClass, BaseName, PublishedName, C_Name, Uniquifier, DataType, LadrDataType, ReadOnly )
function PrintVarFile(   vIdx, iIdx) {

    if ((VarDefFile == "") || (VarExternFile == ""))
        return

    # start define
    FldCnt = split(VarExternFile, FldArray, /[\\\/]/)
    ExternFileDefine = toupper(FldArray[FldCnt])
    gsub(/["\\\/ ]*/, "", ExternFileDefine)
    gsub(/\./, "_", ExternFileDefine)
    printf "#ifndef  %s\n", ExternFileDefine                                        > VarExternFile
    printf "#define  %s\n", ExternFileDefine                                        > VarExternFile
    printf "/////////////\n\n"                                                      > VarExternFile

    # print global variables
    printf "// Global\n"                                                            > VarDefFile
    printf "#include <stdint.h>\n"                                                  > VarExternFile
    printf "#define WORD  uint32_t\n"                                               > VarExternFile
    printf "\n"                                                                     > VarExternFile
    printf "// Global\n"                                                            > VarExternFile
    cName = ""
    for (vIdx=1; (VarList[cName, vIdx] != ""); ++vIdx)
    {
        FldCnt = split(VarList[cName, vIdx], FldArrayV, /[ ]*;[ ]*/)

        if (match(FldArrayV[3], /^[ \t]*const[ \t]+/))
            ReadOnly = "RdOnly"
        else
            ReadOnly = ""

        if (FldArrayV[3] == "complex")
        {
            printf "struct %s;\n", GetDataType(cName, FldArrayV[2])                 > VarExternFile
            printf "extern %s %s;\n", GetDataType(cName, FldArrayV[2]), FldArrayV[2]  > VarExternFile
        }
        else
            printf "extern %s %s;\n", FldArrayV[3], FldArrayV[2]                    > VarExternFile

        sub(/^[ \t]*volatile[ \t]+/, "", FldArrayV[3])
        sub(/^[ \t]*const[ \t]+/, "", FldArrayV[3])
        sub(/^[ \t]*static[ \t]+/, "", FldArrayV[3])
        sub(/^[ \t]*reg[ \t]+/, "", FldArrayV[3])

        printf "VAR_ENTRY(%s, %s, %s, %s, %s, \"\", %s, %s, %s)\n",
               cName, FldArrayV[1], FldArrayV[2], FldArrayV[2], FldArrayV[2], FldArrayV[3], LadrDataType[FldArrayV[3]], ReadOnly  > VarDefFile
    }

    # define
    printf "\n/////////////\n"                                                      > VarExternFile
    printf "#endif  // %s\n", ExternFileDefine                                      > VarExternFile
    printf "#ifdef  VAR_STATIC_INIT\n"                                              > VarExternFile
    printf "/////////////\n"                                                        > VarExternFile

    # print class variables
    for (cName in ClassList)
    {
        for (vIdx=1; (VarList[cName, vIdx] != ""); ++vIdx)
        {
            FldCnt = split(VarList[cName, vIdx], FldArrayV, /[ ]*;[ ]*/)

            if (match(FldArrayV[3], /^[ \t]*const[ \t]+/))
                ReadOnly = "RdOnly"
            else
                ReadOnly = ""

            sub(/^[ \t]*const[ \t]+/, "", FldArrayV[3])

            for (iIdx=1; (InstanceList[cName, iIdx] != ""); ++iIdx)
            {
                FldCnt = split(InstanceList[cName, iIdx], FldArrayI, /[ ]*;[ ]*/)
                if ((FldArrayI[1] in InstanceModifier) && (FldArrayV[4] != "CV"))
                {
                    FldCnt = split(InstanceModifier[FldArrayI[1]], FldArrayIm, /[ ]*;[ ]*/)
                    Uniquifier = FldArrayIm[1]
                }
                else
                    Uniquifier = "\"\""

                if ((vIdx == 1) && (iIdx == 1))
                {
                    printf "\n"                                                     > VarDefFile
                    printf "// from file %s\n", ClassList[cName]                    > VarDefFile
                    printf "\n"                                                     > VarExternFile
                    printf "#include \"%s\"\n", ClassList[cName]                    > VarExternFile
                }
                if (vIdx == 1)
                    printf "extern %s %s;\n", cName, FldArrayI[1]                   > VarExternFile
                if ((FldArrayV[4] == "CV") && (iIdx == 1))
                    printf "%s %s::%s=0;\n", FldArrayV[3], cName, FldArrayV[2]      > VarExternFile

                if (FldArrayV[4] != "CV")
                    printf "VAR_ENTRY(%s, %s, %s, %s%s%s, %s.%s, %s, %s, %s, %s)\n",
                           cName, FldArrayV[1], FldArrayV[2], FldArrayI[2], FldArrayV[2], FldArrayI[3], FldArrayI[1], FldArrayV[2], Uniquifier, FldArrayV[3], LadrDataType[FldArrayV[3]], ReadOnly      > VarDefFile
                else if (iIdx == 1)
                    printf "VAR_ENTRY(%s, %s, %s, %s, %s.%s, %s, %s, %s, %s)\n",
                           cName, FldArrayV[1], FldArrayV[2], FldArrayV[2], FldArrayI[1], FldArrayV[2], Uniquifier, FldArrayV[3], LadrDataType[FldArrayV[3]], ReadOnly    > VarDefFile
            }
        }
    }

    # close define
    printf "\n/////////////\n"                                                      > VarExternFile
    printf "#endif  // VAR_STATIC_INIT\n"                                           > VarExternFile
}


# =============================================================================
# print diagnostic definition files
# DIAG_ENTRY( Class, OrigClass, BaseName, PublishedName, C_Name, Uniquifier, Bridge )
function PrintDiagFile(   dIdx, iIdx, ExternFileDefine) {

    if ((DiagDefFile == "") || (DiagExternFile == ""))
        return

    # start define
    FldCnt = split(DiagExternFile, FldArray, /[\\\/]/)
    ExternFileDefine = toupper(FldArray[FldCnt])
    gsub(/["\\\/ ]*/, "", ExternFileDefine)
    gsub(/\./, "_", ExternFileDefine)
    printf "#ifndef  %s\n", ExternFileDefine                                        > DiagExternFile
    printf "#define  %s\n", ExternFileDefine                                        > DiagExternFile
    printf "/////////////\n\n"                                                      > DiagExternFile

    # print global diagnostics
    printf "// Global\n"                                                            > DiagDefFile
    printf "// Global\n"                                                            > DiagExternFile
    cName = ""
    for (dIdx=1; (DiagList[cName, dIdx] != ""); ++dIdx)
    {
        FldCnt = split(DiagList[cName, dIdx], FldArrayD, /[ ]*;[ ]*/)
        printf "extern int DIAG_%s;\n", FldArrayD[2]                                > DiagExternFile
        printf "DIAG_ENTRY(%s, %s, %s, %s, DIAG_%s, \"\", )\n",
               cName, FldArrayD[1], FldArrayD[2], FldArrayD[2], FldArrayD[2] > DiagDefFile
    }

    # define
    printf "\n/////////////\n"                                                      > DiagExternFile
    printf "#endif  // %s\n", ExternFileDefine                                      > DiagExternFile
    printf "#ifdef  DIAG_STATIC_INIT\n"                                             > DiagExternFile
    printf "/////////////\n"                                                        > DiagExternFile

    # print class diagnostics
    for (cName in ClassList)
    {
        for (dIdx=1; (DiagList[cName, dIdx] != ""); ++dIdx)
        {
            FldCnt = split(DiagList[cName, dIdx], FldArrayD, /[ ]*;[ ]*/)
            for (iIdx=1; (InstanceList[cName, iIdx] != ""); ++iIdx)
            {
                FldCnt = split(InstanceList[cName, iIdx], FldArrayI, /[ ]*;[ ]*/)
                if ((FldArrayI[1] in InstanceModifier) && (FldArrayD[3] != "CD"))
                {
                    FldCnt = split(InstanceModifier[FldArrayI[1]], FldArrayIm, /[ ]*;[ ]*/)
                    Uniquifier = FldArrayIm[1]
                    Bridge     = FldArrayIm[2]
                }
                else
                {
                    Uniquifier = "\"\""
                    Bridge     = ""
                }

                if ((dIdx == 1) && (iIdx == 1))
                {
                    printf "\n"                                                     > DiagDefFile
                    printf "// from file %s\n", ClassList[cName]                    > DiagDefFile
                    printf "\n"                                                     > DiagExternFile
                    printf "#include \"%s\"\n", ClassList[cName]                    > DiagExternFile
                }
                if (dIdx == 1)
                    printf "extern %s %s;\n", cName, FldArrayI[1]                   > DiagExternFile
                if ((FldArrayD[3] == "CD") && (iIdx == 1))
                    printf "int %s::DIAG_%s=0;\n", cName, FldArrayD[2]              > DiagExternFile

                if (FldArrayD[3] != "CD")
                    printf "DIAG_ENTRY(%s, %s, %s, %s%s%s, %s.DIAG_%s, %s, %s)\n",
                           cName, FldArrayD[1], FldArrayD[2], FldArrayI[2], FldArrayD[2], FldArrayI[3], FldArrayI[1], FldArrayD[2], Uniquifier, Bridge   > DiagDefFile
                else if (iIdx == 1)
                    printf "DIAG_ENTRY(%s, %s, %s, %s, %s.DIAG_%s, %s, %s)\n",
                           cName, FldArrayD[1], FldArrayD[2], FldArrayD[2], FldArrayI[1], FldArrayD[2], Uniquifier, Bridge  > DiagDefFile
            }
        }
    }

    # close define
    printf "\n/////////////\n"                                                      > DiagExternFile
    printf "#endif  // DIAG_STATIC_INIT\n"                                          > DiagExternFile
}


# =============================================================================
# print datapass definition files
function PrintPassFile(   pIdx, iIdx, XferDefinition ) {

    if ((PassDefFile == "") || (PassExternFile == ""))
        return

    for (tIdx=1; (PassXferType[tIdx] != ""); ++tIdx)
    {
        XferDefinition = PassXferType[tIdx]
        sub(/_/, "_to_", XferDefinition)
        sub(/_F/, "_FILT", XferDefinition)

        printf "\n\n"                                                               > PassDefFile
        printf "#ifdef %s\n", XferDefinition                                        > PassDefFile

        # print global data passing
        cName = ""
        for (pIdx=1; (PassList[cName, pIdx] != ""); ++pIdx)
        {
            FldCnt = split(PassList[cName, pIdx], FldArrayP, /[ ]*;[ ]*/)
            if (tolower(FldArrayP[5]) == "filtered")
                FldArrayP[4] = FldArrayP[4] "_F"
            if (FldArrayP[4] == PassXferType[tIdx])
                printf "   PASS_ENTRY(%s, %s)\n", FldArrayP[2], FldArrayP[3]        > PassDefFile
        }

        # print class data passing
        for (cName in ClassList)
        {
            # skip classes with no data passing
            if (PassList[cName, 1] == "")
                continue
            # check each instance of the class
            for (iIdx=1; (InstanceList[cName, iIdx] != ""); ++iIdx)
            {
                FldCnt = split(InstanceList[cName, iIdx], FldArrayI, /[ ]*;[ ]*/)
                Task1 = ""
                # determine if instance is Task1 or Task1A when Task 1 is the source
                if (match(PassXferType[tIdx], /T1A*_T[23]/))
                {
                    if (FldArrayI[1] in InstanceModifier)
                    {
                        FldCnt = split(InstanceModifier[FldArrayI[1]], FldArrayIm, /[ ]*;[ ]*/)
                        if (tolower(FldArrayIm[3]) == "task1a")
                            Task1 = "T1A"
                        else
                            Task1 = "T1"
                    }
                }
                # check each data pass for this instance
                for (pIdx=1; (PassList[cName, pIdx] != ""); ++pIdx)
                {
                    FldCnt = split(PassList[cName, pIdx], FldArrayP, /[ ]*;[ ]*/)
                    sub(/^T1A*/, Task1, FldArrayP[4])
                    if (tolower(FldArrayP[5]) == "filtered")
                        FldArrayP[4] = FldArrayP[4] "_F"

                    if (FldArrayP[4] == PassXferType[tIdx])
                    {
                        printf "   PASS_ENTRY(%s.%s, %s.%s)\n", FldArrayI[1], FldArrayP[2], FldArrayI[1], FldArrayP[3]        > PassDefFile
                    }
                }
            }
        }

        printf "#endif // %s\n", XferDefinition                                     > PassDefFile
    }

    #--------------------------------------------------------------------------
    # start define
    FldCnt = split(PassExternFile, FldArray, /[\\\/]/)
    ExternFileDefine = toupper(FldArray[FldCnt])
    gsub(/["\\\/ ]*/, "", ExternFileDefine)
    gsub(/\./, "_", ExternFileDefine)
    printf "#ifndef  %s\n", ExternFileDefine                                        > PassExternFile
    printf "#define  %s\n", ExternFileDefine                                        > PassExternFile
    printf "/////////////\n\n"                                                      > PassExternFile

    # print global extern information
    printf "#include <stdint.h>\n"                                                  > PassExternFile
    printf "#define WORD  uint32_t\n"                                               > PassExternFile
    printf "\n"                                                                     > PassExternFile
    printf "// Global\n"                                                            > PassExternFile
    cName = ""
    for (pIdx=1; (PassList[cName, pIdx] != ""); ++pIdx)
    {
        FldCnt = split(PassList[cName, pIdx], FldArrayP, /[ ]*;[ ]*/)
        if (match(FldArrayP[2], /\./))
        {
            iName = substr(FldArrayP[2], 1, RSTART-1)
            if ((iName in InstanceToClass))
            {
                printf "#include \"%s\"\n", ClassList[InstanceToClass[iName]]       > PassExternFile
                printf "extern %s %s;\n", InstanceToClass[iName], iName             > PassExternFile
            }
        }
        else if (GetDataType(cName, FldArrayP[2]) == "")
            printf "// unknown DataType for %s\n", FldArrayP[2]                     > PassExternFile
        else if (match(FldArrayP[2], /\[ *[[:digit:]]+ *\]/))
            printf "extern %s %s[];\n", GetDataType(cName, FldArrayP[2]), substr(FldArrayP[2], 1, RSTART-1)  > PassExternFile
        else
            printf "extern %s %s;\n", GetDataType(cName, FldArrayP[2]), FldArrayP[2]       > PassExternFile

        if (match(FldArrayP[3], /\./))
            iName = substr(FldArrayP[3], 1, RSTART-1)
        else if (GetDataType(cName, FldArrayP[3]) == "")
            printf "// unknown DataType for %s\n", FldArrayP[3]                     > PassExternFile
        else if (match(FldArrayP[3], /\[ *[[:digit:]]+ *\]/))
            printf "extern %s %s[];\n", GetDataType(cName, FldArrayP[3]), substr(FldArrayP[3], 1, RSTART-1)  > PassExternFile
        else
            printf "extern %s %s;\n", GetDataType(cName, FldArrayP[3]), FldArrayP[3]       > PassExternFile
    }

    # print class extern information
    for (cName in ClassList)
    {
        if (PassList[cName, 1] != "")
        {
            for (iIdx=1; (InstanceList[cName, iIdx] != ""); ++iIdx)
            {
                FldCnt = split(InstanceList[cName, iIdx], FldArrayI, /[ ]*;[ ]*/)
                if (iIdx == 1)
                {
                    printf "\n"                                                     > PassExternFile
                    printf "#include \"%s\"\n", ClassList[cName]                    > PassExternFile
                }
                printf "extern %s %s;\n", cName, FldArrayI[1]                       > PassExternFile
            }
        }
    }

    # close define
    printf "\n/////////////\n"                                                      > PassExternFile
    printf "#endif  // %s\n", ExternFileDefine                                      > PassExternFile
}


# =============================================================================
# print class instance file
function PrintInstanceFile(   iIdx) {

    if (InstanceFile == "")
        return

    printf "\n"                                                                     > InstanceFile
    for (cName in ClassList)
    {
        if (ClassAutomation[cName] > 0)
        {
            for (iIdx=1; (InstanceList[cName, iIdx] != ""); ++iIdx)
            {
                FldCnt = split(InstanceList[cName, iIdx], FldArrayI, /[ ]*;[ ]*/)
                printf "%s; %s; %s; %s\n", cName, FldArrayI[1], FldArrayI[2], FldArrayI[3]   > InstanceFile
            }
        }
    }
}


# =============================================================================
function PrintShortSummary() {
    printf "\n"                                                                 > "/dev/stderr"
    printf "=========================================\n"                        > "/dev/stderr"
    printf "elapse time  %d sec\n", systime()-StartTime                         > "/dev/stderr"
    printf "Total Classes cnt  %4d\n", ClassCnt                                 > "/dev/stderr"
    printf "Total Instance cnt %4d\n", InstanceCnt                              > "/dev/stderr"
}


# =============================================================================
function PrintBaseClassSummary(   TempClass) {
    printf "\n"                                                                 > "/dev/stderr"
    printf "=========================================\n"                        > "/dev/stderr"

    for (cName in ClassList)
    {
        if ((ClassAutomation[cName] > 0) && (cName in BaseClassList))
        {
            printf "%s", cName                                                  > "/dev/stderr"
            TempClass = cName

            # walk the base class list inheriting all information
            while (TempClass in BaseClassList)
            {
                TempClass = BaseClassList[TempClass]
                printf " --> %s", TempClass                                     > "/dev/stderr"
            }
            printf "\n"                                                         > "/dev/stderr"
        }
    }
}


# =============================================================================
function PrintInstanceSummary(   Idx) {
    printf "\n"                                                                 > "/dev/stderr"
    printf "=========================================\n"                        > "/dev/stderr"
    printf "elapse time  %d sec\n", systime()-StartTime                         > "/dev/stderr"
    printf "Total Classes cnt  %4d\n", ClassCnt                                 > "/dev/stderr"
    printf "Total Instance cnt %4d\n", InstanceCnt                              > "/dev/stderr"

    for (cName in ClassList)
    {
        for (Idx=1; (InstanceList[cName, Idx] != ""); ++Idx)
        {
            if (ClassAutomation[cName] > 0)
            {
                if (Idx == 1)
                    printf "class %s\n", cName                                  > "/dev/stderr"
                printf "   Instance %s\n", InstanceList[cName, Idx]             > "/dev/stderr"
            }
        }
    }
}


# =============================================================================
function PrintFV_Summary(   FV_Cnt, iCnt, FirstFV) {
    printf "\n"                                                                 > "/dev/stderr"
    printf "=========================================\n"                        > "/dev/stderr"
    printf "elapse time  %d sec\n", systime()-StartTime                         > "/dev/stderr"
    printf "\n"                                                                 > "/dev/stderr"

    FV_Cnt = 0
    printf "Global\n"                                                           > "/dev/stderr"
    cName = ""
    for (IdxName in FV_List)
    {
        FldCnt = split(IdxName, FldArray, SUBSEP)
        if  (FldArray[1] == cName)
        {
            printf "   FV     %s\n", FldArray[2]                                > "/dev/stderr"
            ++FV_Cnt
        }
    }

    for (cName in ClassList)
    {
        FirstFV = 1
        iCnt=1
        while (InstanceList[cName, iCnt] != "")
            ++iCnt
        --iCnt

        for (IdxName in FV_List)
        {
            FldCnt = split(IdxName, FldArray, SUBSEP)
            if  (FldArray[1] == cName)
            {
                if  (FirstFV)
                    printf "\nclass %s in %s\n", cName, ClassList[cName]        > "/dev/stderr"
                printf "   FV     %s\n", FldArray[2]                            > "/dev/stderr"
                FV_Cnt += iCnt
                FirstFV = 0
            }
        }
    }

    printf "\n"                                                                 > "/dev/stderr"
    Idx = 0
    for (cName in FV_List)
        ++Idx
    printf "Total SET_FV cnt       %4d\n", Idx                                  > "/dev/stderr"
    printf "Total final value cnt  %4d\n", FV_Cnt                               > "/dev/stderr"
    printf "\n"                                                                 > "/dev/stderr"
}


# =============================================================================
function PrintPassSummary(   Idx) {
    printf "\n"                                                                 > "/dev/stderr"
    printf "=========================================\n"                        > "/dev/stderr"
    printf "elapse time  %d sec\n", systime()-StartTime                         > "/dev/stderr"

    printf "Global\n"                                                           > "/dev/stderr"
    cName = ""
    for (Idx=1; (PassList[cName, Idx] != ""); ++Idx)
    {
        printf "   Pass     %s\n", PassList[cName, Idx]                         > "/dev/stderr"
    }

    for (cName in ClassList)
    {
        if (ClassAutomation[cName] > 0)
        {
            printf "class %s in %s\n", cName, ClassList[cName]                  > "/dev/stderr"
            for (Idx=1; (PassList[cName, Idx] != ""); ++Idx)
            {
                printf "   Pass     %s\n", PassList[cName, Idx]                 > "/dev/stderr"
            }
        }
    }
}


# =============================================================================
function PrintSummary(   Idx) {
    printf "\n"                                                                 > "/dev/stderr"
    printf "=========================================\n"                        > "/dev/stderr"
    printf "elapse time  %d sec\n", systime()-StartTime                         > "/dev/stderr"
    printf "Total Classes cnt  %4d\n", ClassCnt                                 > "/dev/stderr"
    printf "Total Instance cnt %4d\n", InstanceCnt                              > "/dev/stderr"

    printf "Global\n"                                                           > "/dev/stderr"
    cName = ""
    for (Idx=1; (ParmList[cName, Idx] != ""); ++Idx)
    {
        printf "   Parm     %s\n", ParmList[cName, Idx]                         > "/dev/stderr"
    }
    for (Idx=1; (VarList[cName, Idx] != ""); ++Idx)
    {
        printf "   Var      %s\n", VarList[cName, Idx]                          > "/dev/stderr"
    }
    for (Idx=1; (DiagList[cName, Idx] != ""); ++Idx)
    {
        printf "   Diag     %s\n", DiagList[cName, Idx]                         > "/dev/stderr"
    }
    for (Idx=1; (PassList[cName, Idx] != ""); ++Idx)
    {
        printf "   Pass     %s\n", PassList[cName, Idx]                         > "/dev/stderr"
    }

    for (cName in ClassList)
    {
        if (ClassAutomation[cName] > 0)
        {
            printf "class %s in %s\n", cName, ClassList[cName]                  > "/dev/stderr"
            for (Idx=1; (InstanceList[cName, Idx] != ""); ++Idx)
            {
                printf "   Instance %s\n", InstanceList[cName, Idx]             > "/dev/stderr"
            }
            for (Idx=1; (ParmList[cName, Idx] != ""); ++Idx)
            {
                printf "   Parm     %s\n", ParmList[cName, Idx]                 > "/dev/stderr"
            }
            for (Idx=1; (VarList[cName, Idx] != ""); ++Idx)
            {
                printf "   Var      %s\n", VarList[cName, Idx]                  > "/dev/stderr"
            }
            for (Idx=1; (DiagList[cName, Idx] != ""); ++Idx)
            {
                printf "   Diag     %s\n", DiagList[cName, Idx]                 > "/dev/stderr"
            }
            for (Idx=1; (PassList[cName, Idx] != ""); ++Idx)
            {
                printf "   Pass     %s\n", PassList[cName, Idx]                 > "/dev/stderr"
            }
        }
    }
}


# =============================================================================
function PrintGlobalVar() {
    printf "\n"                                                                 > "Log.txt"
    printf "=========================================\n"                        > "Log.txt"
    printf "Global Variables\n"                                                 > "Log.txt"

    for (vName in GlobalVarList)
    {
        printf "   %s  %s\n", GlobalVarList[vName], vName                       > "Log.txt"
    }
}
