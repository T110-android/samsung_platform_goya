/* Copyright (C) 2001-2006 Artifex Software, Inc.
   All Rights Reserved.
  
  This file is part of GNU ghostscript

  GNU ghostscript is free software; you can redistribute it and/or
  modify it under the terms of the version 2 of the GNU General Public
  License as published by the Free Software Foundation.

  GNU ghostscript is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along with
  ghostscript; see the file COPYING. If not, write to the Free Software Foundation,
  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

*/

/* $Id: gsmatrix.h,v 1.8 2008/05/04 14:34:49 Arabidopsis Exp $ */
/* Definition of matrices and client interface to matrix routines */

#ifndef gsmatrix_INCLUDED
#  define gsmatrix_INCLUDED

/* See p. 65 of the PostScript manual for the semantics of */
/* transformation matrices. */

/* Structure for a transformation matrix. */
#define _matrix_body\
  float xx, xy, yx, yy, tx, ty
struct gs_matrix_s {
    _matrix_body;
};
struct gs_matrix_double_s {
  double xx, xy, yx, yy, tx, ty;
};

#ifndef gs_matrix_DEFINED
#  define gs_matrix_DEFINED
typedef struct gs_matrix_s gs_matrix;
#endif
#ifndef gs_matrix_double_DEFINED
#  define gs_matrix_double_DEFINED
typedef struct gs_matrix_double_s gs_matrix_double;
#endif

/* Macro for initializing constant matrices */
#define constant_matrix_body(xx, xy, yx, yy, tx, ty)\
  (float)(xx), (float)(xy), (float)(yx),\
  (float)(yy), (float)(tx), (float)(ty)

/* Macros for testing whether matrix coefficients are zero, */
/* for shortcuts when the matrix is simple. */
#define is_xxyy(pmat) is_fzero2((pmat)->xy, (pmat)->yx)
#define is_xyyx(pmat) is_fzero2((pmat)->xx, (pmat)->yy)

/* The identity matrix (for structure initialization) */
#define identity_matrix_body\
  constant_matrix_body(1, 0, 0, 1, 0, 0)

/* Matrix creation */
void gs_make_identity(gs_matrix *);
int gs_make_translation(floatp, floatp, gs_matrix *),
    gs_make_scaling(floatp, floatp, gs_matrix *),
    gs_make_rotation(floatp, gs_matrix *);

/* Matrix arithmetic */
int gs_matrix_multiply(const gs_matrix *, const gs_matrix *, gs_matrix *),
    gs_matrix_multiply_double(const gs_matrix_double *, const gs_matrix *, gs_matrix_double *),
    gs_matrix_invert(const gs_matrix *, gs_matrix *),
    gs_matrix_invert_to_double(const gs_matrix *, gs_matrix_double *),
    gs_matrix_translate(const gs_matrix *, floatp, floatp, gs_matrix *),
    gs_matrix_scale(const gs_matrix *, floatp, floatp, gs_matrix *),
    gs_matrix_rotate(const gs_matrix *, floatp, gs_matrix *);

/* Coordinate transformation */
int gs_point_transform(floatp, floatp, const gs_matrix *, gs_point *),
    gs_point_transform_inverse(floatp, floatp, const gs_matrix *, gs_point *),
    gs_distance_transform(floatp, floatp, const gs_matrix *, gs_point *),
    gs_distance_transform_inverse(floatp, floatp, const gs_matrix *, gs_point *),
    gs_points_bbox(const gs_point[4], gs_rect *),
    gs_bbox_transform_only(const gs_rect *, const gs_matrix *, gs_point[4]),
    gs_bbox_transform(const gs_rect *, const gs_matrix *, gs_rect *),
    gs_bbox_transform_inverse(const gs_rect *, const gs_matrix *, gs_rect *);

/* Serialization */
#ifndef stream_DEFINED
#  define stream_DEFINED
typedef struct stream_s stream;
#endif
int sget_matrix(stream *, gs_matrix *);
int sput_matrix(stream *, const gs_matrix *);

#endif /* gsmatrix_INCLUDED */
