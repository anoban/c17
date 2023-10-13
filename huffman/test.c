#ifdef _DEBUG
    #define DEBUG
#endif

#include <assert.h>

#include "huffman.h"

// perfecto :)
static inline void test_getbit(void) {
    uint8_t array[] = { 40,  230, 18,  242, 52,  6,   12,  145, 50,  116, 3,   45,  221, 165, 59,  151, 111, 162, 77,  160,
                        59,  157, 251, 206, 108, 129, 17,  18,  72,  180, 118, 129, 173, 126, 162, 230, 253, 205, 70,  42,
                        246, 70,  208, 118, 3,   66,  88,  191, 196, 12,  92,  248, 190, 109, 69,  87,  37,  207, 50,  160,
                        201, 71,  168, 177, 179, 209, 210, 53,  67,  208, 210, 188, 141, 179, 188, 114, 2,   120, 17,  236,
                        236, 80,  159, 238, 117, 206, 125, 113, 29,  4,   230, 234, 138, 8,   56,  53,  54,  189, 105, 53 };

    assert(getbit(array, 0) == 0);
    assert(getbit(array, 1) == 0);
    assert(getbit(array, 2) == 0);
    assert(getbit(array, 3) == 1);
    assert(getbit(array, 4) == 0);
    assert(getbit(array, 5) == 1);
    assert(getbit(array, 6) == 0);
    assert(getbit(array, 7) == 0);
    assert(getbit(array, 8) == 0);
    assert(getbit(array, 9) == 1);
    assert(getbit(array, 10) == 1);
    assert(getbit(array, 11) == 0);
    assert(getbit(array, 12) == 0);
    assert(getbit(array, 13) == 1);
    assert(getbit(array, 14) == 1);
    assert(getbit(array, 15) == 1);
    assert(getbit(array, 16) == 0);
    assert(getbit(array, 17) == 1);
    assert(getbit(array, 18) == 0);
    assert(getbit(array, 19) == 0);
    assert(getbit(array, 20) == 1);
    assert(getbit(array, 21) == 0);
    assert(getbit(array, 22) == 0);
    assert(getbit(array, 23) == 0);
    assert(getbit(array, 24) == 0);
    assert(getbit(array, 25) == 1);
    assert(getbit(array, 26) == 0);
    assert(getbit(array, 27) == 0);
    assert(getbit(array, 28) == 1);
    assert(getbit(array, 29) == 1);
    assert(getbit(array, 30) == 1);
    assert(getbit(array, 31) == 1);
    assert(getbit(array, 32) == 0);
    assert(getbit(array, 33) == 0);
    assert(getbit(array, 34) == 1);
    assert(getbit(array, 35) == 0);
    assert(getbit(array, 36) == 1);
    assert(getbit(array, 37) == 1);
    assert(getbit(array, 38) == 0);
    assert(getbit(array, 39) == 0);
    assert(getbit(array, 40) == 0);
    assert(getbit(array, 41) == 1);
    assert(getbit(array, 42) == 1);
    assert(getbit(array, 43) == 0);
    assert(getbit(array, 44) == 0);
    assert(getbit(array, 45) == 0);
    assert(getbit(array, 46) == 0);
    assert(getbit(array, 47) == 0);
    assert(getbit(array, 48) == 0);
    assert(getbit(array, 49) == 0);
    assert(getbit(array, 50) == 1);
    assert(getbit(array, 51) == 1);
    assert(getbit(array, 52) == 0);
    assert(getbit(array, 53) == 0);
    assert(getbit(array, 54) == 0);
    assert(getbit(array, 55) == 0);
    assert(getbit(array, 56) == 1);
    assert(getbit(array, 57) == 0);
    assert(getbit(array, 58) == 0);
    assert(getbit(array, 59) == 0);
    assert(getbit(array, 60) == 1);
    assert(getbit(array, 61) == 0);
    assert(getbit(array, 62) == 0);
    assert(getbit(array, 63) == 1);
    assert(getbit(array, 64) == 0);
    assert(getbit(array, 65) == 1);
    assert(getbit(array, 66) == 0);
    assert(getbit(array, 67) == 0);
    assert(getbit(array, 68) == 1);
    assert(getbit(array, 69) == 1);
    assert(getbit(array, 70) == 0);
    assert(getbit(array, 71) == 0);
    assert(getbit(array, 72) == 0);
    assert(getbit(array, 73) == 0);
    assert(getbit(array, 74) == 1);
    assert(getbit(array, 75) == 0);
    assert(getbit(array, 76) == 1);
    assert(getbit(array, 77) == 1);
    assert(getbit(array, 78) == 1);
    assert(getbit(array, 79) == 0);
    assert(getbit(array, 80) == 1);
    assert(getbit(array, 81) == 1);
    assert(getbit(array, 82) == 0);
    assert(getbit(array, 83) == 0);
    assert(getbit(array, 84) == 0);
    assert(getbit(array, 85) == 0);
    assert(getbit(array, 86) == 0);
    assert(getbit(array, 87) == 0);
    assert(getbit(array, 88) == 1);
    assert(getbit(array, 89) == 0);
    assert(getbit(array, 90) == 1);
    assert(getbit(array, 91) == 1);
    assert(getbit(array, 92) == 0);
    assert(getbit(array, 93) == 1);
    assert(getbit(array, 94) == 0);
    assert(getbit(array, 95) == 0);
    assert(getbit(array, 96) == 1);
    assert(getbit(array, 97) == 0);
    assert(getbit(array, 98) == 1);
    assert(getbit(array, 99) == 1);
    assert(getbit(array, 100) == 1);
    assert(getbit(array, 101) == 0);
    assert(getbit(array, 102) == 1);
    assert(getbit(array, 103) == 1);
    assert(getbit(array, 104) == 1);
    assert(getbit(array, 105) == 0);
    assert(getbit(array, 106) == 1);
    assert(getbit(array, 107) == 0);
    assert(getbit(array, 108) == 0);
    assert(getbit(array, 109) == 1);
    assert(getbit(array, 110) == 0);
    assert(getbit(array, 111) == 1);
    assert(getbit(array, 112) == 1);
    assert(getbit(array, 113) == 1);
    assert(getbit(array, 114) == 0);
    assert(getbit(array, 115) == 1);
    assert(getbit(array, 116) == 1);
    assert(getbit(array, 117) == 1);
    assert(getbit(array, 118) == 0);
    assert(getbit(array, 119) == 0);
    assert(getbit(array, 120) == 1);
    assert(getbit(array, 121) == 1);
    assert(getbit(array, 122) == 1);
    assert(getbit(array, 123) == 0);
    assert(getbit(array, 124) == 1);
    assert(getbit(array, 125) == 0);
    assert(getbit(array, 126) == 0);
    assert(getbit(array, 127) == 1);
    assert(getbit(array, 128) == 1);
    assert(getbit(array, 129) == 1);
    assert(getbit(array, 130) == 1);
    assert(getbit(array, 131) == 1);
    assert(getbit(array, 132) == 0);
    assert(getbit(array, 133) == 1);
    assert(getbit(array, 134) == 1);
    assert(getbit(array, 135) == 0);
    assert(getbit(array, 136) == 0);
    assert(getbit(array, 137) == 1);
    assert(getbit(array, 138) == 0);
    assert(getbit(array, 139) == 0);
    assert(getbit(array, 140) == 0);
    assert(getbit(array, 141) == 1);
    assert(getbit(array, 142) == 0);
    assert(getbit(array, 143) == 1);
    assert(getbit(array, 144) == 1);
    assert(getbit(array, 145) == 0);
    assert(getbit(array, 146) == 1);
    assert(getbit(array, 147) == 1);
    assert(getbit(array, 148) == 0);
    assert(getbit(array, 149) == 0);
    assert(getbit(array, 150) == 1);
    assert(getbit(array, 151) == 0);
    assert(getbit(array, 152) == 0);
    assert(getbit(array, 153) == 0);
    assert(getbit(array, 154) == 0);
    assert(getbit(array, 155) == 0);
    assert(getbit(array, 156) == 0);
    assert(getbit(array, 157) == 1);
    assert(getbit(array, 158) == 0);
    assert(getbit(array, 159) == 1);
    assert(getbit(array, 160) == 1);
    assert(getbit(array, 161) == 1);
    assert(getbit(array, 162) == 0);
    assert(getbit(array, 163) == 1);
    assert(getbit(array, 164) == 1);
    assert(getbit(array, 165) == 1);
    assert(getbit(array, 166) == 0);
    assert(getbit(array, 167) == 0);
    assert(getbit(array, 168) == 1);
    assert(getbit(array, 169) == 0);
    assert(getbit(array, 170) == 1);
    assert(getbit(array, 171) == 1);
    assert(getbit(array, 172) == 1);
    assert(getbit(array, 173) == 0);
    assert(getbit(array, 174) == 0);
    assert(getbit(array, 175) == 1);
    assert(getbit(array, 176) == 1);
    assert(getbit(array, 177) == 1);
    assert(getbit(array, 178) == 0);
    assert(getbit(array, 179) == 1);
    assert(getbit(array, 180) == 1);
    assert(getbit(array, 181) == 1);
    assert(getbit(array, 182) == 1);
    assert(getbit(array, 183) == 1);
    assert(getbit(array, 184) == 0);
    assert(getbit(array, 185) == 1);
    assert(getbit(array, 186) == 1);
    assert(getbit(array, 187) == 1);
    assert(getbit(array, 188) == 0);
    assert(getbit(array, 189) == 0);
    assert(getbit(array, 190) == 1);
    assert(getbit(array, 191) == 1);
    assert(getbit(array, 192) == 0);
    assert(getbit(array, 193) == 0);
    assert(getbit(array, 194) == 1);
    assert(getbit(array, 195) == 1);
    assert(getbit(array, 196) == 0);
    assert(getbit(array, 197) == 1);
    assert(getbit(array, 198) == 1);
    assert(getbit(array, 199) == 0);
    assert(getbit(array, 200) == 1);
    assert(getbit(array, 201) == 0);
    assert(getbit(array, 202) == 0);
    assert(getbit(array, 203) == 0);
    assert(getbit(array, 204) == 0);
    assert(getbit(array, 205) == 0);
    assert(getbit(array, 206) == 0);
    assert(getbit(array, 207) == 1);
    assert(getbit(array, 208) == 1);
    assert(getbit(array, 209) == 0);
    assert(getbit(array, 210) == 0);
    assert(getbit(array, 211) == 0);
    assert(getbit(array, 212) == 1);
    assert(getbit(array, 213) == 0);
    assert(getbit(array, 214) == 0);
    assert(getbit(array, 215) == 0);
    assert(getbit(array, 216) == 0);
    assert(getbit(array, 217) == 1);
    assert(getbit(array, 218) == 0);
    assert(getbit(array, 219) == 0);
    assert(getbit(array, 220) == 1);
    assert(getbit(array, 221) == 0);
    assert(getbit(array, 222) == 0);
    assert(getbit(array, 223) == 0);
    assert(getbit(array, 224) == 0);
    assert(getbit(array, 225) == 0);
    assert(getbit(array, 226) == 0);
    assert(getbit(array, 227) == 1);
    assert(getbit(array, 228) == 0);
    assert(getbit(array, 229) == 0);
    assert(getbit(array, 230) == 1);
    assert(getbit(array, 231) == 0);
    assert(getbit(array, 232) == 0);
    assert(getbit(array, 233) == 0);
    assert(getbit(array, 234) == 1);
    assert(getbit(array, 235) == 0);
    assert(getbit(array, 236) == 1);
    assert(getbit(array, 237) == 1);
    assert(getbit(array, 238) == 0);
    assert(getbit(array, 239) == 1);
    assert(getbit(array, 240) == 0);
    assert(getbit(array, 241) == 1);
    assert(getbit(array, 242) == 1);
    assert(getbit(array, 243) == 0);
    assert(getbit(array, 244) == 1);
    assert(getbit(array, 245) == 1);
    assert(getbit(array, 246) == 1);
    assert(getbit(array, 247) == 0);
    assert(getbit(array, 248) == 1);
    assert(getbit(array, 249) == 0);
    assert(getbit(array, 250) == 0);
    assert(getbit(array, 251) == 0);
    assert(getbit(array, 252) == 0);
    assert(getbit(array, 253) == 0);
    assert(getbit(array, 254) == 0);
    assert(getbit(array, 255) == 1);
    assert(getbit(array, 256) == 1);
    assert(getbit(array, 257) == 0);
    assert(getbit(array, 258) == 1);
    assert(getbit(array, 259) == 1);
    assert(getbit(array, 260) == 0);
    assert(getbit(array, 261) == 1);
    assert(getbit(array, 262) == 0);
    assert(getbit(array, 263) == 1);
    assert(getbit(array, 264) == 0);
    assert(getbit(array, 265) == 1);
    assert(getbit(array, 266) == 1);
    assert(getbit(array, 267) == 1);
    assert(getbit(array, 268) == 1);
    assert(getbit(array, 269) == 1);
    assert(getbit(array, 270) == 1);
    assert(getbit(array, 271) == 0);
    assert(getbit(array, 272) == 0);
    assert(getbit(array, 273) == 1);
    assert(getbit(array, 274) == 0);
    assert(getbit(array, 275) == 0);
    assert(getbit(array, 276) == 0);
    assert(getbit(array, 277) == 1);
    assert(getbit(array, 278) == 0);
    assert(getbit(array, 279) == 1);
    assert(getbit(array, 280) == 0);
    assert(getbit(array, 281) == 1);
    assert(getbit(array, 282) == 1);
    assert(getbit(array, 283) == 0);
    assert(getbit(array, 284) == 0);
    assert(getbit(array, 285) == 1);
    assert(getbit(array, 286) == 1);
    assert(getbit(array, 287) == 1);
    assert(getbit(array, 288) == 1);
    assert(getbit(array, 289) == 0);
    assert(getbit(array, 290) == 1);
    assert(getbit(array, 291) == 1);
    assert(getbit(array, 292) == 1);
    assert(getbit(array, 293) == 1);
    assert(getbit(array, 294) == 1);
    assert(getbit(array, 295) == 1);
    assert(getbit(array, 296) == 1);
    assert(getbit(array, 297) == 0);
    assert(getbit(array, 298) == 1);
    assert(getbit(array, 299) == 1);
    assert(getbit(array, 300) == 0);
    assert(getbit(array, 301) == 0);
    assert(getbit(array, 302) == 1);
    assert(getbit(array, 303) == 1);
    assert(getbit(array, 304) == 0);
    assert(getbit(array, 305) == 1);
    assert(getbit(array, 306) == 1);
    assert(getbit(array, 307) == 0);
    assert(getbit(array, 308) == 0);
    assert(getbit(array, 309) == 0);
    assert(getbit(array, 310) == 1);
    assert(getbit(array, 311) == 0);
    assert(getbit(array, 312) == 0);
    assert(getbit(array, 313) == 1);
    assert(getbit(array, 314) == 0);
    assert(getbit(array, 315) == 1);
    assert(getbit(array, 316) == 0);
    assert(getbit(array, 317) == 1);
    assert(getbit(array, 318) == 0);
    assert(getbit(array, 319) == 0);
    assert(getbit(array, 320) == 0);
    assert(getbit(array, 321) == 1);
    assert(getbit(array, 322) == 1);
    assert(getbit(array, 323) == 0);
    assert(getbit(array, 324) == 1);
    assert(getbit(array, 325) == 1);
    assert(getbit(array, 326) == 1);
    assert(getbit(array, 327) == 1);
    assert(getbit(array, 328) == 0);
    assert(getbit(array, 329) == 1);
    assert(getbit(array, 330) == 1);
    assert(getbit(array, 331) == 0);
    assert(getbit(array, 332) == 0);
    assert(getbit(array, 333) == 0);
    assert(getbit(array, 334) == 1);
    assert(getbit(array, 335) == 0);
    assert(getbit(array, 336) == 0);
    assert(getbit(array, 337) == 0);
    assert(getbit(array, 338) == 0);
    assert(getbit(array, 339) == 0);
    assert(getbit(array, 340) == 1);
    assert(getbit(array, 341) == 0);
    assert(getbit(array, 342) == 1);
    assert(getbit(array, 343) == 1);
    assert(getbit(array, 344) == 0);
    assert(getbit(array, 345) == 1);
    assert(getbit(array, 346) == 1);
    assert(getbit(array, 347) == 0);
    assert(getbit(array, 348) == 1);
    assert(getbit(array, 349) == 1);
    assert(getbit(array, 350) == 1);
    assert(getbit(array, 351) == 0);
    assert(getbit(array, 352) == 1);
    assert(getbit(array, 353) == 1);
    assert(getbit(array, 354) == 0);
    assert(getbit(array, 355) == 0);
    assert(getbit(array, 356) == 0);
    assert(getbit(array, 357) == 0);
    assert(getbit(array, 358) == 0);
    assert(getbit(array, 359) == 0);
    assert(getbit(array, 360) == 0);
    assert(getbit(array, 361) == 1);
    assert(getbit(array, 362) == 0);
    assert(getbit(array, 363) == 0);
    assert(getbit(array, 364) == 0);
    assert(getbit(array, 365) == 0);
    assert(getbit(array, 366) == 1);
    assert(getbit(array, 367) == 0);
    assert(getbit(array, 368) == 0);
    assert(getbit(array, 369) == 0);
    assert(getbit(array, 370) == 0);
    assert(getbit(array, 371) == 1);
    assert(getbit(array, 372) == 1);
    assert(getbit(array, 373) == 0);
    assert(getbit(array, 374) == 1);
    assert(getbit(array, 375) == 0);
    assert(getbit(array, 376) == 1);
    assert(getbit(array, 377) == 1);
    assert(getbit(array, 378) == 1);
    assert(getbit(array, 379) == 1);
    assert(getbit(array, 380) == 1);
    assert(getbit(array, 381) == 1);
    assert(getbit(array, 382) == 0);
    assert(getbit(array, 383) == 1);
    assert(getbit(array, 384) == 0);
    assert(getbit(array, 385) == 0);
    assert(getbit(array, 386) == 1);
    assert(getbit(array, 387) == 0);
    assert(getbit(array, 388) == 0);
    assert(getbit(array, 389) == 0);
    assert(getbit(array, 390) == 1);
    assert(getbit(array, 391) == 1);
    assert(getbit(array, 392) == 0);
    assert(getbit(array, 393) == 0);
    assert(getbit(array, 394) == 1);
    assert(getbit(array, 395) == 1);
    assert(getbit(array, 396) == 0);
    assert(getbit(array, 397) == 0);
    assert(getbit(array, 398) == 0);
    assert(getbit(array, 399) == 0);
    assert(getbit(array, 400) == 0);
    assert(getbit(array, 401) == 0);
    assert(getbit(array, 402) == 1);
    assert(getbit(array, 403) == 1);
    assert(getbit(array, 404) == 1);
    assert(getbit(array, 405) == 0);
    assert(getbit(array, 406) == 1);
    assert(getbit(array, 407) == 0);
    assert(getbit(array, 408) == 0);
    assert(getbit(array, 409) == 0);
    assert(getbit(array, 410) == 0);
    assert(getbit(array, 411) == 1);
    assert(getbit(array, 412) == 1);
    assert(getbit(array, 413) == 1);
    assert(getbit(array, 414) == 1);
    assert(getbit(array, 415) == 1);
    assert(getbit(array, 416) == 0);
    assert(getbit(array, 417) == 1);
    assert(getbit(array, 418) == 1);
    assert(getbit(array, 419) == 1);
    assert(getbit(array, 420) == 1);
    assert(getbit(array, 421) == 1);
    assert(getbit(array, 422) == 0);
    assert(getbit(array, 423) == 1);
    assert(getbit(array, 424) == 1);
    assert(getbit(array, 425) == 0);
    assert(getbit(array, 426) == 1);
    assert(getbit(array, 427) == 1);
    assert(getbit(array, 428) == 0);
    assert(getbit(array, 429) == 1);
    assert(getbit(array, 430) == 1);
    assert(getbit(array, 431) == 0);
    assert(getbit(array, 432) == 1);
    assert(getbit(array, 433) == 0);
    assert(getbit(array, 434) == 1);
    assert(getbit(array, 435) == 0);
    assert(getbit(array, 436) == 0);
    assert(getbit(array, 437) == 0);
    assert(getbit(array, 438) == 1);
    assert(getbit(array, 439) == 0);
    assert(getbit(array, 440) == 1);
    assert(getbit(array, 441) == 1);
    assert(getbit(array, 442) == 1);
    assert(getbit(array, 443) == 0);
    assert(getbit(array, 444) == 1);
    assert(getbit(array, 445) == 0);
    assert(getbit(array, 446) == 1);
    assert(getbit(array, 447) == 0);
    assert(getbit(array, 448) == 1);
    assert(getbit(array, 449) == 0);
    assert(getbit(array, 450) == 1);
    assert(getbit(array, 451) == 0);
    assert(getbit(array, 452) == 0);
    assert(getbit(array, 453) == 1);
    assert(getbit(array, 454) == 0);
    assert(getbit(array, 455) == 0);
    assert(getbit(array, 456) == 1);
    assert(getbit(array, 457) == 1);
    assert(getbit(array, 458) == 1);
    assert(getbit(array, 459) == 1);
    assert(getbit(array, 460) == 0);
    assert(getbit(array, 461) == 0);
    assert(getbit(array, 462) == 1);
    assert(getbit(array, 463) == 1);
    assert(getbit(array, 464) == 0);
    assert(getbit(array, 465) == 1);
    assert(getbit(array, 466) == 0);
    assert(getbit(array, 467) == 0);
    assert(getbit(array, 468) == 1);
    assert(getbit(array, 469) == 1);
    assert(getbit(array, 470) == 0);
    assert(getbit(array, 471) == 0);
    assert(getbit(array, 472) == 0);
    assert(getbit(array, 473) == 0);
    assert(getbit(array, 474) == 0);
    assert(getbit(array, 475) == 0);
    assert(getbit(array, 476) == 0);
    assert(getbit(array, 477) == 1);
    assert(getbit(array, 478) == 0);
    assert(getbit(array, 479) == 1);
    assert(getbit(array, 480) == 1);
    assert(getbit(array, 481) == 0);
    assert(getbit(array, 482) == 0);
    assert(getbit(array, 483) == 1);
    assert(getbit(array, 484) == 0);
    assert(getbit(array, 485) == 0);
    assert(getbit(array, 486) == 1);
    assert(getbit(array, 487) == 1);
    assert(getbit(array, 488) == 1);
    assert(getbit(array, 489) == 1);
    assert(getbit(array, 490) == 1);
    assert(getbit(array, 491) == 0);
    assert(getbit(array, 492) == 0);
    assert(getbit(array, 493) == 0);
    assert(getbit(array, 494) == 1);
    assert(getbit(array, 495) == 0);
    assert(getbit(array, 496) == 0);
    assert(getbit(array, 497) == 0);
    assert(getbit(array, 498) == 0);
    assert(getbit(array, 499) == 1);
    assert(getbit(array, 500) == 0);
    assert(getbit(array, 501) == 1);
    assert(getbit(array, 502) == 0);
    assert(getbit(array, 503) == 1);
    assert(getbit(array, 504) == 1);
    assert(getbit(array, 505) == 0);
    assert(getbit(array, 506) == 0);
    assert(getbit(array, 507) == 0);
    assert(getbit(array, 508) == 1);
    assert(getbit(array, 509) == 1);
    assert(getbit(array, 510) == 0);
    assert(getbit(array, 511) == 1);
    assert(getbit(array, 512) == 1);
    assert(getbit(array, 513) == 1);
    assert(getbit(array, 514) == 0);
    assert(getbit(array, 515) == 0);
    assert(getbit(array, 516) == 1);
    assert(getbit(array, 517) == 1);
    assert(getbit(array, 518) == 0);
    assert(getbit(array, 519) == 1);
    assert(getbit(array, 520) == 1);
    assert(getbit(array, 521) == 0);
    assert(getbit(array, 522) == 0);
    assert(getbit(array, 523) == 0);
    assert(getbit(array, 524) == 1);
    assert(getbit(array, 525) == 0);
    assert(getbit(array, 526) == 1);
    assert(getbit(array, 527) == 1);
    assert(getbit(array, 528) == 0);
    assert(getbit(array, 529) == 1);
    assert(getbit(array, 530) == 0);
    assert(getbit(array, 531) == 0);
    assert(getbit(array, 532) == 1);
    assert(getbit(array, 533) == 0);
    assert(getbit(array, 534) == 1);
    assert(getbit(array, 535) == 1);
    assert(getbit(array, 536) == 1);
    assert(getbit(array, 537) == 0);
    assert(getbit(array, 538) == 1);
    assert(getbit(array, 539) == 0);
    assert(getbit(array, 540) == 1);
    assert(getbit(array, 541) == 1);
    assert(getbit(array, 542) == 0);
    assert(getbit(array, 543) == 0);
    assert(getbit(array, 544) == 1);
    assert(getbit(array, 545) == 1);
    assert(getbit(array, 546) == 0);
    assert(getbit(array, 547) == 0);
    assert(getbit(array, 548) == 0);
    assert(getbit(array, 549) == 0);
    assert(getbit(array, 550) == 1);
    assert(getbit(array, 551) == 0);
    assert(getbit(array, 552) == 0);
    assert(getbit(array, 553) == 0);
    assert(getbit(array, 554) == 0);
    assert(getbit(array, 555) == 0);
    assert(getbit(array, 556) == 1);
    assert(getbit(array, 557) == 0);
    assert(getbit(array, 558) == 1);
    assert(getbit(array, 559) == 1);
    assert(getbit(array, 560) == 0);
    assert(getbit(array, 561) == 1);
    assert(getbit(array, 562) == 0);
    assert(getbit(array, 563) == 0);
    assert(getbit(array, 564) == 1);
    assert(getbit(array, 565) == 0);
    assert(getbit(array, 566) == 1);
    assert(getbit(array, 567) == 1);
    assert(getbit(array, 568) == 0);
    assert(getbit(array, 569) == 0);
    assert(getbit(array, 570) == 1);
    assert(getbit(array, 571) == 1);
    assert(getbit(array, 572) == 1);
    assert(getbit(array, 573) == 1);
    assert(getbit(array, 574) == 0);
    assert(getbit(array, 575) == 1);
    assert(getbit(array, 576) == 1);
    assert(getbit(array, 577) == 0);
    assert(getbit(array, 578) == 1);
    assert(getbit(array, 579) == 1);
    assert(getbit(array, 580) == 0);
    assert(getbit(array, 581) == 0);
    assert(getbit(array, 582) == 0);
    assert(getbit(array, 583) == 1);
    assert(getbit(array, 584) == 1);
    assert(getbit(array, 585) == 1);
    assert(getbit(array, 586) == 0);
    assert(getbit(array, 587) == 0);
    assert(getbit(array, 588) == 1);
    assert(getbit(array, 589) == 1);
    assert(getbit(array, 590) == 0);
    assert(getbit(array, 591) == 1);
    assert(getbit(array, 592) == 0);
    assert(getbit(array, 593) == 0);
    assert(getbit(array, 594) == 1);
    assert(getbit(array, 595) == 1);
    assert(getbit(array, 596) == 1);
    assert(getbit(array, 597) == 1);
    assert(getbit(array, 598) == 0);
    assert(getbit(array, 599) == 1);
    assert(getbit(array, 600) == 0);
    assert(getbit(array, 601) == 1);
    assert(getbit(array, 602) == 0);
    assert(getbit(array, 603) == 0);
    assert(getbit(array, 604) == 1);
    assert(getbit(array, 605) == 1);
    assert(getbit(array, 606) == 1);
    assert(getbit(array, 607) == 0);
    assert(getbit(array, 608) == 0);
    assert(getbit(array, 609) == 1);
    assert(getbit(array, 610) == 0);
    assert(getbit(array, 611) == 0);
    assert(getbit(array, 612) == 0);
    assert(getbit(array, 613) == 0);
    assert(getbit(array, 614) == 0);
    assert(getbit(array, 615) == 0);
    assert(getbit(array, 616) == 0);
    assert(getbit(array, 617) == 0);
    assert(getbit(array, 618) == 0);
    assert(getbit(array, 619) == 1);
    assert(getbit(array, 620) == 1);
    assert(getbit(array, 621) == 1);
    assert(getbit(array, 622) == 1);
    assert(getbit(array, 623) == 0);
    assert(getbit(array, 624) == 1);
    assert(getbit(array, 625) == 0);
    assert(getbit(array, 626) == 0);
    assert(getbit(array, 627) == 0);
    assert(getbit(array, 628) == 1);
    assert(getbit(array, 629) == 0);
    assert(getbit(array, 630) == 0);
    assert(getbit(array, 631) == 0);
    assert(getbit(array, 632) == 0);
    assert(getbit(array, 633) == 0);
    assert(getbit(array, 634) == 1);
    assert(getbit(array, 635) == 1);
    assert(getbit(array, 636) == 0);
    assert(getbit(array, 637) == 1);
    assert(getbit(array, 638) == 1);
    assert(getbit(array, 639) == 1);
    assert(getbit(array, 640) == 0);
    assert(getbit(array, 641) == 0);
    assert(getbit(array, 642) == 1);
    assert(getbit(array, 643) == 1);
    assert(getbit(array, 644) == 0);
    assert(getbit(array, 645) == 1);
    assert(getbit(array, 646) == 1);
    assert(getbit(array, 647) == 1);
    assert(getbit(array, 648) == 0);
    assert(getbit(array, 649) == 0);
    assert(getbit(array, 650) == 0);
    assert(getbit(array, 651) == 0);
    assert(getbit(array, 652) == 1);
    assert(getbit(array, 653) == 0);
    assert(getbit(array, 654) == 1);
    assert(getbit(array, 655) == 0);
    assert(getbit(array, 656) == 1);
    assert(getbit(array, 657) == 1);
    assert(getbit(array, 658) == 1);
    assert(getbit(array, 659) == 1);
    assert(getbit(array, 660) == 1);
    assert(getbit(array, 661) == 0);
    assert(getbit(array, 662) == 0);
    assert(getbit(array, 663) == 1);
    assert(getbit(array, 664) == 0);
    assert(getbit(array, 665) == 1);
    assert(getbit(array, 666) == 1);
    assert(getbit(array, 667) == 1);
    assert(getbit(array, 668) == 0);
    assert(getbit(array, 669) == 1);
    assert(getbit(array, 670) == 1);
    assert(getbit(array, 671) == 1);
    assert(getbit(array, 672) == 1);
    assert(getbit(array, 673) == 0);
    assert(getbit(array, 674) == 1);
    assert(getbit(array, 675) == 0);
    assert(getbit(array, 676) == 1);
    assert(getbit(array, 677) == 1);
    assert(getbit(array, 678) == 1);
    assert(getbit(array, 679) == 0);
    assert(getbit(array, 680) == 0);
    assert(getbit(array, 681) == 1);
    assert(getbit(array, 682) == 1);
    assert(getbit(array, 683) == 1);
    assert(getbit(array, 684) == 0);
    assert(getbit(array, 685) == 0);
    assert(getbit(array, 686) == 1);
    assert(getbit(array, 687) == 1);
    assert(getbit(array, 688) == 1);
    assert(getbit(array, 689) == 0);
    assert(getbit(array, 690) == 1);
    assert(getbit(array, 691) == 1);
    assert(getbit(array, 692) == 1);
    assert(getbit(array, 693) == 1);
    assert(getbit(array, 694) == 1);
    assert(getbit(array, 695) == 0);
    assert(getbit(array, 696) == 1);
    assert(getbit(array, 697) == 0);
    assert(getbit(array, 698) == 0);
    assert(getbit(array, 699) == 0);
    assert(getbit(array, 700) == 1);
    assert(getbit(array, 701) == 1);
    assert(getbit(array, 702) == 1);
    assert(getbit(array, 703) == 0);
    assert(getbit(array, 704) == 1);
    assert(getbit(array, 705) == 0);
    assert(getbit(array, 706) == 1);
    assert(getbit(array, 707) == 1);
    assert(getbit(array, 708) == 1);
    assert(getbit(array, 709) == 0);
    assert(getbit(array, 710) == 0);
    assert(getbit(array, 711) == 0);
    assert(getbit(array, 712) == 0);
    assert(getbit(array, 713) == 0);
    assert(getbit(array, 714) == 1);
    assert(getbit(array, 715) == 0);
    assert(getbit(array, 716) == 0);
    assert(getbit(array, 717) == 0);
    assert(getbit(array, 718) == 0);
    assert(getbit(array, 719) == 0);
    assert(getbit(array, 720) == 0);
    assert(getbit(array, 721) == 1);
    assert(getbit(array, 722) == 1);
    assert(getbit(array, 723) == 0);
    assert(getbit(array, 724) == 0);
    assert(getbit(array, 725) == 1);
    assert(getbit(array, 726) == 1);
    assert(getbit(array, 727) == 1);
    assert(getbit(array, 728) == 0);
    assert(getbit(array, 729) == 1);
    assert(getbit(array, 730) == 0);
    assert(getbit(array, 731) == 1);
    assert(getbit(array, 732) == 0);
    assert(getbit(array, 733) == 1);
    assert(getbit(array, 734) == 1);
    assert(getbit(array, 735) == 1);
    assert(getbit(array, 736) == 0);
    assert(getbit(array, 737) == 1);
    assert(getbit(array, 738) == 0);
    assert(getbit(array, 739) == 1);
    assert(getbit(array, 740) == 0);
    assert(getbit(array, 741) == 0);
    assert(getbit(array, 742) == 0);
    assert(getbit(array, 743) == 1);
    assert(getbit(array, 744) == 0);
    assert(getbit(array, 745) == 0);
    assert(getbit(array, 746) == 0);
    assert(getbit(array, 747) == 1);
    assert(getbit(array, 748) == 0);
    assert(getbit(array, 749) == 0);
    assert(getbit(array, 750) == 0);
    assert(getbit(array, 751) == 0);
    assert(getbit(array, 752) == 0);
    assert(getbit(array, 753) == 0);
    assert(getbit(array, 754) == 0);
    assert(getbit(array, 755) == 1);
    assert(getbit(array, 756) == 1);
    assert(getbit(array, 757) == 1);
    assert(getbit(array, 758) == 0);
    assert(getbit(array, 759) == 0);
    assert(getbit(array, 760) == 1);
    assert(getbit(array, 761) == 0);
    assert(getbit(array, 762) == 1);
    assert(getbit(array, 763) == 0);
    assert(getbit(array, 764) == 1);
    assert(getbit(array, 765) == 1);
    assert(getbit(array, 766) == 0);
    assert(getbit(array, 767) == 0);
    assert(getbit(array, 768) == 0);
    assert(getbit(array, 769) == 1);
    assert(getbit(array, 770) == 1);
    assert(getbit(array, 771) == 0);
    assert(getbit(array, 772) == 1);
    assert(getbit(array, 773) == 1);
    assert(getbit(array, 774) == 0);
    assert(getbit(array, 775) == 0);
    assert(getbit(array, 776) == 1);
    assert(getbit(array, 777) == 0);
    assert(getbit(array, 778) == 1);
    assert(getbit(array, 779) == 1);
    assert(getbit(array, 780) == 1);
    assert(getbit(array, 781) == 1);
    assert(getbit(array, 782) == 0);
    assert(getbit(array, 783) == 1);
    assert(getbit(array, 784) == 1);
    assert(getbit(array, 785) == 0);
    assert(getbit(array, 786) == 0);
    assert(getbit(array, 787) == 1);
    assert(getbit(array, 788) == 0);
    assert(getbit(array, 789) == 1);
    assert(getbit(array, 790) == 1);
    assert(getbit(array, 791) == 0);
    assert(getbit(array, 792) == 1);
    assert(getbit(array, 793) == 0);
    assert(getbit(array, 794) == 1);
    assert(getbit(array, 795) == 0);
    assert(getbit(array, 796) == 1);
    assert(getbit(array, 797) == 1);
    assert(getbit(array, 798) == 0);
    assert(getbit(array, 799) == 0);

    return;
}

int main(void) {
    test_getbit();
    return 0;
}