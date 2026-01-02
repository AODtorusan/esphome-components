// SPDX-License-Identifier: MIT

// File: dpt.h
// Author: Simon Billemont (Since 2026)

#pragma once

#include <string>
#include <tuple>

namespace esphome {
namespace knx {

/*
See KNX document "System Specifications", Interworking, Datapoint Types for details
*/

// Notations and format
// Symbol   Field
// A        Character
// A[n]     String of n characters
// B        Boolean / Bit set
// C        Control
// E        Exponent
// F        Floating point value
// M        Mantissa
// N        eNumeration
// r        Reserved bit or field
// S        Sign
// U        Unsigned value
// V        2's Complement signed value
// Z8       Standardised Status/Command B8. Encoding as in DPT_StatusGen

enum KnxDptFormat {
  KNX_DPT_FORMAT_B1 = 0,
  KNX_DPT_FORMAT_B2,
  KNX_DPT_FORMAT_B1U3,
  KNX_DPT_FORMAT_A8,
  KNX_DPT_FORMAT_U8,
  KNX_DPT_FORMAT_V8,
  KNX_DPT_FORMAT_B5N3,
  KNX_DPT_FORMAT_U16,
  KNX_DPT_FORMAT_V16,
  KNX_DPT_FORMAT_F16,
  KNX_DPT_FORMAT_N3N5R2N6R2N6,
  KNX_DPT_FORMAT_R3N5R4N4R1U7,
  KNX_DPT_FORMAT_U32,
  KNX_DPT_FORMAT_V32,
  KNX_DPT_FORMAT_F32,
  // KNX_DPT_FORMAT_U4U4U4U4U4U4B4N4,
  // KNX_DPT_FORMAT_A112,
  // KNX_DPT_FORMAT_R2U6,
  // KNX_DPT_FORMAT_B1R1U6,
  // KNX_DPT_FORMAT_U8R4U4R3U5U3U5R2U6B16,
  // KNX_DPT_FORMAT_N8,
  // KNX_DPT_FORMAT_B8,
  // KNX_DPT_FORMAT_B16,
  // KNX_DPT_FORMAT_N2,
  // KNX_DPT_FORMAT_AN,
  // KNX_DPT_FORMAT_U4U4,
  // KNX_DPT_FORMAT_R1B1U6,
  // KNX_DPT_FORMAT_B32,
  // KNX_DPT_FORMAT_V64,
  // KNX_DPT_FORMAT_B24,
  // KNX_DPT_FORMAT_N3,
  // KNX_DPT_FORMAT_B1Z8,
  // KNX_DPT_FORMAT_N8Z8,
  // KNX_DPT_FORMAT_U8Z8,
  // KNX_DPT_FORMAT_U16Z8,
  // KNX_DPT_FORMAT_V8Z8,
  // KNX_DPT_FORMAT_V16Z8,
  // KNX_DPT_FORMAT_U16N8,
  // KNX_DPT_FORMAT_U8B8,
  // KNX_DPT_FORMAT_V16B8,
  // KNX_DPT_FORMAT_V16B16,
  // KNX_DPT_FORMAT_U8N8,
  // KNX_DPT_FORMAT_V16V16V16,
  // KNX_DPT_FORMAT_V16V16V16V16,
  // KNX_DPT_FORMAT_V16U8B8,
  // KNX_DPT_FORMAT_V16U8B16,
  // KNX_DPT_FORMAT_U16U8N8N8P8,
  // KNX_DPT_FORMAT_U5U5U16,
  // KNX_DPT_FORMAT_V32Z8,
  // KNX_DPT_FORMAT_U8N8N8N8B8B8,
  // KNX_DPT_FORMAT_U16V16,
  // KNX_DPT_FORMAT_N16U32,
  // KNX_DPT_FORMAT_F16F16F16,
  // KNX_DPT_FORMAT_V8N8N8,
  // KNX_DPT_FORMAT_V16V16N8N8,
  // KNX_DPT_FORMAT_U16U8,
  // KNX_DPT_FORMAT_V32N8Z8,
  // KNX_DPT_FORMAT_U16U32U8N8,
  // KNX_DPT_FORMAT_A8A8A8A8,
  // KNX_DPT_FORMAT_U8U8U8,
  // KNX_DPT_FORMAT_A8A8
};


// Define all the KNX Datapoint IDs
enum KnxDpt {
  KNX_DPT_1_001, // 1.001 B1 DPT_Switch
  KNX_DPT_1_002, // 1.002 B1 DPT_Bool
  KNX_DPT_1_003, // 1.003 B1 DPT_Enable
  KNX_DPT_1_004, // 1.004 B1 DPT_Ramp
  KNX_DPT_1_005, // 1.005 B1 DPT_Alarm
  KNX_DPT_1_006, // 1.006 B1 DPT_BinaryValue
  KNX_DPT_1_007, // 1.007 B1 DPT_Step
  KNX_DPT_1_008, // 1.008 B1 DPT_UpDown
  KNX_DPT_1_009, // 1.009 B1 DPT_OpenClose
  KNX_DPT_1_010, // 1.010 B1 DPT_Start
  KNX_DPT_1_011, // 1.011 B1 DPT_State
  KNX_DPT_1_012, // 1.012 B1 DPT_Invert
  KNX_DPT_1_013, // 1.013 B1 DPT_DimSendStyle
  KNX_DPT_1_014, // 1.014 B1 DPT_InputSource
  KNX_DPT_1_015, // 1.015 B1 DPT_Reset
  KNX_DPT_1_016, // 1.016 B1 DPT_Ack
  KNX_DPT_1_017, // 1.017 B1 DPT_Trigger
  KNX_DPT_1_018, // 1.018 B1 DPT_Occupancy
  KNX_DPT_1_019, // 1.019 B1 DPT_Window_Door
  KNX_DPT_1_021, // 1.021 B1 DPT_LogicalFunction
  KNX_DPT_1_022, // 1.022 B1 DPT_Scene_AB
  KNX_DPT_1_023, // 1.023 B1 DPT_ShutterBlinds_Mode
  KNX_DPT_1_024, // 1.024 B1 DPT_DayNight
  KNX_DPT_1_100, // 1.100 B1 DPT_Heat/Cool
  KNX_DPT_1_1200, // 1.1200 B1 DPT_ConsumerProducer
  KNX_DPT_1_1201, // 1.1201 B1 DPT_EnergyDirection
  KNX_DPT_2_001, // 2.001 B2 DPT_Switch_Control
  KNX_DPT_2_002, // 2.002 B2 DPT_Bool_Control
  KNX_DPT_2_003, // 2.003 B2 DPT_Enable_Control
  KNX_DPT_2_004, // 2.004 B2 DPT_Ramp_Control
  KNX_DPT_2_005, // 2.005 B2 DPT_Alarm_Control
  KNX_DPT_2_006, // 2.006 B2 DPT_BinaryValue_Control
  KNX_DPT_2_007, // 2.007 B2 DPT_Step_Control
  KNX_DPT_2_008, // 2.008 B2 DPT_Direction1_Control
  KNX_DPT_2_009, // 2.009 B2 DPT_Direction2_Control
  KNX_DPT_2_010, // 2.010 B2 DPT_Start_Control
  KNX_DPT_2_011, // 2.011 B2 DPT_State_Control
  KNX_DPT_2_012, // 2.012 B2 DPT_Invert_Control
  KNX_DPT_3_007, // 3.007 B1U3 DPT_Control_Dimming
  KNX_DPT_3_008, // 3.008 B1U3 DPT_Control_Blinds
  KNX_DPT_4_001, // 4.001 A8 DPT_Char_ASCII
  KNX_DPT_4_002, // 4.002 A8 DPT_Char_8859_1
  KNX_DPT_5_001, // 5.001 U8 DPT_Scaling
  KNX_DPT_5_003, // 5.003 U8 DPT_Angle
  KNX_DPT_5_004, // 5.004 U8 DPT_Percent_U8
  KNX_DPT_5_005, // 5.005 U8 DPT_DecimalFactor
  KNX_DPT_5_006, // 5.006 U8 DPT_Tariff
  KNX_DPT_5_010, // 5.010 U8 DPT_Value_1_Ucount
  KNX_DPT_6_001, // 6.001 V8 DPT_Percent_V8
  KNX_DPT_6_010, // 6.010 V8 DPT_Value_1_Count
  KNX_DPT_6_020, // 6.020 B5N3 DPT_Status_Mode3
  KNX_DPT_7_001, // 7.001 U16 DPT_Value_2_Ucount
  KNX_DPT_7_002, // 7.002 U16 DPT_TimePeriodMsec
  KNX_DPT_7_003, // 7.003 U16 DPT_TimePeriod10MSec
  KNX_DPT_7_004, // 7.004 U16 DPT_TimePeriod100MSec
  KNX_DPT_7_005, // 7.005 U16 DPT_TimePeriodSec
  KNX_DPT_7_006, // 7.006 U16 DPT_TimePeriodMin
  KNX_DPT_7_007, // 7.007 U16 DPT_TimePeriodHrs
  KNX_DPT_7_010, // 7.010 U16 DPT_PropDataType
  KNX_DPT_7_011, // 7.011 U16 DPT_Length_mm
  KNX_DPT_7_012, // 7.012 U16 DPT_UElCurrentmA
  KNX_DPT_7_013, // 7.013 U16 DPT_Brightness
  KNX_DPT_7_600, // 7.600 U16 DPT_Absolute_Colour_Temperature
  KNX_DPT_8_001, // 8.001 V16 DPT_Value_2_Count
  KNX_DPT_8_002, // 8.002 V16 DPT_DeltaTimeMsec
  KNX_DPT_8_003, // 8.003 V16 DPT_DeltaTime10MSec
  KNX_DPT_8_004, // 8.004 V16 DPT_DeltaTime100MSec
  KNX_DPT_8_005, // 8.005 V16 DPT_DeltaTimeSec
  KNX_DPT_8_006, // 8.006 V16 DPT_DeltaTimeMin
  KNX_DPT_8_007, // 8.007 V16 DPT_DeltaTimeHrs
  KNX_DPT_8_010, // 8.010 V16 DPT_Percent_V16
  KNX_DPT_8_011, // 8.011 V16 DPT_Rotation_Angle
  KNX_DPT_8_012, // 8.012 V16 DPT_Length_m
  KNX_DPT_9_001, // 9.001 F16 DPT_Value_Temp
  KNX_DPT_9_002, // 9.002 F16 DPT_Value_Tempd
  KNX_DPT_9_003, // 9.003 F16 DPT_Value_Tempa
  KNX_DPT_9_004, // 9.004 F16 DPT_Value_Lux
  KNX_DPT_9_005, // 9.005 F16 DPT_Value_Wsp
  KNX_DPT_9_006, // 9.006 F16 DPT_Value_Pres
  KNX_DPT_9_007, // 9.007 F16 DPT_Value_Humidity
  KNX_DPT_9_008, // 9.008 F16 DPT_Value_AirQuality
  KNX_DPT_9_009, // 9.009 F16 DPT_Value_AirFlow
  KNX_DPT_9_010, // 9.010 F16 DPT_Value_Time1
  KNX_DPT_9_011, // 9.011 F16 DPT_Value_Time2
  KNX_DPT_9_020, // 9.020 F16 DPT_Value_Volt
  KNX_DPT_9_021, // 9.021 F16 DPT_Value_Curr
  KNX_DPT_9_022, // 9.022 F16 DPT_PowerDensity
  KNX_DPT_9_023, // 9.023 F16 DPT_KelvinPerPercent
  KNX_DPT_9_024, // 9.024 F16 DPT_Power
  KNX_DPT_9_025, // 9.025 F16 DPT_Value_Volume_Flow
  KNX_DPT_9_026, // 9.026 F16 DPT_Rain_Amount
  KNX_DPT_9_027, // 9.027 F16 DPT_Value_Temp_F
  KNX_DPT_9_028, // 9.028 F16 DPT_Value_Wsp_kmh
  KNX_DPT_9_029, // 9.029 F16 DPT_Value_Absolute_Humidity
  KNX_DPT_9_030, // 9.030 F16 DPT_Concentration_µgm3
  KNX_DPT_10_001, // 10.001 N3N5r2N6r2N6 DPT_TimeOfDay
  KNX_DPT_11_001, // 11.001 r3N5r4N4r1U7 DPT_Date
  KNX_DPT_12_001, // 12.001 U32 DPT_Value_4_Ucount
  KNX_DPT_12_100, // 12.100 U32 DPT_LongTimePeriod_Sec
  KNX_DPT_12_101, // 12.101 U32 DPT_LongTimePeriod_Min
  KNX_DPT_12_102, // 12.102 U32 DPT_LongTimePeriod_Hrs
  KNX_DPT_12_1200, // 12.1200 U32 DPT_VolumeLiquid_Litre
  KNX_DPT_12_1201, // 12.1201 U32 DPT_Volume_m3
  KNX_DPT_13_001, // 13.001 V32 DPT_Value_4_Count
  KNX_DPT_13_002, // 13.002 V32 DPT_FlowRate_m3/h
  KNX_DPT_13_010, // 13.010 V32 DPT_ActiveEnergy
  KNX_DPT_13_011, // 13.011 V32 DPT_ApparantEnergy
  KNX_DPT_13_012, // 13.012 V32 DPT_ReactiveEnergy
  KNX_DPT_13_013, // 13.013 V32 DPT_ActiveEnergy_kWh
  KNX_DPT_13_014, // 13.014 V32 DPT_ApparantEnergy_kVAh
  KNX_DPT_13_015, // 13.015 V32 DPT_ReactiveEnergy_kVARh
  KNX_DPT_13_016, // 13.016 V32 DPT_ActiveEnergy_MWh
  KNX_DPT_13_100, // 13.100 V32 DPT_LongDeltaTimeSec
  KNX_DPT_13_1200, // 13.1200 V32 DPT_DeltaVolumeLiquid_Litre
  KNX_DPT_13_1201, // 13.1201 V32 DPT_DeltaVolume_m3
  KNX_DPT_14_000, // 14.000 F32 DPT_Value_Acceleration
  KNX_DPT_14_001, // 14.001 F32 DPT_Value_Acceleration_Angular
  KNX_DPT_14_002, // 14.002 F32 DPT_Value_Activation_Energy
  KNX_DPT_14_003, // 14.003 F32 DPT_Value_Activity
  KNX_DPT_14_004, // 14.004 F32 DPT_Value_Mol
  KNX_DPT_14_005, // 14.005 F32 DPT_Value_Amplitude
  KNX_DPT_14_006, // 14.006 F32 DPT_Value_AngleRad
  KNX_DPT_14_007, // 14.007 F32 DPT_Value_AngleDeg
  KNX_DPT_14_008, // 14.008 F32 DPT_Value_Angular_Momentum
  KNX_DPT_14_009, // 14.009 F32 DPT_Value_Angular_Velocity
  KNX_DPT_14_010, // 14.010 F32 DPT_Value_Area
  KNX_DPT_14_011, // 14.011 F32 DPT_Value_Capacitance
  KNX_DPT_14_012, // 14.012 F32 DPT_Value_Charge_DensitySurface
  KNX_DPT_14_013, // 14.013 F32 DPT_Value_Charge_DensityVolume
  KNX_DPT_14_014, // 14.014 F32 DPT_Value_Compressibility
  KNX_DPT_14_015, // 14.015 F32 DPT_Value_Conductance
  KNX_DPT_14_016, // 14.016 F32 DPT_Value_Electrical_Conductivity
  KNX_DPT_14_017, // 14.017 F32 DPT_Value_Density
  KNX_DPT_14_018, // 14.018 F32 DPT_Value_Electric_Charge
  KNX_DPT_14_019, // 14.019 F32 DPT_Value_Electric_Current
  KNX_DPT_14_020, // 14.020 F32 DPT_Value_Electric_CurrentDensity
  KNX_DPT_14_021, // 14.021 F32 DPT_Value_Electric_DipoleMoment
  KNX_DPT_14_022, // 14.022 F32 DPT_Value_Electric_Displacement
  KNX_DPT_14_023, // 14.023 F32 DPT_Value_Electric_FieldStrength
  KNX_DPT_14_024, // 14.024 F32 DPT_Value_Electric_Flux
  KNX_DPT_14_025, // 14.025 F32 DPT_Value_Electric_FluxDensity
  KNX_DPT_14_026, // 14.026 F32 DPT_Value_Electric_Polarization
  KNX_DPT_14_027, // 14.027 F32 DPT_Value_Electric_Potential
  KNX_DPT_14_028, // 14.028 F32 DPT_Value_Electric_PotentialDifference
  KNX_DPT_14_029, // 14.029 F32 DPT_Value_ElectromagneticMoment
  KNX_DPT_14_030, // 14.030 F32 DPT_Value_Electromotive_Force
  KNX_DPT_14_031, // 14.031 F32 DPT_Value_Energy
  KNX_DPT_14_032, // 14.032 F32 DPT_Value_Force
  KNX_DPT_14_033, // 14.033 F32 DPT_Value_Frequency
  KNX_DPT_14_034, // 14.034 F32 DPT_Value_Angular_Frequency
  KNX_DPT_14_035, // 14.035 F32 DPT_Value_Heat_Capacity
  KNX_DPT_14_036, // 14.036 F32 DPT_Value_Heat_FlowRate
  KNX_DPT_14_037, // 14.037 F32 DPT_Value_Heat_Quantity
  KNX_DPT_14_038, // 14.038 F32 DPT_Value_Impedance
  KNX_DPT_14_039, // 14.039 F32 DPT_Value_Length
  KNX_DPT_14_040, // 14.040 F32 DPT_Value_Light_Quantity
  KNX_DPT_14_041, // 14.041 F32 DPT_Value_Luminance
  KNX_DPT_14_042, // 14.042 F32 DPT_Value_Luminous_Flux
  KNX_DPT_14_043, // 14.043 F32 DPT_Value_Luminous_Intensity
  KNX_DPT_14_044, // 14.044 F32 DPT_Value_Magnetic_FieldStrength
  KNX_DPT_14_045, // 14.045 F32 DPT_Value_Magnetic_Flux
  KNX_DPT_14_046, // 14.046 F32 DPT_Value_Magnetic_FluxDensity
  KNX_DPT_14_047, // 14.047 F32 DPT_Value_Magnetic_Moment
  KNX_DPT_14_048, // 14.048 F32 DPT_Value_Magnetic_Polarization
  KNX_DPT_14_049, // 14.049 F32 DPT_Value_Magnetization
  KNX_DPT_14_050, // 14.050 F32 DPT_Value_MagnetomotiveForce
  KNX_DPT_14_051, // 14.051 F32 DPT_Value_Mass
  KNX_DPT_14_052, // 14.052 F32 DPT_Value_MassFlux
  KNX_DPT_14_053, // 14.053 F32 DPT_Value_Momentum
  KNX_DPT_14_054, // 14.054 F32 DPT_Value_Phase_AngleRad
  KNX_DPT_14_055, // 14.055 F32 DPT_Value_Phase_AngleDeg
  KNX_DPT_14_056, // 14.056 F32 DPT_Value_Power
  KNX_DPT_14_057, // 14.057 F32 DPT_Value_Power_Factor
  KNX_DPT_14_058, // 14.058 F32 DPT_Value_Pressure
  KNX_DPT_14_059, // 14.059 F32 DPT_Value_Reactance
  KNX_DPT_14_060, // 14.060 F32 DPT_Value_Resistance
  KNX_DPT_14_061, // 14.061 F32 DPT_Value_Resistivity
  KNX_DPT_14_062, // 14.062 F32 DPT_Value_SelfInductance
  KNX_DPT_14_063, // 14.063 F32 DPT_Value_SolidAngle
  KNX_DPT_14_064, // 14.064 F32 DPT_Value_Sound_Intensity
  KNX_DPT_14_065, // 14.065 F32 DPT_Value_Speed
  KNX_DPT_14_066, // 14.066 F32 DPT_Value_Stress
  KNX_DPT_14_067, // 14.067 F32 DPT_Value_Surface_Tension
  KNX_DPT_14_068, // 14.068 F32 DPT_Value_Common_Temperature
  KNX_DPT_14_069, // 14.069 F32 DPT_Value_Absolute_Temperature
  KNX_DPT_14_070, // 14.070 F32 DPT_Value_TemperatureDifference
  KNX_DPT_14_071, // 14.071 F32 DPT_Value_Thermal_Capacity
  KNX_DPT_14_072, // 14.072 F32 DPT_Value_Thermal_Conductivity
  KNX_DPT_14_073, // 14.073 F32 DPT_Value_ThermoelectricPower
  KNX_DPT_14_074, // 14.074 F32 DPT_Value_Time
  KNX_DPT_14_075, // 14.075 F32 DPT_Value_Torque
  KNX_DPT_14_076, // 14.076 F32 DPT_Value_Volume
  KNX_DPT_14_077, // 14.077 F32 DPT_Value_Volume_Flux
  KNX_DPT_14_078, // 14.078 F32 DPT_Value_Weight
  KNX_DPT_14_079, // 14.079 F32 DPT_Value_Work
  KNX_DPT_14_080, // 14.080 F32 DPT_Value_ApparentPower
  KNX_DPT_14_1200, // 14.1200 F32 DPT_Volume_Flux_Meter
  KNX_DPT_14_1201, // 14.1201 F32 DPT_Volume_Flux_ls
  // KNX_DPT_15_000, // 15.000 U4U4U4U4U4U4B4N4 DPT_Access_Data
  // KNX_DPT_16_000, // 16.000 A112 DPT_String_ASCII
  // KNX_DPT_16_001, // 16.001 A112 DPT_String_8859_1
  // KNX_DPT_17_001, // 17.001 r2U6 DPT_SceneNumber
  // KNX_DPT_18_001, // 18.001 B1r1U6 DPT_SceneControl
  // KNX_DPT_19_001, // 19.001 U8[r4U4][r3U5][U3U5][r2U6][r2U6]B16 DPT_DateTime
  // KNX_DPT_20_001, // 20.001 N8 DPT_SCLOMode
  // KNX_DPT_20_002, // 20.002 N8 DPT_BuildingMode
  // KNX_DPT_20_003, // 20.003 N8 DPT_OccMode
  // KNX_DPT_20_004, // 20.004 N8 DPT_Priority
  // KNX_DPT_20_005, // 20.005 N8 DPT_LightApplicationMode
  // KNX_DPT_20_006, // 20.006 N8 DPT_ApplicationArea
  // KNX_DPT_20_007, // 20.007 N8 DPT_AlarmClassType
  // KNX_DPT_20_008, // 20.008 N8 DPT_PSUMode
  // KNX_DPT_20_011, // 20.011 N8 DPT_ErrorClass_System
  // KNX_DPT_20_012, // 20.012 N8 DPT_ErrorClass_HVAC
  // KNX_DPT_20_013, // 20.013 N8 DPT_Time_Delay
  // KNX_DPT_20_014, // 20.014 N8 DPT_Beaufort_Wind_Force_Scale
  // KNX_DPT_20_017, // 20.017 N8 DPT_SensorSelect
  // KNX_DPT_20_020, // 20.020 N8 DPT_ActuatorConnectType
  // KNX_DPT_20_021, // 20.021 N8 DPT_Cloud_Cover
  // KNX_DPT_20_022, // 20.022 N8 DPT_PowerReturnMode
  // KNX_DPT_20_100, // 20.100 N8 DPT_FuelType
  // KNX_DPT_20_101, // 20.101 N8 DPT_BurnerType
  // KNX_DPT_20_102, // 20.102 N8 DPT_HVACMode
  // KNX_DPT_20_103, // 20.103 N8 DPT_DHWMode
  // KNX_DPT_20_104, // 20.104 N8 DPT_LoadPriority
  // KNX_DPT_20_105, // 20.105 N8 DPT_HVACContrMode
  // KNX_DPT_20_106, // 20.106 N8 DPT_HVACEmergMode
  // KNX_DPT_20_107, // 20.107 N8 DPT_ChangeoverMode
  // KNX_DPT_20_108, // 20.108 N8 DPT_ValveMode
  // KNX_DPT_20_109, // 20.109 N8 DPT_DamperMode
  // KNX_DPT_20_110, // 20.110 N8 DPT_HeaterMode
  // KNX_DPT_20_111, // 20.111 N8 DPT_FanMode
  // KNX_DPT_20_112, // 20.112 N8 DPT_MasterSlaveMode
  // KNX_DPT_20_113, // 20.113 N8 DPT_StatusRoomSetp
  // KNX_DPT_20_114, // 20.114 N8 DPT_Metering_DeviceType
  // KNX_DPT_20_115, // 20.115 N8 DPT_HumDehumMode
  // KNX_DPT_20_120, // 20.120 N8 DPT_ADAType
  // KNX_DPT_20_121, // 20.121 N8 DPT_BackupMode
  // KNX_DPT_20_122, // 20.122 N8 DPT_StartSynchronization
  // KNX_DPT_20_600, // 20.600 N8 DPT_Behaviour_Lock_Unlock
  // KNX_DPT_20_601, // 20.601 N8 DPT_Behaviour_Bus_Power_Up_Down
  // KNX_DPT_20_602, // 20.602 N8 DPT_DALI_Fade_Time
  // KNX_DPT_20_603, // 20.603 N8 DPT_BlinkingMode
  // KNX_DPT_20_604, // 20.604 N8 DPT_LightControlMode
  // KNX_DPT_20_605, // 20.605 N8 DPT_SwitchPBModel
  // KNX_DPT_20_606, // 20.606 N8 DPT_PBAction
  // KNX_DPT_20_607, // 20.607 N8 DPT_DimmPBModel
  // KNX_DPT_20_608, // 20.608 N8 DPT_SwitchOnMode
  // KNX_DPT_20_609, // 20.609 N8 DPT_LoadTypeSet
  // KNX_DPT_20_610, // 20.610 N8 DPT_LoadTypeDetected
  // KNX_DPT_20_611, // 20.611 N8 DPT_Converter_Test_Control
  // KNX_DPT_20_612, // 20.612 N8 DPT_Converter_Control
  // KNX_DPT_20_613, // 20.613 N8 DPT_Converter_Data_Request
  // KNX_DPT_20_801, // 20.801 N8 DPT_SABExceptBehaviour
  // KNX_DPT_20_802, // 20.802 N8 DPT_SABBehaviour_Lock_Unlock
  // KNX_DPT_20_803, // 20.803 N8 DPT_SSSBMode
  // KNX_DPT_20_804, // 20.804 N8 DPT_BlindsControlMode
  // KNX_DPT_20_1000, // 20.1000 N8 DPT_CommMode
  // KNX_DPT_20_1001, // 20.1001 N8 DPT_AddInfoTypes
  // KNX_DPT_20_1002, // 20.1002 N8 DPT_RF_ModeSelect
  // KNX_DPT_20_1003, // 20.1003 N8 DPT_RF_FilterSelect
  // KNX_DPT_20_1004, // 20.1004 N8 DPT_Medium
  // KNX_DPT_20_1005, // 20.1005 N8 DPT_PB_Function
  // KNX_DPT_20_1200, // 20.1200 N8 DPT_MBus_BreakerValve_State
  // KNX_DPT_20_1202, // 20.1202 N8 DPT_Gas_Measurement_Condition
  // KNX_DPT_20_1203, // 20.1203 N8 DPT_Breaker_Status
  // KNX_DPT_20_1204, // 20.1204 N8 DPT_Euridis_Communication_Interface_Status
  // KNX_DPT_20_1205, // 20.1205 N8 DPT_PLC_Status
  // KNX_DPT_20_1206, // 20.1206 N8 DPT_Peak_Event_Notice
  // KNX_DPT_20_1207, // 20.1207 N8 DPT_Peak_Event
  // KNX_DPT_20_1208, // 20.1208 N8 DPT_TIC_Type
  // KNX_DPT_20_1209, // 20.1209 N8 DPT_Type_TIC_Channel
  // KNX_DPT_21_001, // 21.001 B8 DPT_StatusGen
  // KNX_DPT_21_002, // 21.002 B8 DPT_Device_Control
  // KNX_DPT_21_100, // 21.100 B8 DPT_ForceSign
  // KNX_DPT_21_101, // 21.101 B8 DPT_ForceSignCool
  // KNX_DPT_21_102, // 21.102 B8 DPT_StatusRHC
  // KNX_DPT_21_103, // 21.103 B8 DPT_StatusSDHWC
  // KNX_DPT_21_104, // 21.104 B8 DPT_FuelTypeSet
  // KNX_DPT_21_105, // 21.105 B8 DPT_StatusRCC
  // KNX_DPT_21_106, // 21.106 B8 DPT_StatusAHU
  // KNX_DPT_21_601, // 21.601 B8 DPT_LightActuatorErrorInfo
  // KNX_DPT_21_1000, // 21.1000 B8 DPT_RF_ModeInfo
  // KNX_DPT_21_1001, // 21.1001 B8 DPT_RF_FilterInfo
  // KNX_DPT_21_1002, // 21.1002 B8 DPT_Security_Report
  // KNX_DPT_21_1010, // 21.1010 B8 DPT_Channel_Activation_8
  // KNX_DPT_21_1200, // 21.1200 B8 DPT_VirtualDryContact
  // KNX_DPT_21_1201, // 21.1201 B8 DPT_Phases_Status
  // KNX_DPT_22_100, // 22.100 B16 DPT_StatusDHWC
  // KNX_DPT_22_101, // 22.101 B16 DPT_StatusRHCC
  // KNX_DPT_22_1000, // 22.1000 B16 DPT_Media
  // KNX_DPT_22_1010, // 22.1010 B16 DPT_Channel_Activation_16
  // KNX_DPT_23_001, // 23.001 N2 DPT_OnOff_Action
  // KNX_DPT_23_002, // 23.002 N2 DPT_Alarm_Reaction
  // KNX_DPT_23_003, // 23.003 N2 DPT_UpDown_Action
  // KNX_DPT_23_102, // 23.102 N2 DPT_HVAC_PB_Action
  // KNX_DPT_24_001, // 24.001 A[n] DPT_VarString_8859_1
  // KNX_DPT_25_1000, // 25.1000 U4U4 DPT_DoubleNibble
  // KNX_DPT_26_001, // 26.001 r1b1U6 DPT_SceneInfo
  // KNX_DPT_27_001, // 27.001 B32 DPT_CombinedInfoOnOff
  // KNX_DPT_28_001, // 28.001 A[n] DPT_UTF-8
  // KNX_DPT_29_010, // 29.010 V64 DPT_ActiveEnergy_V64
  // KNX_DPT_29_011, // 29.011 V64 DPT_ApparantEnergy_V64
  // KNX_DPT_29_012, // 29.012 V64 DPT_ReactiveEnergy_V64
  // KNX_DPT_30_1010, // 30.1010 B24 DPT_Channel_Activation_24
  // KNX_DPT_31_101, // 31.101 N3 DPT_PB_Action_HVAC_Extended
  // KNX_DPT_200_100, // 200.100 B1Z8 DPT_Heat/Cool_Z
  // KNX_DPT_200_101, // 200.101 B1Z8 DPT_BinaryValue_Z
  // KNX_DPT_201_100, // 201.100 N8Z8 DPT_HVACMode_Z
  // KNX_DPT_201_102, // 201.102 N8Z8 DPT_DHWMode_Z
  // KNX_DPT_201_104, // 201.104 N8Z8 DPT_HVACContrMode_Z
  // KNX_DPT_201_105, // 201.105 N8Z8 DPT_EnablH/Cstage_Z
  // KNX_DPT_201_107, // 201.107 N8Z8 DPT_BuildingMode_Z
  // KNX_DPT_201_108, // 201.108 N8Z8 DPT_OccMode_Z
  // KNX_DPT_201_109, // 201.109 N8Z8 DPT_HVACEmergMode_Z
  // KNX_DPT_202_001, // 202.001 U8Z8 DPT_RelValue_Z
  // KNX_DPT_202_002, // 202.002 U8Z8 DPT_UCountValue8_Z
  // KNX_DPT_203_002, // 203.002 U16Z8 DPT_TimePeriodMsec_Z
  // KNX_DPT_203_003, // 203.003 U16Z8 DPT_TimePeriod10Msec_Z
  // KNX_DPT_203_004, // 203.004 U16Z8 DPT_TimePeriod100Msec_Z
  // KNX_DPT_203_005, // 203.005 U16Z8 DPT_TimePeriodSec_Z
  // KNX_DPT_203_006, // 203.006 U16Z8 DPT_TimePeriodMin_Z
  // KNX_DPT_203_007, // 203.007 U16Z8 DPT_TimePeriodHrs_Z
  // KNX_DPT_203_011, // 203.011 U16Z8 DPT_UFlowRateLiter/h_Z
  // KNX_DPT_203_012, // 203.012 U16Z8 DPT_UCountValue16_Z
  // KNX_DPT_203_013, // 203.013 U16Z8 DPT_UElCurrentμA_Z
  // KNX_DPT_203_014, // 203.014 U16Z8 DPT_PowerKW_Z
  // KNX_DPT_203_015, // 203.015 U16Z8 DPT_AtmPressureAbs_Z
  // KNX_DPT_203_017, // 203.017 U16Z8 DPT_PercentU16_Z
  // KNX_DPT_203_100, // 203.100 U16Z8 DPT_HVACAirQual_Z
  // KNX_DPT_203_101, // 203.101 U16Z8 DPT_WindSpeed_Z
  // KNX_DPT_203_102, // 203.102 U16Z8 DPT_SunIntensity_Z
  // KNX_DPT_203_104, // 203.104 U16Z8 DPT_HVACAirFlowAbs_Z
  // KNX_DPT_204_001, // 204.001 V8Z8 DPT_RelSignedValue_Z
  // KNX_DPT_205_002, // 205.002 V16Z8 DPT_DeltaTimeMsec_Z
  // KNX_DPT_205_003, // 205.003 V16Z8 DPT_DeltaTime10Msec_Z
  // KNX_DPT_205_004, // 205.004 V16Z8 DPT_DeltaTime100Msec_Z
  // KNX_DPT_205_005, // 205.005 V16Z8 DPT_DeltaTimeSec_Z
  // KNX_DPT_205_006, // 205.006 V16Z8 DPT_DeltaTimeMin_Z
  // KNX_DPT_205_007, // 205.007 V16Z8 DPT_DeltaTimeHrs_Z
  // KNX_DPT_205_017, // 205.017 V16Z8 DPT_Percent_V16_Z
  // KNX_DPT_205_100, // 205.100 V16Z8 DPT_TempHVACAbs_Z
  // KNX_DPT_205_101, // 205.101 V16Z8 DPT_TempHVACRel_Z
  // KNX_DPT_205_102, // 205.102 V16Z8 DPT_HVACAirFlowRel_Z
  // KNX_DPT_205_103, // 205.103 V16Z8 DPT_HVACAirQualiRel_Z
  // KNX_DPT_206_100, // 206.100 U16N8 DPT_HVACModeNext
  // KNX_DPT_206_102, // 206.102 U16N8 DPT_DHWModeNext
  // KNX_DPT_206_104, // 206.104 U16N8 DPT_OccModeNext
  // KNX_DPT_206_105, // 206.105 U16N8 DPT_BuildingModeNext
  // KNX_DPT_207_100, // 207.100 U8B8 DPT_StatusBUC
  // KNX_DPT_207_101, // 207.101 U8B8 DPT_LockSign
  // KNX_DPT_207_102, // 207.102 U8B8 DPT_ValueDemBOC
  // KNX_DPT_207_104, // 207.104 U8B8 DPT_ActPosDemAbs
  // KNX_DPT_207_105, // 207.105 U8B8 DPT_StatusAct
  // KNX_DPT_207_600, // 207.600 U8B8 DPT_StatusLightingActuator
  // KNX_DPT_209_100, // 209.100 V16B8 DPT_StatusHPM
  // KNX_DPT_209_101, // 209.101 V16B8 DPT_TempRoomDemAbs
  // KNX_DPT_209_102, // 209.102 V16B8 DPT_StatusCPM
  // KNX_DPT_209_103, // 209.103 V16B8 DPT_StatusWTC
  // KNX_DPT_210_100, // 210.100 V16B16 DPT_TempFlowWaterDemAbs
  // KNX_DPT_211_100, // 211.100 U8N8 DPT_EnergyDemWater
  // KNX_DPT_212_100, // 212.100 V16V16V16 DPT_TempRoomSetpSetShift[3]
  // KNX_DPT_212_101, // 212.101 V16V16V16 DPT_TempRoomSetpSet[3]
  // KNX_DPT_213_100, // 213.100 V16V16V16V16 DPT_TempRoomSetpSet[4]
  // KNX_DPT_213_101, // 213.101 V16V16V16V16 DPT_TempDHWSetpSet[4]
  // KNX_DPT_213_102, // 213.102 V16V16V16V16 DPT_TempRoomSetpSetShift[4]
  // KNX_DPT_214_100, // 214.100 V16U8B8 DPT_PowerFlowWaterDemHPM
  // KNX_DPT_214_101, // 214.101 V16U8B8 DPT_PowerFlowWaterDemCPM
  // KNX_DPT_215_100, // 215.100 V16U8B16 DPT_StatusBOC
  // KNX_DPT_215_101, // 215.101 V16U8B16 DPT_StatusCC
  // KNX_DPT_216_100, // 216.100 U16U8N8B8 DPT_SpecHeatProd
  // KNX_DPT_217_001, // 217.001 U5U5U6 DPT_Version
  // KNX_DPT_218_001, // 218.001 V32Z8 DPT_VolumeLiter_Z
  // KNX_DPT_218_002, // 218.002 V32Z8 DPT_FlowRate_m3/h_Z
  // KNX_DPT_219_001, // 219.001 U8N8N8N8B8B8 DPT_AlarmInfo
  // KNX_DPT_220_100, // 220.100 U16V16 DPT_TempHVACAbsNext
  // KNX_DPT_221_001, // 221.001 N16U32 DPT_SerNum
  // KNX_DPT_222_100, // 222.100 F16F16F16 DPT_TempRoomSetpSetF16[3]
  // KNX_DPT_222_101, // 222.101 F16F16F16 DPT_TempRoomSetpSetShiftF16[3]
  // KNX_DPT_223_100, // 223.100 V8N8N8 DPT_EnergyDemAir
  // KNX_DPT_224_100, // 224.100 V16V16N8N8 DPT_TempSupplyAirSetpSet
  // KNX_DPT_225_001, // 225.001 U16U8 DPT_ScalingSpeed
  // KNX_DPT_225_002, // 225.002 U16U8 DPT_Scaling_Step_Time
  // KNX_DPT_225_003, // 225.003 U16U8 DPT_TariffNext
  // KNX_DPT_229_001, // 229.001 V32N8Z8 DPT_MeteringValue
  // KNX_DPT_230_1000, // 230.1000 U16U32U8N8 DPT_MBus_Address
  // KNX_DPT_231_001, // 231.001 A8A8A8A8 DPT_Locale_ASCII
  // KNX_DPT_232_600, // 232.600 U8U8U8 DPT_Colour_RGB
  // KNX_DPT_234_001, // 234.001 A8A8 DPT_LanguageCodeAlpha2_ASCII
  // KNX_DPT_234_002, // 234.002 A8A8 DPT_RegionCodeAlpha2_ASCII
  // KNX_DPT_235_001, // 235.001 V32U8B8 DPT_Tariff_ActiveEnergy
  // KNX_DPT_236_001, // 236.001 B1N3N4 DPT_Prioritised_Mode_Control
  // KNX_DPT_237_600, // 237.600 B10U6 DPT_DALI_Control_Gear_Diagnostic
  // KNX_DPT_238_001, // 238.001 B2U6 DPT_SceneConfig
  // KNX_DPT_238_600, // 238.600 B2U6 DPT_DALI_Diagnostics
  // KNX_DPT_239_001, // 239.001 U8r7B1 DPT_FlaggedScaling
  // KNX_DPT_240_800, // 240.800 U8U8B8 DPT_CombinedPosition
  // KNX_DPT_241_800, // 241.800 U8U8B16 DPT_StatusSAB
  // KNX_DPT_242_600, // 242.600 U16U16U8r6B2 DPT_Colour_xyY
  // KNX_DPT_243_600, // 243.600 U16U16U16U8r6B2 DPT_Colour_Transition_xyY
  // KNX_DPT_244_600, // 244.600 N4B4N2N2N2N2 DPT_Converter_Status
  // KNX_DPT_245_600, // 245.600 N4N4N4N2N2N2N2U16U8 DPT_Converter_Test_Result
  // KNX_DPT_246_600, // 246.600 r5B3U8 DPT_Battery_Info
  // KNX_DPT_247_600, // 247.600 U8[r4U4][r3U5][U3U5][r2U6][r2U6]B8-[B1r7]U16U16 DPT_Converter_Test_Info
  // KNX_DPT_248_600, // 248.600 N 8U 8U 8U 8 B8 DPT_Converter_Info_Fix
  // KNX_DPT_250_600, // 250.600 r 4B1U 3r 4B 1U 3B 8 DPT_Brightness_Colour_Temperature_Control
  // KNX_DPT_251_600, // 251.600 U 8U 8U 8U 8r 8r 4 B4 DPT_Colour_RGBW
  // KNX_DPT_252_600, // 252.600 r 4B1U 3r 4B 1U 3r 4B1 U 3r 4B1 U 3B8 DPT_Relative_Control_RGBW
  // KNX_DPT_253_600, // 253.600 r 4B1U 3r 4B 1U 3r 4B1 U 3B8 DPT_Relative_Control_xyY
  // KNX_DPT_254_600, // 254.600 r 4B1U 3r 4B 1U 3r 4B1 U 3 DPT_Relative_Control_RGB
  // KNX_DPT_255_001, // 255.001 F 32F 32 DPT_GeographicalLocation
  // KNX_DPT_256_001, // 256.001 U 8[r 4U 4][r 3U 5][U 3U 5][r 2U 6][r 2 U 6]B16U 8[r 4U 4][r 3U 5][U 3 U 5][r 2U 6][r 2U 6]B 16 DPT_DateTime_Period
  // KNX_DPT_257_1200, // 257.1200 F 32F 32F 32 DPT_Value_Electric_Current_3
  // KNX_DPT_257_1201, // 257.1201 F 32F 32F 32 DPT_Value_Electric_Potential_3
  // KNX_DPT_257_1202, // 257.1202 F 32F 32F 32 DPT_Value_ApparentPower_3
  // KNX_DPT_265_001, // 265.001 U 8[r 4U 4][r 3U 5][U 3U 5][r 2U 6][r 2 U 6]B16 B1 DPT_DateTime_Switch
  // KNX_DPT_265_005, // 265.005 U 8[r 4U 4][r 3U 5][U 3U 5][r 2U 6][r 2 U 6]B16 B1 DPT_DateTime_Alarm
  // KNX_DPT_265_009, // 265.009 U 8[r 4U 4][r 3U 5][U 3U 5][r 2U 6][r 2 U 6]B16 B1 DPT_DateTime_OpenClose
  // KNX_DPT_265_011, // 265.011 U 8[r 4U 4][r 3U 5][U 3U 5][r 2U 6][r 2 U 6]B16 B1 DPT_DateTime_State
  // KNX_DPT_265_012, // 265.012 U 8[r 4U 4][r 3U 5][U 3U 5][r 2U 6][r 2 U 6]B16 B1 DPT_DateTime_Invert
  // KNX_DPT_265_1200, // 265.1200 U 8[r 4U 4][r 3U 5][U 3U 5][r 2U 6][r 2 U 6]B16 B1 DPT_DateTime_ConsumerProducer
  // KNX_DPT_265_1201, // 265.1201 U 8[r 4U 4][r 3U 5][U 3U 5][r 2U 6][r 2 U 6]B16 B1 DPT_DateTime_EnergyDirection
  // KNX_DPT_266_027, // 266.027 U 8[r 4U 4][r 3U 5][U 3U 5][r 2U 6][r 2 U 6]B16F 32 DPT_DateTime_Value_Electric_Potential
  // KNX_DPT_266_056, // 266.056 U 8[r 4U 4][r 3U 5][U 3U 5][r 2U 6][r 2 U 6]B16F 32 DPT_DateTime_Value_Power
  // KNX_DPT_266_080, // 266.080 U 8[r 4U 4][r 3U 5][U 3U 5][r 2U 6][r 2 U 6]B16F 32 DPT_DateTime_Value_ApparentPower
  // KNX_DPT_267_001, // 267.001 U 8[r 4U 4][r 3U 5][U 3U 5][r 2U 6][r 2 U 6]B16 A[n] DPT_DateTime_UTF-8
  // KNX_DPT_268_1203, // 268.1203 U 8[r 4U 4][r 3U 5][U 3U 5][r 2U 6][r 2 U 6]B16N 8 DPT_DateTime_Breaker_Status
  // KNX_DPT_268_1204, // 268.1204 U 8[r 4U 4][r 3U 5][U 3U 5][r 2U 6][r 2 U 6]B16N 8 DPT_DateTime_Euridis_Communication_Interface_Status
  // KNX_DPT_268_1205, // 268.1205 U 8[r 4U 4][r 3U 5][U 3U 5][r 2U 6][r 2 U 6]B16N 8 DPT_DateTime_PLC_Status
  // KNX_DPT_268_1206, // 268.1206 U8[r4U4][r3U5][U3U5][r2U6][r2U6]B16N8 DPT_DateTime_Peak_Notice
  // KNX_DPT_269_1200, // 269.1200 U8[r 4U 4][r 3U 5][U 3U 5][r 2U 6][r 2 U 6]B16 V32U8B8 DPT_DateTime_Tariff_ActiveEnergy
  // KNX_DPT_270_1200, // 270.1200 U 8[r 4U 4][r 3U 5][U 3U 5][r 2U 6][r 2 U 6]B16F 32F32F 32 DPT_DateTime_Value_Electric_Current_3
  // KNX_DPT_270_1201, // 270.1201 U 8[r 4U 4][r 3U 5][U 3U 5][r 2U 6][r 2 U 6]B16F 32F32F 32 DPT_DateTime_Value_Electric_Potential_3
  // KNX_DPT_270_1202, // 270.1202 U 8[r 4U 4][r 3U 5][U 3U 5][r 2U 6][r 2 U 6]B16F 32F32F 32 DPT_DateTime_Value_ApparentPower_3
  // KNX_DPT_271_1200, // 271.1200 [N 3U 5][r 2U 6][r 2U 6][U 4 U 4]U 8[U 6N 2][r 1B7] DPT_TariffDayProfile
  // KNX_DPT_272_600, // 272.600 N 8U 16U 16U 8U 8 DPT_Converter_Info
  // KNX_DPT_273_001, // 273.001 B8U 16U 8F 16F 16 DPT_Forecast_Temperature
  // KNX_DPT_273_002, // 273.002 B8U 16U 8F 16F 16 DPT_Forecast_WindSpeed
  // KNX_DPT_273_003, // 273.003 B8U 16U 8F 16F 16 DPT_Forecast_RelativeHumidity
  // KNX_DPT_273_004, // 273.004 B8U 16U 8F 16F 16 DPT_Forecast_AbsoluteHumidity
  // KNX_DPT_273_005, // 273.005 B8U 16U 8F 16F 16 DPT_Forecast_CO2
  // KNX_DPT_273_006, // 273.006 B8U 16U 8F 16F 16 DPT_Forecast_AirPollutants
  // KNX_DPT_273_007, // 273.007 B8U 16U 8F 16F 16 DPT_Forecast_SunIntensity
  // KNX_DPT_274_001, // 274.001 B8U 16U 8 U 8U 8 DPT_Forecast_Wind_Direction
  // KNX_DPT_276_1200, // 276.1200 U 8U 8U 8r 3B 5 DPT_ERL_Status
  // KNX_DPT_277_1200, // 277.1200 A[12](V32U 8B8)[4] DPT_4_EnergyRegisters
  // KNX_DPT_278_1200, // 278.1200 A[12](V32U 8B8)[5] DPT_5_EnergyRegisters
  // KNX_DPT_279_1200, // 279.1200 A[12](V32U 8B8)[6] DPT_6_EnergyRegisters
  // KNX_DPT_280_1200, // 280.1200 A[12](V32U 8B8)[11] DPT_11_EnergyRegisters
  // KNX_DPT_281_1200, // 281.1200 U 8[r 4U 4][r 3U 5][U 3U 5][r 2U 6][r 2 U 6]B16 A[12](V32U 8 B8)[4] DPT_DateTime_4_EnergyRegisters
  // KNX_DPT_282_1200, // 282.1200 U 8[r 4U 4][r 3U 5][U 3U 5][r 2U 6][r 2 U 6]B16 A[12](V32U 8 B8)[5] DPT_DateTime_5_EnergyRegisters
  // KNX_DPT_283_1200, // 283.1200 U 8[r 4U 4][r 3U 5][U 3U 5][r 2U 6][r 2 U 6]B16 A[12](V32U 8 B8)[6] DPT_DateTime_6_EnergyRegisters
  // KNX_DPT_284_1200, // 284.1200 U 8[r 4U 4][r 3U 5][U 3U 5][r 2U 6][r 2 U 6]B16 A[12](V32U 8 B8)[11] DPT_DateTime_11_EnergyRegisters
  // Do not change
  KNX_DPT_COUNT
};

// The index of the array is also the value if the KnxDPT to look it up
static const std::tuple<uint16_t, uint16_t, KnxDptFormat, std::string> dpt_table[] = {
  /* KNX_DPT_1_001 */ std::tuple(1, 1, KNX_DPT_FORMAT_B1, "DPT_Switch"),
  /* KNX_DPT_1_002 */ std::tuple(1, 2, KNX_DPT_FORMAT_B1, "DPT_Bool"),
  /* KNX_DPT_1_003 */ std::tuple(1, 3, KNX_DPT_FORMAT_B1, "DPT_Enable"),
  /* KNX_DPT_1_004 */ std::tuple(1, 4, KNX_DPT_FORMAT_B1, "DPT_Ramp"),
  /* KNX_DPT_1_005 */ std::tuple(1, 5, KNX_DPT_FORMAT_B1, "DPT_Alarm"),
  /* KNX_DPT_1_006 */ std::tuple(1, 6, KNX_DPT_FORMAT_B1, "DPT_BinaryValue"),
  /* KNX_DPT_1_007 */ std::tuple(1, 7, KNX_DPT_FORMAT_B1, "DPT_Step"),
  /* KNX_DPT_1_008 */ std::tuple(1, 8, KNX_DPT_FORMAT_B1, "DPT_UpDown"),
  /* KNX_DPT_1_009 */ std::tuple(1, 9, KNX_DPT_FORMAT_B1, "DPT_OpenClose"),
  /* KNX_DPT_1_010 */ std::tuple(1, 10, KNX_DPT_FORMAT_B1, "DPT_Start"),
  /* KNX_DPT_1_011 */ std::tuple(1, 11, KNX_DPT_FORMAT_B1, "DPT_State"),
  /* KNX_DPT_1_012 */ std::tuple(1, 12, KNX_DPT_FORMAT_B1, "DPT_Invert"),
  /* KNX_DPT_1_013 */ std::tuple(1, 13, KNX_DPT_FORMAT_B1, "DPT_DimSendStyle"),
  /* KNX_DPT_1_014 */ std::tuple(1, 14, KNX_DPT_FORMAT_B1, "DPT_InputSource"),
  /* KNX_DPT_1_015 */ std::tuple(1, 15, KNX_DPT_FORMAT_B1, "DPT_Reset"),
  /* KNX_DPT_1_016 */ std::tuple(1, 16, KNX_DPT_FORMAT_B1, "DPT_Ack"),
  /* KNX_DPT_1_017 */ std::tuple(1, 17, KNX_DPT_FORMAT_B1, "DPT_Trigger"),
  /* KNX_DPT_1_018 */ std::tuple(1, 18, KNX_DPT_FORMAT_B1, "DPT_Occupancy"),
  /* KNX_DPT_1_019 */ std::tuple(1, 19, KNX_DPT_FORMAT_B1, "DPT_Window_Door"),
  /* KNX_DPT_1_021 */ std::tuple(1, 21, KNX_DPT_FORMAT_B1, "DPT_LogicalFunction"),
  /* KNX_DPT_1_022 */ std::tuple(1, 22, KNX_DPT_FORMAT_B1, "DPT_Scene_AB"),
  /* KNX_DPT_1_023 */ std::tuple(1, 23, KNX_DPT_FORMAT_B1, "DPT_ShutterBlinds_Mode"),
  /* KNX_DPT_1_024 */ std::tuple(1, 24, KNX_DPT_FORMAT_B1, "DPT_DayNight"),
  /* KNX_DPT_1_100 */ std::tuple(1, 100, KNX_DPT_FORMAT_B1, "DPT_Heat/Cool"),
  /* KNX_DPT_1_1200 */ std::tuple(1, 1200, KNX_DPT_FORMAT_B1, "DPT_ConsumerProducer"),
  /* KNX_DPT_1_1201 */ std::tuple(1, 1201, KNX_DPT_FORMAT_B1, "DPT_EnergyDirection"),
  /* KNX_DPT_2_001 */ std::tuple(2, 1, KNX_DPT_FORMAT_B2, "DPT_Switch_Control"),
  /* KNX_DPT_2_002 */ std::tuple(2, 2, KNX_DPT_FORMAT_B2, "DPT_Bool_Control"),
  /* KNX_DPT_2_003 */ std::tuple(2, 3, KNX_DPT_FORMAT_B2, "DPT_Enable_Control"),
  /* KNX_DPT_2_004 */ std::tuple(2, 4, KNX_DPT_FORMAT_B2, "DPT_Ramp_Control"),
  /* KNX_DPT_2_005 */ std::tuple(2, 5, KNX_DPT_FORMAT_B2, "DPT_Alarm_Control"),
  /* KNX_DPT_2_006 */ std::tuple(2, 6, KNX_DPT_FORMAT_B2, "DPT_BinaryValue_Control"),
  /* KNX_DPT_2_007 */ std::tuple(2, 7, KNX_DPT_FORMAT_B2, "DPT_Step_Control"),
  /* KNX_DPT_2_008 */ std::tuple(2, 8, KNX_DPT_FORMAT_B2, "DPT_Direction1_Control"),
  /* KNX_DPT_2_009 */ std::tuple(2, 9, KNX_DPT_FORMAT_B2, "DPT_Direction2_Control"),
  /* KNX_DPT_2_010 */ std::tuple(2, 10, KNX_DPT_FORMAT_B2, "DPT_Start_Control"),
  /* KNX_DPT_2_011 */ std::tuple(2, 11, KNX_DPT_FORMAT_B2, "DPT_State_Control"),
  /* KNX_DPT_2_012 */ std::tuple(2, 12, KNX_DPT_FORMAT_B2, "DPT_Invert_Control"),
  /* KNX_DPT_3_007 */ std::tuple(3, 7, KNX_DPT_FORMAT_B1U3, "DPT_Control_Dimming"),
  /* KNX_DPT_3_008 */ std::tuple(3, 8, KNX_DPT_FORMAT_B1U3, "DPT_Control_Blinds"),
  /* KNX_DPT_4_001 */ std::tuple(4, 1, KNX_DPT_FORMAT_A8, "DPT_Char_ASCII"),
  /* KNX_DPT_4_002 */ std::tuple(4, 2, KNX_DPT_FORMAT_A8, "DPT_Char_8859_1"),
  /* KNX_DPT_5_001 */ std::tuple(5, 1, KNX_DPT_FORMAT_U8, "DPT_Scaling"),
  /* KNX_DPT_5_003 */ std::tuple(5, 3, KNX_DPT_FORMAT_U8, "DPT_Angle"),
  /* KNX_DPT_5_004 */ std::tuple(5, 4, KNX_DPT_FORMAT_U8, "DPT_Percent_U8"),
  /* KNX_DPT_5_005 */ std::tuple(5, 5, KNX_DPT_FORMAT_U8, "DPT_DecimalFactor"),
  /* KNX_DPT_5_006 */ std::tuple(5, 6, KNX_DPT_FORMAT_U8, "DPT_Tariff"),
  /* KNX_DPT_5_010 */ std::tuple(5, 10, KNX_DPT_FORMAT_U8, "DPT_Value_1_Ucount"),
  /* KNX_DPT_6_001 */ std::tuple(6, 1, KNX_DPT_FORMAT_V8, "DPT_Percent_V8"),
  /* KNX_DPT_6_010 */ std::tuple(6, 10, KNX_DPT_FORMAT_V8, "DPT_Value_1_Count"),
  /* KNX_DPT_6_020 */ std::tuple(6, 20, KNX_DPT_FORMAT_B5N3, "DPT_Status_Mode3"),
  /* KNX_DPT_7_001 */ std::tuple(7, 1, KNX_DPT_FORMAT_U16, "DPT_Value_2_Ucount"),
  /* KNX_DPT_7_002 */ std::tuple(7, 2, KNX_DPT_FORMAT_U16, "DPT_TimePeriodMsec"),
  /* KNX_DPT_7_003 */ std::tuple(7, 3, KNX_DPT_FORMAT_U16, "DPT_TimePeriod10MSec"),
  /* KNX_DPT_7_004 */ std::tuple(7, 4, KNX_DPT_FORMAT_U16, "DPT_TimePeriod100MSec"),
  /* KNX_DPT_7_005 */ std::tuple(7, 5, KNX_DPT_FORMAT_U16, "DPT_TimePeriodSec"),
  /* KNX_DPT_7_006 */ std::tuple(7, 6, KNX_DPT_FORMAT_U16, "DPT_TimePeriodMin"),
  /* KNX_DPT_7_007 */ std::tuple(7, 7, KNX_DPT_FORMAT_U16, "DPT_TimePeriodHrs"),
  /* KNX_DPT_7_010 */ std::tuple(7, 10, KNX_DPT_FORMAT_U16, "DPT_PropDataType"),
  /* KNX_DPT_7_011 */ std::tuple(7, 11, KNX_DPT_FORMAT_U16, "DPT_Length_mm"),
  /* KNX_DPT_7_012 */ std::tuple(7, 12, KNX_DPT_FORMAT_U16, "DPT_UElCurrentmA"),
  /* KNX_DPT_7_013 */ std::tuple(7, 13, KNX_DPT_FORMAT_U16, "DPT_Brightness"),
  /* KNX_DPT_7_600 */ std::tuple(7, 600, KNX_DPT_FORMAT_U16, "DPT_Absolute_Colour_Temperature"),
  /* KNX_DPT_8_001 */ std::tuple(8, 1, KNX_DPT_FORMAT_V16, "DPT_Value_2_Count"),
  /* KNX_DPT_8_002 */ std::tuple(8, 2, KNX_DPT_FORMAT_V16, "DPT_DeltaTimeMsec"),
  /* KNX_DPT_8_003 */ std::tuple(8, 3, KNX_DPT_FORMAT_V16, "DPT_DeltaTime10MSec"),
  /* KNX_DPT_8_004 */ std::tuple(8, 4, KNX_DPT_FORMAT_V16, "DPT_DeltaTime100MSec"),
  /* KNX_DPT_8_005 */ std::tuple(8, 5, KNX_DPT_FORMAT_V16, "DPT_DeltaTimeSec"),
  /* KNX_DPT_8_006 */ std::tuple(8, 6, KNX_DPT_FORMAT_V16, "DPT_DeltaTimeMin"),
  /* KNX_DPT_8_007 */ std::tuple(8, 7, KNX_DPT_FORMAT_V16, "DPT_DeltaTimeHrs"),
  /* KNX_DPT_8_010 */ std::tuple(8, 10, KNX_DPT_FORMAT_V16, "DPT_Percent_V16"),
  /* KNX_DPT_8_011 */ std::tuple(8, 11, KNX_DPT_FORMAT_V16, "DPT_Rotation_Angle"),
  /* KNX_DPT_8_012 */ std::tuple(8, 12, KNX_DPT_FORMAT_V16, "DPT_Length_m"),
  /* KNX_DPT_9_001 */ std::tuple(9, 1, KNX_DPT_FORMAT_F16, "DPT_Value_Temp"),
  /* KNX_DPT_9_002 */ std::tuple(9, 2, KNX_DPT_FORMAT_F16, "DPT_Value_Tempd"),
  /* KNX_DPT_9_003 */ std::tuple(9, 3, KNX_DPT_FORMAT_F16, "DPT_Value_Tempa"),
  /* KNX_DPT_9_004 */ std::tuple(9, 4, KNX_DPT_FORMAT_F16, "DPT_Value_Lux"),
  /* KNX_DPT_9_005 */ std::tuple(9, 5, KNX_DPT_FORMAT_F16, "DPT_Value_Wsp"),
  /* KNX_DPT_9_006 */ std::tuple(9, 6, KNX_DPT_FORMAT_F16, "DPT_Value_Pres"),
  /* KNX_DPT_9_007 */ std::tuple(9, 7, KNX_DPT_FORMAT_F16, "DPT_Value_Humidity"),
  /* KNX_DPT_9_008 */ std::tuple(9, 8, KNX_DPT_FORMAT_F16, "DPT_Value_AirQuality"),
  /* KNX_DPT_9_009 */ std::tuple(9, 9, KNX_DPT_FORMAT_F16, "DPT_Value_AirFlow"),
  /* KNX_DPT_9_010 */ std::tuple(9, 10, KNX_DPT_FORMAT_F16, "DPT_Value_Time1"),
  /* KNX_DPT_9_011 */ std::tuple(9, 11, KNX_DPT_FORMAT_F16, "DPT_Value_Time2"),
  /* KNX_DPT_9_020 */ std::tuple(9, 20, KNX_DPT_FORMAT_F16, "DPT_Value_Volt"),
  /* KNX_DPT_9_021 */ std::tuple(9, 21, KNX_DPT_FORMAT_F16, "DPT_Value_Curr"),
  /* KNX_DPT_9_022 */ std::tuple(9, 22, KNX_DPT_FORMAT_F16, "DPT_PowerDensity"),
  /* KNX_DPT_9_023 */ std::tuple(9, 23, KNX_DPT_FORMAT_F16, "DPT_KelvinPerPercent"),
  /* KNX_DPT_9_024 */ std::tuple(9, 24, KNX_DPT_FORMAT_F16, "DPT_Power"),
  /* KNX_DPT_9_025 */ std::tuple(9, 25, KNX_DPT_FORMAT_F16, "DPT_Value_Volume_Flow"),
  /* KNX_DPT_9_026 */ std::tuple(9, 26, KNX_DPT_FORMAT_F16, "DPT_Rain_Amount"),
  /* KNX_DPT_9_027 */ std::tuple(9, 27, KNX_DPT_FORMAT_F16, "DPT_Value_Temp_F"),
  /* KNX_DPT_9_028 */ std::tuple(9, 28, KNX_DPT_FORMAT_F16, "DPT_Value_Wsp_kmh"),
  /* KNX_DPT_9_029 */ std::tuple(9, 29, KNX_DPT_FORMAT_F16, "DPT_Value_Absolute_Humidity"),
  /* KNX_DPT_9_030 */ std::tuple(9, 30, KNX_DPT_FORMAT_F16, "DPT_Concentration_µgm3"),
  /* KNX_DPT_10_001 */ std::tuple(10, 1, KNX_DPT_FORMAT_N3N5R2N6R2N6, "DPT_TimeOfDay"),
  /* KNX_DPT_11_001 */ std::tuple(11, 1, KNX_DPT_FORMAT_R3N5R4N4R1U7, "DPT_Date"),
  /* KNX_DPT_12_001 */ std::tuple(12, 1, KNX_DPT_FORMAT_U32, "DPT_Value_4_Ucount"),
  /* KNX_DPT_12_100 */ std::tuple(12, 100, KNX_DPT_FORMAT_U32, "DPT_LongTimePeriod_Sec"),
  /* KNX_DPT_12_101 */ std::tuple(12, 101, KNX_DPT_FORMAT_U32, "DPT_LongTimePeriod_Min"),
  /* KNX_DPT_12_102 */ std::tuple(12, 102, KNX_DPT_FORMAT_U32, "DPT_LongTimePeriod_Hrs"),
  /* KNX_DPT_12_1200 */ std::tuple(12, 1200, KNX_DPT_FORMAT_U32, "DPT_VolumeLiquid_Litre"),
  /* KNX_DPT_12_1201 */ std::tuple(12, 1201, KNX_DPT_FORMAT_U32, "DPT_Volume_m3"),
  /* KNX_DPT_13_001 */ std::tuple(13, 1, KNX_DPT_FORMAT_V32, "DPT_Value_4_Count"),
  /* KNX_DPT_13_002 */ std::tuple(13, 2, KNX_DPT_FORMAT_V32, "DPT_FlowRate_m3/h"),
  /* KNX_DPT_13_010 */ std::tuple(13, 10, KNX_DPT_FORMAT_V32, "DPT_ActiveEnergy"),
  /* KNX_DPT_13_011 */ std::tuple(13, 11, KNX_DPT_FORMAT_V32, "DPT_ApparantEnergy"),
  /* KNX_DPT_13_012 */ std::tuple(13, 12, KNX_DPT_FORMAT_V32, "DPT_ReactiveEnergy"),
  /* KNX_DPT_13_013 */ std::tuple(13, 13, KNX_DPT_FORMAT_V32, "DPT_ActiveEnergy_kWh"),
  /* KNX_DPT_13_014 */ std::tuple(13, 14, KNX_DPT_FORMAT_V32, "DPT_ApparantEnergy_kVAh"),
  /* KNX_DPT_13_015 */ std::tuple(13, 15, KNX_DPT_FORMAT_V32, "DPT_ReactiveEnergy_kVARh"),
  /* KNX_DPT_13_016 */ std::tuple(13, 16, KNX_DPT_FORMAT_V32, "DPT_ActiveEnergy_MWh"),
  /* KNX_DPT_13_100 */ std::tuple(13, 100, KNX_DPT_FORMAT_V32, "DPT_LongDeltaTimeSec"),
  /* KNX_DPT_13_1200 */ std::tuple(13, 1200, KNX_DPT_FORMAT_V32, "DPT_DeltaVolumeLiquid_Litre"),
  /* KNX_DPT_13_1201 */ std::tuple(13, 1201, KNX_DPT_FORMAT_V32, "DPT_DeltaVolume_m3"),
  /* KNX_DPT_14_000 */ std::tuple(14, 0, KNX_DPT_FORMAT_F32, "DPT_Value_Acceleration"),
  /* KNX_DPT_14_001 */ std::tuple(14, 1, KNX_DPT_FORMAT_F32, "DPT_Value_Acceleration_Angular"),
  /* KNX_DPT_14_002 */ std::tuple(14, 2, KNX_DPT_FORMAT_F32, "DPT_Value_Activation_Energy"),
  /* KNX_DPT_14_003 */ std::tuple(14, 3, KNX_DPT_FORMAT_F32, "DPT_Value_Activity"),
  /* KNX_DPT_14_004 */ std::tuple(14, 4, KNX_DPT_FORMAT_F32, "DPT_Value_Mol"),
  /* KNX_DPT_14_005 */ std::tuple(14, 5, KNX_DPT_FORMAT_F32, "DPT_Value_Amplitude"),
  /* KNX_DPT_14_006 */ std::tuple(14, 6, KNX_DPT_FORMAT_F32, "DPT_Value_AngleRad"),
  /* KNX_DPT_14_007 */ std::tuple(14, 7, KNX_DPT_FORMAT_F32, "DPT_Value_AngleDeg"),
  /* KNX_DPT_14_008 */ std::tuple(14, 8, KNX_DPT_FORMAT_F32, "DPT_Value_Angular_Momentum"),
  /* KNX_DPT_14_009 */ std::tuple(14, 9, KNX_DPT_FORMAT_F32, "DPT_Value_Angular_Velocity"),
  /* KNX_DPT_14_010 */ std::tuple(14, 10, KNX_DPT_FORMAT_F32, "DPT_Value_Area"),
  /* KNX_DPT_14_011 */ std::tuple(14, 11, KNX_DPT_FORMAT_F32, "DPT_Value_Capacitance"),
  /* KNX_DPT_14_012 */ std::tuple(14, 12, KNX_DPT_FORMAT_F32, "DPT_Value_Charge_DensitySurface"),
  /* KNX_DPT_14_013 */ std::tuple(14, 13, KNX_DPT_FORMAT_F32, "DPT_Value_Charge_DensityVolume"),
  /* KNX_DPT_14_014 */ std::tuple(14, 14, KNX_DPT_FORMAT_F32, "DPT_Value_Compressibility"),
  /* KNX_DPT_14_015 */ std::tuple(14, 15, KNX_DPT_FORMAT_F32, "DPT_Value_Conductance"),
  /* KNX_DPT_14_016 */ std::tuple(14, 16, KNX_DPT_FORMAT_F32, "DPT_Value_Electrical_Conductivity"),
  /* KNX_DPT_14_017 */ std::tuple(14, 17, KNX_DPT_FORMAT_F32, "DPT_Value_Density"),
  /* KNX_DPT_14_018 */ std::tuple(14, 18, KNX_DPT_FORMAT_F32, "DPT_Value_Electric_Charge"),
  /* KNX_DPT_14_019 */ std::tuple(14, 19, KNX_DPT_FORMAT_F32, "DPT_Value_Electric_Current"),
  /* KNX_DPT_14_020 */ std::tuple(14, 20, KNX_DPT_FORMAT_F32, "DPT_Value_Electric_CurrentDensity"),
  /* KNX_DPT_14_021 */ std::tuple(14, 21, KNX_DPT_FORMAT_F32, "DPT_Value_Electric_DipoleMoment"),
  /* KNX_DPT_14_022 */ std::tuple(14, 22, KNX_DPT_FORMAT_F32, "DPT_Value_Electric_Displacement"),
  /* KNX_DPT_14_023 */ std::tuple(14, 23, KNX_DPT_FORMAT_F32, "DPT_Value_Electric_FieldStrength"),
  /* KNX_DPT_14_024 */ std::tuple(14, 24, KNX_DPT_FORMAT_F32, "DPT_Value_Electric_Flux"),
  /* KNX_DPT_14_025 */ std::tuple(14, 25, KNX_DPT_FORMAT_F32, "DPT_Value_Electric_FluxDensity"),
  /* KNX_DPT_14_026 */ std::tuple(14, 26, KNX_DPT_FORMAT_F32, "DPT_Value_Electric_Polarization"),
  /* KNX_DPT_14_027 */ std::tuple(14, 27, KNX_DPT_FORMAT_F32, "DPT_Value_Electric_Potential"),
  /* KNX_DPT_14_028 */ std::tuple(14, 28, KNX_DPT_FORMAT_F32, "DPT_Value_Electric_PotentialDifference"),
  /* KNX_DPT_14_029 */ std::tuple(14, 29, KNX_DPT_FORMAT_F32, "DPT_Value_ElectromagneticMoment"),
  /* KNX_DPT_14_030 */ std::tuple(14, 30, KNX_DPT_FORMAT_F32, "DPT_Value_Electromotive_Force"),
  /* KNX_DPT_14_031 */ std::tuple(14, 31, KNX_DPT_FORMAT_F32, "DPT_Value_Energy"),
  /* KNX_DPT_14_032 */ std::tuple(14, 32, KNX_DPT_FORMAT_F32, "DPT_Value_Force"),
  /* KNX_DPT_14_033 */ std::tuple(14, 33, KNX_DPT_FORMAT_F32, "DPT_Value_Frequency"),
  /* KNX_DPT_14_034 */ std::tuple(14, 34, KNX_DPT_FORMAT_F32, "DPT_Value_Angular_Frequency"),
  /* KNX_DPT_14_035 */ std::tuple(14, 35, KNX_DPT_FORMAT_F32, "DPT_Value_Heat_Capacity"),
  /* KNX_DPT_14_036 */ std::tuple(14, 36, KNX_DPT_FORMAT_F32, "DPT_Value_Heat_FlowRate"),
  /* KNX_DPT_14_037 */ std::tuple(14, 37, KNX_DPT_FORMAT_F32, "DPT_Value_Heat_Quantity"),
  /* KNX_DPT_14_038 */ std::tuple(14, 38, KNX_DPT_FORMAT_F32, "DPT_Value_Impedance"),
  /* KNX_DPT_14_039 */ std::tuple(14, 39, KNX_DPT_FORMAT_F32, "DPT_Value_Length"),
  /* KNX_DPT_14_040 */ std::tuple(14, 40, KNX_DPT_FORMAT_F32, "DPT_Value_Light_Quantity"),
  /* KNX_DPT_14_041 */ std::tuple(14, 41, KNX_DPT_FORMAT_F32, "DPT_Value_Luminance"),
  /* KNX_DPT_14_042 */ std::tuple(14, 42, KNX_DPT_FORMAT_F32, "DPT_Value_Luminous_Flux"),
  /* KNX_DPT_14_043 */ std::tuple(14, 43, KNX_DPT_FORMAT_F32, "DPT_Value_Luminous_Intensity"),
  /* KNX_DPT_14_044 */ std::tuple(14, 44, KNX_DPT_FORMAT_F32, "DPT_Value_Magnetic_FieldStrength"),
  /* KNX_DPT_14_045 */ std::tuple(14, 45, KNX_DPT_FORMAT_F32, "DPT_Value_Magnetic_Flux"),
  /* KNX_DPT_14_046 */ std::tuple(14, 46, KNX_DPT_FORMAT_F32, "DPT_Value_Magnetic_FluxDensity"),
  /* KNX_DPT_14_047 */ std::tuple(14, 47, KNX_DPT_FORMAT_F32, "DPT_Value_Magnetic_Moment"),
  /* KNX_DPT_14_048 */ std::tuple(14, 48, KNX_DPT_FORMAT_F32, "DPT_Value_Magnetic_Polarization"),
  /* KNX_DPT_14_049 */ std::tuple(14, 49, KNX_DPT_FORMAT_F32, "DPT_Value_Magnetization"),
  /* KNX_DPT_14_050 */ std::tuple(14, 50, KNX_DPT_FORMAT_F32, "DPT_Value_MagnetomotiveForce"),
  /* KNX_DPT_14_051 */ std::tuple(14, 51, KNX_DPT_FORMAT_F32, "DPT_Value_Mass"),
  /* KNX_DPT_14_052 */ std::tuple(14, 52, KNX_DPT_FORMAT_F32, "DPT_Value_MassFlux"),
  /* KNX_DPT_14_053 */ std::tuple(14, 53, KNX_DPT_FORMAT_F32, "DPT_Value_Momentum"),
  /* KNX_DPT_14_054 */ std::tuple(14, 54, KNX_DPT_FORMAT_F32, "DPT_Value_Phase_AngleRad"),
  /* KNX_DPT_14_055 */ std::tuple(14, 55, KNX_DPT_FORMAT_F32, "DPT_Value_Phase_AngleDeg"),
  /* KNX_DPT_14_056 */ std::tuple(14, 56, KNX_DPT_FORMAT_F32, "DPT_Value_Power"),
  /* KNX_DPT_14_057 */ std::tuple(14, 57, KNX_DPT_FORMAT_F32, "DPT_Value_Power_Factor"),
  /* KNX_DPT_14_058 */ std::tuple(14, 58, KNX_DPT_FORMAT_F32, "DPT_Value_Pressure"),
  /* KNX_DPT_14_059 */ std::tuple(14, 59, KNX_DPT_FORMAT_F32, "DPT_Value_Reactance"),
  /* KNX_DPT_14_060 */ std::tuple(14, 60, KNX_DPT_FORMAT_F32, "DPT_Value_Resistance"),
  /* KNX_DPT_14_061 */ std::tuple(14, 61, KNX_DPT_FORMAT_F32, "DPT_Value_Resistivity"),
  /* KNX_DPT_14_062 */ std::tuple(14, 62, KNX_DPT_FORMAT_F32, "DPT_Value_SelfInductance"),
  /* KNX_DPT_14_063 */ std::tuple(14, 63, KNX_DPT_FORMAT_F32, "DPT_Value_SolidAngle"),
  /* KNX_DPT_14_064 */ std::tuple(14, 64, KNX_DPT_FORMAT_F32, "DPT_Value_Sound_Intensity"),
  /* KNX_DPT_14_065 */ std::tuple(14, 65, KNX_DPT_FORMAT_F32, "DPT_Value_Speed"),
  /* KNX_DPT_14_066 */ std::tuple(14, 66, KNX_DPT_FORMAT_F32, "DPT_Value_Stress"),
  /* KNX_DPT_14_067 */ std::tuple(14, 67, KNX_DPT_FORMAT_F32, "DPT_Value_Surface_Tension"),
  /* KNX_DPT_14_068 */ std::tuple(14, 68, KNX_DPT_FORMAT_F32, "DPT_Value_Common_Temperature"),
  /* KNX_DPT_14_069 */ std::tuple(14, 69, KNX_DPT_FORMAT_F32, "DPT_Value_Absolute_Temperature"),
  /* KNX_DPT_14_070 */ std::tuple(14, 70, KNX_DPT_FORMAT_F32, "DPT_Value_TemperatureDifference"),
  /* KNX_DPT_14_071 */ std::tuple(14, 71, KNX_DPT_FORMAT_F32, "DPT_Value_Thermal_Capacity"),
  /* KNX_DPT_14_072 */ std::tuple(14, 72, KNX_DPT_FORMAT_F32, "DPT_Value_Thermal_Conductivity"),
  /* KNX_DPT_14_073 */ std::tuple(14, 73, KNX_DPT_FORMAT_F32, "DPT_Value_ThermoelectricPower"),
  /* KNX_DPT_14_074 */ std::tuple(14, 74, KNX_DPT_FORMAT_F32, "DPT_Value_Time"),
  /* KNX_DPT_14_075 */ std::tuple(14, 75, KNX_DPT_FORMAT_F32, "DPT_Value_Torque"),
  /* KNX_DPT_14_076 */ std::tuple(14, 76, KNX_DPT_FORMAT_F32, "DPT_Value_Volume"),
  /* KNX_DPT_14_077 */ std::tuple(14, 77, KNX_DPT_FORMAT_F32, "DPT_Value_Volume_Flux"),
  /* KNX_DPT_14_078 */ std::tuple(14, 78, KNX_DPT_FORMAT_F32, "DPT_Value_Weight"),
  /* KNX_DPT_14_079 */ std::tuple(14, 79, KNX_DPT_FORMAT_F32, "DPT_Value_Work"),
  /* KNX_DPT_14_080 */ std::tuple(14, 80, KNX_DPT_FORMAT_F32, "DPT_Value_ApparentPower"),
  /* KNX_DPT_14_1200 */ std::tuple(14, 1200, KNX_DPT_FORMAT_F32, "DPT_Volume_Flux_Meter"),
  /* KNX_DPT_14_1201 */ std::tuple(14, 1201, KNX_DPT_FORMAT_F32, "DPT_Volume_Flux_ls"),
};

static_assert(KNX_DPT_COUNT == sizeof(dpt_table)/sizeof(*dpt_table), "Array initializer list does not match enum");


}  // namespace knx
}  // namespace esphome
