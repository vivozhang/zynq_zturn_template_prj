/*=============================================================================
* Product:     GE Power Conversion Control Using MACC
* Host:        MACC Running TMS320C31
* File:        Y20RTHM.H
* Language:    TI 'C'
* Copyright:   2003.... GE Drives and Controls, Inc., Salem VA
*
* Abstract:  Constants, function prototypes for bridge thermal model.
*
* Functions in module:  None
*
*==============================================================================
* 030214 SAB  Initial create.
* 030215 SAB  Add prototype for TempConversionT2
* 030218 SAB  Add 5 move externs for temp vco registers.
* 030224 SAB  Define BtmOTFlgs
* 032228 SAB  Add ambient temp register externs
* 030305 SAB  Use only one hardware mapped vco temperature register
* 030310 SAB  Remove DSP temperature select
* 040107 SAB  Remove AmbTempTable
* 040224 SAB  Remove MACC MODE condtionals
* 050408 IB   Add base plate alarm data structure
* 061206 SAB  Add InletAirTemp, PDCTemp, CFCTemp, BrgCoolTemp
* 070102 SAB  Add Lw to all TempTh1,2,7,8 vars and create Hi vars
* 070108 SAB  Add temp tables for thermal I/O thermsisters 1-4.
* 070111 SAB  Increase table size for TempTabLowT1 from 236 to 246
*             and TempTabLowT2 from 144 to 164
* 070124 IB   Add references to support thermal protection based on thermister
* 070130 SAB  Add TempIOFbkVld
* 070208 SAB  BrgAmbTemp --> MACCAmbTemp
* 070209 IB   Add MACCOTAlmAct.
* 070217 SAB  Add OTTrpDlyOpen
* 070316 IB   Add vars an a function to implement temperature delayed trips
* 070403 SAB  Add four delta resistance alarm flags
* 070409 IB   Rename OTTrpDlyOpen as TherBadDlySm. Add four ThrmOpen flags and
*             coolant under temperature timer.
* 070809 IB   Add CoolOTAlmAct
* 070828 IB   Add TTripPend for IONET interface
* 111110 SAB  Define "TEMP_DATA_PTS_PLUS" and use it to define TempTable
*             instead of "129"
*==============================================================================
* Last Mod: August 28, 2007 09:01:45AM
*==============================================================================
*/

#ifndef __Y0BRTHM_H__
#define __Y0BRTHM_H__

#endif

/******************************************************************************
* Function Prototypes
******************************************************************************/

extern   void AmbientTemperatureProtection(void);
extern   void IOThermalConversion(void);
extern   void ThermistBasedProt(void);
extern   void TemperatureShutdownT3(void);
extern   void ThermistRunPermissive(void);
extern   void ThermalOpenFlg(void);
extern   void ThermalShrtFlg(void);
extern   void SealInSuicide(void);
extern   void ThermistTempOkCheck(void);
extern   void TempValidCheck(void);
