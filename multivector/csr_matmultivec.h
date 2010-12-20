/*BHEADER**********************************************************************
 * Copyright (c) 2008,  Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 * This file is part of HYPRE.  See file COPYRIGHT for details.
 *
 * HYPRE is free software; you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License (as published by the Free
 * Software Foundation) version 2.1 dated February 1999.
 *
 * $Revision$
 ***********************************************************************EHEADER*/




#ifndef CSR_MULTIMATVEC_H
#define CSR_MULTIMATVEC_H

#include "seq_mv.h"
#include "seq_multivector.h"

#ifdef __cplusplus
extern "C" {
#endif
/*--------------------------------------------------------------------------
 * hypre_CSRMatrixMatMultivec
 *--------------------------------------------------------------------------*/
HYPRE_Int
hypre_CSRMatrixMatMultivec(double alpha, hypre_CSRMatrix *A,
                           hypre_Multivector *x, double beta,
                           hypre_Multivector *y);
                            

/*--------------------------------------------------------------------------
 * hypre_CSRMatrixMultiMatvecT
 *
 *   Performs y <- alpha * A^T * x + beta * y
 *
 *   From Van Henson's modification of hypre_CSRMatrixMatvec.
 *--------------------------------------------------------------------------*/

HYPRE_Int
hypre_CSRMatrixMatMultivecT(double alpha, hypre_CSRMatrix *A,
                            hypre_Multivector *x, double beta,
                            hypre_Multivector *y);
                             
#ifdef __cplusplus
}
#endif

#endif /* CSR_MATMULTIVEC_H */
