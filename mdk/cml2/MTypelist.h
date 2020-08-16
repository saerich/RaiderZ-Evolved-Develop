#ifndef _MTYPELIST_H
#define _MTYPELIST_H

#define TYPELIST_1(T1) ::TL::Typelist<T1, ::TL::NullType>

#define TYPELIST_2(T1, T2) ::TL::Typelist<T1, TYPELIST_1(T2) >

#define TYPELIST_3(T1, T2, T3) ::TL::Typelist<T1, TYPELIST_2(T2, T3) >

#define TYPELIST_4(T1, T2, T3, T4) \
	::TL::Typelist<T1, TYPELIST_3(T2, T3, T4) >

#define TYPELIST_5(T1, T2, T3, T4, T5) \
	::TL::Typelist<T1, TYPELIST_4(T2, T3, T4, T5) >

#define TYPELIST_6(T1, T2, T3, T4, T5, T6) \
	::TL::Typelist<T1, TYPELIST_5(T2, T3, T4, T5, T6) >

#define TYPELIST_7(T1, T2, T3, T4, T5, T6, T7) \
	::TL::Typelist<T1, TYPELIST_6(T2, T3, T4, T5, T6, T7) >

#define TYPELIST_8(T1, T2, T3, T4, T5, T6, T7, T8) \
	::TL::Typelist<T1, TYPELIST_7(T2, T3, T4, T5, T6, T7, T8) >

#define TYPELIST_9(T1, T2, T3, T4, T5, T6, T7, T8, T9) \
	::TL::Typelist<T1, TYPELIST_8(T2, T3, T4, T5, T6, T7, T8, T9) >

#define TYPELIST_10(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10) \
	::TL::Typelist<T1, TYPELIST_9(T2, T3, T4, T5, T6, T7, T8, T9, T10) >

#define TYPELIST_11(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11) \
	::TL::Typelist<T1, TYPELIST_10(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11) >

#define TYPELIST_12(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12) \
	::TL::Typelist<T1, TYPELIST_11(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12) >

#define TYPELIST_13(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13) \
	::TL::Typelist<T1, TYPELIST_12(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13) >

#define TYPELIST_14(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14) \
	::TL::Typelist<T1, TYPELIST_13(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14) >

#define TYPELIST_15(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15) \
	::TL::Typelist<T1, TYPELIST_14(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15) >

#define TYPELIST_16(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16) \
	::TL::Typelist<T1, TYPELIST_15(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16) >

#define TYPELIST_17(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17) \
	::TL::Typelist<T1, TYPELIST_16(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17) >

#define TYPELIST_18(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18) \
	::TL::Typelist<T1, TYPELIST_17(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18) >

#define TYPELIST_19(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19) \
	::TL::Typelist<T1, TYPELIST_18(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19) >

#define TYPELIST_20(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20) \
	::TL::Typelist<T1, TYPELIST_19(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20) >

#define TYPELIST_21(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21) \
	::TL::Typelist<T1, TYPELIST_20(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21) >

#define TYPELIST_22(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22) \
	::TL::Typelist<T1, TYPELIST_21(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22) >

#define TYPELIST_23(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23) \
	::TL::Typelist<T1, TYPELIST_22(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23) >

#define TYPELIST_24(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24) \
	::TL::Typelist<T1, TYPELIST_23(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24) >

#define TYPELIST_25(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25) \
	::TL::Typelist<T1, TYPELIST_24(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25) >

#define TYPELIST_26(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26) \
	::TL::Typelist<T1, TYPELIST_25(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26) >

#define TYPELIST_27(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27) \
	::TL::Typelist<T1, TYPELIST_26(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27) >

#define TYPELIST_28(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28) \
	::TL::Typelist<T1, TYPELIST_27(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28) >

#define TYPELIST_29(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29) \
	::TL::Typelist<T1, TYPELIST_28(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29) >

#define TYPELIST_30(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30) \
	::TL::Typelist<T1, TYPELIST_29(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30) >

#define TYPELIST_31(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31) \
	::TL::Typelist<T1, TYPELIST_30(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31) >

#define TYPELIST_32(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32) \
	::TL::Typelist<T1, TYPELIST_31(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32) >

#define TYPELIST_33(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33) \
	::TL::Typelist<T1, TYPELIST_32(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33) >

#define TYPELIST_34(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34) \
	::TL::Typelist<T1, TYPELIST_33(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34) >

#define TYPELIST_35(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
	T31, T32, T33, T34, T35) \
	::TL::Typelist<T1, TYPELIST_34(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
	T31, T32, T33, T34, T35) >

#define TYPELIST_36(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
	T31, T32, T33, T34, T35, T36) \
	::TL::Typelist<T1, TYPELIST_35(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
	T31, T32, T33, T34, T35, T36) >

#define TYPELIST_37(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
	T31, T32, T33, T34, T35, T36, T37) \
	::TL::Typelist<T1, TYPELIST_36(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
	T31, T32, T33, T34, T35, T36, T37) >

#define TYPELIST_38(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
	T31, T32, T33, T34, T35, T36, T37, T38) \
	::TL::Typelist<T1, TYPELIST_37(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
	T31, T32, T33, T34, T35, T36, T37, T38) >

#define TYPELIST_39(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
	T31, T32, T33, T34, T35, T36, T37, T38, T39) \
	::TL::Typelist<T1, TYPELIST_38(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
	T31, T32, T33, T34, T35, T36, T37, T38, T39) >

