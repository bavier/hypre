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





#ifndef HYPRE_AMG_DATA_HEADER
#define HYPRE_AMG_DATA_HEADER


/*--------------------------------------------------------------------------
 * hypre_AMGData
 *--------------------------------------------------------------------------*/

typedef struct
{
   /* setup params */
   HYPRE_Int      levmax;
   HYPRE_Int      ncg;
   double   ecg;
   HYPRE_Int      nwt;
   double   ewt;
   HYPRE_Int      nstr;

   /* solve params */
   HYPRE_Int      ncyc;
   HYPRE_Int      Fcycle_flag;
   HYPRE_Int      Vstar_flag;
   HYPRE_Int     *mu;
   HYPRE_Int     *ntrlx;
   HYPRE_Int     *iprlx;

   /* output params */
   HYPRE_Int      ioutdat;

   /* log file name */
   char     log_file_name[256];

   /* problem data */
   hypre_Matrix  *A;             
   HYPRE_Int      num_variables; 
   HYPRE_Int      num_unknowns;  
   HYPRE_Int      num_points;    
   HYPRE_Int     *iu;            
   HYPRE_Int     *ip;            
   HYPRE_Int     *iv;            

   /* data generated by the setup phase */
   HYPRE_Int      num_levels;
   HYPRE_Int      ndimu;
   HYPRE_Int      ndimp;
   HYPRE_Int      ndima;
   HYPRE_Int      ndimb;
   hypre_Matrix  *P;                 /* this is temporary */
   HYPRE_Int     *icdep;
   HYPRE_Int     *imin;
   HYPRE_Int     *imax;
   HYPRE_Int     *ipmn;
   HYPRE_Int     *ipmx;
   HYPRE_Int     *icg;
   HYPRE_Int     *ifg;
   hypre_Matrix **A_array;
   hypre_Matrix **P_array;
   hypre_VectorInt **IU_array;
   hypre_VectorInt **IP_array;
   hypre_VectorInt **IV_array;
   hypre_VectorInt **ICG_array;    
   HYPRE_Int     *leva;
   HYPRE_Int     *levb;
   HYPRE_Int     *levv;
   HYPRE_Int     *levp;
   HYPRE_Int     *levpi;
   HYPRE_Int     *levi;
   HYPRE_Int     *numa;
   HYPRE_Int     *numb;
   HYPRE_Int     *numv;
   HYPRE_Int     *nump;

   /* data generated in the solve phase */
   hypre_Vector   *Vtemp;
   double   *vtmp;
   HYPRE_Int       cycle_op_count;

} hypre_AMGData;

/*--------------------------------------------------------------------------
 * Accessor functions for the hypre_AMGData structure
 *--------------------------------------------------------------------------*/

/* setup params */
#define hypre_AMGDataLevMax(amg_data)       ((amg_data) -> levmax)
#define hypre_AMGDataNCG(amg_data)          ((amg_data) -> ncg)
#define hypre_AMGDataECG(amg_data)          ((amg_data) -> ecg)
#define hypre_AMGDataNWT(amg_data)          ((amg_data) -> nwt)
#define hypre_AMGDataEWT(amg_data)          ((amg_data) -> ewt)
#define hypre_AMGDataNSTR(amg_data)         ((amg_data) -> nstr)
		  		      
/* solve params */
#define hypre_AMGDataNCyc(amg_data)         ((amg_data) -> ncyc)
#define hypre_AMGDataFcycleFlag(amg_data)   ((amg_data) -> Fcycle_flag)
#define hypre_AMGDataVstarFlag(amg_data)    ((amg_data) -> Vstar_flag)
#define hypre_AMGDataMU(amg_data)           ((amg_data) -> mu)
#define hypre_AMGDataNTRLX(amg_data)        ((amg_data) -> ntrlx)
#define hypre_AMGDataIPRLX(amg_data)        ((amg_data) -> iprlx)
		  		      
