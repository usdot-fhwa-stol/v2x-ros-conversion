/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "J2735_201603_2023-06-22.asn"
 * 	`asn1c -pdu=MessageFrame -fcompound-names -no-gen-APER -no-gen-JER`
 */

#ifndef	_IntersectionCollision_H_
#define	_IntersectionCollision_H_


#include <asn_application.h>

/* Including external dependencies */
#include "DSRC_MsgCount.h"
#include "TemporaryID.h"
#include "MinuteOfTheYear.h"
#include "IntersectionReferenceID.h"
#include "ApproachOrLane.h"
#include "VehicleEventFlags.h"
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct BSMcoreData;
struct PathHistory;
struct PathPrediction;
struct Reg_BasicSafetyMessage;

/* IntersectionCollision */
typedef struct IntersectionCollision {
	DSRC_MsgCount_t	 msgCnt;
	TemporaryID_t	 id;
	MinuteOfTheYear_t	*timeStamp;	/* OPTIONAL */
	struct BSMcoreData	*partOne;	/* OPTIONAL */
	struct PathHistory	*path;	/* OPTIONAL */
	struct PathPrediction	*pathPrediction;	/* OPTIONAL */
	IntersectionReferenceID_t	 intersectionID;
	ApproachOrLane_t	 laneNumber;
	VehicleEventFlags_t	 eventFlag;
	struct IntersectionCollision__regional {
		A_SEQUENCE_OF(struct Reg_BasicSafetyMessage) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *regional;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} IntersectionCollision_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_IntersectionCollision;
extern asn_SEQUENCE_specifics_t asn_SPC_IntersectionCollision_specs_1;
extern asn_TYPE_member_t asn_MBR_IntersectionCollision_1[10];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "BSMcoreData.h"
#include "PathHistory.h"
#include "PathPrediction.h"
#include "RegionalExtension.h"

#endif	/* _IntersectionCollision_H_ */
#include <asn_internal.h>
