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
#include <pthread.h>
#include <assert.h>
#include <math.h>

pthread_t threads[Number_of_threads];
int thread_args[Number_of_threads];

/* Update motion using single core computation */
void update_motion(void) 
{
    /* Loop variables are declared in the register to make code faster */
    register int n;
    register int l;

    for (n = 0; n < N_planets; n++)
    {
        planet[n].x = planet[n].x + planet[n].vx*h;  /* Update positions of the CB's */
        planet[n].y = planet[n].y + planet[n].vy*h;
        planet[n].z = planet[n].z + planet[n].vz*h;
    }


    /*   Calculate distances between CB's   */

    for (n = 0; n < N_planets; n++)
    {
        for (l = 0; l < N_planets; l++)
        {
            if (n > l)
            {
            r[n][l] = sqrt(pow((planet[n].x - planet[l].x),2) + pow((planet[n].y - planet[l].y),2) + pow((planet[n].z - planet[l].z),2));
            }
            else
            {
            continue;  /* continue statement is necessary because r[n][l] is triangular and therefore not defined when n<=l */
            }  
        }	
    }

	/*   Calculate accelerations of the CB's   */
    for (n = 0; n < N_planets; n++)
    {
        planet[n].ax = 0;  	/* Set accelerations to 0 */
        planet[n].ay = 0;
        planet[n].az = 0;
        for (l = 0; l < N_planets; l++)
        {
            if (n == l)
            {
                planet[n].ax = planet[n].ax;
                planet[n].ay = planet[n].ay;
                planet[n].az = planet[n].az;
            }
            else if (n > l)
            {
                planet[n].ax = planet[n].ax+ -G*(planet[l].m)*(planet[n].x-planet[l].x) / pow(r[n][l],3)   ;
                planet[n].ay = planet[n].ay+ -G*(planet[l].m)*(planet[n].y-planet[l].y) / pow(r[n][l],3)   ;
                planet[n].az = planet[n].az+ -G*(planet[l].m)*(planet[n].z-planet[l].z) / pow(r[n][l],3)   ;
            } 
            else
            {
                planet[n].ax = planet[n].ax+ -G*(planet[l].m)*(planet[n].x-planet[l].x) / pow(r[l][n],3)   ;
                planet[n].ay = planet[n].ay+ -G*(planet[l].m)*(planet[n].y-planet[l].y) / pow(r[l][n],3)   ;
                planet[n].az = planet[n].az+ -G*(planet[l].m)*(planet[n].z-planet[l].z) / pow(r[l][n],3)   ;
            }				
        }
    }

    /*   Calculate velocities of the CB's   */
    for (n = 0; n < N_planets; n++)
    {
        planet[n].vx = planet[n].vx + planet[n].ax*h; 
        planet[n].vy = planet[n].vy + planet[n].ay*h;
        planet[n].vz = planet[n].vz + planet[n].az*h;
    } 
}

