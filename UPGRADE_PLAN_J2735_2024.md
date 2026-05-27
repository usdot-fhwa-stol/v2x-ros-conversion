# Plan: Upgrade J2735 2016 → 2024 — ROS Message Definitions + ros-conversion

## Context

The ros-conversion and CARMAMsgs packages are being upgraded from J2735 2016 to J2735 2024.
The J2735 2024 standard (September 2024 release) introduces 500 new types, removes 1 type
(typo fix), and modifies fields in 15 existing types compared to the 2016 standard.

Ji Liu has already done the mechanical groundwork:
- `cpp_message` now depends on `stol-j2735-2024-carma` (commit 77e04b4)
- ASN.1 type name prefixes fixed in `BSM_Message.cpp` (e.g. `ITIS_VehicleGroupAffected_t`, `Common_Latitude_t`)

The full upgrade requires two phases:
1. **Group 0:** Create/update ROS `.msg` files in `CARMAMsgs/j2735_v2x_msgs` and
   `CARMAMsgs/carma_v2x_msgs` to match the 2024 ASN.1 schema
2. **Groups 1-7:** Update converter code in `ros-conversion/` to compile against 2024
   headers and handle new/deprecated fields

Group 1 (compile-breaking field renames) is already complete.

**Sources:**
- `j2735_202409/*.asn` — 43 ASN.1 modules defining the 2024 standard
- `compare_2016_vs_2024.json` — authoritative diff (500 new types, 1 removed, 15 types with field changes)

**Out of scope:** TravelerDataFrame — no TIM encoder/decoder exists in ros-conversion.

---

## Change Groups

### Group 0 — ROS Message Definitions Upgrade

#### 0.1 Priority Tiers

| Tier | Criteria | Approx new .msg files |
|------|----------|-----------------------|
| **P0 — Compile-critical** | Types referenced by existing handlers (BSM, SPAT, MAP, PSM, SDSM) that changed or added new dependencies in 2024 | ~10 |
| **P1 — Safety-critical new messages** | New top-level V2X message types: RSM, CCM, MSCM, TLSM, RWM | ~194 |
| **P2 — Deferred** | Toll/charging (J3217, ISO17573), probe data (J2945-C), placeholder types (PSM2, RGA, TSPaT, SCPR, SCPS = NULL) | ~235 |

---

#### 0.2 P0 — Compile-Critical Prerequisites

These new `.msg` files must exist before Groups 2-7 can update their parent types.

##### 0.2a New .msg files in `j2735_v2x_msgs/msg/`

| New file | ASN.1 source | Required by | Fields |
|----------|-------------|-------------|--------|
| `Axles.msg` | Common | Group 2 (VehicleData) | `int32 total_axles` (required), optional `int32 front_axles`/`rear_axles` with exists flags |
| `TrailersJ2945Slash1B.msg` | Common | Group 3 (SupplementalVehicleExtensions) | `TrailerUnitDescJ2945Slash1B[] trailers` (1..8) |
| `TrailerUnitDescJ2945Slash1B.msg` | Common | Group 3 | Full trailer unit fields per J2945/1B |
| `SchoolBusJ2945Slash1C.msg` | Common | Group 3 | `bool flashing_amber_lights`, `bool flashing_red_lights` + 5 optional booleans |
| `RptVehicleClass.msg` | J2945-C | Group 3 | ENUMERATED: FHWA vehicle classes (1-13) |
| `RoadAuthorityID.msg` | Common | Group 6 (IntersectionGeometry/State) | CHOICE: `full_rd_auth_id` (OID bytes) or `rel_rd_auth_id` (Relative-OID bytes) |

##### 0.2b carma_v2x_msgs mirrors needed

| j2735_v2x_msgs type | carma_v2x_msgs mirror? | Reason |
|---------------------|----------------------|--------|
| Axles | **Yes** | VehicleData.msg exists in carma_v2x_msgs |
| TrailersJ2945Slash1B | **Yes** | SupplementalVehicleExtensions.msg exists |
| TrailerUnitDescJ2945Slash1B | **Yes** | Referenced by TrailersJ2945Slash1B |
| SchoolBusJ2945Slash1C | **Yes** | Referenced by SupplementalVehicleExtensions |
| RptVehicleClass | No | Enum pass-through, no unit conversion |
| RoadAuthorityID | No | OID bytes pass-through, no unit conversion |

---

#### 0.3 P1 — New Safety-Critical Message Types

For each new top-level message, create `.msg` files in `j2735_v2x_msgs/msg/` for all types
in the corresponding ASN.1 module. Follow existing naming: remove hyphens, PascalCase
(e.g. `LaneAttributes-Barrier` → `LaneAttributesBarrier.msg`).

##### 0.3a CooperativeControlMessage (J2945/6) — 27 types

**ASN:** `J2945-6-CooperativeControlMessage-2024-rel-v1.1.1.asn` | **DSRCmsgID:** 40

