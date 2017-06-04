##############################################################################
#
#  TITLE:       MarkStat Version Definition
#
##############################################################################
#
# Product Version Definition
#===========================
# PRODUCT_NAME is for product identification in Sys for ToolboxST, TFS (Area Path) and as the directory name, no spaces
PRODUCT_NAME        = WindDFIG02
# PRODUCT_NAME_SPACES is for Labels and Release Notes
PRODUCT_NAME_SPACES = WindDFIG02
# Product.txt in the Product directory should have the PRODUCT_ALIAS in the name
# PRODUCT_ALIAS is the Product name the Customer sees on Product Help (40 char is reasonable)
PRODUCT_ALIAS       = 690 DFIG
PRODUCT_MONIKER     = 2x DFIG
VER_MAJOR = 01
VER_MINOR = 02
VER_PATCH = 03
STATUS    = E

BUILD_NUM = 00001

PRODUCT_NUM = 1
PARM_SIZE   = 3500
VAR_SIZE    = 9000

# Version Definitions
#====================
#PRODUCT_VER      = V$(VER_MAJOR)$(VER_MINOR)$(VER_PATCH)$(STATUS)
PRODUCT_VER       = Main
CORE_VER          = V13.00.00C-00208
PRODDEF_VER       = 1.22
BOOT_VER_P1       = V020301C
BASE_VER_P1       = V030506C
BOOT_VER_P4       = V030508C
BASE_VER_P4       = V030508C
MARKVIe_HELP_VER  = V04.06
MARKSTAT_HELP_VER = V01.00

# Team Server Directory Definitions
#==================================
TFS_ROOT         = $$/PlatformSoftware/
CORE_TFS_DIR     = V13.00


# Directory Definitions
#======================
CORE_ROOT     = ../../../Core
PRODDEF_ROOT  = ../../../../ProductDefInterfaces

# Partial Build Control definitions
#==================================
ifndef CORE_LIB_LIST
CORE_LIB_LIST=alarmbase appapi adlapi celapi diagapi egdapi fastudp ffapi gesslapi ioappapi nvramapi platformapi ptpapi sdiapi seqapi soeapi syncapi sysapi wfmsioapi xmlparserapi
endif
ifndef CORE_SRC_LIST
CORE_SRC_LIST=adlrouter alarm app cdl cel cs cginicinfo cgisysinfo dhcp diag egd hw hwdog idle llm modbus ntp nvram ptp sdi seq slinger soe syncd sys sysshutdown tunnel wfms
endif
ifndef CPULIST
CPULIST=x86
endif

# Build Features: Delete entry if feature is not supported
#=========================================================
SUPPORTS_BLK_LEGACY   = 1     # Supports the Legacy Block Library
SUPPORTS_BLK_STANDARD = 1     # Supports the Standard Block Library
SUPPORTS_BLK_TURBINE  = 1     # Supports the Turbine Block Library

# Other Defines
#==============
RELEASE_NOTES   = Embedded/Notebooks/WindDFIG02/Tab08 Releases/WindDFIG02 Product Software Release History.docx
CONTROLLER_NAME = Mark Stat Power Conversion
