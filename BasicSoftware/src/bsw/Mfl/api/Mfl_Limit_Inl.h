


#ifndef MFL_LIMIT_INL_H
#define MFL_LIMIT_INL_H


/*
 **********************************************************************************************************************
 *
 * List of services
 *
 * Mfl_Prv_Limit_f32_Inl
 * Mfl_Prv_RateLimiter_f32_Inl
 *
 **********************************************************************************************************************
 */

/*
 **********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************
 */

LOCAL_INLINE float32 Mfl_Prv_Limit_f32_Inl(float32 val, float32 lowLim, float32 upLim);
LOCAL_INLINE float32 Mfl_Prv_RateLimiter_f32_Inl(float32 newval, float32 oldval, float32 maxdif);

/*
 **********************************************************************************************************************
 * Implementations
 **********************************************************************************************************************
 */

/**
 **********************************************************************************************************************
 * Mfl_Limit_f32
 *
 * \brief Limits an float32 input value to lower or upper limit
 *
 * Function returns input value if it is between lower an upper limit. Otherwhise the limit will be returned.
 *
 * Restrictions:
 * - upLim must be bigger than lowLim
 *
 * \param  float32 val      Input value
 * \param  float32 lowLim   Lower limit
 * \param  float32 upLim    Upper limit
 * \return float32          Range limited value
 **********************************************************************************************************************
 */
#ifndef MFL_PRV_LIMIT_F32_INL_REMAPPED
LOCAL_INLINE float32 Mfl_Prv_Limit_f32_Inl(float32 val, float32 lowLim, float32 upLim)
{
    return ((val <= lowLim) ? (lowLim) : ((val >= upLim) ? (upLim) : (val)));
}
#endif


/**
 **********************************************************************************************************************
 * Mfl_RateLimiter_f32
 *
 * \brief Limits the rate of increase/decrease of float32 input value by maxdif
 *
 * Function returns input value if its rate of increase/decrease is wihtin maxdif
 *          returns oldval + maxdif if its rate of increase exceeds maxdif
 *          returns oldval - maxdif if its rate of decrease exceeds maxdif
 *
 * Restrictions:
 * - maxdif should be absolute
 *
 * \param  float32 newval   Input value to be limited
 * \param  float32 oldval   previous value of newval
 * \param  float32 maxdif   maximum difference allowed between oldval and newval
 * \return float32          Ratelimited value
 **********************************************************************************************************************
 */
#ifndef MFL_PRV_RATELIMITER_F32_INL_REMAPPED
LOCAL_INLINE float32 Mfl_Prv_RateLimiter_f32_Inl(float32 newval, float32 oldval, float32 maxdif)
{
    float32 res_f32;
    float32 tmp_f32;

    /* Calculate the absolute value of newval - oldval
     if newval > oldval, then tmp_f32 will have newval - oldval
     if newval < oldval, then tmp_f32 will have oldval - newval    */
    tmp_f32 = Mfl_Prv_Abs_f32_Inl(newval - oldval);

    /* Check if the rate of increase/decrease of newval has exceeded maxdif */
    if (tmp_f32 > maxdif)
    {
        /* If rate of increase has exceeded maxdif, limit the newval to (oldval + maxdif)  */
        if (newval > oldval)
        {
            res_f32 = oldval + maxdif;
        }
        /* If rate of decrease has exceeded maxdif, limit the newval to (oldval - maxdif)  */
        else
        {
            res_f32 = oldval - maxdif;
        }
    }
    /* If the rate of increase/decrease is within maxdif return newval */
    else
    {
        res_f32 = newval;
    }

    return res_f32;
}
#endif




/* MFL_LIMIT_INL_H */
#endif
