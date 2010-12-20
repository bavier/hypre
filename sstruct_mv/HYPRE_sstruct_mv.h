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


#ifndef HYPRE_SSTRUCT_MV_HEADER
#define HYPRE_SSTRUCT_MV_HEADER

#include "HYPRE_utilities.h"
#include "HYPRE.h"
#include "HYPRE_struct_mv.h"
#include "HYPRE_IJ_mv.h"

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------------------------------------------
 *--------------------------------------------------------------------------*/

/**
 * @name SStruct System Interface
 *
 * This interface represents a semi-structured-grid conceptual view of a linear
 * system.
 *
 * @memo A semi-structured-grid conceptual interface
 **/
/*@{*/

/*--------------------------------------------------------------------------
 *--------------------------------------------------------------------------*/

/**
 * @name SStruct Grids
 **/
/*@{*/

struct hypre_SStructGrid_struct;
/**
 * A grid object is constructed out of several structured ``parts'' and an
 * optional unstructured ``part''.  Each structured part has its own abstract
 * index space.
 **/
typedef struct hypre_SStructGrid_struct *HYPRE_SStructGrid;

/**
 * An enumerated type that supports cell centered, node centered, face centered,
 * and edge centered variables.  Face centered variables are split into x-face,
 * y-face, and z-face variables, and edge centered variables are split into
 * x-edge, y-edge, and z-edge variables.  The edge centered variable types are
 * only used in 3D.  In 2D, edge centered variables are handled by the face
 * centered types.
 *
 * Variables are referenced relative to an abstract (cell centered) index in the
 * following way:
 * \begin{itemize}
 * \item cell centered variables are aligned with the index;
 * \item node centered variables are aligned with the cell corner
 *       at relative index (1/2, 1/2, 1/2);
 * \item x-face, y-face, and z-face centered variables are aligned
 *       with the faces at relative indexes (1/2, 0, 0), (0, 1/2, 0),
 *       and (0, 0, 1/2), respectively;
 * \item x-edge, y-edge, and z-edge centered variables are aligned
 *       with the edges at relative indexes (0, 1/2, 1/2), (1/2, 0, 1/2),
 *       and (1/2, 1/2, 0), respectively.
 * \end{itemize}
 *
 * The supported identifiers are:
 * \begin{itemize}
 * \item {\tt HYPRE\_SSTRUCT\_VARIABLE\_CELL}
 * \item {\tt HYPRE\_SSTRUCT\_VARIABLE\_NODE}
 * \item {\tt HYPRE\_SSTRUCT\_VARIABLE\_XFACE}
 * \item {\tt HYPRE\_SSTRUCT\_VARIABLE\_YFACE}
 * \item {\tt HYPRE\_SSTRUCT\_VARIABLE\_ZFACE}
 * \item {\tt HYPRE\_SSTRUCT\_VARIABLE\_XEDGE}
 * \item {\tt HYPRE\_SSTRUCT\_VARIABLE\_YEDGE}
 * \item {\tt HYPRE\_SSTRUCT\_VARIABLE\_ZEDGE}
 * \end{itemize}
 *
 * NOTE: Although variables are referenced relative to a unique abstract
 * cell-centered index, some variables are associated with multiple grid cells.
 * For example, node centered variables in 3D are associated with 8 cells (away
 * from boundaries).  Although grid cells are distributed uniquely to different
 * processes, variables may be owned by multiple processes because they may be
 * associated with multiple cells.
 **/
typedef HYPRE_Int HYPRE_SStructVariable;

#define HYPRE_SSTRUCT_VARIABLE_UNDEFINED -1
#define HYPRE_SSTRUCT_VARIABLE_CELL       0
#define HYPRE_SSTRUCT_VARIABLE_NODE       1
#define HYPRE_SSTRUCT_VARIABLE_XFACE      2
#define HYPRE_SSTRUCT_VARIABLE_YFACE      3
#define HYPRE_SSTRUCT_VARIABLE_ZFACE      4
#define HYPRE_SSTRUCT_VARIABLE_XEDGE      5
#define HYPRE_SSTRUCT_VARIABLE_YEDGE      6
#define HYPRE_SSTRUCT_VARIABLE_ZEDGE      7

/**
 * Create an {\tt ndim}-dimensional grid object with {\tt nparts} structured
 * parts.
 **/
HYPRE_Int
HYPRE_SStructGridCreate(MPI_Comm           comm,
                        HYPRE_Int          ndim,
                        HYPRE_Int          nparts,
                        HYPRE_SStructGrid *grid);

/**
 * Destroy a grid object.  An object should be explicitly destroyed using this
 * destructor when the user's code no longer needs direct access to it.  Once
 * destroyed, the object must not be referenced again.  Note that the object may
 * not be deallocated at the completion of this call, since there may be
 * internal package references to the object.  The object will then be destroyed
 * when all internal reference counts go to zero.
 **/