Platooning/cooperative adaptive cruise control. Contains BSMcoreData + Part II extensions.

Key types: `CooperativeControlMessage`, `CCMPartIIcontent`, `LightVehicleCCMExtensions`,
`HeavyTruckCCMExtensions`, `LongitudinalControlState`, `AccelTimeConstant`, `AxesMovement`,
`TimeConstant`, `RoadGrade`, `BrakePedalPosition`, `AcceleratorPedalPosition`, `Torque`,
`Yaw`, `Pitch`, `Roll`, `PitchRate`, `RollRate`, `SeparationDistance`, `TotalMass`,
`MaxAvailableAcceleration`, `MaxAvailableDeceleration`, `CCMFaultMode`, `ManeuverID`, `FrontCutIn`

**carma_v2x_msgs conversions:** `SeparationDistance` (0.1m LSB), `MaxAvailableAcceleration/Deceleration`
(0.01 m/s² LSB), `Torque` (5 N·m LSB), `TotalMass` (100 kg LSB), `TimeConstant` (0.1s LSB),
`RoadGrade` (0.2% LSB), pedal positions (0.5% LSB).

**New files:** ~27 j2735_v2x_msgs + ~8 carma_v2x_msgs

##### 0.3b ManeuverSharingAndCoordinatingMessage (J3186) — 23 types

**ASN:** `J3186-ManeuverSharingandCoordinatingMessage-2024-rel-v1.1.asn` | **DSRCmsgID:** 42

Cooperative maneuver negotiation (lane changes, merges).

Key types: `ManeuverSharingAndCoordinatingMessage`, `MSCMType`, `Maneuver`, `SubManeuver`,
`SubManeuverList`, `CurrentStateData`, `TargetRoadResource`, `TRRType`, `TRRLocation` (CHOICE),
`TRRLocationType1/2/3`, `SurroundingVehicleInfo`, `TemporaryIDList`, `TemporaryIDPointer`,
`ObjectDistance`, `PositionOffsetXYZ`, `TRRLength`, `LaneOffset`, `ResponseFlag`, `ReasonCode`,
`ManeuverExecutionStatus`

**Name conflicts:** `ManeuverID`, `PositionOffsetXYZ`, `ObjectDistance` are also defined in CCM
and SDSM. Use a single shared definition in `j2735_v2x_msgs` with the superset range.

**New files:** ~23 j2735_v2x_msgs + ~5 carma_v2x_msgs

##### 0.3c TrafficLightStatusMessage (J2735) — 17 types

**ASN:** `J2735-TrafficLightStatusMessage-2024-rel-v1.1.2.asn` | **DSRCmsgID:** 50

Detailed traffic signal state. Complements existing SPAT handling.

Key types: `TrafficLightStatusMessage`, `TrafficLightStatusList`, `TrafficLightStatus`,
`TrafficLightID` (CHOICE), `TrafficLightType`, `TrafficLightIntervalType`,
`TrafficLightControllerStatus` (BIT STRING), `TrafficLightOperationStatus` (BIT STRING),
`TrafficLightingStatus`, `TrafficLightDirectionCode`, `ConsecutiveTrafficLight`,
`PermissiveNonProtected`, `ActuatedInterval`, `PedestrianCall`, `ReservedBit`,
`TimeInSecondB16`, `TimeInSecondB8`

**New files:** ~17 j2735_v2x_msgs + ~3 carma_v2x_msgs

##### 0.3d RoadWeatherMessage (J2945/3) — 52 types

**ASN:** `J2945-3-RoadWeatherMessage-2024-rel-v2.1.asn` | **DSRCmsgID:** 34

Weather data upload/download for V2X environmental perception.

Key types: `RoadWeatherMessage` (CHOICE), `RoadWeatherUpload`, `RoadWeatherDownload`,
`VehicleReport`, `WDMSFleetData`, `SnapShot`, plus ~20 NTCIP data types (`NtcipEssData`,
`NtcipAirTemperature`, `NtcipWind`, etc.) and ~10 measurement StdDev types.

**Common module dependencies (shared with RSM):** `FrictionInformation`,
`DescriptionOfRoadSurface` (CHOICE), plus 8 surface sub-types and their Type enums:
`PortlandCement`/`PortlandCementType`, `AsphaltOrTar`/`AsphaltOrTarType`,
`Gravel`/`GravelType`, `Grass`/`GrassType`, `Cinders`/`CindersType`, `Rock`/`RockType`,
`Ice`/`IceType`, `Snow`/`SnowType`, `RoadSurfaceCondition`, `RoadRoughness`.

**New files:** ~52 j2735_v2x_msgs + ~10 Common surface types + ~8 carma_v2x_msgs

##### 0.3e RoadSafetyMessage (J2945/4) — 41 types

**ASN:** `J2945-4-RoadSafetyMessage-2024-rel-v1.1.1.asn` | **DSRCmsgID:** 33

