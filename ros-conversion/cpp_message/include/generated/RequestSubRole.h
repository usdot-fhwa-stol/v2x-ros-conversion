/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "DSRC"
 * 	found in "J2735_201603_2023-06-22.asn"
 * 	`asn1c -pdu=MessageFrame -fcompound-names -no-gen-APER -no-gen-JER`
 */

#ifndef	_RequestSubRole_H_
#define	_RequestSubRole_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum RequestSubRole {
	RequestSubRole_requestSubRoleUnKnown	= 0,
	RequestSubRole_requestSubRole1	= 1,
	RequestSubRole_requestSubRole2	= 2,
	RequestSubRole_requestSubRole3	= 3,
	RequestSubRole_requestSubRole4	= 4,
	RequestSubRole_requestSubRole5	= 5,
	RequestSubRole_requestSubRole6	= 6,
	RequestSubRole_requestSubRole7	= 7,
	RequestSubRole_requestSubRole8	= 8,
	RequestSubRole_requestSubRole9	= 9,
	RequestSubRole_requestSubRole10	= 10,
	RequestSubRole_requestSubRole11	= 11,
	RequestSubRole_requestSubRole12	= 12,
	RequestSubRole_requestSubRole13	= 13,
	RequestSubRole_requestSubRole14	= 14,
	RequestSubRole_requestSubRoleReserved	= 15
} e_RequestSubRole;

/* RequestSubRole */
typedef long	 RequestSubRole_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_RequestSubRole_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_RequestSubRole;
extern const asn_INTEGER_specifics_t asn_SPC_RequestSubRole_specs_1;
asn_struct_free_f RequestSubRole_free;
asn_struct_print_f RequestSubRole_print;
asn_constr_check_f RequestSubRole_constraint;
ber_type_decoder_f RequestSubRole_decode_ber;
der_type_encoder_f RequestSubRole_encode_der;
xer_type_decoder_f RequestSubRole_decode_xer;
xer_type_encoder_f RequestSubRole_encode_xer;
oer_type_decoder_f RequestSubRole_decode_oer;
oer_type_encoder_f RequestSubRole_encode_oer;
per_type_decoder_f RequestSubRole_decode_uper;
per_type_encoder_f RequestSubRole_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _RequestSubRole_H_ */
#include <asn_internal.h>
