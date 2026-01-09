#ifndef CAN_DECODE_AUTO_H
#define CAN_DECODE_AUTO_H

#include <stdint.h>
#include "esp_err.h"
#include "can.h"

<<<<<<< HEAD
#define ESPCONTROL_ID 0x10
#define MCUSTATUSTELEMCAR_ID 0x11
#define MCUSTATUSTELEMPITS_ID 0x12
#define MCUSTATUSIMDMONITOR_ID 0x13
#define MCUSTATUSLOGGER_ID 0x14
#define MCUSTATUSPDU_ID 0x15
#define STATUSAPPS_ID 0x16
#define MCUSTATUSSCREEN_ID 0x17
#define MCUSTATUSDASH_ID 0x18
#define SETCURRENT_ID 0x24
#define IMDDATA_ID 0x40
#define SETBREAKCURRENT_ID 0x44
#define SETERPM_ID 0x64
#define STATUSAPPSSENSOR_ID 0x81
#define SETPOSTION_ID 0x84
#define SETRELATIVECURRENT_ID 0xA4
#define SETRELATIVEBREAK_ID 0xC4
#define SETDIGOUT_ID 0xE4
#define SETCURRENTMAXAC_ID 0x104
#define SETBREAKCURRENTMAX_ID 0x124
#define SETCURRENTMAXDC_ID 0x144
#define SETBREAKCURRENTMAXDC_ID 0x164
#define DRIVEENABLE_ID 0x184
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
#define INVERTERDATA1_ID 0x404
#define INVERTERDATA2_ID 0x424
#define INVERTERDATA3_ID 0x444
#define INVERTERDATA4_ID 0x464
#define INVERTERDATA5_ID 0x484
#define CELLVOLTAGES1_ID 0x6A0
#define BMSDATA1_ID 0x6B0
#define BMSDATA2_ID 0x6B1
#define PACKVOLTAGE_ID 0x6B2
#define PACKTEMPERATURE_ID 0x6B3
#define CELLVOLTAGES2_ID 0x6B4
#define CELLVOLTAGES3_ID 0x6B5