Road hazard/incident alerts with geospatial containers. Largest new safety-critical module.

Key types: `RoadSafetyMessage`, `CommonContainer`, `ContentContainer` (CHOICE),
`CurveContainer`, `DynamicInfoContainer`, `IncidentsContainer`, `LaneClosureContainer`,
`ReducedSpeedZoneContainer`, `SituationalContainer`, plus ~30 supporting/primitive types
(`ApplicableHeading`, `AreaType`, `BroadRegion`, `CrossLinking`, `EventInfo`, `LaneInfo`,
`NodePointLLE`, `Obstructions`, `Path`, `PathList`, `RegionInfo`, `RSMPolygon`, `VisualLink`,
`AudioLink`, etc.)

**Dependencies:** Imports `FrictionInformation` + surface types from Common (created in 0.3d),
`NTCIPEssVisibility` from RWM (created in 0.3d), `Circle`/`Zoom` from TravelerInformation
(already in j2735_v2x_msgs).

**New files:** ~41 j2735_v2x_msgs + ~5 carma_v2x_msgs

##### 0.3f Additional SPAT / Common Module Types

New types in existing modules that should be created for completeness:

**SPAT module:** `SpeedAdvice`, `PedestrianBicycleDetect`, `WaitOnStopline`, `ZoneLength`,
`TimeIntervalConfidence`, `TimeMark` (~6 files)

**Common module shared types:** `FrictionInformation`, `DescriptionOfRoadSurface`, 8 surface
sub-types + Type enums, `RoadSurfaceCondition`, `RoadRoughness` (~14 files, created as part of 0.3d)

---

#### 0.4 P2 — Deferred (no action this release)

| Module | Types | Reason |
|--------|-------|--------|
| ISO17573 EFC | 109 | Toll infrastructure |
| J3217 (all toll/charging msgs) | 65 | Toll infrastructure |
| J2945-C ProbeDataConfig/Report | 61 | No CARMA handler (except `RptVehicleClass` → P0) |
| Placeholder types (PSM2, RGA, TSPaT, SCPR, SCPS) | 5 | Defined as `NULL`, no content |

---

#### 0.5 SignPriority Typo Fix

The 2024 standard renames `SignPrority` → `SignPriority`. No `SignPrority.msg` exists in
`j2735_v2x_msgs` — **no action needed**.

---

#### 0.6 Implementation Order for Group 0

1. **0.2a+b** — Create 6 P0 `.msg` files in j2735_v2x_msgs + 4 carma_v2x_msgs mirrors → unblocks Groups 2-3, 6
2. **0.3f** — Create new SPAT + Common shared types
3. **0.3a** — CCM (depends only on existing types)
4. **0.3b** — MSCM (depends only on existing types + shared primitives)
5. **0.3c** — TLSM (self-contained)
6. **0.3d** — RWM (many NTCIP types; also creates Common surface types)
7. **0.3e** — RSM (depends on Common surface types from 0.3d, SPAT SpeedAdvice from 0.3f)
8. Create carma_v2x_msgs mirrors for all P1 top-level messages

---

#### 0.7 Verification for Group 0

1. `colcon build --packages-select j2735_v2x_msgs carma_v2x_msgs` — must succeed with all new `.msg` files
2. Every `.msg` referencing another package's type must have the dependency declared in `package.xml`
3. Cross-check: all P0 and P1 types from `compare_2016_vs_2024.json` have corresponding `.msg` files

---

#### 0.8 New File Count Summary

| Package | P0 | P1 | Total |
|---------|----|----|-------|
| `j2735_v2x_msgs/msg/` | 6 | ~160 | ~166 |
| `carma_v2x_msgs/msg/` | 4 | ~34 | ~38 |
| **Total** | **10** | **~194** | **~204** |

#### 0.9 `doNotUse` Field Audit and Cleanup

The J2735 2024 standard renames several previously-active fields to `doNotUse` to
deprecate them while preserving wire-format backward compatibility. This step identifies
all such fields, updates ROS `.msg` definitions, and defines the handling policy for
converter code.

##### 0.9a Complete inventory of `doNotUse` fields

Searched all 43 ASN.1 modules in `j2735_202409/*.asn` for `doNotUse`. 12 occurrences found
across 4 types in 2 message families:

**BSM — `BasicSafetyMessage` module:**

