// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/basictypes.h"
#include "net/base/x509_cert_types.h"
#include "testing/gtest/include/gtest/gtest.h"

//  0:d=0  hl=2 l=  95 cons: SEQUENCE
//  2:d=1  hl=2 l=  11 cons:  SET
//  4:d=2  hl=2 l=   9 cons:   SEQUENCE
//  6:d=3  hl=2 l=   3 prim:    OBJECT            :countryName
// 11:d=3  hl=2 l=   2 prim:    PRINTABLESTRING   :US
// 15:d=1  hl=2 l=  23 cons:  SET
// 17:d=2  hl=2 l=  21 cons:   SEQUENCE
// 19:d=3  hl=2 l=   3 prim:    OBJECT            :organizationName
// 24:d=3  hl=2 l=  14 prim:    PRINTABLESTRING   :VeriSign, Inc.
// 40:d=1  hl=2 l=  55 cons:  SET
// 42:d=2  hl=2 l=  53 cons:   SEQUENCE
// 44:d=3  hl=2 l=   3 prim:    OBJECT            :organizationalUnitName
// 49:d=3  hl=2 l=  46 prim:    PRINTABLESTRING   :Class 1 Public Primary Certification Authority
static const uint8 VerisignDN[] = {
  0x30, 0x5f, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02,
  0x55, 0x53, 0x31, 0x17, 0x30, 0x15, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x13, 0x0e,
  0x56, 0x65, 0x72, 0x69, 0x53, 0x69, 0x67, 0x6e, 0x2c, 0x20, 0x49, 0x6e, 0x63,
  0x2e, 0x31, 0x37, 0x30, 0x35, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x13, 0x2e, 0x43,
  0x6c, 0x61, 0x73, 0x73, 0x20, 0x31, 0x20, 0x50, 0x75, 0x62, 0x6c, 0x69, 0x63,
  0x20, 0x50, 0x72, 0x69, 0x6d, 0x61, 0x72, 0x79, 0x20, 0x43, 0x65, 0x72, 0x74,
  0x69, 0x66, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x41, 0x75, 0x74,
  0x68, 0x6f, 0x72, 0x69, 0x74, 0x79
};

//  0:d=0  hl=2 l= 125 cons: SEQUENCE
//  2:d=1  hl=2 l=  11 cons:  SET
//  4:d=2  hl=2 l=   9 cons:   SEQUENCE
//  6:d=3  hl=2 l=   3 prim:    OBJECT            :countryName
// 11:d=3  hl=2 l=   2 prim:    PRINTABLESTRING   :IL
// 15:d=1  hl=2 l=  22 cons:  SET
// 17:d=2  hl=2 l=  20 cons:   SEQUENCE
// 19:d=3  hl=2 l=   3 prim:    OBJECT            :organizationName
// 24:d=3  hl=2 l=  13 prim:    PRINTABLESTRING   :StartCom Ltd.
// 39:d=1  hl=2 l=  43 cons:  SET
// 41:d=2  hl=2 l=  41 cons:   SEQUENCE
// 43:d=3  hl=2 l=   3 prim:    OBJECT            :organizationalUnitName
// 48:d=3  hl=2 l=  34 prim:    PRINTABLESTRING   :Secure Digital Certificate Signing
// 84:d=1  hl=2 l=  41 cons:  SET
// 86:d=2  hl=2 l=  39 cons:   SEQUENCE
// 88:d=3  hl=2 l=   3 prim:    OBJECT            :commonName
// 93:d=3  hl=2 l=  32 prim:    PRINTABLESTRING   :StartCom Certification Authority
static const uint8 StartComDN[] = {
  0x30, 0x7d, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02,
  0x49, 0x4c, 0x31, 0x16, 0x30, 0x14, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x13, 0x0d,
  0x53, 0x74, 0x61, 0x72, 0x74, 0x43, 0x6f, 0x6d, 0x20, 0x4c, 0x74, 0x64, 0x2e,
  0x31, 0x2b, 0x30, 0x29, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x13, 0x22, 0x53, 0x65,
  0x63, 0x75, 0x72, 0x65, 0x20, 0x44, 0x69, 0x67, 0x69, 0x74, 0x61, 0x6c, 0x20,
  0x43, 0x65, 0x72, 0x74, 0x69, 0x66, 0x69, 0x63, 0x61, 0x74, 0x65, 0x20, 0x53,
  0x69, 0x67, 0x6e, 0x69, 0x6e, 0x67, 0x31, 0x29, 0x30, 0x27, 0x06, 0x03, 0x55,
  0x04, 0x03, 0x13, 0x20, 0x53, 0x74, 0x61, 0x72, 0x74, 0x43, 0x6f, 0x6d, 0x20,
  0x43, 0x65, 0x72, 0x74, 0x69, 0x66, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e,
  0x20, 0x41, 0x75, 0x74, 0x68, 0x6f, 0x72, 0x69, 0x74, 0x79
};

