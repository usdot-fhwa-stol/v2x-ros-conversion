/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "SDSM"
 * 	found in "J2735_201603_2023-06-22.asn"
 * 	`asn1c -pdu=MessageFrame -fcompound-names -no-gen-APER -no-gen-JER`
 */

#ifndef	_DetectedObjectOptionalData_H_
#define	_DetectedObjectOptionalData_H_


#include <asn_application.h>

/* Including external dependencies */
#include "DetectedVehicleData.h"
#include "DetectedVRUData.h"
#include "DetectedObstacleData.h"
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum DetectedObjectOptionalData_PR {
	DetectedObjectOptionalData_PR_NOTHING,	/* No components present */
	DetectedObjectOptionalData_PR_detVeh,
	DetectedObjectOptionalData_PR_detVRU,
	DetectedObjectOptionalData_PR_detObst
} DetectedObjectOptionalData_PR;

/* DetectedObjectOptionalData */
typedef struct DetectedObjectOptionalData {
	DetectedObjectOptionalData_PR present;
	union DetectedObjectOptionalData_u {
		DetectedVehicleData_t	 detVeh;
		DetectedVRUData_t	 detVRU;
		DetectedObstacleData_t	 detObst;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DetectedObjectOptionalData_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DetectedObjectOptionalData;
extern asn_CHOICE_specifics_t asn_SPC_DetectedObjectOptionalData_specs_1;
extern asn_TYPE_member_t asn_MBR_DetectedObjectOptionalData_1[3];
extern asn_per_constraints_t asn_PER_type_DetectedObjectOptionalData_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _DetectedObjectOptionalData_H_ */
#include <asn_internal.h>
