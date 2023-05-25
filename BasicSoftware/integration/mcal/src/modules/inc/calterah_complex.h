#ifndef CALTERAH_COMPLEX_H
#define CALTERAH_COMPLEX_H
#include "math.h"

typedef struct complex {
        float r;
        float i;
} complex_t;

static inline void cadd(complex_t *in1, complex_t *in2, complex_t *dout)
{
        dout->r = in1->r + in2->r;
        dout->i = in1->i + in2->i;
}

static inline void csub(complex_t *in1, complex_t *in2, complex_t *dout)
{
        dout->r = in1->r - in2->r;
        dout->i = in1->i - in2->i;
}

static complex_t cconj(complex_t *in1)
{
        complex_t dout;
        dout.r = in1->r;
        dout.i = -1 * in1->i;
        return dout;
}

static inline void cmult(complex_t *in1, complex_t *in2, complex_t *dout)
{
        /* using a automatic variable tmp is to prevent the situation where dout is in1 or in2 */
        complex_t tmp;
        tmp.r = in1->r * in2->r - in1->i * in2->i;
        tmp.i = in1->r * in2->i + in1->i * in2->r;
        dout->r = tmp.r;
        dout->i = tmp.i;
}

static inline void cmult_conj(complex_t *in1, complex_t *in2, complex_t *dout)
{
        /* using a automatic variable tmp is to prevent the situation where dout is in1 or in2 */
        complex_t tmp;
        tmp.r = in1->r * in2->r + in1->i * in2->i;
        tmp.i = in1->r * (-in2->i) + in1->i * in2->r; //in1*conj(in2)
        dout->r = tmp.r;
        dout->i = tmp.i;
}

static inline void crmult(complex_t *in1, float in2, complex_t *dout)
{
        dout->r = in1->r * in2;
        dout->i = in1->i * in2;
}

static inline float mag_sqr(complex_t *in)
{
        return in->r * in->r + in->i * in->i;
}


void cmult_cum(complex_t *in1, complex_t *in2, complex_t *dout);
void cmult_conj_cum(complex_t *in1, complex_t *in2, complex_t *dout);
complex_t dot_product(complex_t *v1, complex_t *v2, int elem_num);

static inline complex_t expj(float theta)
{
        complex_t ret;
        ret.r = cosf(theta);
        ret.i = sinf(theta);
        return ret;
}

#endif
