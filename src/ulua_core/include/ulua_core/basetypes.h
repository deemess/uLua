#ifndef BASETYPES_H
#define BASETYPES_H

#include <stdint.h>

typedef uint8_t lu08;
typedef uint16_t lu16;
typedef uint32_t lu32;

typedef int8_t ls08;
typedef int16_t ls16;
typedef int32_t ls32;

#define ULUA_NULL 0
typedef lu08 BOOL;
typedef lu08 ULUA_BOOL;
#define ULUA_TRUE  1
#define ULUA_FALSE 0

//bit operation
#define SETBIT(var,bit) var = var | bit
#define RESBIT(var,bit) var = var & (~bit)

//callback functions type
typedef void (*readBytes)(lu08* buff, lu16 offset, lu16 size);

//vm types
//variable types
typedef enum {
    REGISTER_VAR_NULL,
    REGISTER_VAR_BOOLEAN,
    REGISTER_VAR_NUMBER,
    REGISTER_VAR_FLOAT,
    REGISTER_VAR_CLOSURE,
    REGISTER_VAR_MEMVAR,
    REGISTER_VAR_NATIVE_FUNC,
    REGISTER_VAR_TABLE
} vartype;

typedef struct {
    vartype type;
    union {
        lu32   numval;
        float floatval;
        void* pointer;
    };
} vmregister;

#endif