//  0:d=0  hl=3 l= 174 cons: SEQUENCE
//  3:d=1  hl=2 l=  11 cons:  SET
//  5:d=2  hl=2 l=   9 cons:   SEQUENCE
//  7:d=3  hl=2 l=   3 prim:    OBJECT            :countryName
// 12:d=3  hl=2 l=   2 prim:    PRINTABLESTRING   :US
// 16:d=1  hl=2 l=  11 cons:  SET
// 18:d=2  hl=2 l=   9 cons:   SEQUENCE
// 20:d=3  hl=2 l=   3 prim:    OBJECT            :stateOrProvinceName
// 25:d=3  hl=2 l=   2 prim:    PRINTABLESTRING   :UT
// 29:d=1  hl=2 l=  23 cons:  SET
// 31:d=2  hl=2 l=  21 cons:   SEQUENCE
// 33:d=3  hl=2 l=   3 prim:    OBJECT            :localityName
// 38:d=3  hl=2 l=  14 prim:    PRINTABLESTRING   :Salt Lake City
// 54:d=1  hl=2 l=  30 cons:  SET
// 56:d=2  hl=2 l=  28 cons:   SEQUENCE
// 58:d=3  hl=2 l=   3 prim:    OBJECT            :organizationName
// 63:d=3  hl=2 l=  21 prim:    PRINTABLESTRING   :The USERTRUST Network
// 86:d=1  hl=2 l=  33 cons:  SET
// 88:d=2  hl=2 l=  31 cons:   SEQUENCE
// 90:d=3  hl=2 l=   3 prim:    OBJECT            :organizationalUnitName
// 95:d=3  hl=2 l=  24 prim:    PRINTABLESTRING   :http://www.usertrust.com
//121:d=1  hl=2 l=  54 cons:  SET
//123:d=2  hl=2 l=  52 cons:   SEQUENCE
//125:d=3  hl=2 l=   3 prim:    OBJECT            :commonName
//130:d=3  hl=2 l=  45 prim:    PRINTABLESTRING   :UTN-USERFirst-Client Authentication and Email
static const uint8 UserTrustDN[] = {
  0x30, 0x81, 0xae, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13,
  0x02, 0x55, 0x53, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x08, 0x13,
  0x02, 0x55, 0x54, 0x31, 0x17, 0x30, 0x15, 0x06, 0x03, 0x55, 0x04, 0x07, 0x13,
  0x0e, 0x53, 0x61, 0x6c, 0x74, 0x20, 0x4c, 0x61, 0x6b, 0x65, 0x20, 0x43, 0x69,
  0x74, 0x79, 0x31, 0x1e, 0x30, 0x1c, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x13, 0x15,
  0x54, 0x68, 0x65, 0x20, 0x55, 0x53, 0x45, 0x52, 0x54, 0x52, 0x55, 0x53, 0x54,
  0x20, 0x4e, 0x65, 0x74, 0x77, 0x6f, 0x72, 0x6b, 0x31, 0x21, 0x30, 0x1f, 0x06,
  0x03, 0x55, 0x04, 0x0b, 0x13, 0x18, 0x68, 0x74, 0x74, 0x70, 0x3a, 0x2f, 0x2f,
  0x77, 0x77, 0x77, 0x2e, 0x75, 0x73, 0x65, 0x72, 0x74, 0x72, 0x75, 0x73, 0x74,
  0x2e, 0x63, 0x6f, 0x6d, 0x31, 0x36, 0x30, 0x34, 0x06, 0x03, 0x55, 0x04, 0x03,
  0x13, 0x2d, 0x55, 0x54, 0x4e, 0x2d, 0x55, 0x53, 0x45, 0x52, 0x46, 0x69, 0x72,
  0x73, 0x74, 0x2d, 0x43, 0x6c, 0x69, 0x65, 0x6e, 0x74, 0x20, 0x41, 0x75, 0x74,
  0x68, 0x65, 0x6e, 0x74, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x61,
  0x6e, 0x64, 0x20, 0x45, 0x6d, 0x61, 0x69, 0x6c
};