#define TYPELIST_40(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
	T31, T32, T33, T34, T35, T36, T37, T38, T39, T40) \
	::TL::Typelist<T1, TYPELIST_39(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
	T31, T32, T33, T34, T35, T36, T37, T38, T39, T40) >

#define TYPELIST_41(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
	T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41) \
	::TL::Typelist<T1, TYPELIST_40(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
	T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41) >

#define TYPELIST_42(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
	T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42) \
	::TL::Typelist<T1, TYPELIST_41(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
	T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42) >

#define TYPELIST_43(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
	T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43) \
	::TL::Typelist<T1, TYPELIST_42(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
	T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43) >

#define TYPELIST_44(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
	T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44) \
	::TL::Typelist<T1, TYPELIST_43(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
	T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44) >

#define TYPELIST_45(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
	T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
	T41, T42, T43, T44, T45) \
	::TL::Typelist<T1, TYPELIST_44(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
	T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
	T41, T42, T43, T44, T45) >

#define TYPELIST_46(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
	T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
	T41, T42, T43, T44, T45, T46) \
	::TL::Typelist<T1, TYPELIST_45(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
	T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
	T41, T42, T43, T44, T45, T46) >

#define TYPELIST_47(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
	T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
	T41, T42, T43, T44, T45, T46, T47) \
	::TL::Typelist<T1, TYPELIST_46(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
	T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
	T41, T42, T43, T44, T45, T46, T47) >

#define TYPELIST_48(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
	T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
	T41, T42, T43, T44, T45, T46, T47, T48) \
	::TL::Typelist<T1, TYPELIST_47(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
	T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
	T41, T42, T43, T44, T45, T46, T47, T48) >

#define TYPELIST_49(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
	T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
	T41, T42, T43, T44, T45, T46, T47, T48, T49) \
	::TL::Typelist<T1, TYPELIST_48(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
	T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
	T41, T42, T43, T44, T45, T46, T47, T48, T49) >

#define TYPELIST_50(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
	T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
	T41, T42, T43, T44, T45, T46, T47, T48, T49, T50) \
	::TL::Typelist<T1, TYPELIST_49(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
	T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
	T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
	T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
	T41, T42, T43, T44, T45, T46, T47, T48, T49, T50) >

namespace TL {

	// NullType
	class NullType 
	{
	public:
		struct Head { private: Head(); };
		struct Tail { private: Tail(); };
	};

	// Typelist
	template <class T, class U>
	struct Typelist
	{
		typedef T Head;
		typedef U Tail;
	};

	//////////////////////////////////////////////////////////////////////////

	// TypeList Maker
	template
		<
		typename T1  = NullType, typename T2  = NullType, typename T3  = NullType,
		typename T4  = NullType, typename T5  = NullType, typename T6  = NullType,
		typename T7  = NullType, typename T8  = NullType, typename T9  = NullType,
		typename T10 = NullType, typename T11 = NullType, typename T12 = NullType,
		typename T13 = NullType, typename T14 = NullType, typename T15 = NullType,
		typename T16 = NullType, typename T17 = NullType, typename T18 = NullType
		> 
	struct MakeTypelist
	{
	private:
		typedef typename MakeTypelist
			<
			T2 , T3 , T4 , 
			T5 , T6 , T7 , 
			T8 , T9 , T10, 
			T11, T12, T13,
			T14, T15, T16, 
			T17, T18
			>
			::Result TailResult;

	public:
		typedef Typelist<T1, TailResult> Result;
	};

	template<>
	struct MakeTypelist
		<
		NullType, NullType, NullType, 
		NullType, NullType, NullType, 
		NullType, NullType, NullType, 
		NullType, NullType, NullType,
		NullType, NullType, NullType,
		NullType, NullType, NullType
		>{
			typedef NullType Result;
	};


	// Length
	template <class TList> struct Length;

	template <> 
	struct Length<NullType>
	{
		enum { value = 0 };
	};

	template <class TList>
	struct Length
	{
	private:
		typedef typename TList::Head Head;
		typedef typename TList::Tail Tail;

	public:
		enum { value = 1 + Length<Tail>::value };
	};

	// Index Of type
	template <class TList, class T> 
	struct IndexOf
	{
		typedef typename TList::Head Head;
		typedef typename TList::Tail Tail;

	private:

		template<class TList1>
		struct In
		{
		private:
			typedef typename TList1::Tail Tail;

			enum { temp = (IndexOf<Tail, T>::value) };

		public:
			enum { value = temp == -1 ? -1 : 1 + temp  };
		};

		template<>
		struct In< Typelist<T, Tail> >
		{
			enum { value = 0 };
		};

		template<>
		struct In<NullType>
		{
			enum { value = -1 };
		};

	public:
		enum { value = In<TList>::value };
	};

	// type at index
	template <class TList, unsigned int index> 
	struct TypeAt
	{
		typedef typename TList::Head Head;
		typedef typename TList::Tail Tail;

	private:
		template<unsigned int i>
		struct In
		{
			typedef typename TypeAt<Tail, i - 1>::Result Result;
		};

		template<>
		struct In<0>
		{
			typedef Head Result;
		};

	public:
		typedef typename In<index>::Result Result;
	};

} //end of namespace TL

#endif