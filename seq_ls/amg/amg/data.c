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





/******************************************************************************
 *
 * Member functions for hypre_AMGData structure
 *
 *****************************************************************************/

#include "headers.h"


/*--------------------------------------------------------------------------
 * hypre_AMGNewData
 *--------------------------------------------------------------------------*/

hypre_AMGData   *hypre_AMGNewData(levmax, ncg, ecg, nwt, ewt, nstr,
		       ncyc, mu, ntrlx, iprlx,
		       ioutdat, cycle_op_count,
		       log_file_name)
HYPRE_Int     levmax;
HYPRE_Int     ncg;
double  ecg;
HYPRE_Int     nwt;
double  ewt;
HYPRE_Int     nstr;
HYPRE_Int     ncyc;
HYPRE_Int    *mu;
HYPRE_Int    *ntrlx;
HYPRE_Int    *iprlx;
HYPRE_Int     ioutdat;
HYPRE_Int     cycle_op_count;
char   *log_file_name;
{
   hypre_AMGData  *amg_data;

   amg_data = hypre_CTAlloc(hypre_AMGData, 1);

   hypre_AMGDataLevMax(amg_data)  = levmax;
   hypre_AMGDataNCG(amg_data)     = ncg;
   hypre_AMGDataECG(amg_data)     = ecg;
   hypre_AMGDataNWT(amg_data)     = nwt;
   hypre_AMGDataEWT(amg_data)     = ewt;
   hypre_AMGDataNSTR(amg_data)    = nstr;
   				    
   hypre_AMGDataNCyc(amg_data)    = ncyc;
   hypre_AMGDataMU(amg_data)      = mu;
   hypre_AMGDataNTRLX(amg_data)   = ntrlx;
   hypre_AMGDataIPRLX(amg_data)   = iprlx;
   				    
   hypre_AMGDataIOutDat(amg_data) = ioutdat;
   hypre_AMGDataCycleOpCount(amg_data) = cycle_op_count;

   hypre_sprintf(hypre_AMGDataLogFileName(amg_data), "%s", log_file_name); 

   return amg_data;
}

/*--------------------------------------------------------------------------
 * hypre_AMGFreeData
 *--------------------------------------------------------------------------*/

void      hypre_AMGFreeData(amg_data)
hypre_AMGData  *amg_data;
{
   if (amg_data)
   {
      /* solve params */
      hypre_TFree(hypre_AMGDataMU(amg_data));
      hypre_TFree(hypre_AMGDataNTRLX(amg_data));
      hypre_TFree(hypre_AMGDataIPRLX(amg_data));

      /* problem data */
      /* data generated by the setup phase */
      hypre_TFree(hypre_AMGDataICDep(amg_data));
      hypre_TFree(hypre_AMGDataIMin(amg_data));
      hypre_TFree(hypre_AMGDataIMax(amg_data));
      hypre_TFree(hypre_AMGDataIPMN(amg_data));
      hypre_TFree(hypre_AMGDataIPMX(amg_data));
      hypre_TFree(hypre_AMGDataICG(amg_data));
      hypre_TFree(hypre_AMGDataIFG(amg_data));
      hypre_TFree(hypre_AMGDataVecTemp(amg_data));
      hypre_TFree(hypre_AMGDataICGArray(amg_data));
      hypre_TFree(hypre_AMGDataLevA(amg_data));
      hypre_TFree(hypre_AMGDataLevB(amg_data));
      hypre_TFree(hypre_AMGDataLevV(amg_data));
      hypre_TFree(hypre_AMGDataLevP(amg_data));   
      hypre_TFree(hypre_AMGDataLevPI(amg_data));   
      hypre_TFree(hypre_AMGDataLevI(amg_data));
      hypre_TFree(hypre_AMGDataNumA(amg_data));
      hypre_TFree(hypre_AMGDataNumB(amg_data));
      hypre_TFree(hypre_AMGDataNumV(amg_data));
      hypre_TFree(hypre_AMGDataNumP(amg_data));

      hypre_FreeVector(hypre_AMGDataVtemp(amg_data));

      hypre_TFree(hypre_AMGDataPArray(amg_data));
      hypre_TFree(hypre_AMGDataAArray(amg_data));

      hypre_TFree(amg_data);
   }
}