HYPRE_Int
HYPRE_SStructGridDestroy(HYPRE_SStructGrid grid);

/**
 * Set the extents for a box on a structured part of the grid.
 **/
HYPRE_Int
HYPRE_SStructGridSetExtents(HYPRE_SStructGrid  grid,
                            HYPRE_Int          part,
                            HYPRE_Int         *ilower,
                            HYPRE_Int         *iupper);

/**
 * Describe the variables that live on a structured part of the grid.
 **/
HYPRE_Int
HYPRE_SStructGridSetVariables(HYPRE_SStructGrid      grid,
                              HYPRE_Int              part,
                              HYPRE_Int              nvars,
                              HYPRE_SStructVariable *vartypes);

/**
 * Describe additional variables that live at a particular index.  These
 * variables are appended to the array of variables set in
 * \Ref{HYPRE_SStructGridSetVariables}, and are referenced as such.
 *
 * NOTE: This routine is not yet supported.
 **/
HYPRE_Int
HYPRE_SStructGridAddVariables(HYPRE_SStructGrid      grid,
                              HYPRE_Int              part,
                              HYPRE_Int             *index,
                              HYPRE_Int              nvars,
                              HYPRE_SStructVariable *vartypes);

/**
 * Set the ordering of variables in a finite element problem.  This overrides
 * the default ordering described below.
 *
 * Array {\tt ordering} is composed of blocks of size (1 + {\tt ndim}).  Each
 * block indicates a specific variable in the element and the ordering of the
 * blocks defines the ordering of the variables.  A block contains a variable
 * number followed by an offset direction relative to the element's center.  For
 * example, a block containing (2, 1, -1, 0) means variable 2 on the edge
 * located in the (1, -1, 0) direction from the center of the element.  Note
 * that here variable 2 must be of type {\tt ZEDGE} for this to make sense.  The
 * {\tt ordering} array must account for all variables in the element.  This
 * routine can only be called after \Ref{HYPRE_SStructGridSetVariables}.
 *
 * The default ordering for element variables (var, i, j, k) varies fastest in
 * index i, followed by j, then k, then var.  For example, if var 0, var 1, and
 * var 2 are declared to be XFACE, YFACE, and NODE variables, respectively, then
 * the default ordering (in 2D) would first list the two XFACE variables, then
 * the two YFACE variables, then the four NODE variables as follows:
 *
 * (0,-1,0), (0,1,0), (1,0,-1), (1,0,1), (2,-1,-1), (2,1,-1), (2,-1,1), (2,1,1)
 **/
HYPRE_Int
HYPRE_SStructGridSetFEMOrdering(HYPRE_SStructGrid  grid,
                                HYPRE_Int          part,
                                HYPRE_Int         *ordering);

/**
 * Describe how regions just outside of a part relate to other parts.  This is
 * done a box at a time.
 *
 * Parts {\tt part} and {\tt nbor\_part} must be different, except in the case
 * where only cell-centered data is used.
 *
 * Indexes should increase from {\tt ilower} to {\tt iupper}.  It is not
 * necessary that indexes increase from {\tt nbor\_ilower} to {\tt
 * nbor\_iupper}.
 * 
 * The {\tt index\_map} describes the mapping of indexes 0, 1, and 2 on part
 * {\tt part} to the corresponding indexes on part {\tt nbor\_part}.  For
 * example, triple (1, 2, 0) means that indexes 0, 1, and 2 on part {\tt part}
 * map to indexes 1, 2, and 0 on part {\tt nbor\_part}, respectively.
 *
 * The {\tt index\_dir} describes the direction of the mapping in {\tt
 * index\_map}.  For example, triple (1, 1, -1) means that for indexes 0 and 1,
 * increasing values map to increasing values on {\tt nbor\_part}, while for
 * index 2, decreasing values map to increasing values.
 *
 * NOTE: All parts related to each other via this routine must have an identical
 * list of variables and variable types.  For example, if part 0 has only two
 * variables on it, a cell centered variable and a node centered variable, and
 * we declare part 1 to be a neighbor of part 0, then part 1 must also have only
 * two variables on it, and they must be of type cell and node.  In addition,
 * variables associated with FACEs or EDGEs must be grouped together and listed
 * in X, Y, Z order.  This is to enable the code to correctly associate
 * variables on one part with variables on its neighbor part when a coordinate
 * transformation is specified.  For example, an XFACE variable on one part may
 * correspond to a YFACE variable on a neighbor part under a particular
 * tranformation, and the code determines this association by assuming that the
 * variable lists are as noted here.
 **/
HYPRE_Int
HYPRE_SStructGridSetNeighborPart(HYPRE_SStructGrid  grid,
                                 HYPRE_Int          part,
                                 HYPRE_Int         *ilower,
                                 HYPRE_Int         *iupper,
                                 HYPRE_Int          nbor_part,
                                 HYPRE_Int         *nbor_ilower,
                                 HYPRE_Int         *nbor_iupper,
                                 HYPRE_Int         *index_map,
                                 HYPRE_Int         *index_dir);

