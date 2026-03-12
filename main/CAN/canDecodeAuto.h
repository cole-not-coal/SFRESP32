#ifndef CAN_DECODE_AUTO_H
#define CAN_DECODE_AUTO_H

#include <stdint.h>
#include "esp_err.h"
#include "can.h"
#include "esp_twai.h"
#include "esp_twai_onchip.h"

extern bool BRestart;
extern bool BClearMinMax;
extern bool BClearErrors;
extern uint8_t tLastTaskTime1msTelemCar;
extern uint8_t tMaxTaskTime1msTelemCar;
extern uint8_t tLastTaskTime100msTelemCar;
extern uint8_t tMaxTaskTime100msTelemCar;
extern uint8_t tLastTaskTimeBGTelemCar;
extern uint8_t tMaxTaskTimeBGTelemCar;
extern uint16_t tSincePowerUpTelemCar;
extern uint8_t NLastResetReasonTelemCar;
extern uint8_t tLastTaskTime1msTelemPits;
extern uint8_t tMaxTaskTime1msTelemPits;
extern uint8_t tLastTaskTime100msTelemPits;
extern uint8_t tMaxTaskTime100msTelemPits;
extern uint8_t tLastTaskTimeBGTelemPits;
extern uint8_t tMaxTaskTimeBGTelemPits;
extern uint16_t tSincePowerUpTelemPits;
extern uint8_t NLastResetReasonTelemPits;
extern uint8_t tLastTaskTime1msIMDMon;
extern uint8_t tMaxTaskTime1msIMDMon;
extern uint8_t tLastTaskTime100msIMDMon;
extern uint8_t tMaxTaskTime100msIMDMon;
extern uint8_t tLastTaskTimeBGIMDMon;
extern uint8_t tMaxTaskTimeBGIMDMon;
extern uint16_t tSincePowerUpIMDMon;
extern uint8_t NLastResetReasonIMDMon;
extern uint8_t tLastTaskTime1msLogger;
extern uint8_t tMaxTaskTime1msLogger;
extern uint8_t tLastTaskTime100msLogger;
extern uint8_t tMaxTaskTime100msLogger;
extern uint8_t tLastTaskTimeBGLogger;
extern uint8_t tMaxTaskTimeBGLogger;
extern uint16_t tSincePowerUpLogger;
extern uint8_t NLastResetReasonLogger;
extern uint8_t tLastTaskTime1msPDU;
extern uint8_t tMaxTaskTime1msPDU;
extern uint8_t tLastTaskTime100msPDU;
extern uint8_t tMaxTaskTime100msPDU;
extern uint8_t tLastTaskTimeBGPDU;
extern uint8_t tMaxTaskTimeBGPDU;
extern uint16_t tSincePowerUpPDU;
extern uint8_t NLastResetReasonPDU;
extern uint8_t tLastTaskTime1msAPPS;
extern uint8_t tMaxTaskTime1msAPPS;
extern uint8_t tLastTaskTime100msAPPS;
extern uint8_t tMaxTaskTime100msAPPS;
extern uint8_t tLastTaskTimeBGAPPS;
extern uint8_t tMaxTaskTimeBGAPPS;
extern uint16_t tSincePowerUpAPPS;
extern uint8_t NLastResetReasonAPPS;
extern uint8_t tLastTaskTime1msScreen;
extern uint8_t tMaxTaskTime1msScreen;
extern uint8_t tLastTaskTime100msScreen;
extern uint8_t tMaxTaskTime100msScreen;
extern uint8_t tLastTaskTimeBGScreen;
extern uint8_t tMaxTaskTimeBGScreen;
extern uint16_t tSincePowerUpScreen;
extern uint8_t NLastResetReasonScreen;
extern uint8_t tLastTaskTime1msDash;
extern uint8_t tMaxTaskTime1msDash;
extern uint8_t tLastTaskTime100msDash;
extern uint8_t tMaxTaskTime100msDash;
extern uint8_t tLastTaskTimeBGDash;
extern uint8_t tMaxTaskTimeBGDash;
extern uint16_t tSincePowerUpDash;
extern uint8_t NLastResetReasonDash;
extern uint8_t tLastTaskTime1msDyno;
extern uint8_t tMaxTaskTime1msDyno;
extern uint8_t tLastTaskTime100msDyno;
extern uint8_t tMaxTaskTime100msDyno;
extern uint8_t tLastTaskTimeBGDyno;
extern uint8_t tMaxTaskTimeBGDyno;
extern uint16_t tSincePowerUpDyno;
extern uint8_t NLastResetReasonDyno;
extern uint8_t tLastTaskTime1msTempMon;
extern uint8_t tMaxTaskTime1msTempMon;
extern uint8_t tLastTaskTime100msTempMon;
extern uint8_t tMaxTaskTime100msTempMon;
extern uint8_t tLastTaskTimeBGTempMon;
extern uint8_t tMaxTaskTimeBGTempMon;
extern uint16_t tSincePowerUpTempMon;
extern uint8_t NLastResetReasonTempMon;
extern float CMD_TargetAcCurrent;
extern uint8_t CellID;
extern float VCell[112];
extern float RCell[112];
extern bool BBalancingCell[112];
extern float VOpenCell[112];
extern uint8_t CheckSum_CellVoltages;
extern bool BIMDOff;
extern bool BIMDUndervoltage;
extern bool BIMDStarting;
extern bool BIMDSSTGood;
extern bool BIMDDeviceError;
extern bool BIMDGroundConnectionFault;
extern bool BIMDInvalidState;
extern float RIsolation;
extern float CMD_TargetBrakeCurrent;
extern float CMD_TargetSpeed;
extern float rAPPs[2];
extern float rAPPsFinal;
extern bool BThrottleOK;
extern bool BAPPSFail[2];
extern bool BAPPSDrift;
extern float CMD_TargetPosition;
extern float VDynoPressureRaw[3];
extern float VDynoCoolantFlowRaw;
extern float VDynoTempRaw[3];
extern float pDynoPressure[3];
extern float VDynoCoolantFlow;
extern float TDynoTemp[3];
<<<<<<< HEAD
extern float rFanDutyManual;
extern float rPumpDutyManual;
extern uint8_t NFanMode;
extern uint8_t NPumpMode;
=======
>>>>>>> 4d16582 (Fixed Screen)
extern float CMD_TargetRelativeCurrent;
extern float CMD_TargeRelativeBrakeCurrent;
extern bool CMD_SetDigOutput[4];
extern float CMD_MaxAcCurrent;
extern float CMD_MaxAcBrakeCurrent;
extern float CMD_MaxDcCurrent;
extern float CMD_MaxDcBrakeCurrent;
extern uint8_t CMD_DriveEnable;
extern float TFRTireChannel[16];
extern float TFLTireChannel[16];
extern float TRRTireChannel[16];
extern float TRLTireChannel[16];
extern uint8_t ControlMode;
extern float TargetIq;
extern float MotorPosition;
extern uint8_t isMotorStill;
extern float Actual_ERPM;
extern float Actual_Duty;
extern float Actual_InputVoltage;
extern float Actual_ACCurrent;
extern float Actual_DCCurrent;
extern float Actual_TempController;
extern float Actual_TempMotor;
extern uint8_t Actual_FaultCode;
extern float Actual_FOC_id;
extern float Actual_FOC_iq;
extern float Actual_Throttle;
extern float Actual_Brake;
extern bool Digital_output_[4];
extern bool Digital_input_[4];
extern uint8_t Drive_enable;
extern bool Motor_temp_limit;
extern bool Motor_accel_limit;
extern bool Input_voltage_limit;
extern bool IGBT_temp_limit;
extern bool IGBT_accel_limit;
extern bool Drive_enable_limit;
extern bool DC_current_limit;
extern bool Capacitor_temp_limit;
extern bool Power_limit;
extern bool RPM_max_limit;
extern bool RPM_min_limit;
extern uint8_t CAN_map_version;
extern float MaxAcCurrent;
extern float AvailableMaxAcCurrent;
extern float MinAcCurrent;
extern float AvailableMinAcCurrent;
extern float MaxDcCurrent;
extern float AvailableMaxDcCurrent;
extern float MinDcCurrent;
extern float AvailableMinDcCurrent;
extern float Pack_Current;
extern float Pack_Inst_Voltage;
extern float Pack_SOC;
extern float Pack_Resistance;
extern uint8_t CheckSum_CellStats1;
extern float Pack_CCL;
extern float Pack_DCL;
extern float Pack_DOD;
extern float Pack_Open_Voltage;
extern float Maximum_Pack_Voltage;
extern float Minimum_Pack_Voltage;
extern float High_Cell_Voltage;
extern float Avg_Cell_Voltage;
extern float Low_Cell_Voltage;
extern uint8_t High_Cell_Voltage_ID;
extern uint8_t Low_Cell_Voltage_ID;
extern float High_Opencell_Voltage;
extern float Avg_Opencell_Voltage;
extern float Low_Opencell_Voltage;
extern uint8_t High_Opencell_ID;
extern uint8_t Low_Opencell_ID;
extern float High_Cell_Resistance;
extern float Avg_Cell_Resistance;
extern float Low_Cell_Resistance;
extern uint8_t High_Intres_ID;
extern uint8_t Low_Intres_ID;
extern float Maximum_Cell_Voltage;
extern bool DTC_P0A08_Charger_Safety_Relay_Fault;
extern uint16_t NTCellID;
extern int8_t TCell[110];
extern int8_t TCellMin;
extern int8_t TCellMax;
extern uint8_t NTCellMaxID;
extern uint8_t NTCellMinID;
extern uint8_t NTempMonNumber;
extern int8_t TCellAvg;
extern uint8_t NCellTemps;
extern uint8_t CheckSum_BMSCellTemp;
extern uint32_t NTempMonJ1939Address;
extern uint8_t NTempMonTargetAddress;