/* output params */
#define hypre_AMGDataIOutDat(amg_data)      ((amg_data) -> ioutdat)
				      
/* log file name */
#define hypre_AMGDataLogFileName(amg_data)  ((amg_data) -> log_file_name)

/* problem data */
#define hypre_AMGDataA(amg_data)            ((amg_data) -> A)
#define hypre_AMGDataNumVariables(amg_data) ((amg_data) -> num_variables)
#define hypre_AMGDataNumUnknowns(amg_data)  ((amg_data) -> num_unknowns)
#define hypre_AMGDataNumPoints(amg_data)    ((amg_data) -> num_points)
#define hypre_AMGDataIU(amg_data)           ((amg_data) -> iu)
#define hypre_AMGDataIP(amg_data)           ((amg_data) -> ip)
#define hypre_AMGDataIV(amg_data)           ((amg_data) -> iv)
				      
/* data generated by the setup phase */
#define hypre_AMGDataP(amg_data)            ((amg_data) -> P)
#define hypre_AMGDataNumLevels(amg_data)    ((amg_data) -> num_levels)
#define hypre_AMGDataNDIMU(amg_data)        ((amg_data) -> ndimu)
#define hypre_AMGDataNDIMP(amg_data)        ((amg_data) -> ndimp)
#define hypre_AMGDataNDIMA(amg_data)        ((amg_data) -> ndima)
#define hypre_AMGDataNDIMB(amg_data)        ((amg_data) -> ndimb)
#define hypre_AMGDataICDep(amg_data)        ((amg_data) -> icdep)
#define hypre_AMGDataIMin(amg_data)         ((amg_data) -> imin)
#define hypre_AMGDataIMax(amg_data)         ((amg_data) -> imax)
#define hypre_AMGDataIPMN(amg_data)         ((amg_data) -> ipmn)
#define hypre_AMGDataIPMX(amg_data)         ((amg_data) -> ipmx)
#define hypre_AMGDataICG(amg_data)          ((amg_data) -> icg)
#define hypre_AMGDataIFG(amg_data)          ((amg_data) -> ifg)
#define hypre_AMGDataAArray(amg_data)       ((amg_data) -> A_array)
#define hypre_AMGDataPArray(amg_data)       ((amg_data) -> P_array)
#define hypre_AMGDataIUArray(amg_data)      ((amg_data) -> IU_array)
#define hypre_AMGDataIPArray(amg_data)      ((amg_data) -> IP_array)  
#define hypre_AMGDataIVArray(amg_data)      ((amg_data) -> IV_array)  
#define hypre_AMGDataICGArray(amg_data)     ((amg_data) -> ICG_array)  
#define hypre_AMGDataLevA(amg_data)         ((amg_data) -> leva)
#define hypre_AMGDataLevB(amg_data)         ((amg_data) -> levb)
#define hypre_AMGDataLevV(amg_data)         ((amg_data) -> levv)
#define hypre_AMGDataLevP(amg_data)         ((amg_data) -> levp)
#define hypre_AMGDataLevPI(amg_data)        ((amg_data) -> levpi)
#define hypre_AMGDataLevI(amg_data)         ((amg_data) -> levi)
#define hypre_AMGDataNumA(amg_data)         ((amg_data) -> numa)
#define hypre_AMGDataNumB(amg_data)         ((amg_data) -> numb)
#define hypre_AMGDataNumV(amg_data)         ((amg_data) -> numv)
#define hypre_AMGDataNumP(amg_data)         ((amg_data) -> nump)
				      
/* data generated in the solve phase */
#define hypre_AMGDataVecTemp(amg_data)      ((amg_data) -> vtmp)
#define hypre_AMGDataVtemp(amg_data)        ((amg_data) -> Vtemp)
#define hypre_AMGDataCycleOpCount(amg_data) ((amg_data) -> cycle_op_count)  

#endif
