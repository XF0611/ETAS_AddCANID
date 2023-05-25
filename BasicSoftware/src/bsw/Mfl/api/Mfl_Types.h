


#ifndef MFL_TYPES_H
#define MFL_TYPES_H


/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
 */
/* Structure definition for Dead Time routine */
typedef struct
{
    float32 dsintStatic;     /* Time since the last pack was written */
    float32* lszStatic;      /* Pointer to actual buffer position */
    float32* dtbufBegStatic; /* Pointer to begin of buffer */
    float32* dtbufEndStatic; /* Pointer to end of buffer */
} Mfl_DeadTimeParam_Type;

/*********************************************************************************************************************/

/* Structure definition for Debouncing parameters */
typedef struct
{
    float32 TimeHighLow;     /* Time for a High to Low transition, given in 10ms steps */
    float32 TimeLowHigh;     /* Time for a Low to High transition, given in 10ms steps */
} Mfl_DebounceParam_Type;

/*********************************************************************************************************************/

/* Structure definition for Debouncing state variables */
typedef struct
{
    boolean XOld;            /* Old input value from last call */
    float32 Timer;           /* Timer for internal state */
} Mfl_DebounceState_Type;

/*********************************************************************************************************************/

/* Controller limit value structure */
typedef struct
{
    float32 Min_C;           /* Minimum limit value */
    float32 Max_C;           /* Maximum limit value */
} Mfl_Limits_Type;

/*********************************************************************************************************************/

/* System and Time equivalent parameter Structure for PD controller routine */
typedef struct
{
    float32 K_C;             /* Amplification factor */
    float32 Tv_C;            /* Lead time */
} Mfl_ParamPD_Type;

/*********************************************************************************************************************/

/* System and Time equivalent parameter Structure for PI additive (Type 1 and Type 2) controller routine */
typedef struct
{
    float32 K_C;             /* Amplification factor */
    float32 Tnrec_C;         /* Tnrec_C Reciprocal follow up time (1/Tn) */
} Mfl_ParamPI_Type;

/*********************************************************************************************************************/

/* System and Time equivalent parameter Structure for PID additive (Type 1 and Type 2) controller routine */
typedef struct
{
    float32 K_C;             /* Amplification factor */
    float32 Tv_C;            /* Lead time */
    float32 Tnrec_C;         /* Reciprocal follow up time (1/Tn) */
} Mfl_ParamPID_Type;

/*********************************************************************************************************************/

/* Structure definition for Ramp routine */
typedef struct
{
    float32 SlopePos;        /* Positive slope for ramp in absolute value */
    float32 SlopeNeg;        /* Negative slope for ramp in absolute value */
} Mfl_ParamRamp_Type;

/*********************************************************************************************************************/

/* System State Structure for DT1-Type1 controller routine */
typedef struct
{
    float32 X1;              /* Input value, one time step before */
    float32 X2;              /* Input value, two time steps before */
    float32 Y1;              /* Output value, one time step before */
} Mfl_StateDT1Typ1_Type;

/*********************************************************************************************************************/

/* System State Structure for DT1-Type2 controller routine */
typedef struct
{
    float32 X1;              /* Input value, one time step before */
    float32 Y1;              /* Output value, one time step before */
} Mfl_StateDT1Typ2_Type;

/*********************************************************************************************************************/

/* System State Structure for I controller routine */
typedef struct
{
    float32 X1;              /* Input value, one time step before */
    float32 Y1;              /* Output value, one time step before */
} Mfl_StateI_Type;

/*********************************************************************************************************************/

/* System State Structure for PD controller routine */
typedef struct
{
    float32 X1;              /* Input value, one time step before */
    float32 Y1;              /* Output value, one time step before */
} Mfl_StatePD_Type;

/*********************************************************************************************************************/

/* System State Structure for PI additive (Type 1 and Type 2) controller routine */
typedef struct
{
    float32 X1;              /* Input value, one time step before */
    float32 Y1;              /* Output value, one time step before */
} Mfl_StatePI_Type;

/*********************************************************************************************************************/

/* System State Structure for PID additive (Type 1 and Type 2) controller routine */
typedef struct
{
    float32 X1;              /* Input value, one time step before */
    float32 X2;              /* Input value, two time steps before */
    float32 Y1;              /* Output value, one time step before */
} Mfl_StatePID_Type;

/*********************************************************************************************************************/

/* System State Structure for PT1 controller routine */
typedef struct
{
    float32 X1;              /* Input value, one time step before */
    float32 Y1;              /* Output value, one time step before */
} Mfl_StatePT1_Type;

/*********************************************************************************************************************/

/* Structure definition for Ramp routine */
typedef struct
{
    float32 State;           /* State of the ramp */
    sint8 Dir;               /* Ramp direction */
    sint8 Switch;            /* Position of switch */
} Mfl_StateRamp_Type;

/*********************************************************************************************************************/

/* Union definition to overcome aliasing problem */
/* MR12 DIR 1.1 VIOLATION: Float is used in union to extract bit pattern. */
typedef union
{
    float32 f32;            /* Float value */
    uint32 u32;             /* Bit representation of float */
} Mfl_f32u32u;

/*********************************************************************************************************************/

/* Activation of specific AR version */

#define MFL_AR42                        0
#define MFL_AR44                        1

#define MFL_ENABLED_AR_VERSION          MFL_AR42


/*
 **********************************************************************************************************************
 * Hash defines, symbols
 **********************************************************************************************************************
 */

/* Defines for ramp functionality */
#define MFL_RAMPDIR_RISING    (1)             /* Definition for a rising ramp                             */
#define MFL_RAMPDIR_FALLING   (-1)            /* Definition for a fallin ramp                             */
#define MFL_RAMPDIR_END       (0)             /* Definition for a ramp that has reached its end value     */
#define MFL_RAMPSWITCH_XA     (1)             /* Definition for switch functionality: Use target A        */
#define MFL_RAMPSWITCH_XB     (-1)            /* Definition for switch functionality: Use target B        */

/*Defines for ArcTan2 functionality */
#if (MFL_ENABLED_AR_VERSION == MFL_AR44)
#define MFL_POS_HALFPI        (1.5707964f)    /* Definition for the positive value of PI/2                */
#define MFL_NEG_HALFPI        (-1.5707964f)   /* Definition for the negative value of PI/2                */
#define MFL_PI                (3.1415927f)    /* Definition for the value of PI                           */
#endif

/* Defines for data type limits */
#define MFL_MAXSINT8          (0x7f)
#define MFL_MINSINT8          (-(MFL_MAXSINT8) - 1)
#define MFL_MAXUINT8          (0xff)
#define MFL_MAXUINT8_U        (0xffu)
#define MFL_MAXUINT16         (0xffff)
#define MFL_MAXUINT16_U       (0xffffu)
#define MFL_MINUINT16         (0x0)
#define MFL_MAXSINT16         (0x7fff)
#define MFL_MINSINT16         (-(MFL_MAXSINT16) - 1)
#define MFL_MAXUINT32         (0xffffffffuL)
#define MFL_MINUINT32         (0x0uL)
#define MFL_MAXSINT32         (0x7fffffffL)
#define MFL_MINSINT32         (-(MFL_MAXSINT32) - 1L)


/* Tables for elementar functions */
extern const float32 Mfl_ATanTable_caf32[];
extern const float32 Mfl_LogTable_caf32[];
extern const float32 Mfl_ExpIntegralTable_caf32[];
extern const float32 Mfl_ExpDecimalTable_caf32[];





/* MFL_TYPES_H */
#endif