//  0:d=0  hl=3 l= 190 cons: SEQUENCE
//  3:d=1  hl=2 l=  63 cons:  SET
//  5:d=2  hl=2 l=  61 cons:   SEQUENCE
//  7:d=3  hl=2 l=   3 prim:    OBJECT            :commonName
// 12:d=3  hl=2 l=  54 prim:    UTF8STRING        :T??RKTRUST Elektronik Sertifika Hizmet Sa??lay??c??s??
// 68:d=1  hl=2 l=  11 cons:  SET
// 70:d=2  hl=2 l=   9 cons:   SEQUENCE
// 72:d=3  hl=2 l=   3 prim:    OBJECT            :countryName
// 77:d=3  hl=2 l=   2 prim:    PRINTABLESTRING   :TR
// 81:d=1  hl=2 l=  15 cons:  SET
// 83:d=2  hl=2 l=  13 cons:   SEQUENCE
// 85:d=3  hl=2 l=   3 prim:    OBJECT            :localityName
// 90:d=3  hl=2 l=   6 prim:    UTF8STRING        :Ankara
// 98:d=1  hl=2 l=  93 cons:  SET
//100:d=2  hl=2 l=  91 cons:   SEQUENCE
//102:d=3  hl=2 l=   3 prim:    OBJECT            :organizationName
//107:d=3  hl=2 l=  84 prim:    UTF8STRING        :T??RKTRUST Bilgi ??leti??im ve Bili??im G??venli??i Hizmetleri A.??. (c) Kas??m 2005
static const uint8 TurkTrustDN[] = {
  0x30, 0x81, 0xbe, 0x31, 0x3f, 0x30, 0x3d, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c,
  0x36, 0x54, 0xc3, 0x9c, 0x52, 0x4b, 0x54, 0x52, 0x55, 0x53, 0x54, 0x20, 0x45,
  0x6c, 0x65, 0x6b, 0x74, 0x72, 0x6f, 0x6e, 0x69, 0x6b, 0x20, 0x53, 0x65, 0x72,
  0x74, 0x69, 0x66, 0x69, 0x6b, 0x61, 0x20, 0x48, 0x69, 0x7a, 0x6d, 0x65, 0x74,
  0x20, 0x53, 0x61, 0xc4, 0x9f, 0x6c, 0x61, 0x79, 0xc4, 0xb1, 0x63, 0xc4, 0xb1,
  0x73, 0xc4, 0xb1, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13,
  0x02, 0x54, 0x52, 0x31, 0x0f, 0x30, 0x0d, 0x06, 0x03, 0x55, 0x04, 0x07, 0x0c,
  0x06, 0x41, 0x6e, 0x6b, 0x61, 0x72, 0x61, 0x31, 0x5d, 0x30, 0x5b, 0x06, 0x03,
  0x55, 0x04, 0x0a, 0x0c, 0x54, 0x54, 0xc3, 0x9c, 0x52, 0x4b, 0x54, 0x52, 0x55,
  0x53, 0x54, 0x20, 0x42, 0x69, 0x6c, 0x67, 0x69, 0x20, 0xc4, 0xb0, 0x6c, 0x65,
  0x74, 0x69, 0xc5, 0x9f, 0x69, 0x6d, 0x20, 0x76, 0x65, 0x20, 0x42, 0x69, 0x6c,
  0x69, 0xc5, 0x9f, 0x69, 0x6d, 0x20, 0x47, 0xc3, 0xbc, 0x76, 0x65, 0x6e, 0x6c,
  0x69, 0xc4, 0x9f, 0x69, 0x20, 0x48, 0x69, 0x7a, 0x6d, 0x65, 0x74, 0x6c, 0x65,
  0x72, 0x69, 0x20, 0x41, 0x2e, 0xc5, 0x9e, 0x2e, 0x20, 0x28, 0x63, 0x29, 0x20,
  0x4b, 0x61, 0x73, 0xc4, 0xb1, 0x6d, 0x20, 0x32, 0x30, 0x30, 0x35, 0x30, 0x1e,
  0x17, 0x0d, 0x30, 0x35, 0x31, 0x31, 0x30, 0x37, 0x31, 0x30, 0x30, 0x37, 0x35,
  0x37
};