/**
 * Describe how regions inside a part are shared with regions in other parts.
 *
 * Parts {\tt part} and {\tt shared\_part} must be different.
 *
 * Indexes should increase from {\tt ilower} to {\tt iupper}.  It is not
 * necessary that indexes increase from {\tt shared\_ilower} to {\tt
 * shared\_iupper}.  This is to maintain consistency with the {\tt
 * SetNeighborPart} function, which is also able to describe shared regions but
 * in a more limited fashion.
 *
 * The {\tt offset} is a triple (in 3D) used to indicate the dimensionality of
 * the shared set of data and its position with respect to the box extents {\tt
 * ilower} and {\tt iupper} on part {\tt part}.  The dimensionality is given by
 * the number of 0's in the triple, and the position is given by plus or minus
 * 1's.  For example: (0, 0, 0) indicates sharing of all data in the given box;
 * (1, 0, 0) indicates sharing of data on the faces in the (1, 0, 0) direction;
 * (1, 0, -1) indicates sharing of data on the edges in the (1, 0, -1)
 * direction; and (1, -1, 1) indicates sharing of data on the nodes in the (1,
 * -1, 1) direction.  To ensure the dimensionality, it is required that for
 * every nonzero entry, the corresponding extents of the box are the same.  For
 * example, if {\tt offset} is (0, 1, 0), then (2, 1, 3) and (10, 1, 15) are
 * valid box extents, whereas (2, 1, 3) and (10, 7, 15) are invalid (because 1
 * and 7 are not the same).
 *
 * The {\tt shared\_offset} is used in the same way as {\tt offset}, but with
 * respect to the box extents {\tt shared\_ilower} and {\tt shared\_iupper} on
 * part {\tt shared\_part}.
 * 
 * The {\tt index\_map} describes the mapping of indexes 0, 1, and 2 on part
 * {\tt part} to the corresponding indexes on part {\tt shared\_part}.  For
 * example, triple (1, 2, 0) means that indexes 0, 1, and 2 on part {\tt part}
 * map to indexes 1, 2, and 0 on part {\tt shared\_part}, respectively.
 *
 * The {\tt index\_dir} describes the direction of the mapping in {\tt
 * index\_map}.  For example, triple (1, 1, -1) means that for indexes 0 and 1,
 * increasing values map to increasing values on {\tt shared\_part}, while for
 * index 2, decreasing values map to increasing values.
 *
 * NOTE: All parts related to each other via this routine must have an identical
 * list of variables and variable types.  For example, if part 0 has only two
 * variables on it, a cell centered variable and a node centered variable, and
 * we declare part 1 to have shared regions with part 0, then part 1 must also
 * have only two variables on it, and they must be of type cell and node.  In
 * addition, variables associated with FACEs or EDGEs must be grouped together
 * and listed in X, Y, Z order.  This is to enable the code to correctly
 * associate variables on one part with variables on a shared part when a
 * coordinate transformation is specified.  For example, an XFACE variable on
 * one part may correspond to a YFACE variable on a shared part under a
 * particular tranformation, and the code determines this association by
 * assuming that the variable lists are as noted here.
 **/
HYPRE_Int
HYPRE_SStructGridSetSharedPart(HYPRE_SStructGrid  grid,
                               HYPRE_Int          part,
                               HYPRE_Int         *ilower,
                               HYPRE_Int         *iupper,
                               HYPRE_Int         *offset,
                               HYPRE_Int          shared_part,
                               HYPRE_Int         *shared_ilower,
                               HYPRE_Int         *shared_iupper,
                               HYPRE_Int         *shared_offset,
                               HYPRE_Int         *index_map,
                               HYPRE_Int         *index_dir);

/**
 * Add an unstructured part to the grid.  The variables in the unstructured part
 * of the grid are referenced by a global rank between 0 and the total number of
 * unstructured variables minus one.  Each process owns some unique consecutive
 * range of variables, defined by {\tt ilower} and {\tt iupper}.
 *
 * NOTE: This is just a placeholder.  This part of the interface is not finished.
 **/
HYPRE_Int
HYPRE_SStructGridAddUnstructuredPart(HYPRE_SStructGrid grid,
                                     HYPRE_Int         ilower,
                                     HYPRE_Int         iupper);

/**
 * Finalize the construction of the grid before using.
 **/
HYPRE_Int
HYPRE_SStructGridAssemble(HYPRE_SStructGrid grid);