| # | Parent type | 2024 field name | 2016 field name | ASN.1 type | OPTIONAL? |
|---|-------------|----------------|-----------------|------------|-----------|
| 1 | `SpecialVehicleExtensions` | `doNotUse` | `trailers` | `TrailerData` | OPTIONAL |
| 2 | `SupplementalVehicleExtensions` | `doNotUse1` | `weatherReport` | `WeatherReport` | OPTIONAL |
| 3 | `SupplementalVehicleExtensions` | `doNotUse2` | `weatherProbe` | `WeatherProbe` | OPTIONAL |
| 4 | `SupplementalVehicleExtensions` | `doNotUse3` | `obstacle` | `ObstacleDetection` | OPTIONAL |
| 5 | `SupplementalVehicleExtensions` | `doNotUse4` | `speedProfile` | `SpeedProfile` | OPTIONAL |
| 6 | `SupplementalVehicleExtensions` | `doNotUse5` | `theRTCM` | `RTCMPackage` | OPTIONAL |
| 7 | `TrailerData` | `doNotUse` | `notUsed` / `sspIndex` | `SSPindex` | REQUIRED |
| 8 | `VehicleData` | `doNotUse` | `trailerWeight` | `TrailerWeight` | OPTIONAL |

**TIM — `TravelerInformation` module:**

| # | Parent type | 2024 field name | 2016 field name | ASN.1 type | OPTIONAL? |
|---|-------------|----------------|-----------------|------------|-----------|
| 9 | `TravelerDataFrame` | `doNotUse1` | `sspTimRights` | `SSPindex` | REQUIRED |
| 10 | `TravelerDataFrame` | `doNotUse2` | `sspLocationRights` | `SSPindex` | REQUIRED |
| 11 | `TravelerDataFrame` | `doNotUse3` | `sspMsgRights1` | `SSPindex` | REQUIRED |
| 12 | `TravelerDataFrame` | `doNotUse4` | `sspMsgRights2` | `SSPindex` | REQUIRED |

##### 0.9b Current `.msg` state and required changes

**OPTIONAL `doNotUse` fields — remove from `.msg` presence vectors:**

These fields should be removed from the presence vector constants and their field
declarations dropped entirely. They must not be decoded or encoded.

| `.msg` file | Field to remove | Presence constant to remove |
|------------|-----------------|----------------------------|
| `j2735_v2x_msgs/msg/SpecialVehicleExtensions.msg` | `trailers` (TrailerData) | `HAS_TRAILERS = 4` |
| `j2735_v2x_msgs/msg/SupplementalVehicleExtensions.msg` | `weather_report` | `HAS_WEATHER_REPORT = 8` |
| `j2735_v2x_msgs/msg/SupplementalVehicleExtensions.msg` | `weather_probe` | `HAS_WEATHER_PROBE = 16` |
| `j2735_v2x_msgs/msg/SupplementalVehicleExtensions.msg` | `obstacle` | `HAS_OBSTACLE = 32` |
| `j2735_v2x_msgs/msg/SupplementalVehicleExtensions.msg` | `speed_profile` | `HAS_SPEED_PROFILE = 128` |
| `j2735_v2x_msgs/msg/SupplementalVehicleExtensions.msg` | `the_rtcm` | `HAS_THE_RTCM = 256` |
| `j2735_v2x_msgs/msg/VehicleData.msg` | `trailer_weight` | `HAS_TRAILER_WEIGHT = 8` |
| `carma_v2x_msgs/msg/SpecialVehicleExtensions.msg` | `trailers` | `HAS_TRAILERS = 4` |
| `carma_v2x_msgs/msg/SupplementalVehicleExtensions.msg` | `weather_report` | `HAS_WEATHER_REPORT = 8` |
| `carma_v2x_msgs/msg/SupplementalVehicleExtensions.msg` | `weather_probe` | `HAS_WEATHER_PROBE = 16` |
| `carma_v2x_msgs/msg/SupplementalVehicleExtensions.msg` | `obstacle` | `HAS_OBSTACLE = 32` |
| `carma_v2x_msgs/msg/SupplementalVehicleExtensions.msg` | `speed_profile` | `HAS_SPEED_PROFILE = 128` |
| `carma_v2x_msgs/msg/SupplementalVehicleExtensions.msg` | `the_rtcm` | `HAS_THE_RTCM = 256` |
| `carma_v2x_msgs/msg/VehicleData.msg` | `trailer_weight` | `HAS_TRAILER_WEIGHT = 8` |

**REQUIRED `doNotUse` fields — rename but keep (wire-format placeholder):**

These are REQUIRED in the ASN.1 SEQUENCE, so they must remain in the `.msg` to preserve
encoding position. Rename the field to `do_not_use` and add a comment.

| `.msg` file | Current field | New field name |
|------------|---------------|----------------|
| `j2735_v2x_msgs/msg/TrailerData.msg` | `uint8 ssp_index` | `uint8 do_not_use` |
| `carma_v2x_msgs/msg/TrailerData.msg` | `uint8 ssp_index` | `uint8 do_not_use` |
| `j2735_v2x_msgs/msg/TravelerDataFrame.msg` | `do_not_use1..4` | Already correct (from TIM commit) |

##### 0.9c Converter code policy for `doNotUse` fields

**Decode (ASN → ROS):** Do NOT decode `doNotUse` fields into ROS messages. If the ASN.1
struct's `doNotUse` pointer is non-null (for OPTIONAL fields) or the value is non-zero
(for REQUIRED SSPindex fields), log a `RCLCPP_WARN` with the message:
`"Received non-empty doNotUse field '<field_name>' in <TypeName> — ignoring per J2735 2024"`

