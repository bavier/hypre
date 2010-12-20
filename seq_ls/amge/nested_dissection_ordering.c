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






#include "headers.h"  

/*****************************************************************************
 *
 * builds nested dissection ordering based on "node_level"
 *
 * i_node_level[i] = # faces
 * the node "i" belongs to;
 *
 * at all levels created throughout the AMGeMatrixTopology process; 
 *
 * if i_node_level = i_node_HB_level as input, the output
 *
 *           "level_node" 
 *
 *    is the HB node ordering;
 *
 ****************************************************************************/

HYPRE_Int AMGeNestedDissectionOrdering(HYPRE_Int *i_node_level,
				 HYPRE_Int num_nodes, 
				 HYPRE_Int level, 

				 HYPRE_Int **j_node_level_pointer,

				 HYPRE_Int **i_level_node_pointer, 
				 HYPRE_Int **j_level_node_pointer, 

				 HYPRE_Int *num_levels_pointer)
{
  HYPRE_Int ierr = 0;
  HYPRE_Int i,j,k,l;  

  HYPRE_Int *j_node_level, *i_level_node, *j_level_node;

  HYPRE_Int min_level, max_level;

  j_node_level = hypre_CTAlloc(HYPRE_Int, num_nodes);

  max_level = 0;
  min_level = level;
  for (i=0; i < num_nodes; i++)
    {
      if (max_level < i_node_level[i]) 
	max_level = i_node_level[i];

      if (min_level > i_node_level[i]) 
	min_level = i_node_level[i];

    }

  hypre_printf("level: %d, max_level: %d, min_level: %d\n", level, max_level,
	 min_level);

  for (i=0; i < num_nodes; i++)
    {
      j_node_level[i] = i_node_level[i]-min_level;
      i_node_level[i] = i;
    }

  i_node_level[num_nodes] = num_nodes;


  ierr = transpose_matrix_create(&i_level_node, &j_level_node,

				 i_node_level, j_node_level,
				 num_nodes, max_level-min_level+1);


  *num_levels_pointer = max_level-min_level+1;
  *i_level_node_pointer = i_level_node;
  *j_level_node_pointer = j_level_node;

  *j_node_level_pointer = j_node_level; 


  hypre_printf("\n==============================================================\n");
  hypre_printf("\n     n e s t e d   d i s s e c t i o n   o r d e r i n g:     \n");
  hypre_printf("\n==============================================================\n");


  for (l=0; l < max_level-min_level+1; l++)
    {
      hypre_printf("level: %d contains %d nodes: \n", l, 
	     i_level_node[l+1]-i_level_node[l]);
      /*
      for (k=i_level_node[l]; k < i_level_node[l+1]; k++)
	hypre_printf(" %d, ", j_level_node[k]);
	*/
      hypre_printf("\n\n");
    }
  hypre_printf("\n==============================================================\n");
  hypre_printf("num_nodes %d and num_nodes counted: %d\n\n\n",
	 num_nodes, i_level_node[max_level-min_level+1]);


  return ierr;

}
