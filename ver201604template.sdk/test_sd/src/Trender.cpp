//////////////////////////////////////////////////////////////////////////////
// TITLE: Trender.cpp
// DESCRIPTION:
//     Trender functions, supported by ProductDLL (DLL #1),
//     and available to Model-specific DLL's (DLL #2's).
// ENTRY:
// COPYRIGHT:   Copyright (c) 2015 by GENERAL ELECTRIC COMPANY U.S.A.
///////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <string>
using namespace std;

#include "MarkStatDLL.h"
using namespace MarkStatDLL;


//-----------------------------------------------------------------------------
static void*        pContractDiagnosticReset = NULL;
static unsigned     DiagReset;
static unsigned     TaskBCnt;

static ContractListType ListDiagnosticReset[] = {
    {"IScratch4", &DiagReset},
};
void* MakeContractDiagnosticReset   (void){
    void*   pContract;
    printf("  MakeContract DiagnosticReset......");
    pContract = MarkStatDLLApi::CreateContract(
                    sizeof(ListDiagnosticReset)/
                    sizeof(ListDiagnosticReset[0]),
                    ListDiagnosticReset, READ_VAR );
    return (pContract);
}

//-----------------------------------------------------------------------------
void ResetDiagnostics()
{
    int TaskBCnt_Z1;

    // run background 4 times after stopped pushing diagnostics
    for (int Cnt=0; (Cnt < 4); ++Cnt)
    {
        TaskBCnt_Z1 = TaskBCnt;
        while (TaskBCnt == TaskBCnt_Z1)
        {
            MarkStatDLLApi::Step();
        }
    }

    // reset diagnostic flag and run background once more
    DiagReset = 1;
    MarkStatDLLApi::ExecContract(pContractDiagnosticReset);

    TaskBCnt_Z1 = TaskBCnt;
    while (TaskBCnt == TaskBCnt_Z1)
    {
        MarkStatDLLApi::Step();
    }

    DiagReset = 0;
    MarkStatDLLApi::ExecContract(pContractDiagnosticReset);

}

///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
void*    pContractTrender = NULL;
//------------------

ContractListType ListTrender[] = {
    {"EventTmSec"       },
    {"EventTmMsec"      },
	{"R_TrqRef"},
		{"R_TrqFbk"},
	{"VARCmd"},
	{"R_SpdRefSim"},
    //
    {"TripFlt"          },
    {"NoFlt"            },
    //
    {"RunRdy"           },
    {"RunReq"           },
    {"Running"          },
    //
    {"L_BrgPwrEnb"      },
    {"R_BrgPwrEnb"      },
    //
    {"SeqSt"            },
    {"L_SeqSt"          },
    {"R_SeqSt"          },
    //
    {"R_SyncTskSt"      },
    {"R_SyncTskTm"      },
    //
    {"S_VregErr"        },
    {"S_VregCmd"        },
    {"S_VMagT2"         },
    {"S_VregInt"        },
    {"S_VregOut"        },
    {"SimMode"          },
    {"XSimMode"         },
    {"DsimMode"         },
    {"LocalMode"        },
    //
    {"T1St"             },
    {"T1ASt"            },
    {"T2St"             },
    {"T3St"             },
    {"T1T2St"           },
    {"T1AT2St"          },

    {"TempOk"           },

    {"L_SpinRdy"        },
    {"R_SpinRdy"        },
    {"SpinRdy"          },
    {"L_RunRdy"         },
    {"R_RunRdy"         },
    //
    {"VgridIsOk"        },
    {"VdcIsOk"          },
    {"DbRdy"            },
    //
    {"R_SpdFbkFil"      },
	{"GmPwrGridT2"        },
    {"QregFbk"          },
    //
    {"L_IOffsVld"       },
    {"R_VOffsVld"       },
    {"R_VdcRdy"         },
    {"R_SpdLow"         },
    {"R_SpdHigh"        },
//
    {"R_SpdFbkT2"       },
    {"GmVab"            },
    {"L_Vab"            },
    {"S_Vab"            },
    //{"S_GmVab"        },     (WindDFIG01 Only)
    {"GmVabGrid"        },   //(WindDFIG02 Only)
    {"R_VdcFbk"         },
    //{"R1_VdbBrg"      },     (WindDFIG01 Only)
    //{"R2_VdbBrg"      },     (WindDFIG01 Only)
    //
    {"GmPllState"       },
    {"GmPllW"           },
};
//------------------
void* MakeContractTrender(void){
    void*   pContract;
    printf("  MakeContract Trender..............");
    pContract = MarkStatDLLApi::CreateContract(
                sizeof(ListTrender)/
                sizeof(ListTrender[0]),
                       ListTrender,
                       "TrendFile01.csv");
    return (pContract);
}
//-----------------------------------------------------------------------------