**Encode (ROS → ASN):** Do NOT encode `doNotUse` fields. For OPTIONAL fields, leave the
pointer as `nullptr`. For REQUIRED SSPindex fields, set to `0`.

##### 0.9d Renumbering of presence vector constants

After removing `doNotUse` fields, the remaining presence vector constants keep their
original bit values to avoid breaking existing code that checks specific bits.
New fields added in Groups 2-3 continue from the next available power-of-2.

---

### Group 1 — Compile-breaking field renames in `BSM_Message.cpp`

The 2024 ASN.1 C headers renamed these struct members. The current code will **not compile**
against `stol-j2735-2024-carma` until these are fixed.

File: `ros-conversion/ros-conversion/cpp_message/src/BSM_Message.cpp`

| Type | Old member | New member |
|------|-----------|-----------|
| `SpecialVehicleExtensions` | `->trailers` | `->doNotUse` |
| `SupplementalVehicleExtensions` | `->weatherReport` | `->doNotUse1` |
| `SupplementalVehicleExtensions` | `->weatherProbe` | `->doNotUse2` |
| `SupplementalVehicleExtensions` | `->obstacle` | `->doNotUse3` |
| `SupplementalVehicleExtensions` | `->speedProfile` | `->doNotUse4` |
| `SupplementalVehicleExtensions` | `->theRTCM` | `->doNotUse5` |
| `VehicleData` | `->trailerWeight` | `->doNotUse` |

**Policy for deprecated fields (UPDATED per 0.9c):** Do NOT process these fields.
Remove all existing decode/encode logic for `doNotUse` fields. Instead:
- **Decode:** If the `doNotUse` pointer is non-null, log `RCLCPP_WARN` and skip.
- **Encode:** Leave `doNotUse` pointers as `nullptr` (do not populate from ROS msg).

The corresponding ROS `.msg` fields are removed (per 0.9b), so there is nothing to
read from or write to on the ROS side.

**Specific code to remove in `BSM_Message.cpp`:**
- `SpecialVehicleExtensions.doNotUse` (trailers): ~180 lines of trailer decode (lines ~578–754)
  and ~100 lines of trailer encode (lines ~2020–2088). Replace with `RCLCPP_WARN`.
- `SupplementalVehicleExtensions.doNotUse1` (weatherReport): decode ~lines 842–870 + encode ~2236–2283.
- `SupplementalVehicleExtensions.doNotUse2` (weatherProbe): decode ~lines 872–920 + encode ~2288–2344.
- `SupplementalVehicleExtensions.doNotUse3` (obstacle): decode ~lines 922–960 + encode ~2440–2504.
- `SupplementalVehicleExtensions.doNotUse4` (speedProfile): decode ~lines 962–990 + encode ~2532–2554.
- `VehicleData.doNotUse` (trailerWeight): decode ~line 480 + encode ~2217–2228.
- `SupplementalVehicleExtensions.doNotUse5` (theRTCM): not implemented — no code to remove.

**Also remove from `bsm_convertor.cpp`:**
- `trailer_weight` conversion (~line 324–326)
- `weather_report` conversion (~lines 330–363)
- `weather_probe` conversion (following weather_report)
- `obstacle` conversion
- `speed_profile` conversion
- `the_rtcm` — not implemented, nothing to remove

---

### Group 2 — New fields: `VehicleData` (BSM Part II)

**2024 `VehicleData` struct** (header: `VehicleData.h`) adds four extension fields:
- `BOOLEAN_t *trailerPresent`
- `PivotPointDescription_t *pivotPoint`
- `Axles_t *axles` (totalAxles, optional frontAxles, rearAxles)
- `long *leanAngle`

#### 2a. New .msg types needed in `CARMAMsgs/j2735_v2x_msgs/msg/`
- `Axles.msg` — `int32 total_axles`, optional `int32 front_axles` / `rear_axles` with exists flags
  (No .msg exists yet; `Axles_t` is defined in `Axles.h`)

#### 2b. Update `j2735_v2x_msgs/msg/VehicleData.msg`
- **Remove** `HAS_TRAILER_WEIGHT = 8` and `trailer_weight` field (doNotUse per 0.9b)
- Add presence flags: `HAS_TRAILER_PRESENT=16`, `HAS_PIVOT_POINT=32`, `HAS_AXLES=64`, `HAS_LEAN_ANGLE=128`
- Add fields: `bool trailer_present`, `j2735_v2x_msgs/PivotPointDescription pivot_point`,
  `j2735_v2x_msgs/Axles axles`, `int32 lean_angle`

#### 2c. Mirror changes in `CARMAMsgs/carma_v2x_msgs/msg/VehicleData.msg`

