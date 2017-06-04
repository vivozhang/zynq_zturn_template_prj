if NOT EXIST  .\Work             MKDIR .\Work
if NOT EXIST  .\Work\PreProcess  MKDIR .\Work\PreProcess

set CL=/P /FiWork\PreProcess\  /DPRE_PROC
"%VS140COMNTOOLS%../IDE/devenv.exe" /rebuild debug Mark7TestApp.sln
pause
