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
* Contents: Native middle-level C interface to LAPACK function sstevx
* Author: Intel Corporation
*****************************************************************************/

#include "lapacke_utils.h"

lapack_int API_SUFFIX(LAPACKE_sstevx_work)( int matrix_layout, char jobz, char range,
                                lapack_int n, float* d, float* e, float vl,
                                float vu, lapack_int il, lapack_int iu,
                                float abstol, lapack_int* m, float* w, float* z,
                                lapack_int ldz, float* work, lapack_int* iwork,
                                lapack_int* ifail )
{
    lapack_int info = 0;
    if( matrix_layout == LAPACK_COL_MAJOR ) {
        /* Call LAPACK function and adjust info */
        LAPACK_sstevx( &jobz, &range, &n, d, e, &vl, &vu, &il, &iu, &abstol, m,
                       w, z, &ldz, work, iwork, ifail, &info );
        if( info < 0 ) {
            info = info - 1;
        }
    } else if( matrix_layout == LAPACK_ROW_MAJOR ) {
        lapack_int ncols_z = ( API_SUFFIX(LAPACKE_lsame)( range, 'a' ) ||
                             API_SUFFIX(LAPACKE_lsame)( range, 'v' ) ) ? n :
                             ( API_SUFFIX(LAPACKE_lsame)( range, 'i' ) ? (iu-il+1) : 1);
        lapack_int ldz_t = MAX(1,n);
        float* z_t = NULL;
        /* Check leading dimension(s) */
        if( ldz < ncols_z ) {
            info = -15;
            API_SUFFIX(LAPACKE_xerbla)( "LAPACKE_sstevx_work", info );
            return info;
        }
        /* Allocate memory for temporary array(s) */
        if( API_SUFFIX(LAPACKE_lsame)( jobz, 'v' ) ) {
            z_t = (float*)
                LAPACKE_malloc( sizeof(float) * ldz_t * MAX(1,ncols_z) );
            if( z_t == NULL ) {
                info = LAPACK_TRANSPOSE_MEMORY_ERROR;
                goto exit_level_0;
            }
        }
        /* Call LAPACK function and adjust info */
        LAPACK_sstevx( &jobz, &range, &n, d, e, &vl, &vu, &il, &iu, &abstol, m,
                       w, z_t, &ldz_t, work, iwork, ifail, &info );
        if( info < 0 ) {
            info = info - 1;
        }
        /* Transpose output matrices */
        if( API_SUFFIX(LAPACKE_lsame)( jobz, 'v' ) ) {
            API_SUFFIX(LAPACKE_sge_trans)( LAPACK_COL_MAJOR, n, ncols_z, z_t, ldz_t, z,
                               ldz );
        }
        /* Release memory and exit */
        if( API_SUFFIX(LAPACKE_lsame)( jobz, 'v' ) ) {
            LAPACKE_free( z_t );
        }
exit_level_0:
        if( info == LAPACK_TRANSPOSE_MEMORY_ERROR ) {
            API_SUFFIX(LAPACKE_xerbla)( "LAPACKE_sstevx_work", info );
        }
    } else {
        info = -1;
        API_SUFFIX(LAPACKE_xerbla)( "LAPACKE_sstevx_work", info );
    }
    return info;
}