#### 2d. `BSM_Message.cpp` — add decode/encode for new fields + remove `doNotUse`
- **Remove** existing decode/encode for `trailerWeight` / `doNotUse`. Replace with
  `RCLCPP_WARN` if `vehicleData->doNotUse` is non-null on decode (per 0.9c).
- In decode path: check `vehicleData->trailerPresent`, `->pivotPoint`, `->axles`, `->leanAngle`
  and populate the ROS msg with presence flags set accordingly.
- In encode path: if presence flag set, allocate and fill the corresponding ASN struct pointers.
  Leave `doNotUse` as `nullptr`.

#### 2e. `bsm_convertor.cpp` — add conversion for new fields + remove `doNotUse`
- **Remove** existing `trailer_weight` conversion (the field no longer exists in the .msg)
- `trailer_present`: boolean, pass through (no unit conversion)
- `pivot_point`: already handled elsewhere in the file — reuse `decode_pivot_point_description()`
- `axles`: pass through integer counts, no unit conversion
- `lean_angle`: verify unit from standard (likely tenths of degrees); apply conversion constant

---

### Group 3 — New fields: `SupplementalVehicleExtensions` (BSM Part II)

**2024 adds three extension fields** (header: `SupplementalVehicleExtensions.h`):
- `RptVehicleClass_t *fhwaVehicleClass`
- `TrailersJ2945Slash1B_t *trailers` (list of `TrailerUnitDescJ2945Slash1B`, **different type** from old `TrailerData`)
- `SchoolBusJ2945Slash1C_t *schoolBus` (boolean school bus status flags)

#### 3a. New .msg types needed in `j2735_v2x_msgs/msg/`
- `TrailersJ2945Slash1B.msg` — wraps `TrailerUnitDescJ2945Slash1B[]` list
- `TrailerUnitDescJ2945Slash1B.msg` — fields per `TrailerUnitDescJ2945Slash1B.h`
- `SchoolBusJ2945Slash1C.msg` — boolean flags from `SchoolBusJ2945Slash1C.h`
  (flashingAmberLights, flashingRedLights, + 5 optional booleans with exists flags)
- `RptVehicleClass.msg` — enum for FHWA vehicle classes (from `RptVehicleClass.h`)

#### 3b. Update `j2735_v2x_msgs/msg/SupplementalVehicleExtensions.msg`
- **Remove** `doNotUse` fields and their presence constants (per 0.9b):
  `HAS_WEATHER_REPORT`, `HAS_WEATHER_PROBE`, `HAS_OBSTACLE`, `HAS_SPEED_PROFILE`, `HAS_THE_RTCM`
  and fields `weather_report`, `weather_probe`, `obstacle`, `speed_profile`, `the_rtcm`
- Add presence flags: `HAS_FHWA_VEHICLE_CLASS=512`, `HAS_TRAILERS=1024`, `HAS_SCHOOL_BUS=2048`
- Add fields: `j2735_v2x_msgs/RptVehicleClass fhwa_vehicle_class`,
  `j2735_v2x_msgs/TrailersJ2945Slash1B trailers`,
  `j2735_v2x_msgs/SchoolBusJ2945Slash1C school_bus`

#### 3c. Mirror changes in `carma_v2x_msgs/msg/SupplementalVehicleExtensions.msg`
- Same removals and additions as 3b

#### 3d. `BSM_Message.cpp` — add decode/encode for new extension fields + remove `doNotUse`
- **Remove** all existing decode/encode logic for `weatherReport` (`doNotUse1`),
  `weatherProbe` (`doNotUse2`), `obstacle` (`doNotUse3`), `speedProfile` (`doNotUse4`).
  (`theRTCM` / `doNotUse5` was never implemented — no code to remove.)
- Replace each with an `RCLCPP_WARN` if the `doNotUse*` pointer is non-null on decode (per 0.9c).
- On encode, leave all `doNotUse*` pointers as `nullptr`.
- Add decode/encode for new fields: check pointer, set presence flag, populate ROS fields.

#### 3e. `bsm_convertor.cpp` — add conversion for new fields + remove `doNotUse`
- **Remove** existing conversion for `weather_report`, `weather_probe`, `obstacle`,
  `speed_profile`, `the_rtcm` (these fields no longer exist in the .msg)
- `fhwa_vehicle_class`: pass-through enum integer, no conversion
- `trailers`: new list-based type; iterate and convert each `TrailerUnitDescJ2945Slash1B`
- `school_bus`: boolean flags, pass through

---

### Group 4 — `VehicleEventFlags.msg` — new bit constant

File: `CARMAMsgs/j2735_v2x_msgs/msg/VehicleEventFlags.msg`

The 2024 `VehicleEventFlags.h` defines `VehicleEventFlags_eventJackKnife = 13`.
Add constant: `uint16 EVENT_JACK_KNIFE=8192`  (bit 13 → 2^13 = 8192)

No conversion code changes needed (BIT STRING passes through as integer).