esp_err_t ESPControl(CAN_frame_t stFrame, uint8_t* BRestart, uint8_t* BClearMinMax, uint8_t* BClearErrors);
=======
esp_err_t ESPControl(CAN_frame_t stFrame, uint8_t* BRestart, uint8_t* BClearMinMax, uint8_t* BClearErrors, uint16_t* Spare);
>>>>>>> 9105219 (CAN message decode script)
esp_err_t MCUStatusTelemCar(CAN_frame_t stFrame, float* tLastTaskTime1ms, float* tMaxTaskTime1ms, float* tLastTaskTime100ms, float* tMaxTaskTime100ms, float* tLastTaskTimeBG, float* tMaxTaskTimeBG, float* tSincePowerUp, uint8_t* NLastResetReason);
esp_err_t MCUStatusTelemPits(CAN_frame_t stFrame, float* tLastTaskTime1ms, float* tMaxTaskTime1ms, float* tLastTaskTime100ms, float* tMaxTaskTime100ms, float* tLastTaskTimeBG, float* tMaxTaskTimeBG, float* tSincePowerUp, uint8_t* NLastResetReason);
esp_err_t MCUStatusIMDMonitor(CAN_frame_t stFrame, float* tLastTaskTime1ms, float* tMaxTaskTime1ms, float* tLastTaskTime100ms, float* tMaxTaskTime100ms, float* tLastTaskTimeBG, float* tMaxTaskTimeBG, float* tSincePowerUp, uint8_t* NLastResetReason);
esp_err_t MCUStatusLogger(CAN_frame_t stFrame, float* tLastTaskTime1ms, float* tMaxTaskTime1ms, float* tLastTaskTime100ms, float* tMaxTaskTime100ms, float* tLastTaskTimeBG, float* tMaxTaskTimeBG, float* tSincePowerUp, uint8_t* NLastResetReason);
esp_err_t MCUStatusPDU(CAN_frame_t stFrame, float* tLastTaskTime1ms, float* tMaxTaskTime1ms, float* tLastTaskTime100ms, float* tMaxTaskTime100ms, float* tLastTaskTimeBG, float* tMaxTaskTimeBG, float* tSincePowerUp, uint8_t* NLastResetReason);
esp_err_t StatusAPPS(CAN_frame_t stFrame, float* tLastTaskTime1ms, float* tMaxTaskTime1ms, float* tLastTaskTime100ms, float* tMaxTaskTime100ms, float* tLastTaskTimeBG, float* tMaxTaskTimeBG, float* tSincePowerUp, uint8_t* NLastResetReason);
esp_err_t MCUStatusScreen(CAN_frame_t stFrame, float* tLastTaskTime1ms, float* tMaxTaskTime1ms, float* tLastTaskTime100ms, float* tMaxTaskTime100ms, float* tLastTaskTimeBG, float* tMaxTaskTimeBG, float* tSincePowerUp, uint8_t* NLastResetReason);
esp_err_t MCUStatusDash(CAN_frame_t stFrame, float* tLastTaskTime1ms, float* tMaxTaskTime1ms, float* tLastTaskTime100ms, float* tMaxTaskTime100ms, float* tLastTaskTimeBG, float* tMaxTaskTimeBG, float* tSincePowerUp, uint8_t* NLastResetReason);
esp_err_t SetCurrent(CAN_frame_t stFrame, float* ACCurrentSET);
<<<<<<< HEAD
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
=======
esp_err_t IMDData(CAN_frame_t stFrame, uint8_t* BIMDOff, uint8_t* BIMDUndervoltage, uint8_t* BIMDStarting, uint8_t* BIMDSSTGood, uint8_t* BIMDDeviceError, uint8_t* BIMDGroundConnectionFault, uint8_t* BIMDInvalidState, uint8_t* Spare, float* RIsolation);
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
esp_err_t DriveEnable(CAN_frame_t stFrame, uint8_t* Driveenable, uint8_t* RESERVED);
>>>>>>> 9105219 (CAN message decode script)
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
<<<<<<< HEAD
esp_err_t InverterData2(CAN_frame_t stFrame, float* ACCurrent, float* DCCurrent);
esp_err_t InverterData3(CAN_frame_t stFrame, float* ControllerTemperature, float* MotorTemperature, uint8_t* FaultCode);
esp_err_t InverterData4(CAN_frame_t stFrame, float* Id, float* Iq);
esp_err_t InverterData5(CAN_frame_t stFrame, int8_t* ThrottleSignal, int8_t* BrakeSignal, uint8_t* DigitalIn1, uint8_t* DigitalIn2, uint8_t* DigitalIn3, uint8_t* DigitalIn4, uint8_t* DigitalOut1, uint8_t* DigitalOut2, uint8_t* DigitalOut3, uint8_t* DigitalOut4, uint8_t* DriveEnable, uint8_t* CapacitorTempLimit, uint8_t* DCCurrentLimit, uint8_t* DriveEnableLimit, uint8_t* IGBTAccelerationLimit, uint8_t* IGBTTemperatureLimit, uint8_t* InputVoltageLimit, uint8_t* TempAccelThrottle, uint8_t* MotorTemperatureLimit, uint8_t* RPMminlimit, uint8_t* RPMMaxlimit, uint8_t* Powerlimit, uint8_t* CANmapversion);
esp_err_t CellVoltages1(CAN_frame_t stFrame, uint8_t* NCellID, float* VCell, float* RCellInternal, float* VCellOC, uint8_t* Checksum);
esp_err_t BMSData1(CAN_frame_t stFrame, float* IPack, float* VPack, uint8_t* rPackSOC, uint16_t* NRelayState, float* Checksum);
=======
esp_err_t InverterData2(CAN_frame_t stFrame, float* ACCurrent, float* DCCurrent, uint32_t* RESERVED);
esp_err_t InverterData3(CAN_frame_t stFrame, float* ControllerTemperature, float* MotorTemperature, uint8_t* FaultCode, uint32_t* RESERVED);
esp_err_t InverterData4(CAN_frame_t stFrame, float* Id, float* Iq);
esp_err_t InverterData5(CAN_frame_t stFrame, int8_t* ThrottleSignal, int8_t* BrakeSignal, uint8_t* DigitalIn1, uint8_t* DigitalIn2, uint8_t* DigitalIn3, uint8_t* DigitalIn4, uint8_t* DigitalOut1, uint8_t* DigitalOut2, uint8_t* DigitalOut3, uint8_t* DigitalOut4, uint8_t* DriveEnable, uint8_t* CapacitorTempLimit, uint8_t* DCCurrentLimit, uint8_t* DriveEnableLimit, uint8_t* IGBTAccelerationLimit, uint8_t* IGBTTemperatureLimit, uint8_t* InputVoltageLimit, uint8_t* TempAccelThrottle, uint8_t* MotorTemperatureLimit, uint8_t* RPMminlimit, uint8_t* RPMMaxlimit, uint8_t* Powerlimit, uint8_t* CANmapversion);
esp_err_t CellVoltages1(CAN_frame_t stFrame, uint8_t* NCellID, float* VCell, float* RCellInternal, float* VCellOC, uint8_t* Checksum);
esp_err_t BMSData1(CAN_frame_t stFrame, float* IPack, float* PackInstVoltage, uint8_t* PackSOC, uint16_t* RelayState, float* Checksum, float* IPack, float* VPack, uint8_t* rPackSOC, uint16_t* NRelayState, float* Checksum);
>>>>>>> 9105219 (CAN message decode script)
esp_err_t BMSData2(CAN_frame_t stFrame, uint16_t* IPackDischargeLimit, uint8_t* THigh, uint8_t* LowTemperature, float* Checksum);
esp_err_t PackVoltage(CAN_frame_t stFrame, float* VPackOpenCircuit, float* VPackMax, float* VPackMin, float* VBMSSupply12V);
esp_err_t PackTemperature(CAN_frame_t stFrame, float* TPackHigh, float* TPackLow, float* TPackAvg, float* TBMSInternal);
esp_err_t CellVoltages2(CAN_frame_t stFrame, float* VCellLow, float* VCellHigh, float* VCellAvg, float* VCellLowOC);
esp_err_t CellVoltages3(CAN_frame_t stFrame, float* VCellHighOC, float* VCellAvgOC, float* VCellMaximum, float* VCellMinimum, uint8_t* NVCellMaxID, uint8_t* NVCellMinID);

#endif
