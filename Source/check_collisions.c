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
#include <stdlib.h>


void check_collisions(void)   /* Check collisions and manage memory */
{   

    int number_Collisions = 0;

    /* Loop variables are declared in the register to make code faster */
    register int n;
    register int l;
    /* This planet_copy is used as a temporary variable so that we can resize the planet array and free memory */
    Planet *planet_copy; 

    for (n = 0; n < N_planets; n++)
    {
        for (l = 0; l < N_planets; l++)
	{
	    switch ((int)(n > l))
	    {
		case 1:
                    if ((r[n][l] < 0.05*AU) && (n != l) && (planet[l].planet_id != 0 && planet[n].planet_id != 0)) /* Collisions occur if distance is less than 0.05AU - tweak this value if you wish! */
		    {  
                        number_Collisions++;
                        planet[l].planet_id = 0;
                        planet[n].vx = (planet[n].vx*planet[n].m + planet[l].vx*planet[l].m) / (planet[n].m + planet[l].m);
                        planet[n].vy = (planet[n].vy*planet[n].m + planet[l].vy*planet[l].m) / (planet[n].m + planet[l].m);
                        planet[n].vz = (planet[n].vz*planet[n].m + planet[l].vz*planet[l].m) / (planet[n].m + planet[l].m);
                        planet[n].m = planet[n].m + planet[l].m;
		    }
                    break;
                case 0: // if (n<l) do the same thing but with r[l][n] instead
                    if ( (r[l][n] < 0.05*AU) && (n != l) && (planet[l].planet_id != 0 && planet[n].planet_id != 0)) /* Collisions occur if distance is less than 0.05AU - tweak this value if you wish! */
		    {  
                        number_Collisions++;
                        planet[l].planet_id = 0;
                        planet[n].vx = (planet[n].vx*planet[n].m + planet[l].vx*planet[l].m) / (planet[n].m + planet[l].m);
                        planet[n].vy = (planet[n].vy*planet[n].m + planet[l].vy*planet[l].m) / (planet[n].m + planet[l].m);
                        planet[n].vz = (planet[n].vz*planet[n].m + planet[l].vz*planet[l].m) / (planet[n].m + planet[l].m);
                        planet[n].m = planet[n].m + planet[l].m;
		    }
                    break;
	    }
	}
    }

    if (number_Collisions != 0) /* Following code runs if there are collisions between planets */
    {
        int dummy = 0;

        for (n = 0; n < N_planets; n++)
	{
            if (planet[n].planet_id != 0)
	    {
	        dummy++;
	    }
	}
	
        planet_copy = (Planet*)malloc(dummy * sizeof(Planet));

        dummy = 0;

        for (n = 0; n < N_planets; n++)
	{
            if (planet[n].planet_id != 0)
	    {
	        planet_copy[dummy] = planet[n];
                dummy++;
	    }
	}
		
        planet = (Planet*)realloc(planet,dummy * sizeof(Planet));

        for (n = 0; n < dummy; n++)
	{
            planet[n] = planet_copy[n];
        } 

        N_planets = dummy;
        number_Collisions = 0;
        free(planet_copy);
    } 
}

