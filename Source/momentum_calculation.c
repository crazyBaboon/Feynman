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
 
#include "main.h"
#include <math.h>


int calculate_momentum(Momentum *m){

double linear_x = 0;
double linear_y = 0;
double linear_z = 0;
double angular_x = 0;
double angular_y = 0;
double angular_z = 0;

register int n;	
register int l;
	
/* Calculate linear momentum of particles */
    for (n=0; n<N_planets; n++)
    {
	    linear_x = linear_x + planet[n].m * planet[n].vx; 
	    linear_y = linear_y + planet[n].m * planet[n].vy; 
	    linear_z = linear_z + planet[n].m * planet[n].vz; 
    }

m->linear_x = linear_x;
m->linear_y = linear_y;
m->linear_z = linear_z;

/* Calculate angular momentum of particles */


    for (n=0; n<N_planets; n++)
    {
	    angular_x = angular_x + planet[n].m * (planet[n].y * planet[n].vz - planet[n].z * planet[n].vy);
	    angular_y = angular_y + planet[n].m * (planet[n].z * planet[n].vx - planet[n].x * planet[n].vz);
	    angular_z = angular_z + planet[n].m * (planet[n].x * planet[n].vy - planet[n].y * planet[n].vx);
    }



m->angular_x = angular_x;
m->angular_y = angular_y;
m->angular_z = angular_z;

return 0;
}
