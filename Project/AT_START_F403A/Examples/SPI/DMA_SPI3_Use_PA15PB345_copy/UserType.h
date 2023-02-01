/*
 * @Date: 2022-08-30 17:30:09
 * @Autor: YangZhiQiang
 * @LastEditors: ZhiQiang
 * @Description: 
 * @CodingFormat: UTF-8
 * @LastEditTime: 2022-11-07 14:31:47
 * @FilePath: \MDKe:\Desktop\PRJ\GiteePrj\MyPrj222\PublicCode\Head\UserType.h
 * @Else: 
 */
#ifndef __USERTYPE_H
#define __USERTYPE_H

#include "stdint.h"
#include "UserDefine.h"

#define USER_DEBUG(...) printf(__VA_ARGS__)

#define GPIO_PIN_SET                    Bit_SET	        //高电平
#define GPIO_PIN_RESET					Bit_RESET       //低电平
#define ENUM_BIT_STATE                  BitState

#define GpioWriteBit(Port,Pin,State)    GPIO_WriteBit(Port,Pin,State)

//#define __nop()                         __NOP()

#define ARRAY_SIZE(X)  (sizeof(X)/sizeof(X[0]))

typedef enum
{
    UserFail = 0,
    UserOk,

    UserReset = UserFail,
    UserSet = UserOk,

    UserDisable = UserFail,
    UserEnable = UserOk,
}t_UserState;

typedef enum
{
    BAND_Start = 0,
    BAND_698 = BAND_Start,
    BAND_728,   // 下
    BAND_776,    // 223四频没有这个
    BAND_746,   // 下
    BAND_824,
    BAND_869,   // 下
    BAND_1710,
    BAND_2110,  // 下
    BAND_1850,
    BAND_1930,  // 下
    BAND_END,
}t_Band;

typedef enum
{
    BandNorm = 0,
    BandTest,
    BandSec,                // 自激震荡
    BandSSec,               // 疑似自激震荡
    BandRemoveSec,
    BandLargeSing,
    BandRetainTooLarge,
    BandSysStateImd,
    BandClose,
    BandNull,
    BandFccSecClose,
    BandWeakSecClose,
    BandSleep,
    BandStateEnd,
}t_BandState;

typedef struct 
{
    t_Band ThisLink;
    t_Band UpLink;
    t_Band Downlink;
}t_IndexOfLink;


typedef struct 
{
    char* Str;
}t_BandStateName;

extern const t_IndexOfLink IndexOfLink[BAND_END];
extern const t_BandStateName BandStateName[BandStateEnd];

#endif // !__USERTYPE_H