/**
 * Set the periodicity a particular part.
 *
 * The argument {\tt periodic} is an {\tt ndim}-dimensional integer array that
 * contains the periodicity for each dimension.  A zero value for a dimension
 * means non-periodic, while a nonzero value means periodic and contains the
 * actual period.  For example, periodicity in the first and third dimensions
 * for a 10x11x12 part is indicated by the array [10,0,12].
 *
 * NOTE: Some of the solvers in hypre have power-of-two restrictions on the size
 * of the periodic dimensions.
 **/
HYPRE_Int
HYPRE_SStructGridSetPeriodic(HYPRE_SStructGrid  grid,
                             HYPRE_Int          part,
                             HYPRE_Int         *periodic);
/**
 * Setting ghost in the sgrids.
 **/
HYPRE_Int
HYPRE_SStructGridSetNumGhost(HYPRE_SStructGrid  grid,
                             HYPRE_Int         *num_ghost);

/*@}*/

/*--------------------------------------------------------------------------
 *--------------------------------------------------------------------------*/

/**
 * @name SStruct Stencils
 **/
/*@{*/

struct hypre_SStructStencil_struct;
/**
 * The stencil object.
 **/
typedef struct hypre_SStructStencil_struct *HYPRE_SStructStencil;

/**
 * Create a stencil object for the specified number of spatial dimensions and
 * stencil entries.
 **/
HYPRE_Int
HYPRE_SStructStencilCreate(HYPRE_Int             ndim,
                           HYPRE_Int             size,
                           HYPRE_SStructStencil *stencil);

/**
 * Destroy a stencil object.
 **/
HYPRE_Int
HYPRE_SStructStencilDestroy(HYPRE_SStructStencil stencil);

/**
 * Set a stencil entry.
 **/
HYPRE_Int
HYPRE_SStructStencilSetEntry(HYPRE_SStructStencil  stencil,
                             HYPRE_Int             entry,
                             HYPRE_Int            *offset,
                             HYPRE_Int             var);

/*@}*/

/*--------------------------------------------------------------------------
 *--------------------------------------------------------------------------*/

/**
 * @name SStruct Graphs
 **/
/*@{*/

struct hypre_SStructGraph_struct;
/**
 * The graph object is used to describe the nonzero structure of a matrix.
 **/
typedef struct hypre_SStructGraph_struct *HYPRE_SStructGraph;

/**
 * Create a graph object.
 **/
HYPRE_Int
HYPRE_SStructGraphCreate(MPI_Comm             comm,
                         HYPRE_SStructGrid    grid,
                         HYPRE_SStructGraph  *graph);

/**
 * Destroy a graph object.
 **/
HYPRE_Int
HYPRE_SStructGraphDestroy(HYPRE_SStructGraph graph);

/**
 * Set the domain grid.
 **/
HYPRE_Int
HYPRE_SStructGraphSetDomainGrid(HYPRE_SStructGraph graph,
                                HYPRE_SStructGrid  domain_grid);

/**
 * Set the stencil for a variable on a structured part of the grid.
 **/
HYPRE_Int
HYPRE_SStructGraphSetStencil(HYPRE_SStructGraph   graph,
                             HYPRE_Int            part,
                             HYPRE_Int            var,
                             HYPRE_SStructStencil stencil);

/**
 * Indicate that an FEM approach will be used to set matrix values on this part.
 **/
HYPRE_Int
HYPRE_SStructGraphSetFEM(HYPRE_SStructGraph graph,
                         HYPRE_Int          part);

/**
 * Set the finite element stiffness matrix sparsity.  This overrides the default
 * full sparsity pattern described below.
 *
 * Array {\tt sparsity} contains {\tt nsparse} row/column tuples (I,J) that
 * indicate the nonzeroes of the local stiffness matrix.  The layout of the
 * values passed into the routine \Ref{HYPRE_SStructMatrixAddFEMValues} is
 * determined here.
 *
 * The default sparsity is full (each variable is coupled to all others), and
 * the values passed into the routine \Ref{HYPRE_SStructMatrixAddFEMValues} are
 * assumed to be by rows (that is, column indices vary fastest).
 **/
HYPRE_Int
HYPRE_SStructGraphSetFEMSparsity(HYPRE_SStructGraph  graph,
                                 HYPRE_Int           part,
                                 HYPRE_Int           nsparse,
                                 HYPRE_Int          *sparsity);

/**
 * Add a non-stencil graph entry at a particular index.  This graph entry is
 * appended to the existing graph entries, and is referenced as such.
 *
 * NOTE: Users are required to set graph entries on all processes that own the
 * associated variables.  This means that some data will be multiply defined.
 **/
HYPRE_Int
HYPRE_SStructGraphAddEntries(HYPRE_SStructGraph   graph,
                             HYPRE_Int            part,
                             HYPRE_Int           *index,
                             HYPRE_Int            var,
                             HYPRE_Int            to_part,
                             HYPRE_Int           *to_index,
                             HYPRE_Int            to_var);

/**
 * Finalize the construction of the graph before using.
 **/
