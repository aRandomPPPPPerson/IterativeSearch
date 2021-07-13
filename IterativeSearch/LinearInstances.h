#pragma once
#include "Types.h"
#include <vector>
using std::vector;

vector<WordType> linearTransform_identity(const vector<WordType>& in);

//PRESENT
vector<WordType> linearTransform_present(const vector<WordType> &in);
vector<WordType> linearTransformInv_present(const vector<WordType> &in);

//GIFT
vector<WordType> linearTransform_gift64(const vector<WordType>& in);
vector<WordType> linearTransformInv_gift64(const vector<WordType>& in);
vector<WordType> linearTransform_gift128(const vector<WordType>& in);
vector<WordType> linearTransformInv_gift128(const vector<WordType>& in);

//PUFFIN
vector<WordType> linearTransform_puffin(const vector<WordType>& in);
vector<WordType> linearTransformInv_puffin(const vector<WordType>& in);

//EPCBC
vector<WordType> linearTransform_epcbc48(const vector<WordType>& in);
vector<WordType> linearTransformInv_epcbc48(const vector<WordType>& in);
vector<WordType> linearTransform_epcbc96(const vector<WordType>& in);
vector<WordType> linearTransformInv_epcbc96(const vector<WordType>& in);

//ICEBERG
vector<WordType> linearTransform_iceberg(const vector<WordType>& in);
vector<WordType> linearTransformInv_iceberg(const vector<WordType>& in);

//TRIFLE
vector<WordType> linearTransform_trifle(const vector<WordType>& in);
vector<WordType> linearTransformInv_trifle(const vector<WordType>& in);

//RECTANGLE
vector<WordType> linearTransform_rectangle(const vector<WordType> &in);
vector<WordType> linearTransformInv_rectangle(const vector<WordType> &in);
vector<WordType> linearTransform_knot256(const vector<WordType>& in);
vector<WordType> linearTransformInv_knot256(const vector<WordType>& in);
vector<WordType> linearTransform_knot384(const vector<WordType>& in);
vector<WordType> linearTransformInv_knot384(const vector<WordType>& in);
vector<WordType> linearTransform_knot512(const vector<WordType>& in);
vector<WordType> linearTransformInv_knot512(const vector<WordType>& in);

//SPONGENT
vector<WordType> linearTransform_spongent128(const vector<WordType>& in);
vector<WordType> linearTransformInv_spongent128(const vector<WordType>& in);
vector<WordType> linearTransform_spongent176(const vector<WordType>& in);
vector<WordType> linearTransformInv_spongent176(const vector<WordType>& in);

//ASCON
vector<WordType> linearTransform_ascon(const vector<WordType> &in);
vector<WordType> linearTransformInv_ascon(const vector<WordType> &in);

//ASCON
vector<WordType> linearTransform_pride(const vector<WordType>& in);
vector<WordType> linearTransformInv_pride(const vector<WordType>& in);

//TODO

//AES
vector<WordType> linearTransform_aes(const vector<WordType>& in);
vector<WordType> linearTransformInv_aes(const vector<WordType>& in);

vector<WordType> linearTransform_noekeon(const vector<WordType> &in);
vector<WordType> linearTransformInv_noekeon(const vector<WordType> &in);

vector<WordType> linearTransform_tangram(const vector<WordType> &in);
vector<WordType> linearTransformInv_tangram(const vector<WordType> &in);