---

### Group 5 — `LaneSharing.msg` — removed field, new field

File: `CARMAMsgs/j2735_v2x_msgs/msg/LaneSharing.msg`

The 2024 `LaneSharing.h` renames bit 9 from `pedestrianTraffic` → `reserved`.
- Remove constant `pedestrianTraffic` (if defined as named constant)
- The file currently only has the description comment; no named constants exist
- Add a comment update noting bit 9 is now `reserved` (not `pedestrianTraffic`)

No conversion code changes needed.

---

### Group 6 — `IntersectionGeometry` / `IntersectionState` — new `roadAuthorityID` field

Both 2024 structs have `struct RoadAuthorityID *roadAuthorityID` (OPTIONAL).
`RoadAuthorityID` is a CHOICE: `fullRdAuthID` (OBJECT_IDENTIFIER bytes)
or `relRdAuthID` (RELATIVE-OID bytes).

#### 6a. New .msg types in `j2735_v2x_msgs/msg/`
- `RoadAuthorityID.msg`:
  ```
  uint8 CHOICE_FULL_RD_AUTH_ID=0
  uint8 CHOICE_REL_RD_AUTH_ID=1
  uint8 choice
  uint8[] id_bytes   # OID or Relative-OID encoded as DER byte array
  ```

#### 6b. Update `j2735_v2x_msgs/msg/IntersectionGeometry.msg`
- Add `j2735_v2x_msgs/RoadAuthorityID road_authority_id` and `bool road_authority_id_exists`

#### 6c. Update `j2735_v2x_msgs/msg/IntersectionState.msg`
- Same as 6b

#### 6d. Mirror changes in `carma_v2x_msgs/msg/IntersectionGeometry.msg` and `IntersectionState.msg`

#### 6e. `Map_Message.cpp` — decode/encode `roadAuthorityID`
File: `ros-conversion/ros-conversion/cpp_message/src/Map_Message.cpp`
- Decode: if `intersectionGeometry->roadAuthorityID` is non-null, serialize the OID bytes
  into the `id_bytes` array, set the choice enum and exists flag
- Encode: if exists flag set, allocate and reconstruct the OID from bytes

#### 6f. `SPAT_Message.cpp` — decode/encode `roadAuthorityID`
File: `ros-conversion/ros-conversion/cpp_message/src/SPAT_Message.cpp`
- Same pattern as Map_Message.cpp

#### 6g. `map_convertor.cpp` — pass through `road_authority_id`
In `convertIntersectionGeometry()`: copy `road_authority_id` and `road_authority_id_exists`
from j2735 msg to carma msg (no unit conversion needed).

#### 6h. `spat_convertor.cpp` — pass through `road_authority_id`
In `convertIntersectionState()`: same pass-through copy.

---

### Group 7 — ITIS enum constant additions (CARMAMsgs only)

These are pure additive changes; no conversion code changes needed in ros-conversion.

#### 7a. `ITISVehicleGroupAffected.msg`
- **Bug fix**: `GAS_POWERED_VEHICLES` is currently `9248` but should be `9247`.
  The 2024 header confirms: gas=9247, diesel=9248.