HYPRE_Int
HYPRE_SStructGraphAssemble(HYPRE_SStructGraph graph);

/**
 * Set the storage type of the associated matrix object.  It is used before
 * AddEntries and Assemble to compute the right ranks in the graph.
 * 
 * NOTE: This routine is only necessary for implementation reasons, and will
 * eventually be removed.
 *
 * @see HYPRE_SStructMatrixSetObjectType
 **/
HYPRE_Int
HYPRE_SStructGraphSetObjectType(HYPRE_SStructGraph  graph,
                                HYPRE_Int           type);
/*@}*/

/*--------------------------------------------------------------------------
 *--------------------------------------------------------------------------*/

/**
 * @name SStruct Matrices
 **/
/*@{*/

struct hypre_SStructMatrix_struct;
/**
 * The matrix object.
 **/
typedef struct hypre_SStructMatrix_struct *HYPRE_SStructMatrix;

/**
 * Create a matrix object.
 **/
HYPRE_Int
HYPRE_SStructMatrixCreate(MPI_Comm              comm,
                          HYPRE_SStructGraph    graph,
                          HYPRE_SStructMatrix  *matrix);

/**
 * Destroy a matrix object.
 **/
HYPRE_Int
HYPRE_SStructMatrixDestroy(HYPRE_SStructMatrix matrix);

/**
 * Prepare a matrix object for setting coefficient values.
 **/
HYPRE_Int
HYPRE_SStructMatrixInitialize(HYPRE_SStructMatrix matrix);

/**
 * Set matrix coefficients index by index.  The {\tt values} array is of length
 * {\tt nentries}.
 *
 * NOTE: For better efficiency, use \Ref{HYPRE_SStructMatrixSetBoxValues} to set
 * coefficients a box at a time.
 *
 * NOTE: Users are required to set values on all processes that own the
 * associated variables.  This means that some data will be multiply defined.
 *
 * NOTE: The entries in this routine must all be of the same type: either
 * stencil or non-stencil, but not both.  Also, if they are stencil entries,
 * they must all represent couplings to the same variable type (there are no
 * such restrictions for non-stencil entries).
 *
 * If the matrix is complex, then {\tt values} consists of pairs of doubles
 * representing the real and imaginary parts of each complex value.
 *
 * @see HYPRE_SStructMatrixSetComplex
 **/
HYPRE_Int
HYPRE_SStructMatrixSetValues(HYPRE_SStructMatrix  matrix,
                             HYPRE_Int            part,
                             HYPRE_Int           *index,
                             HYPRE_Int            var,
                             HYPRE_Int            nentries,
                             HYPRE_Int           *entries,
                             double              *values);

/**
 * Add to matrix coefficients index by index.  The {\tt values} array is of
 * length {\tt nentries}.
 *
 * NOTE: For better efficiency, use \Ref{HYPRE_SStructMatrixAddToBoxValues} to
 * set coefficients a box at a time.
 *
 * NOTE: Users are required to set values on all processes that own the
 * associated variables.  This means that some data will be multiply defined.
 *
 * NOTE: The entries in this routine must all be of the same type: either
 * stencil or non-stencil, but not both.  Also, if they are stencil entries,
 * they must all represent couplings to the same variable type.
 *
 * If the matrix is complex, then {\tt values} consists of pairs of doubles
 * representing the real and imaginary parts of each complex value.
 *
 * @see HYPRE_SStructMatrixSetComplex
 **/
HYPRE_Int
HYPRE_SStructMatrixAddToValues(HYPRE_SStructMatrix  matrix,
                               HYPRE_Int            part,
                               HYPRE_Int           *index,
                               HYPRE_Int            var,
                               HYPRE_Int            nentries,
                               HYPRE_Int           *entries,
                               double              *values);

/**
 * Add finite element stiffness matrix coefficients index by index.  The layout
 * of the data in {\tt values} is determined by the routines
 * \Ref{HYPRE_SStructGridSetFEMOrdering} and
 * \Ref{HYPRE_SStructGraphSetFEMSparsity}.
 *
 * If the matrix is complex, then {\tt values} consists of pairs of doubles
 * representing the real and imaginary parts of each complex value.
 *
 * @see HYPRE_SStructMatrixSetComplex
 **/
HYPRE_Int
HYPRE_SStructMatrixAddFEMValues(HYPRE_SStructMatrix  matrix,
                                HYPRE_Int            part,
                                HYPRE_Int           *index,
                                double              *values);

/**
 * Get matrix coefficients index by index.  The {\tt values} array is of length
 * {\tt nentries}.
 *
 * NOTE: For better efficiency, use \Ref{HYPRE_SStructMatrixGetBoxValues} to get
 * coefficients a box at a time.
 *
 * NOTE: Users may get values on any process that owns the associated variables.
 *
 * NOTE: The entries in this routine must all be of the same type: either
 * stencil or non-stencil, but not both.  Also, if they are stencil entries,
 * they must all represent couplings to the same variable type (there are no
 * such restrictions for non-stencil entries).
 *
 * If the matrix is complex, then {\tt values} consists of pairs of doubles
 * representing the real and imaginary parts of each complex value.
 *
 * @see HYPRE_SStructMatrixSetComplex
 **/