// 33:d=2  hl=3 l= 207 cons:   SEQUENCE
// 36:d=3  hl=2 l=  11 cons:    SET
// 38:d=4  hl=2 l=   9 cons:     SEQUENCE
// 40:d=5  hl=2 l=   3 prim:      OBJECT            :countryName
// 45:d=5  hl=2 l=   2 prim:      PRINTABLESTRING   :AT
// 49:d=3  hl=3 l= 139 cons:    SET
// 52:d=4  hl=3 l= 136 cons:     SEQUENCE
// 55:d=5  hl=2 l=   3 prim:      OBJECT            :organizationName
// 60:d=5  hl=3 l= 128 prim:      BMPSTRING         :A-Trust Ges. f??r Sicherheitssysteme im elektr. Datenverkehr GmbH
//191:d=3  hl=2 l=  24 cons:    SET
//193:d=4  hl=2 l=  22 cons:     SEQUENCE
//195:d=5  hl=2 l=   3 prim:      OBJECT            :organizationalUnitName
//200:d=5  hl=2 l=  15 prim:      PRINTABLESTRING   :A-Trust-Qual-01
//217:d=3  hl=2 l=  24 cons:    SET
//219:d=4  hl=2 l=  22 cons:     SEQUENCE
//221:d=5  hl=2 l=   3 prim:      OBJECT            :commonName
//226:d=5  hl=2 l=  15 prim:      PRINTABLESTRING   :A-Trust-Qual-01
static const uint8 ATrustQual01DN[] = {
  0x30, 0x81, 0xcf, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13,
  0x02, 0x41, 0x54, 0x31, 0x81, 0x8b, 0x30, 0x81, 0x88, 0x06, 0x03, 0x55, 0x04,
  0x0a, 0x1e, 0x81, 0x80, 0x00, 0x41, 0x00, 0x2d, 0x00, 0x54, 0x00, 0x72, 0x00,
  0x75, 0x00, 0x73, 0x00, 0x74, 0x00, 0x20, 0x00, 0x47, 0x00, 0x65, 0x00, 0x73,
  0x00, 0x2e, 0x00, 0x20, 0x00, 0x66, 0x00, 0xfc, 0x00, 0x72, 0x00, 0x20, 0x00,
  0x53, 0x00, 0x69, 0x00, 0x63, 0x00, 0x68, 0x00, 0x65, 0x00, 0x72, 0x00, 0x68,
  0x00, 0x65, 0x00, 0x69, 0x00, 0x74, 0x00, 0x73, 0x00, 0x73, 0x00, 0x79, 0x00,
  0x73, 0x00, 0x74, 0x00, 0x65, 0x00, 0x6d, 0x00, 0x65, 0x00, 0x20, 0x00, 0x69,
  0x00, 0x6d, 0x00, 0x20, 0x00, 0x65, 0x00, 0x6c, 0x00, 0x65, 0x00, 0x6b, 0x00,
  0x74, 0x00, 0x72, 0x00, 0x2e, 0x00, 0x20, 0x00, 0x44, 0x00, 0x61, 0x00, 0x74,
  0x00, 0x65, 0x00, 0x6e, 0x00, 0x76, 0x00, 0x65, 0x00, 0x72, 0x00, 0x6b, 0x00,
  0x65, 0x00, 0x68, 0x00, 0x72, 0x00, 0x20, 0x00, 0x47, 0x00, 0x6d, 0x00, 0x62,
  0x00, 0x48, 0x31, 0x18, 0x30, 0x16, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x13, 0x0f,
  0x41, 0x2d, 0x54, 0x72, 0x75, 0x73, 0x74, 0x2d, 0x51, 0x75, 0x61, 0x6c, 0x2d,
  0x30, 0x31, 0x31, 0x18, 0x30, 0x16, 0x06, 0x03, 0x55, 0x04, 0x03, 0x13, 0x0f,
  0x41, 0x2d, 0x54, 0x72, 0x75, 0x73, 0x74, 0x2d, 0x51, 0x75, 0x61, 0x6c, 0x2d,
  0x30, 0x31, 0x30, 0x1e, 0x17
};

