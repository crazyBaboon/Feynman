/* Feynman  - A simple astronomy simulator written in the C programming language
 * Copyright (C) 2016-2017 Nuno Ferreira
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef _UPDATE_MOTION_H_
#define _UPDATE_MOTION_H_

/* Update motion using single core computation */
void update_motion(void);

/* The 'update_accelerations' function is called by the multithread 'update_motion_parallel' function */
void* update_accelerations(void* argument);

/* The 'update_distance_matrix' function is called by the multithread 'update_motion_parallel' function */
void* update_distance_matrix(void* argument);

/* Update motion if multithread is selected (#define Number_of_threads in main.c) */
void update_motion_parallel(void);

#endif