HYPRE_Int
HYPRE_SStructMatrixGetValues(HYPRE_SStructMatrix  matrix,
                             HYPRE_Int            part,
                             HYPRE_Int           *index,
                             HYPRE_Int            var,
                             HYPRE_Int            nentries,
                             HYPRE_Int           *entries,
                             double              *values);

/**
 * Set matrix coefficients a box at a time.  The data in {\tt values} is ordered
 * as follows:
 *
   \begin{verbatim}
   m = 0;
   for (k = ilower[2]; k <= iupper[2]; k++)
      for (j = ilower[1]; j <= iupper[1]; j++)
         for (i = ilower[0]; i <= iupper[0]; i++)
            for (entry = 0; entry < nentries; entry++)
            {
               values[m] = ...;
               m++;
            }
   \end{verbatim}
 *
 * NOTE: Users are required to set values on all processes that own the
 * associated variables.  This means that some data will be multiply defined.
 *
 * NOTE: The entries in this routine must all be of the same type: either
 * stencil or non-stencil, but not both.  Also, if they are stencil entries,
 * they must all represent couplings to the same variable type (there are no
 * such restrictions for non-stencil entries).
 *
 * If the matrix is complex, then {\tt values} consists of pairs of doubles
 * representing the real and imaginary parts of each complex value.
 *
 * @see HYPRE_SStructMatrixSetComplex
 **/
HYPRE_Int
HYPRE_SStructMatrixSetBoxValues(HYPRE_SStructMatrix  matrix,
                                HYPRE_Int            part,
                                HYPRE_Int           *ilower,
                                HYPRE_Int           *iupper,
                                HYPRE_Int            var,
                                HYPRE_Int            nentries,
                                HYPRE_Int           *entries,
                                double              *values);

/**
 * Add to matrix coefficients a box at a time.  The data in {\tt values} is
 * ordered as in \Ref{HYPRE_SStructMatrixSetBoxValues}.
 *
 * NOTE: Users are required to set values on all processes that own the
 * associated variables.  This means that some data will be multiply defined.
 *
 * NOTE: The entries in this routine must all be of stencil type.  Also, they
 * must all represent couplings to the same variable type.
 *
 * If the matrix is complex, then {\tt values} consists of pairs of doubles
 * representing the real and imaginary parts of each complex value.
 *
 * @see HYPRE_SStructMatrixSetComplex
 **/
HYPRE_Int
HYPRE_SStructMatrixAddToBoxValues(HYPRE_SStructMatrix  matrix,
                                  HYPRE_Int            part,
                                  HYPRE_Int           *ilower,
                                  HYPRE_Int           *iupper,
                                  HYPRE_Int            var,
                                  HYPRE_Int            nentries,
                                  HYPRE_Int           *entries,
                                  double              *values);

/**
 * Get matrix coefficients a box at a time.  The data in {\tt values} is
 * ordered as in \Ref{HYPRE_SStructMatrixSetBoxValues}.
 *
 * NOTE: Users may get values on any process that owns the associated variables.
 *
 * NOTE: The entries in this routine must all be of stencil type.  Also, they
 * must all represent couplings to the same variable type.
 *
 * If the matrix is complex, then {\tt values} consists of pairs of doubles
 * representing the real and imaginary parts of each complex value.
 *
 * @see HYPRE_SStructMatrixSetComplex
 **/
HYPRE_Int
HYPRE_SStructMatrixGetBoxValues(HYPRE_SStructMatrix  matrix,
                                HYPRE_Int            part,
                                HYPRE_Int           *ilower,
                                HYPRE_Int           *iupper,
                                HYPRE_Int            var,
                                HYPRE_Int            nentries,
                                HYPRE_Int           *entries,
                                double              *values);

/**
 * Finalize the construction of the matrix before using.
 **/
HYPRE_Int
HYPRE_SStructMatrixAssemble(HYPRE_SStructMatrix matrix);

/**
 * Define symmetry properties for the stencil entries in the matrix.  The
 * boolean argument {\tt symmetric} is applied to stencil entries on part {\tt
 * part} that couple variable {\tt var} to variable {\tt to\_var}.  A value of
 * -1 may be used for {\tt part}, {\tt var}, or {\tt to\_var} to specify
 * ``all''.  For example, if {\tt part} and {\tt to\_var} are set to -1, then
 * the boolean is applied to stencil entries on all parts that couple variable
 * {\tt var} to all other variables.
 * 
 * By default, matrices are assumed to be nonsymmetric.  Significant
 * storage savings can be made if the matrix is symmetric.
 **/