/* The 'update_accelerations' function is called by the multithread 'update_motion_parallel' function */
void* update_accelerations(void* argument)
{
    int thread_number;
    thread_number = *((int *) argument);
    /* indexes of planets to be considered in the nth thread: */

    int N_planets_thread[8]={0,N_planets/4,N_planets/4+1,2*N_planets/4,2*N_planets/4+1,3*N_planets/4,3*N_planets/4+1,N_planets-1};

    /* Loop variables are declared in the register to make code faster */
    register int n;
    register int l;

	
    /* Calculate accelerations of the CB's */
    for (n = N_planets_thread[2*thread_number]; n <= N_planets_thread[2*thread_number+1]; n++)  /* divide the planets per thread */
    {
        planet[n].ax = 0;  	/* Set accelerations to 0 */
        planet[n].ay = 0;
        planet[n].az = 0;
        
	for (l = 0; l < N_planets; l++)
        {
            if(n == l)
            {
                planet[n].ax = planet[n].ax;
                planet[n].ay = planet[n].ay;
                planet[n].az = planet[n].az;
            }
            else if (n > l)
            {
                planet[n].ax = planet[n].ax+ -G*(planet[l].m)*(planet[n].x-planet[l].x) / pow(r[n][l],3);
                planet[n].ay = planet[n].ay+ -G*(planet[l].m)*(planet[n].y-planet[l].y) / pow(r[n][l],3);
                planet[n].az = planet[n].az+ -G*(planet[l].m)*(planet[n].z-planet[l].z) / pow(r[n][l],3);} 
            else
            {
                planet[n].ax = planet[n].ax+ -G*(planet[l].m)*(planet[n].x-planet[l].x) / pow(r[l][n],3);
                planet[n].ay = planet[n].ay+ -G*(planet[l].m)*(planet[n].y-planet[l].y) / pow(r[l][n],3);
                planet[n].az = planet[n].az+ -G*(planet[l].m)*(planet[n].z-planet[l].z) / pow(r[l][n],3);
            }
        }
    }	
    return NULL;
}

/* The 'update_distance_matrix' function is called by the multithread 'update_motion_parallel' function */
void* update_distance_matrix(void* argument)
{
    int thread_number;
    thread_number = *((int *) argument);
    /* indexes of planets to be considered in the nth thread: */
    int N_planets_thread[8] = {0,N_planets/4,N_planets/4+1,2*N_planets/4,2*N_planets/4+1,3*N_planets/4,3*N_planets/4+1,N_planets-1};
    /* Loop variables are declared in the register to make code faster */
    register int n;
    register int l;

    /* Calculate distances between CB's: */

    for (n = 0; n < N_planets; n++)
    {
        for (l = 0; l < N_planets; l++)
        {
            if (n > l)
            {
                r[n][l] = sqrt(pow((planet[n].x-planet[l].x),2) + pow((planet[n].y-planet[l].y),2) + pow((planet[n].z-planet[l].z),2));
            }
            else
            {
                continue;
            }  /* continue statement is necessary because r[n][l] is triangular and therefore not defined when n<=l */
        }
    }
    return NULL;
}

/* Update motion if multithread is selected (#define Number_of_threads in main.c) */
void update_motion_parallel(void) 
{   
    register int n;
    register int l;

    /* Update positions of the CB's */
    for (n = 0; n < N_planets; n++)
    {
        planet[n].x = planet[n].x+planet[n].vx*h;  
        planet[n].y = planet[n].y+planet[n].vy*h;
        planet[n].z = planet[n].z+planet[n].vz*h;
    }
	
    /* The following piece of code set's up the threads to calculate CB's distance matrix */
    int result_code, index; 

    for (index = 0; index < Number_of_threads; ++index) 
    {
        thread_args[index] = index; 
        result_code = pthread_create(&threads[index], NULL, update_distance_matrix, &thread_args[index]);
        assert(0 == result_code);
    }
    	
    for (index = 0; index < Number_of_threads; ++index) 
    {
        result_code = pthread_join(threads[index], NULL);
        assert(0 == result_code);
    }	

    /* The following piece of code set's up the threads to calculate CB's accelerations */

    for (index = 0; index < Number_of_threads; ++index) 
    {
        thread_args[index] = index;
        result_code = pthread_create(&threads[index], NULL, update_accelerations, &thread_args[index]);
        assert(0 == result_code);
    }
	
    for (index = 0; index < Number_of_threads; ++index) 
    {
        result_code = pthread_join(threads[index], NULL);
        assert(0 == result_code); 
    }
    
    /* Calculate velocities of the CB's */
    for (n = 0; n < N_planets; n++)
    {
        planet[n].vx = planet[n].vx + planet[n].ax*h; 
        planet[n].vy = planet[n].vy + planet[n].ay*h;
        planet[n].vz = planet[n].vz + planet[n].az*h;
    }
}