#define ESPCONTROL_ID 0x10
#define MCUSTATUSTELEMCAR_ID 0x11
#define MCUSTATUSTELEMPITS_ID 0x12
#define MCUSTATUSIMDMONITOR_ID 0x13
#define MCUSTATUSLOGGER_ID 0x14
#define MCUSTATUSPDU_ID 0x15
#define STATUSAPPS_ID 0x16
#define MCUSTATUSSCREEN_ID 0x17
#define MCUSTATUSDASH_ID 0x18
#define MCUSTATUSDYNO_ID 0x19
#define MCUSTATUSTEMPMON_ID 0x1A
#define SETACCURRENT_ID 0x24
#define CELLVOLTAGES_ID 0x36
#define IMDDATA_ID 0x40
#define SETBRAKECURRENT_ID 0x44
#define SETERPM_ID 0x64
#define STATUSAPPSSENSOR_ID 0x81
#define SETPOSITION_ID 0x84
#define DYNOPRESSURESRAW_ID 0x90
#define DYNOTEMPSRAW_ID 0x91
#define DYNOPRESSURES_ID 0x92
#define DYNOTEMPS_ID 0x93
<<<<<<< HEAD
#define DYNOCOOLING_ID 0x94
=======
>>>>>>> 4d16582 (Fixed Screen)
#define SETRELCURRENT_ID 0xA4
#define SETRELBRAKECURRENT_ID 0xC4
#define SETDIGOUTPUT_ID 0xE4
#define SETMAXACCURRENT_ID 0x104
#define SETMAXACBRAKECURRENT_ID 0x124
#define SETMAXDCCURRENT_ID 0x144
#define SETMAXDCBRAKECURRENT_ID 0x164
#define SETDRIVEENABLE_ID 0x184
#define FRTIRETEMP1_ID 0x200
#define FRTIRETEMP2_ID 0x201
#define FRTIRETEMP3_ID 0x202
#define FRTIRETEMP4_ID 0x203
#define FLTIRETEMP1_ID 0x204
#define FLTIRETEMP2_ID 0x205
#define FLTIRETEMP3_ID 0x206
#define FLTIRETEMP4_ID 0x207
#define RRTIRETEMP1_ID 0x208
#define RRTIRETEMP2_ID 0x209
#define RRTIRETEMP3_ID 0x20A
#define RRTIRETEMP4_ID 0x20B
#define RLTIRETEMP1_ID 0x20C
#define RLTIRETEMP2_ID 0x20D
#define RLTIRETEMP3_ID 0x20E
#define RLTIRETEMP4_ID 0x20F
#define TARGETIQINFO_ID 0x3E4
#define ERPM_DUTY_VOLTAGE_ID 0x404
#define AC_DC_CURRENT_ID 0x424
#define TEMPERATURES_ID 0x444
#define FOC_ID 0x464
#define INVERTER_MISC_ID 0x484
#define MINMAXACCURRENT_ID 0x4A4
#define MINMAXDCCURRENT_ID 0x4C4
#define CELLSTATS1_ID 0x6B0
#define CELLSTATS2_ID 0x6B1
#define CELLSTATS3_ID 0x6B2
#define CELLSTATS4_ID 0x6B3
#define CELLSTATS5_ID 0x6B4
#define ELCONINTERFACE2_ID 0x1806E5F4
#define ELCONINTERFACE1_ID 0x1806E7F4
#define ELCONINTERFACE3_ID 0x1806E9F4
#define CELLTEMPGENERAL_ID 0x1838F380
#define BMSCELLTEMP_ID 0x1839F380
#define TEMPMONADDRESSCAST_ID 0x18EEFF80

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 4d16582 (Fixed Screen)
esp_err_t ESPControlRx(CAN_frame_t stFrame);
esp_err_t ESPControlTx(twai_node_handle_t stCANBus);
esp_err_t MCUStatusTelemCarRx(CAN_frame_t stFrame);
esp_err_t MCUStatusTelemCarTx(twai_node_handle_t stCANBus);
esp_err_t MCUStatusTelemPitsRx(CAN_frame_t stFrame);
esp_err_t MCUStatusTelemPitsTx(twai_node_handle_t stCANBus);
esp_err_t MCUStatusIMDMonitorRx(CAN_frame_t stFrame);
esp_err_t MCUStatusIMDMonitorTx(twai_node_handle_t stCANBus);
esp_err_t MCUStatusLoggerRx(CAN_frame_t stFrame);
esp_err_t MCUStatusLoggerTx(twai_node_handle_t stCANBus);
esp_err_t MCUStatusPDURx(CAN_frame_t stFrame);
esp_err_t MCUStatusPDUTx(twai_node_handle_t stCANBus);
esp_err_t StatusAPPSRx(CAN_frame_t stFrame);
esp_err_t StatusAPPSTx(twai_node_handle_t stCANBus);
esp_err_t MCUStatusScreenRx(CAN_frame_t stFrame);
esp_err_t MCUStatusScreenTx(twai_node_handle_t stCANBus);
esp_err_t MCUStatusDashRx(CAN_frame_t stFrame);
esp_err_t MCUStatusDashTx(twai_node_handle_t stCANBus);
esp_err_t MCUStatusDynoRx(CAN_frame_t stFrame);
esp_err_t MCUStatusDynoTx(twai_node_handle_t stCANBus);
esp_err_t MCUStatusTempMonRx(CAN_frame_t stFrame);
esp_err_t MCUStatusTempMonTx(twai_node_handle_t stCANBus);
esp_err_t SetAcCurrentRx(CAN_frame_t stFrame);
esp_err_t SetAcCurrentTx(twai_node_handle_t stCANBus);
esp_err_t CellVoltagesRx(CAN_frame_t stFrame);
esp_err_t CellVoltagesTx(twai_node_handle_t stCANBus);
esp_err_t IMDDataRx(CAN_frame_t stFrame);
esp_err_t IMDDataTx(twai_node_handle_t stCANBus);
esp_err_t SetBrakeCurrentRx(CAN_frame_t stFrame);
esp_err_t SetBrakeCurrentTx(twai_node_handle_t stCANBus);
esp_err_t SetERPMRx(CAN_frame_t stFrame);
esp_err_t SetERPMTx(twai_node_handle_t stCANBus);
esp_err_t StatusAPPSSensorRx(CAN_frame_t stFrame);
esp_err_t StatusAPPSSensorTx(twai_node_handle_t stCANBus);
esp_err_t SetPositionRx(CAN_frame_t stFrame);
esp_err_t SetPositionTx(twai_node_handle_t stCANBus);
esp_err_t DynoPressuresRawRx(CAN_frame_t stFrame);
esp_err_t DynoPressuresRawTx(twai_node_handle_t stCANBus);
esp_err_t DynoTempsRawRx(CAN_frame_t stFrame);
esp_err_t DynoTempsRawTx(twai_node_handle_t stCANBus);
esp_err_t DynoPressuresRx(CAN_frame_t stFrame);
esp_err_t DynoPressuresTx(twai_node_handle_t stCANBus);
esp_err_t DynoTempsRx(CAN_frame_t stFrame);
esp_err_t DynoTempsTx(twai_node_handle_t stCANBus);
<<<<<<< HEAD
esp_err_t DynoCoolingRx(CAN_frame_t stFrame);
esp_err_t DynoCoolingTx(twai_node_handle_t stCANBus);
=======
>>>>>>> 4d16582 (Fixed Screen)
esp_err_t SetRelCurrentRx(CAN_frame_t stFrame);
esp_err_t SetRelCurrentTx(twai_node_handle_t stCANBus);
esp_err_t SetRelBrakeCurrentRx(CAN_frame_t stFrame);
esp_err_t SetRelBrakeCurrentTx(twai_node_handle_t stCANBus);
esp_err_t SetDigOutputRx(CAN_frame_t stFrame);
esp_err_t SetDigOutputTx(twai_node_handle_t stCANBus);
esp_err_t SetMaxAcCurrentRx(CAN_frame_t stFrame);
esp_err_t SetMaxAcCurrentTx(twai_node_handle_t stCANBus);
esp_err_t SetMaxAcBrakeCurrentRx(CAN_frame_t stFrame);
esp_err_t SetMaxAcBrakeCurrentTx(twai_node_handle_t stCANBus);
esp_err_t SetMaxDcCurrentRx(CAN_frame_t stFrame);
esp_err_t SetMaxDcCurrentTx(twai_node_handle_t stCANBus);
esp_err_t SetMaxDcBrakeCurrentRx(CAN_frame_t stFrame);
esp_err_t SetMaxDcBrakeCurrentTx(twai_node_handle_t stCANBus);
esp_err_t SetDriveEnableRx(CAN_frame_t stFrame);
esp_err_t SetDriveEnableTx(twai_node_handle_t stCANBus);
esp_err_t FRTireTemp1Rx(CAN_frame_t stFrame);
esp_err_t FRTireTemp1Tx(twai_node_handle_t stCANBus);
esp_err_t FRTireTemp2Rx(CAN_frame_t stFrame);
esp_err_t FRTireTemp2Tx(twai_node_handle_t stCANBus);
esp_err_t FRTireTemp3Rx(CAN_frame_t stFrame);
esp_err_t FRTireTemp3Tx(twai_node_handle_t stCANBus);
esp_err_t FRTireTemp4Rx(CAN_frame_t stFrame);
esp_err_t FRTireTemp4Tx(twai_node_handle_t stCANBus);
esp_err_t FLTireTemp1Rx(CAN_frame_t stFrame);
esp_err_t FLTireTemp1Tx(twai_node_handle_t stCANBus);
esp_err_t FLTireTemp2Rx(CAN_frame_t stFrame);
esp_err_t FLTireTemp2Tx(twai_node_handle_t stCANBus);
esp_err_t FLTireTemp3Rx(CAN_frame_t stFrame);
esp_err_t FLTireTemp3Tx(twai_node_handle_t stCANBus);
esp_err_t FLTireTemp4Rx(CAN_frame_t stFrame);
esp_err_t FLTireTemp4Tx(twai_node_handle_t stCANBus);
esp_err_t RRTireTemp1Rx(CAN_frame_t stFrame);
esp_err_t RRTireTemp1Tx(twai_node_handle_t stCANBus);
esp_err_t RRTireTemp2Rx(CAN_frame_t stFrame);
esp_err_t RRTireTemp2Tx(twai_node_handle_t stCANBus);
esp_err_t RRTireTemp3Rx(CAN_frame_t stFrame);
esp_err_t RRTireTemp3Tx(twai_node_handle_t stCANBus);
esp_err_t RRTireTemp4Rx(CAN_frame_t stFrame);
esp_err_t RRTireTemp4Tx(twai_node_handle_t stCANBus);
esp_err_t RLTireTemp1Rx(CAN_frame_t stFrame);
esp_err_t RLTireTemp1Tx(twai_node_handle_t stCANBus);
esp_err_t RLTireTemp2Rx(CAN_frame_t stFrame);
esp_err_t RLTireTemp2Tx(twai_node_handle_t stCANBus);
esp_err_t RLTireTemp3Rx(CAN_frame_t stFrame);
esp_err_t RLTireTemp3Tx(twai_node_handle_t stCANBus);
esp_err_t RLTireTemp4Rx(CAN_frame_t stFrame);
esp_err_t RLTireTemp4Tx(twai_node_handle_t stCANBus);
esp_err_t TargetIqInfoRx(CAN_frame_t stFrame);
esp_err_t TargetIqInfoTx(twai_node_handle_t stCANBus);
esp_err_t ERPM_DUTY_VOLTAGERx(CAN_frame_t stFrame);
esp_err_t ERPM_DUTY_VOLTAGETx(twai_node_handle_t stCANBus);
esp_err_t AC_DC_currentRx(CAN_frame_t stFrame);
esp_err_t AC_DC_currentTx(twai_node_handle_t stCANBus);
esp_err_t TemperaturesRx(CAN_frame_t stFrame);
esp_err_t TemperaturesTx(twai_node_handle_t stCANBus);
esp_err_t FOCRx(CAN_frame_t stFrame);
esp_err_t FOCTx(twai_node_handle_t stCANBus);
esp_err_t Inverter_MISCRx(CAN_frame_t stFrame);
esp_err_t Inverter_MISCTx(twai_node_handle_t stCANBus);
esp_err_t MinMaxAcCurrentRx(CAN_frame_t stFrame);
esp_err_t MinMaxAcCurrentTx(twai_node_handle_t stCANBus);
esp_err_t MinMaxDcCurrentRx(CAN_frame_t stFrame);
esp_err_t MinMaxDcCurrentTx(twai_node_handle_t stCANBus);
esp_err_t CellStats1Rx(CAN_frame_t stFrame);
esp_err_t CellStats1Tx(twai_node_handle_t stCANBus);
esp_err_t CellStats2Rx(CAN_frame_t stFrame);
esp_err_t CellStats2Tx(twai_node_handle_t stCANBus);
esp_err_t CellStats3Rx(CAN_frame_t stFrame);
esp_err_t CellStats3Tx(twai_node_handle_t stCANBus);
esp_err_t CellStats4Rx(CAN_frame_t stFrame);
esp_err_t CellStats4Tx(twai_node_handle_t stCANBus);
esp_err_t CellStats5Rx(CAN_frame_t stFrame);
esp_err_t CellStats5Tx(twai_node_handle_t stCANBus);
esp_err_t ElconInterface2Rx(CAN_frame_t stFrame);
esp_err_t ElconInterface2Tx(twai_node_handle_t stCANBus);
esp_err_t ElconInterface1Rx(CAN_frame_t stFrame);
esp_err_t ElconInterface1Tx(twai_node_handle_t stCANBus);
esp_err_t ElconInterface3Rx(CAN_frame_t stFrame);
esp_err_t ElconInterface3Tx(twai_node_handle_t stCANBus);
esp_err_t CellTempGeneralRx(CAN_frame_t stFrame);
esp_err_t CellTempGeneralTx(twai_node_handle_t stCANBus);
esp_err_t BMSCellTempRx(CAN_frame_t stFrame);
esp_err_t BMSCellTempTx(twai_node_handle_t stCANBus);
esp_err_t TempMonAddressCastRx(CAN_frame_t stFrame);
esp_err_t TempMonAddressCastTx(twai_node_handle_t stCANBus);
<<<<<<< HEAD
=======
esp_err_t ESPControl(CAN_frame_t stFrame, uint8_t* BRestart, uint8_t* BClearMinMax, uint8_t* BClearErrors);
esp_err_t MCUStatusTelemCar(CAN_frame_t stFrame, float* tLastTaskTime1ms, float* tMaxTaskTime1ms, float* tLastTaskTime100ms, float* tMaxTaskTime100ms, float* tLastTaskTimeBG, float* tMaxTaskTimeBG, float* tSincePowerUp, uint8_t* NLastResetReason);
esp_err_t MCUStatusTelemPits(CAN_frame_t stFrame, float* tLastTaskTime1ms, float* tMaxTaskTime1ms, float* tLastTaskTime100ms, float* tMaxTaskTime100ms, float* tLastTaskTimeBG, float* tMaxTaskTimeBG, float* tSincePowerUp, uint8_t* NLastResetReason);
esp_err_t MCUStatusIMDMonitor(CAN_frame_t stFrame, float* tLastTaskTime1ms, float* tMaxTaskTime1ms, float* tLastTaskTime100ms, float* tMaxTaskTime100ms, float* tLastTaskTimeBG, float* tMaxTaskTimeBG, float* tSincePowerUp, uint8_t* NLastResetReason);
esp_err_t MCUStatusLogger(CAN_frame_t stFrame, float* tLastTaskTime1ms, float* tMaxTaskTime1ms, float* tLastTaskTime100ms, float* tMaxTaskTime100ms, float* tLastTaskTimeBG, float* tMaxTaskTimeBG, float* tSincePowerUp, uint8_t* NLastResetReason);
esp_err_t MCUStatusPDU(CAN_frame_t stFrame, float* tLastTaskTime1ms, float* tMaxTaskTime1ms, float* tLastTaskTime100ms, float* tMaxTaskTime100ms, float* tLastTaskTimeBG, float* tMaxTaskTimeBG, float* tSincePowerUp, uint8_t* NLastResetReason);
esp_err_t StatusAPPS(CAN_frame_t stFrame, float* tLastTaskTime1ms, float* tMaxTaskTime1ms, float* tLastTaskTime100ms, float* tMaxTaskTime100ms, float* tLastTaskTimeBG, float* tMaxTaskTimeBG, float* tSincePowerUp, uint8_t* NLastResetReason);
esp_err_t MCUStatusScreen(CAN_frame_t stFrame, float* tLastTaskTime1ms, float* tMaxTaskTime1ms, float* tLastTaskTime100ms, float* tMaxTaskTime100ms, float* tLastTaskTimeBG, float* tMaxTaskTimeBG, float* tSincePowerUp, uint8_t* NLastResetReason);
esp_err_t MCUStatusDash(CAN_frame_t stFrame, float* tLastTaskTime1ms, float* tMaxTaskTime1ms, float* tLastTaskTime100ms, float* tMaxTaskTime100ms, float* tLastTaskTimeBG, float* tMaxTaskTimeBG, float* tSincePowerUp, uint8_t* NLastResetReason);
esp_err_t SetCurrent(CAN_frame_t stFrame, float* ACCurrentSET);
esp_err_t IMDData(CAN_frame_t stFrame, uint8_t* BIMDOff, uint8_t* BIMDUndervoltage, uint8_t* BIMDStarting, uint8_t* BIMDSSTGood, uint8_t* BIMDDeviceError, uint8_t* BIMDGroundConnectionFault, uint8_t* BIMDInvalidState, float* RIsolation);
esp_err_t SetBreakCurrent(CAN_frame_t stFrame, float* TargetbrakecurrentSET);
esp_err_t SetERPM(CAN_frame_t stFrame, int32_t* TargetERPMSET);
esp_err_t StatusAPPSSensor(CAN_frame_t stFrame, int8_t* rAPPs1, int8_t* rAPPs2, int8_t* rAPPsFinal, uint8_t* BThrottleOK, uint8_t* BAPPS1Fail, uint8_t* BAPPS2Fail, uint8_t* BAPPSDrift);
esp_err_t SetPostion(CAN_frame_t stFrame, float* TargetPositionSET);
esp_err_t SetRelativeCurrent(CAN_frame_t stFrame, float* TargetrelativeACcurrentSET);
esp_err_t SetRelativeBreak(CAN_frame_t stFrame, float* RelativeBrakeACCurrent);
esp_err_t SetDigOut(CAN_frame_t stFrame, uint8_t* DigitalOutput1, uint8_t* DigitalOutput2, uint8_t* DigitalOutput3, uint8_t* DigitalOutput4);
esp_err_t SetCurrentMaxAC(CAN_frame_t stFrame, float* MaxACCurrentSET);
esp_err_t SetBreakCurrentMax(CAN_frame_t stFrame, float* MaxbrakeACcurrentSET);
esp_err_t SetCurrentMaxDC(CAN_frame_t stFrame, float* MaxDCcurrentSET);
esp_err_t SetBreakCurrentMaxDC(CAN_frame_t stFrame, float* MaximumbrakeDCcurrentSET);
esp_err_t DriveEnable(CAN_frame_t stFrame, uint8_t* Driveenable);
esp_err_t FRTireTemp1(CAN_frame_t stFrame, float* TFRTireChannel01, float* TFRTireChannel02, float* TFRTireChannel03, float* TFRTireChannel04);
esp_err_t FRTireTemp2(CAN_frame_t stFrame, float* TFRTireChannel05, float* TFRTireChannel06, float* TFRTireChannel07, float* TFRTireChannel08);
esp_err_t FRTireTemp3(CAN_frame_t stFrame, float* TFRTireChannel09, float* TFRTireChannel10, float* TFRTireChannel11, float* TFRTireChannel12);
esp_err_t FRTireTemp4(CAN_frame_t stFrame, float* TFRTireChannel13, float* TFRTireChannel14, float* TFRTireChannel15, float* TFRTireChannel16);
esp_err_t FLTireTemp1(CAN_frame_t stFrame, float* TFLTireChannel01, float* TFLTireChannel02, float* TFLTireChannel03, float* TFLTireChannel04);
esp_err_t FLTireTemp2(CAN_frame_t stFrame, float* TFLTireChannel05, float* TFLTireChannel06, float* TFLTireChannel07, float* TFLTireChannel08);
esp_err_t FLTireTemp3(CAN_frame_t stFrame, float* TFLTireChannel09, float* TFLTireChannel10, float* TFLTireChannel11, float* TFLTireChannel12);
esp_err_t FLTireTemp4(CAN_frame_t stFrame, float* TFLTireChannel13, float* TFLTireChannel14, float* TFLTireChannel15, float* TFLTireChannel16);
esp_err_t RRTireTemp1(CAN_frame_t stFrame, float* TRRTireChannel01, float* TRRTireChannel02, float* TRRTireChannel03, float* TRRTireChannel04);
esp_err_t RRTireTemp2(CAN_frame_t stFrame, float* TRRTireChannel05, float* TRRTireChannel06, float* TRRTireChannel07, float* TRRTireChannel08);
esp_err_t RRTireTemp3(CAN_frame_t stFrame, float* TRRTireChannel09, float* TRRTireChannel10, float* TRRTireChannel11, float* TRRTireChannel12);
esp_err_t RRTireTemp4(CAN_frame_t stFrame, float* TRRTireChannel13, float* TRRTireChannel14, float* TRRTireChannel15, float* TRRTireChannel16);
esp_err_t RLTireTemp1(CAN_frame_t stFrame, float* TRLTireChannel01, float* TRLTireChannel02, float* TRLTireChannel03, float* TRLTireChannel04);
esp_err_t RLTireTemp2(CAN_frame_t stFrame, float* TRLTireChannel05, float* TRLTireChannel06, float* TRLTireChannel07, float* TRLTireChannel08);
esp_err_t RLTireTemp3(CAN_frame_t stFrame, float* TRLTireChannel09, float* TRLTireChannel10, float* TRLTireChannel11, float* TRLTireChannel12);
esp_err_t RLTireTemp4(CAN_frame_t stFrame, float* TRLTireChannel13, float* TRLTireChannel14, float* TRLTireChannel15, float* TRLTireChannel16);
esp_err_t InverterData1(CAN_frame_t stFrame, int32_t* ERPM, float* DutyCycle, int16_t* InputVoltage);
esp_err_t InverterData2(CAN_frame_t stFrame, float* ACCurrent, float* DCCurrent);
esp_err_t InverterData3(CAN_frame_t stFrame, float* ControllerTemperature, float* MotorTemperature, uint8_t* FaultCode);
esp_err_t InverterData4(CAN_frame_t stFrame, float* Id, float* Iq);
esp_err_t InverterData5(CAN_frame_t stFrame, int8_t* ThrottleSignal, int8_t* BrakeSignal, uint8_t* DigitalIn1, uint8_t* DigitalIn2, uint8_t* DigitalIn3, uint8_t* DigitalIn4, uint8_t* DigitalOut1, uint8_t* DigitalOut2, uint8_t* DigitalOut3, uint8_t* DigitalOut4, uint8_t* DriveEnable, uint8_t* CapacitorTempLimit, uint8_t* DCCurrentLimit, uint8_t* DriveEnableLimit, uint8_t* IGBTAccelerationLimit, uint8_t* IGBTTemperatureLimit, uint8_t* InputVoltageLimit, uint8_t* TempAccelThrottle, uint8_t* MotorTemperatureLimit, uint8_t* RPMminlimit, uint8_t* RPMMaxlimit, uint8_t* Powerlimit, uint8_t* CANmapversion);
esp_err_t CellVoltages1(CAN_frame_t stFrame, uint8_t* NCellID, float* VCell, float* RCellInternal, float* VCellOC, uint8_t* Checksum);
esp_err_t BMSData1(CAN_frame_t stFrame, float* IPack, float* VPack, uint8_t* rPackSOC, uint16_t* NRelayState, float* Checksum);
esp_err_t BMSData2(CAN_frame_t stFrame, uint16_t* IPackDischargeLimit, uint8_t* THigh, uint8_t* LowTemperature, float* Checksum);
esp_err_t PackVoltage(CAN_frame_t stFrame, float* VPackOpenCircuit, float* VPackMax, float* VPackMin, float* VBMSSupply12V);
esp_err_t PackTemperature(CAN_frame_t stFrame, float* TPackHigh, float* TPackLow, float* TPackAvg, float* TBMSInternal);
esp_err_t CellVoltages2(CAN_frame_t stFrame, float* VCellLow, float* VCellHigh, float* VCellAvg, float* VCellLowOC);
esp_err_t CellVoltages3(CAN_frame_t stFrame, float* VCellHighOC, float* VCellAvgOC, float* VCellMaximum, float* VCellMinimum, uint8_t* NVCellMaxID, uint8_t* NVCellMinID);
>>>>>>> 03aad6a (CAN message decode script)
=======
>>>>>>> 4d16582 (Fixed Screen)

#endif