HYPRE_Int
HYPRE_SStructMatrixSetSymmetric(HYPRE_SStructMatrix matrix,
                                HYPRE_Int           part,
                                HYPRE_Int           var,
                                HYPRE_Int           to_var,
                                HYPRE_Int           symmetric);

/**
 * Define symmetry properties for all non-stencil matrix entries.
 **/
HYPRE_Int
HYPRE_SStructMatrixSetNSSymmetric(HYPRE_SStructMatrix matrix,
                                  HYPRE_Int           symmetric);

/**
 * Set the storage type of the matrix object to be constructed.  Currently, {\tt
 * type} can be either {\tt HYPRE\_SSTRUCT} (the default), {\tt HYPRE\_STRUCT},
 * or {\tt HYPRE\_PARCSR}.
 *
 * @see HYPRE_SStructMatrixGetObject
 **/
HYPRE_Int
HYPRE_SStructMatrixSetObjectType(HYPRE_SStructMatrix  matrix,
                                 HYPRE_Int            type);

/**
 * Get a reference to the constructed matrix object.
 *
 * @see HYPRE_SStructMatrixSetObjectType
 **/
HYPRE_Int
HYPRE_SStructMatrixGetObject(HYPRE_SStructMatrix   matrix,
                             void                **object);

/**
 * Set the matrix to be complex.
 **/
HYPRE_Int
HYPRE_SStructMatrixSetComplex(HYPRE_SStructMatrix matrix);

/**
 * Print the matrix to file.  This is mainly for debugging purposes.
 **/
HYPRE_Int
HYPRE_SStructMatrixPrint(const char          *filename,
                         HYPRE_SStructMatrix  matrix,
                         HYPRE_Int            all);

/*@}*/

/*--------------------------------------------------------------------------
 *--------------------------------------------------------------------------*/

/**
 * @name SStruct Vectors
 **/
/*@{*/

struct hypre_SStructVector_struct;
/**
 * The vector object.
 **/
typedef struct hypre_SStructVector_struct *HYPRE_SStructVector;

/**
 * Create a vector object.
 **/
HYPRE_Int
HYPRE_SStructVectorCreate(MPI_Comm              comm,
                          HYPRE_SStructGrid     grid,
                          HYPRE_SStructVector  *vector);

/**
 * Destroy a vector object.
 **/
HYPRE_Int
HYPRE_SStructVectorDestroy(HYPRE_SStructVector vector);

/**
 * Prepare a vector object for setting coefficient values.
 **/
HYPRE_Int
HYPRE_SStructVectorInitialize(HYPRE_SStructVector vector);

/**
 * Set vector coefficients index by index.
 *
 * NOTE: For better efficiency, use \Ref{HYPRE_SStructVectorSetBoxValues} to set
 * coefficients a box at a time.
 *
 * NOTE: Users are required to set values on all processes that own the
 * associated variables.  This means that some data will be multiply defined.
 *
 * If the vector is complex, then {\tt value} consists of a pair of doubles
 * representing the real and imaginary parts of the complex value.
 *
 * @see HYPRE_SStructVectorSetComplex
 **/
HYPRE_Int
HYPRE_SStructVectorSetValues(HYPRE_SStructVector  vector,
                             HYPRE_Int            part,
                             HYPRE_Int           *index,
                             HYPRE_Int            var,
                             double              *value);

/**
 * Add to vector coefficients index by index.
 *
 * NOTE: For better efficiency, use \Ref{HYPRE_SStructVectorAddToBoxValues} to
 * set coefficients a box at a time.
 *
 * NOTE: Users are required to set values on all processes that own the
 * associated variables.  This means that some data will be multiply defined.
 *
 * If the vector is complex, then {\tt value} consists of a pair of doubles
 * representing the real and imaginary parts of the complex value.
 *
 * @see HYPRE_SStructVectorSetComplex
 **/
HYPRE_Int
HYPRE_SStructVectorAddToValues(HYPRE_SStructVector  vector,
                               HYPRE_Int            part,
                               HYPRE_Int           *index,
                               HYPRE_Int            var,
                               double              *value);

/**
 * Add finite element vector coefficients index by index.  The layout of the
 * data in {\tt values} is determined by the routine
 * \Ref{HYPRE_SStructGridSetFEMOrdering}.
 *
 * If the vector is complex, then {\tt values} consists of pairs of doubles
 * representing the real and imaginary parts of each complex value.
 *
 * @see HYPRE_SStructVectorSetComplex
 **/
HYPRE_Int
HYPRE_SStructVectorAddFEMValues(HYPRE_SStructVector  vector,
                                HYPRE_Int            part,
                                HYPRE_Int           *index,
                                double              *values);

