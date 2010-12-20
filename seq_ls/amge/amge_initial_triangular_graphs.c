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




/*****************************************************************************
 * reads from file and creates graphs:
 * element_node, element_face, face_element, boundarysurface_face, and
 * face_node;
 ****************************************************************************/

#include "headers.h"  


HYPRE_Int hypre_AMGeTriangInitialGraphs(HYPRE_Int **i_element_node_pointer,
				  HYPRE_Int **j_element_node_pointer,




				  HYPRE_Int *num_elements_pointer,
				  HYPRE_Int *num_nodes_pointer,

				  HYPRE_Int **i_node_on_boundary_pointer,


				  char *element_node_file,
				  char *node_on_boundary_file)

{
  HYPRE_Int ierr = 0;
  FILE *f;

  HYPRE_Int i,j,k,l;
  HYPRE_Int num_elements, num_faces;
  HYPRE_Int *i_element_face, *j_element_face;
  HYPRE_Int *i_face_face, *j_face_face;


  HYPRE_Int num_boundarynodes;
  HYPRE_Int *i_node_on_boundary;

  HYPRE_Int num_nodes;
  HYPRE_Int *i_element_node, *j_element_node;

  HYPRE_Int element_node_counter;

  double eps = 1.e-4;

  f = fopen(element_node_file, "r");

  hypre_fscanf(f, "%d %d", &num_elements, &num_nodes);


  i_element_node = hypre_CTAlloc(HYPRE_Int, num_elements+1);

  j_element_node = hypre_CTAlloc(HYPRE_Int, 3*num_elements); 

  element_node_counter = 0;
  for (i=0; i < num_elements; i++)
    {
      i_element_node[i] = element_node_counter;
      hypre_fscanf(f, "%d %d %d\n",
	     &j_element_node[element_node_counter],
	     &j_element_node[element_node_counter+1],
	     &j_element_node[element_node_counter+2]);
      element_node_counter+=3; 
    }

  i_element_node[num_elements] = element_node_counter;

  fclose(f); 


  i_node_on_boundary = hypre_CTAlloc(HYPRE_Int, num_nodes);
  for (i=0; i<num_nodes; i++)
    i_node_on_boundary[i] = -1;

  f = fopen(node_on_boundary_file, "r");

  hypre_fscanf(f, "%d ", &num_boundarynodes);

  for (i=0; i < num_boundarynodes; i++)
    {
      hypre_fscanf(f, "%d \n", &j);
      i_node_on_boundary[j] = 0;
    }

  fclose(f);

  /* ================================================================*/
  /*                                                                 */
  /* build element topology:                                         */
  /* ================================================================*/

  *i_node_on_boundary_pointer = i_node_on_boundary;

  *i_element_node_pointer = i_element_node;
  *j_element_node_pointer = j_element_node;


  *num_elements_pointer = num_elements;
  *num_nodes_pointer = num_nodes;


  return ierr;


}
