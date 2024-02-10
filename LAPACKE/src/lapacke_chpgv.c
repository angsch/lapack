/*****************************************************************************
  Copyright (c) 2014, Intel Corp.
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Intel Corporation nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
  THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************
* Contents: Native high-level C interface to LAPACK function chpgv
* Author: Intel Corporation
*****************************************************************************/

#include "lapacke_utils.h"

lapack_int API_SUFFIX(LAPACKE_chpgv)( int matrix_layout, lapack_int itype, char jobz,
                          char uplo, lapack_int n, lapack_complex_float* ap,
                          lapack_complex_float* bp, float* w,
                          lapack_complex_float* z, lapack_int ldz )
{
    lapack_int info = 0;
    float* rwork = NULL;
    lapack_complex_float* work = NULL;
    if( matrix_layout != LAPACK_COL_MAJOR && matrix_layout != LAPACK_ROW_MAJOR ) {
        API_SUFFIX(LAPACKE_xerbla)( "LAPACKE_chpgv", -1 );
        return -1;
    }
#ifndef LAPACK_DISABLE_NAN_CHECK
    if( LAPACKE_get_nancheck() ) {
        /* Optionally check input matrices for NaNs */
        if( API_SUFFIX(LAPACKE_chp_nancheck)( n, ap ) ) {
            return -6;
        }
        if( API_SUFFIX(LAPACKE_chp_nancheck)( n, bp ) ) {
            return -7;
        }
    }
#endif
    /* Allocate memory for working array(s) */
    rwork = (float*)LAPACKE_malloc( sizeof(float) * MAX(1,3*n-2) );
    if( rwork == NULL ) {
        info = LAPACK_WORK_MEMORY_ERROR;
        goto exit_level_0;
    }
    work = (lapack_complex_float*)
        LAPACKE_malloc( sizeof(lapack_complex_float) * MAX(1,2*n-1) );
    if( work == NULL ) {
        info = LAPACK_WORK_MEMORY_ERROR;
        goto exit_level_1;
    }
    /* Call middle-level interface */
    info = API_SUFFIX(LAPACKE_chpgv_work)( matrix_layout, itype, jobz, uplo, n, ap, bp, w, z,
                               ldz, work, rwork );
    /* Release memory and exit */
    LAPACKE_free( work );
exit_level_1:
    LAPACKE_free( rwork );
exit_level_0:
    if( info == LAPACK_WORK_MEMORY_ERROR ) {
        API_SUFFIX(LAPACKE_xerbla)( "LAPACKE_chpgv", info );
    }
    return info;
}