// 34:d=2  hl=3 l= 180 cons:   SEQUENCE
// 37:d=3  hl=2 l=  20 cons:    SET
// 39:d=4  hl=2 l=  18 cons:     SEQUENCE
// 41:d=5  hl=2 l=   3 prim:      OBJECT            :organizationName
// 46:d=5  hl=2 l=  11 prim:      PRINTABLESTRING   :Entrust.net
// 59:d=3  hl=2 l=  64 cons:    SET
// 61:d=4  hl=2 l=  62 cons:     SEQUENCE
// 63:d=5  hl=2 l=   3 prim:      OBJECT            :organizationalUnitName
// 68:d=5  hl=2 l=  55 prim:      T61STRING         :www.entrust.net/CPS_2048 incorp. by ref. (limits liab.)
//125:d=3  hl=2 l=  37 cons:    SET
//127:d=4  hl=2 l=  35 cons:     SEQUENCE
//129:d=5  hl=2 l=   3 prim:      OBJECT            :organizationalUnitName
//134:d=5  hl=2 l=  28 prim:      PRINTABLESTRING   :(c) 1999 Entrust.net Limited
//164:d=3  hl=2 l=  51 cons:    SET
//166:d=4  hl=2 l=  49 cons:     SEQUENCE
//168:d=5  hl=2 l=   3 prim:      OBJECT            :commonName
//173:d=5  hl=2 l=  42 prim:      PRINTABLESTRING   :Entrust.net Certification Authority (2048)
static const uint8 EntrustDN[] = {
  0x30, 0x81, 0xb4, 0x31, 0x14, 0x30, 0x12, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x13,
  0x0b, 0x45, 0x6e, 0x74, 0x72, 0x75, 0x73, 0x74, 0x2e, 0x6e, 0x65, 0x74, 0x31,
  0x40, 0x30, 0x3e, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x14, 0x37, 0x77, 0x77, 0x77,
  0x2e, 0x65, 0x6e, 0x74, 0x72, 0x75, 0x73, 0x74, 0x2e, 0x6e, 0x65, 0x74, 0x2f,
  0x43, 0x50, 0x53, 0x5f, 0x32, 0x30, 0x34, 0x38, 0x20, 0x69, 0x6e, 0x63, 0x6f,
  0x72, 0x70, 0x2e, 0x20, 0x62, 0x79, 0x20, 0x72, 0x65, 0x66, 0x2e, 0x20, 0x28,
  0x6c, 0x69, 0x6d, 0x69, 0x74, 0x73, 0x20, 0x6c, 0x69, 0x61, 0x62, 0x2e, 0x29,
  0x31, 0x25, 0x30, 0x23, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x13, 0x1c, 0x28, 0x63,
  0x29, 0x20, 0x31, 0x39, 0x39, 0x39, 0x20, 0x45, 0x6e, 0x74, 0x72, 0x75, 0x73,
  0x74, 0x2e, 0x6e, 0x65, 0x74, 0x20, 0x4c, 0x69, 0x6d, 0x69, 0x74, 0x65, 0x64,
  0x31, 0x33, 0x30, 0x31, 0x06, 0x03, 0x55, 0x04, 0x03, 0x13, 0x2a, 0x45, 0x6e,
  0x74, 0x72, 0x75, 0x73, 0x74, 0x2e, 0x6e, 0x65, 0x74, 0x20, 0x43, 0x65, 0x72,
  0x74, 0x69, 0x66, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x41, 0x75,
  0x74, 0x68, 0x6f, 0x72, 0x69, 0x74, 0x79, 0x20, 0x28, 0x32, 0x30, 0x34, 0x38,
  0x29
};

