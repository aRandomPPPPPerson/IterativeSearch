#include "SboxInstances.h"

WordType s_ascon[] = { 0x4,0xb,0x1f,0x14,0x1a,0x15,0x9,0x2,0x1b,0x5,0x8,0x12,0x1d,0x3,0x6,0x1c,0x1e,0x13,0x7,0xe,0x0,0xd,0x11,0x18,0x10,0xc,0x1,0x19,0x16,0xa,0xf,0x17 };
WordType s_present[] = { 0xc,0x5,0x6,0xb,0x9,0x0,0xa,0xd,0x3,0xe,0xf,0x8,0x4,0x7,0x1,0x2 };
WordType s_rectangle[] = { 0x6,0x5,0xc,0xa,0x1,0xe,0x7,0x9,0xb,0x0,0x3,0xd,0x8,0xf,0x4,0x2 };
WordType s_knot[] = { 0x4,0x0,0xa,0x7,0xb,0xe,0x1,0xd,0x9,0xf,0x6,0x8,0x5,0x2,0xc,0x3 };
WordType s_gift[] = { 0x1,0xa,0x4,0xc,0x6,0xf,0x3,0x9,0x2,0xd,0xb,0x7,0x5,0x0,0x8,0xe };
WordType s_puffin[] = {0xD,0x7,0x3,0x2,0x9,0xA,0xC,0x1,0xF,0x4,0x5,0xE,0x6,0x0,0xB,0x8};
WordType s_epcbc[] = { 0xc,0x5,0x6,0xb,0x9,0x0,0xa,0xd,0x3,0xe,0xf,0x8,0x4,0x7,0x1,0x2 };
WordType s_trifle[] = { 0x0,0xc,0x9,0x7,0x3,0x5,0xe,0x4,0x6,0xb,0xa,0x2,0xd,0x1,0x8,0xf };
WordType s_pride[] = { 0x0, 0x4, 0x8, 0xf, 0x1, 0x5, 0xe, 0x9, 0x2, 0x7, 0xa, 0xc, 0xb, 0xd, 0x6, 0x3 };
WordType s_spongent[] = { 0xE,0xD,0xB,0x0,0x2,0x1,0x4,0xF,0x7,0xA,0x8,0x5,0x9,0xC,0x3,0x6 };

WordType s_iceberg[] = {
0x24,0xc1,0x38,0x30,0xe7,0x57,0xdf,0x20,0x3e,0x99,0x1a,0x34,0xca,0xd6,0x52,0xfd,
0x40,0x6c,0xd3,0x3d,0x4a,0x59,0xf8,0x77,0xfb,0x61,0x0a,0x56,0xb9,0xd2,0xfc,0xf1,
0x07,0xf5,0x93,0xcd,0x00,0xb6,0x62,0xa7,0x63,0xfe,0x44,0xbd,0x5f,0x92,0x6b,0x68,
0x03,0x4e,0xa2,0x97,0x0b,0x60,0x83,0xa3,0x02,0xe5,0x45,0x67,0xf4,0x13,0x08,0x8b,
0x10,0xce,0xbe,0xb4,0x2a,0x3a,0x96,0x84,0xc8,0x9f,0x14,0xc0,0xc4,0x6f,0x31,0xd9,
0xab,0xae,0x0e,0x64,0x7c,0xda,0x1b,0x05,0xa8,0x15,0xa5,0x90,0x94,0x85,0x71,0x2c,
0x35,0x19,0x26,0x28,0x53,0xe2,0x7f,0x3b,0x2f,0xa9,0xcc,0x2e,0x11,0x76,0xed,0x4d,
0x87,0x5e,0xc2,0xc7,0x80,0xb0,0x6d,0x17,0xb2,0xff,0xe4,0xb7,0x54,0x9d,0xb8,0x66,
0x74,0x9c,0xdb,0x36,0x47,0x5d,0xde,0x70,0xd5,0x91,0xaa,0x3f,0xc9,0xd8,0xf3,0xf2,
0x5b,0x89,0x2d,0x22,0x5c,0xe1,0x46,0x33,0xe6,0x09,0xbc,0xe8,0x81,0x7d,0xe9,0x49,
0xe0,0xb1,0x32,0x37,0xea,0x5a,0xf6,0x27,0x58,0x69,0x8a,0x50,0xba,0xdd,0x51,0xf9,
0x75,0xa1,0x78,0xd0,0x43,0xf7,0x25,0x7b,0x7e,0x1c,0xac,0xd4,0x9a,0x2b,0x42,0xe3,
0x4b,0x01,0x72,0xd7,0x4c,0xfa,0xeb,0x73,0x48,0x8c,0x0c,0xf0,0x6a,0x23,0x41,0xec,
0xb3,0xef,0x1d,0x12,0xbb,0x88,0x0d,0xc3,0x8d,0x4f,0x55,0x82,0xee,0xad,0x86,0x06,
0xa0,0x95,0x65,0xbf,0x7a,0x39,0x98,0x04,0x9b,0x9e,0xa4,0xc6,0xcf,0x6e,0xdc,0xd1,
0xcb,0x1f,0x8f,0x8e,0x3c,0x21,0xa6,0xb5,0x16,0xaf,0xc5,0x18,0x1e,0x0f,0x29,0x79 };


WordType s_aes[] = {
	// 0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f
	0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, // 0
	0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, // 1
	0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, // 2
	0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, // 3
	0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, // 4
	0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, // 5
	0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, // 6
	0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, // 7
	0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, // 8
	0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, // 9
	0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, // a
	0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, // b
	0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, // c
	0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, // d
	0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, // e
	0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };// f
WordType s_tangram[] = { 0x8,0x0,0x3,0xE,0x7,0xD,0xC,0x2,0x6,0xF,0x5,0x9,0xA,0x1,0xB,0x4 };
WordType s_skinny[] = { 0xc,6,9,0,1,0xa,2,0xb,3,8,5,0xd,4,0xe,7,0xf };
WordType s_craft[] = { 0xc,0xa,0xd,0x3,0xe,0xb,0xf,0x7,0x8,0x9,0x1,0x5,0x0,0x2,0x4,0x6 };