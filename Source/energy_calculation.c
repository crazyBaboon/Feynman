/* Feynman  - A simple astronomy simulator written in the C programming language
 * Copyright (C) 2016-2018 Nuno Ferreira
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


void calculate_energies(Energy *e){

    double KE = 0;
    double PE = 0;

    register int n;	
    register int l;
	
    for (n = 0; n < N_planets; n++)
    {
        KE = (KE + 0.5*planet[n].m*(pow(planet[n].vx,2) + pow(planet[n].vy,2) + pow(planet[n].vz,2))  );  /* Calculate Kinetic Energy of the system of particles   K = 0.5*m*(vx^2+vy^2) */
    }

    for (n = 0; n < N_planets; n++)
    {
        for (l = 0; l < N_planets; l++)
        { 
            if(n > l)
            {
                PE = PE - G*(planet[l].m)*(planet[n].m) / (r[n][l]); /* Calculate potential energy of the system   P=-G*m1*m2/r */
            }  
            else
            {
                PE = PE;
	    }
        }
    }

    e->KE = KE;
    e->PE = PE;
}