namespace net {

TEST(X509TypesTest, Matching) {
  CertPrincipal spamco;
  spamco.common_name = "SpamCo Dept. Of Certificization";
  spamco.country_name = "EB";
  spamco.organization_names.push_back("SpamCo Holding Company, LLC");
  spamco.organization_names.push_back("SpamCo Evil Masterminds");
  spamco.organization_unit_names.push_back("Class Z Obfuscation Authority");
  ASSERT_TRUE(spamco.Matches(spamco));

  CertPrincipal bogus;
  EXPECT_FALSE(bogus.Matches(spamco));
  EXPECT_FALSE(spamco.Matches(bogus));

  bogus = spamco;
  EXPECT_TRUE(bogus.Matches(spamco));
  EXPECT_TRUE(spamco.Matches(bogus));

  bogus.organization_names.erase(bogus.organization_names.begin(),
                                 bogus.organization_names.end());
  EXPECT_FALSE(bogus.Matches(spamco));
  EXPECT_FALSE(spamco.Matches(bogus));

  bogus.organization_names.push_back("SpamCo Holding Company, LLC");
  bogus.organization_names.push_back("SpamCo Evil Masterminds");
  EXPECT_TRUE(bogus.Matches(spamco));
  EXPECT_TRUE(spamco.Matches(bogus));

  bogus.locality_name = "Elbosdorf";
  EXPECT_FALSE(bogus.Matches(spamco));
  EXPECT_FALSE(spamco.Matches(bogus));

  bogus.locality_name = "";
  bogus.organization_unit_names.push_back("Q Division");
  EXPECT_FALSE(bogus.Matches(spamco));
  EXPECT_FALSE(spamco.Matches(bogus));
}

TEST(X509TypesTest, ParseDNVerisign) {
  CertPrincipal verisign;
  EXPECT_TRUE(verisign.ParseDistinguishedName(VerisignDN, sizeof(VerisignDN)));
  EXPECT_EQ("", verisign.common_name);
  EXPECT_EQ("US", verisign.country_name);
  ASSERT_EQ(1U, verisign.organization_names.size());
  EXPECT_EQ("VeriSign, Inc.", verisign.organization_names[0]);
  ASSERT_EQ(1U, verisign.organization_unit_names.size());
  EXPECT_EQ("Class 1 Public Primary Certification Authority",
            verisign.organization_unit_names[0]);
}

TEST(X509TypesTest, ParseDNStartcom) {
  CertPrincipal startcom;
  EXPECT_TRUE(startcom.ParseDistinguishedName(StartComDN, sizeof(StartComDN)));
  EXPECT_EQ("StartCom Certification Authority", startcom.common_name);
  EXPECT_EQ("IL", startcom.country_name);
  ASSERT_EQ(1U, startcom.organization_names.size());
  EXPECT_EQ("StartCom Ltd.", startcom.organization_names[0]);
  ASSERT_EQ(1U, startcom.organization_unit_names.size());
  EXPECT_EQ("Secure Digital Certificate Signing",
            startcom.organization_unit_names[0]);
}

TEST(X509TypesTest, ParseDNUserTrust) {
  CertPrincipal usertrust;
  EXPECT_TRUE(usertrust.ParseDistinguishedName(UserTrustDN,
                                               sizeof(UserTrustDN)));
  EXPECT_EQ("UTN-USERFirst-Client Authentication and Email",
            usertrust.common_name);
  EXPECT_EQ("US", usertrust.country_name);
  EXPECT_EQ("UT", usertrust.state_or_province_name);
  EXPECT_EQ("Salt Lake City", usertrust.locality_name);
  ASSERT_EQ(1U, usertrust.organization_names.size());
  EXPECT_EQ("The USERTRUST Network", usertrust.organization_names[0]);
  ASSERT_EQ(1U, usertrust.organization_unit_names.size());
  EXPECT_EQ("http://www.usertrust.com",
            usertrust.organization_unit_names[0]);
}

TEST(X509TypesTest, ParseDNTurkTrust) {
  // Note: This tests parsing UTF8STRINGs.
  CertPrincipal turktrust;
  EXPECT_TRUE(turktrust.ParseDistinguishedName(TurkTrustDN,
                                               sizeof(TurkTrustDN)));
  EXPECT_EQ("T??RKTRUST Elektronik Sertifika Hizmet Sa??lay??c??s??",
            turktrust.common_name);
  EXPECT_EQ("TR", turktrust.country_name);
  EXPECT_EQ("Ankara", turktrust.locality_name);
  ASSERT_EQ(1U, turktrust.organization_names.size());
  EXPECT_EQ("T??RKTRUST Bilgi ??leti??im ve Bili??im G??venli??i Hizmetleri A.??. (c) Kas??m 2005",
            turktrust.organization_names[0]);
}

TEST(X509TypesTest, ParseDNATrust) {
  // Note: This tests parsing 16-bit BMPSTRINGs.
  CertPrincipal atrust;
  EXPECT_TRUE(atrust.ParseDistinguishedName(ATrustQual01DN,
                                            sizeof(ATrustQual01DN)));
  EXPECT_EQ("A-Trust-Qual-01",
            atrust.common_name);
  EXPECT_EQ("AT", atrust.country_name);
  ASSERT_EQ(1U, atrust.organization_names.size());
  EXPECT_EQ("A-Trust Ges. f??r Sicherheitssysteme im elektr. Datenverkehr GmbH",
            atrust.organization_names[0]);
  ASSERT_EQ(1U, atrust.organization_unit_names.size());
  EXPECT_EQ("A-Trust-Qual-01",
            atrust.organization_unit_names[0]);
}

TEST(X509TypesTest, ParseDNEntrust) {
  // Note: This tests parsing T61STRINGs and fields with multiple values.
  CertPrincipal entrust;
  EXPECT_TRUE(entrust.ParseDistinguishedName(EntrustDN,
                                             sizeof(EntrustDN)));
  EXPECT_EQ("Entrust.net Certification Authority (2048)",
            entrust.common_name);
  EXPECT_EQ("", entrust.country_name);
  ASSERT_EQ(1U, entrust.organization_names.size());
  EXPECT_EQ("Entrust.net",
            entrust.organization_names[0]);
  ASSERT_EQ(2U, entrust.organization_unit_names.size());
  EXPECT_EQ("www.entrust.net/CPS_2048 incorp. by ref. (limits liab.)",
            entrust.organization_unit_names[0]);
  EXPECT_EQ("(c) 1999 Entrust.net Limited",
            entrust.organization_unit_names[1]);
}

}  // namespace net