- Add `DIESEL_POWERED_VEHICLES=9248`
- Add 10 new extension values from J2735 2024 standard:
  `commercial-vehicles`, `electric-powered-vehicles`, `hybrid-powered-vehicles`,
  `inherently-low-emission-vehicles`, `motor-driven-cycles`, `non-motorized-vehicles`,
  `recreational-vehicles`, `runaway-vehicles`, `traffic`, `vehicles-with-lugs`.
  **Action required**: look up the assigned ITIS codes in the J2735 2024 ASN source
  (they're extensible values not defined in the compiled C headers).

#### 7b. `ITISGenericLocations.msg`
- Add `COAST` constant — **look up ITIS code** from J2735 2024 standard.

#### 7c. `ITISIncidentResponseEquipment.msg`
- Add `SURVEY_CREWS` constant — **look up ITIS code** from J2735 2024 standard.

#### 7d. `ITISResponderGroupAffected.msg`
- Add `PRIVATE_TOW_UNITS` constant — **look up ITIS code** from J2735 2024 standard.

---

## Implementation Order

1. **Group 0.9** first — update `.msg` files to remove/rename `doNotUse` fields. This
   must happen before Groups 1-3 since the converter code changes depend on the new `.msg`
   definitions.
2. **Group 1** — fix field renames in `BSM_Message.cpp` and remove `doNotUse` decode/encode
   logic. Confirm the package compiles against 2024 headers.
3. **Group 4 & 5** — trivial .msg-only changes, no risk.
4. **Groups 2 & 3** — add new .msg types, update existing .msg files, then add
   encode/decode in `BSM_Message.cpp`, then add conversion in `bsm_convertor.cpp`.
5. **Group 6** — add `RoadAuthorityID.msg`, update IntersectionGeometry/State .msg files,
   then update `Map_Message.cpp` and `SPAT_Message.cpp`, then update convertors.
6. **Group 7** — after obtaining ITIS codes from J2735 2024 standard document,
   add constants to .msg files.

---

## Critical Files

| File | Change |
|------|--------|
| `ros-conversion/cpp_message/src/BSM_Message.cpp` | Group 1 renames + doNotUse removal + Groups 2d/3d new fields |
| `ros-conversion/cpp_message/src/Map_Message.cpp` | Group 6e |
| `ros-conversion/cpp_message/src/SPAT_Message.cpp` | Group 6f |
| `ros-conversion/j2735_convertor/src/bsm_convertor.cpp` | Groups 2e, 3e + doNotUse removal |
| `ros-conversion/j2735_convertor/src/map_convertor.cpp` | Group 6g |
| `ros-conversion/j2735_convertor/src/spat_convertor.cpp` | Group 6h |
| `CARMAMsgs/j2735_v2x_msgs/msg/SpecialVehicleExtensions.msg` | Group 0.9b (remove trailers) |
| `CARMAMsgs/carma_v2x_msgs/msg/SpecialVehicleExtensions.msg` | Group 0.9b (remove trailers) |
| `CARMAMsgs/j2735_v2x_msgs/msg/TrailerData.msg` | Group 0.9b (rename ssp_index) |
| `CARMAMsgs/carma_v2x_msgs/msg/TrailerData.msg` | Group 0.9b (rename ssp_index) |
| `CARMAMsgs/j2735_v2x_msgs/msg/VehicleData.msg` | Group 0.9b + 2b |
| `CARMAMsgs/carma_v2x_msgs/msg/VehicleData.msg` | Group 0.9b + 2c |
| `CARMAMsgs/j2735_v2x_msgs/msg/SupplementalVehicleExtensions.msg` | Group 0.9b + 3b |
| `CARMAMsgs/carma_v2x_msgs/msg/SupplementalVehicleExtensions.msg` | Group 0.9b + 3c |
| `CARMAMsgs/j2735_v2x_msgs/msg/VehicleEventFlags.msg` | Group 4 |
| `CARMAMsgs/j2735_v2x_msgs/msg/LaneSharing.msg` | Group 5 |
| `CARMAMsgs/j2735_v2x_msgs/msg/IntersectionGeometry.msg` | Group 6b |
| `CARMAMsgs/j2735_v2x_msgs/msg/IntersectionState.msg` | Group 6c |
| `CARMAMsgs/carma_v2x_msgs/msg/IntersectionGeometry.msg` | Group 6d |
| `CARMAMsgs/carma_v2x_msgs/msg/IntersectionState.msg` | Group 6d |
| `CARMAMsgs/j2735_v2x_msgs/msg/ITISVehicleGroupAffected.msg` | Group 7a |
| `CARMAMsgs/j2735_v2x_msgs/msg/ITISGenericLocations.msg` | Group 7b |
| `CARMAMsgs/j2735_v2x_msgs/msg/ITISIncidentResponseEquipment.msg` | Group 7c |
| `CARMAMsgs/j2735_v2x_msgs/msg/ITISResponderGroupAffected.msg` | Group 7d |

**New files to create:**
- `CARMAMsgs/j2735_v2x_msgs/msg/Axles.msg`
- `CARMAMsgs/j2735_v2x_msgs/msg/TrailersJ2945Slash1B.msg`
- `CARMAMsgs/j2735_v2x_msgs/msg/TrailerUnitDescJ2945Slash1B.msg`
- `CARMAMsgs/j2735_v2x_msgs/msg/SchoolBusJ2945Slash1C.msg`
- `CARMAMsgs/j2735_v2x_msgs/msg/RptVehicleClass.msg`
- `CARMAMsgs/j2735_v2x_msgs/msg/RoadAuthorityID.msg`

---

## Verification

1. **Compile check**: `colcon build --packages-select j2735_v2x_msgs carma_v2x_msgs cpp_message j2735_convertor`
   — must succeed with zero errors against `stol-j2735-2024-carma`.
2. **Unit tests**: `colcon test --packages-select j2735_convertor` — run existing BSM, MAP, SPAT tests.
3. **Encode/decode round-trip** (manual): For BSM with VehicleData new fields and
   SupplementalVehicleExtensions new extension fields — encode a ROS msg to binary
   and decode back, verify field values are preserved.
4. **Deprecated field handling** (per 0.9c): Verify that a BSM binary containing any
   of the `doNotUse` fields (trailers, weatherReport, weatherProbe, obstacle, speedProfile,
   trailerWeight) decodes without crash and produces `RCLCPP_WARN` log messages.
5. **Verify no data loss**: Confirm that `doNotUse` fields are NOT populated in encoded
   output, even if the input ROS message somehow contained data for removed fields (should
   be impossible with updated .msg, but verify at the ASN level).