/**
 * Get vector coefficients index by index.
 *
 * NOTE: For better efficiency, use \Ref{HYPRE_SStructVectorGetBoxValues} to get
 * coefficients a box at a time.
 *
 * NOTE: Users may only get values on processes that own the associated
 * variables.
 *
 * If the vector is complex, then {\tt value} consists of a pair of doubles
 * representing the real and imaginary parts of the complex value.
 *
 * @see HYPRE_SStructVectorSetComplex
 **/
HYPRE_Int
HYPRE_SStructVectorGetValues(HYPRE_SStructVector  vector,
                             HYPRE_Int            part,
                             HYPRE_Int           *index,
                             HYPRE_Int            var,
                             double              *value);

/**
 * Set vector coefficients a box at a time.  The data in {\tt values} is ordered
 * as follows:
 *
   \begin{verbatim}
   m = 0;
   for (k = ilower[2]; k <= iupper[2]; k++)
      for (j = ilower[1]; j <= iupper[1]; j++)
         for (i = ilower[0]; i <= iupper[0]; i++)
         {
            values[m] = ...;
            m++;
         }
   \end{verbatim}
 *
 * NOTE: Users are required to set values on all processes that own the
 * associated variables.  This means that some data will be multiply defined.
 *
 * If the vector is complex, then {\tt values} consists of pairs of doubles
 * representing the real and imaginary parts of each complex value.
 *
 * @see HYPRE_SStructVectorSetComplex
 **/
HYPRE_Int
HYPRE_SStructVectorSetBoxValues(HYPRE_SStructVector  vector,
                                HYPRE_Int            part,
                                HYPRE_Int           *ilower,
                                HYPRE_Int           *iupper,
                                HYPRE_Int            var,
                                double              *values);

/**
 * Add to vector coefficients a box at a time.  The data in {\tt values} is
 * ordered as in \Ref{HYPRE_SStructVectorSetBoxValues}.
 *
 * NOTE: Users are required to set values on all processes that own the
 * associated variables.  This means that some data will be multiply defined.
 *
 * If the vector is complex, then {\tt values} consists of pairs of doubles
 * representing the real and imaginary parts of each complex value.
 *
 * @see HYPRE_SStructVectorSetComplex
 **/
HYPRE_Int
HYPRE_SStructVectorAddToBoxValues(HYPRE_SStructVector  vector,
                                  HYPRE_Int            part,
                                  HYPRE_Int           *ilower,
                                  HYPRE_Int           *iupper,
                                  HYPRE_Int            var,
                                  double              *values);

/**
 * Get vector coefficients a box at a time.  The data in {\tt values} is ordered
 * as in \Ref{HYPRE_SStructVectorSetBoxValues}.
 *
 * NOTE: Users may only get values on processes that own the associated
 * variables.
 *
 * If the vector is complex, then {\tt values} consists of pairs of doubles
 * representing the real and imaginary parts of each complex value.
 *
 * @see HYPRE_SStructVectorSetComplex
 **/
HYPRE_Int
HYPRE_SStructVectorGetBoxValues(HYPRE_SStructVector  vector,
                                HYPRE_Int            part,
                                HYPRE_Int           *ilower,
                                HYPRE_Int           *iupper,
                                HYPRE_Int            var,
                                double              *values);

/**
 * Finalize the construction of the vector before using.
 **/
HYPRE_Int
HYPRE_SStructVectorAssemble(HYPRE_SStructVector vector);

/**
 * Gather vector data so that efficient {\tt GetValues} can be done.  This
 * routine must be called prior to calling {\tt GetValues} to insure that
 * correct and consistent values are returned, especially for non cell-centered
 * data that is shared between more than one processor.
 **/
HYPRE_Int
HYPRE_SStructVectorGather(HYPRE_SStructVector vector);

/**
 * Set the storage type of the vector object to be constructed.  Currently, {\tt
 * type} can be either {\tt HYPRE\_SSTRUCT} (the default), {\tt HYPRE\_STRUCT},
 * or {\tt HYPRE\_PARCSR}.
 *
 * @see HYPRE_SStructVectorGetObject
 **/
HYPRE_Int
HYPRE_SStructVectorSetObjectType(HYPRE_SStructVector  vector,
                                 HYPRE_Int            type);

/**
 * Get a reference to the constructed vector object.
 *
 * @see HYPRE_SStructVectorSetObjectType
 **/
HYPRE_Int
HYPRE_SStructVectorGetObject(HYPRE_SStructVector   vector,
                             void                **object);

/**
 * Set the vector to be complex.
 **/
HYPRE_Int
HYPRE_SStructVectorSetComplex(HYPRE_SStructVector vector);

/**
 * Print the vector to file.  This is mainly for debugging purposes.
 **/
HYPRE_Int
HYPRE_SStructVectorPrint(const char          *filename,
                         HYPRE_SStructVector  vector,
                         HYPRE_Int            all);

/*@}*/
/*@}*/

/*--------------------------------------------------------------------------
 *--------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif

