#include "calterah_complex.h"

void cmult_cum(complex_t *in1, complex_t *in2, complex_t *dout)
{
        complex_t tmp;
        cmult(in1, in2, &tmp);
        cadd(&tmp, dout, dout);
}

void cmult_conj_cum(complex_t *in1, complex_t *in2, complex_t *dout)
{
        complex_t tmp;
        cmult_conj(in1, in2, &tmp);
        cadd(&tmp, dout, dout);
}

/* rlt = conj(v1)*v2 */
complex_t dot_product(complex_t *v1, complex_t *v2, int elem_num) {
        complex_t rlt;
        rlt.r = 0;
        rlt.i = 0;

        for (int elem_idx = 0; elem_idx < elem_num; elem_idx++) {
                /* it is v1 that needs to be conjugated. */
                cmult_conj_cum(v2 + elem_idx, v1 + elem_idx, &rlt);
        }
        return rlt;
}