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
 
/*
Any Feedback/Suggestions/Improvements greatly appreciated

This program implements the algorithm for N-Body gravitational attraction as suggested
by Richard Feynman on his book 'Lectures on Physics' Vol.I, chapter 9 section 7.

This might not be the most efficient implementation of the algorithm. However it should be
easy to understand and hopefully it will be useful.

This computer program is licensed under the GNU General Public License version 3 or later

BWV 846
*/

#include "main.h"
#include "energy_calculation.h"
#include "momentum_calculation.h"
#include "drawing_and_input_controls.h"
#include "check_collisions.h"
#include "update_motion.h"

#include <stdio.h>
#include <math.h>
#include <GL/freeglut.h>
#include <time.h> 





/////////////////////// DECLARE GLOBAL VARIABLES //////////////////////////////////////////////////////
int N_planets; /* number of celestial bodies (CB's) */
const double AU = 149.59E9;  /* Astronomical unit of distance */
const double G = 6.674E-11; /* Universal gravitation constant */
double h;  /* Time resolution in seconds */
int iteration;  /* variable used to set the time marching algorithm */

double **r;  /* matrix of the distance between CB's. This matrix is triangular so in the code you will find
    lots of tricks involving r[n][l] and r[l][n], just to save up a lot of memory. The extra spaggetti is worth it!
    Alternatively one could re-write the program without the matrix r being declared at all,
    but that would require r to be calculated multiple times and therefore would make the code slower 
    (when many CB's are considered) than if r is only calculated once!*/
   
/* Declare keyboard and mouse control variables. Unfortunately, they have to be global since
 * process_mouse_motion does not accept external arguments. */

Keyboard_control Keyboard;
Mouse_control Mouse;

/* Declare planets to be global, since they are used by every single function */

Planet *planet;












int main(int argc, char **argv){
	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Import settings from text file 
   
    FILE *fp = fopen("input_data/settings.txt", "r");
        if (fp == NULL) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }
        
    char buffer[255]; 

    //Reads time resolution (double h) from the file:
    char time_resolution[255];

    while (fgets(buffer, sizeof buffer, fp) != NULL){
	    if (buffer[0] == 'T' && buffer[1] == 'i' && buffer[2] == 'm'&& buffer[3] == 'e'){
	    fscanf(fp, "%s", time_resolution);
	    h = atof(time_resolution);
        }
    }

    fclose(fp);

    FILE *fp2 = fopen("input_data/positions.txt", "r");
        if (fp2 == NULL) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }


    //reads how many planets there are by counting the number of position coordinates.
    N_planets = -1;
    while (fgets(buffer, sizeof buffer, fp2) != NULL){
        N_planets++;
    }

    //allocates arrays that are the size of N_planets:
    planet = (Planet*)malloc(N_planets * sizeof(Planet));
   
    //create distance matrix r dynamically:
    r=(double **) malloc(N_planets*sizeof(double*));

    fclose(fp2);

    //reads the coordinates of the Celestial Bodies:
    FILE *fp3 = fopen("input_data/positions.txt", "r");
        if (fp3 == NULL) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }
       
    double x;
    double y;  
    double z;      
    int dummy_count = 0;
    char x_coord[255];
    char y_coord[255];
    char z_coord[255];

    for (int n1 = -1; n1<N_planets; n1++){
	    if (n1 == -1){
	        fgets(buffer, sizeof buffer, fp3);
	    }
	    else 
	    {
	        fscanf(fp3, "%s", x_coord);
            planet[n1].x = atof(x_coord)*AU;
	        fscanf(fp3, "%s", y_coord);
	        planet[n1].y = atof(y_coord)*AU;
            fscanf(fp3, "%s", z_coord);
	        planet[n1].z = atof(z_coord)*AU;
        }
    }
	
	
    fclose(fp3);



    //reads the velocities of the Celestial Bodies:
    FILE *fp4 = fopen("input_data/velocities.txt", "r");
        if (fp4 == NULL) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }
       
    double vx;
    double vy;    
    double vz;     
    dummy_count = 0;
    char vx_buffer[255];
    char vy_buffer[255];
    char vz_buffer[255];


    for (int n1 = -1; n1<N_planets; n1++){
	    if (n1 == -1){
	        fgets(buffer, sizeof buffer, fp4);
	    }   
	    else{
	        fscanf(fp4, "%s", vx_buffer);
            planet[n1].vx = atof(vx_buffer);
	        fscanf(fp4, "%s", vy_buffer);
	        planet[n1].vy = atof(vy_buffer);
	        fscanf(fp4, "%s", vz_buffer);
	        planet[n1].vz = atof(vz_buffer);
        }
    }
	
	
    fclose(fp4);

    //reads the masses of the Celestial Bodies:
    FILE *fp5 = fopen("input_data/mass.txt", "r");
    if (fp5 == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
        }
       
    double m; 
    dummy_count=0;
    char mass_buffer[255];


    for (int n1=-1; n1<N_planets; n1++){
	    if (n1==-1){
	    fgets(buffer, sizeof buffer, fp5);
	}else{
	    fscanf(fp5, "%s", mass_buffer);
        planet[n1].m = atof(mass_buffer);
        }
    }
	
	
    fclose(fp5);


    
    /* First define the struct planets, their velocities, mass, accelerations etc... */
    
    register int n;
    register int l;
	
    srand((unsigned int)time(NULL));
	
    iteration = 0;
   
    /* Define distance matrix r*/  
   
    for(n=0;n<N_planets;n++){
        r[n] = (double *) malloc((n+1)*sizeof(double));// r is a triangular matrix (each row n is assigned n columns), saving twice as much memory compared to a square matrix
    } 

    /* Define planets */
 
    for (n=0; n<N_planets; n++)
    {
	    planet[n].planet_id = n+1;
	    planet[n].ax = 0;
	    planet[n].ay = 0;	
	    planet[n].az = 0;
	    planet[n].color.red = (float)(rand() % 2)*10+0.4; //Set random planet colours
	    planet[n].color.green = (float)(rand() % 2)*10+0.4;  
	    planet[n].color.blue = (float)(rand() % 2)*10+0.4;     
    }  
   
    /* Initiate keyboard control variables */

    Keyboard.O_key_pressed = 0;
    Keyboard.P_key_pressed = 0;
    Keyboard.Pause = 0;

    /* Initiate Mouse control variables */

    Mouse.zoom_in_or_out = 1; 
    Mouse.move_right = 0;
    Mouse.move_up = 0;

  
    ///////////////////////Calculate distances between CB's:

    for (n=0; n<N_planets; n++){
        for (l=0; l<N_planets; l++){
	        if (n>l){
            r[n][l] = sqrt(pow((planet[n].x-planet[l].x),2) + pow((planet[n].y-planet[l].y),2) + pow((planet[n].z-planet[l].z),2) ) ;
            }
            else{
                continue;
            }
        }
    }


    ///////////////////////Calculate accelerations of the CB's:
    for (n=0; n<N_planets; n++){
        for (l=0; l<N_planets; l++){
            if (n==l){
	            planet[n].ax = planet[n].ax;
                planet[n].ay = planet[n].ay;	
                planet[n].az = planet[n].az;
            }
            else if (n>l){
                planet[n].ax = planet[n].ax+ -G*(planet[l].m)*(planet[n].x-planet[l].x)/pow(r[n][l],3);
                planet[n].ay = planet[n].ay+ -G*(planet[l].m)*(planet[n].y-planet[l].y)/pow(r[n][l],3); 
                planet[n].az = planet[n].az+ -G*(planet[l].m)*(planet[n].z-planet[l].z)/pow(r[n][l],3); 		
            }
            else{
                planet[n].ax = planet[n].ax+ -G*(planet[l].m)*(planet[n].x-planet[l].x)/pow(r[l][n],3);
                planet[n].ay = planet[n].ay+ -G*(planet[l].m)*(planet[n].y-planet[l].y)/pow(r[l][n],3);	
                planet[n].az = planet[n].az+ -G*(planet[l].m)*(planet[n].z-planet[l].z)/pow(r[l][n],3);		
            }
        }
    }


    ///////////////////////Calculate velocities of the CB's:
    for (n=0; n<N_planets; n++){
        planet[n].vx = planet[n].vx+planet[n].ax*h*0.5;
        planet[n].vy = planet[n].vy+planet[n].ay*h*0.5;
        planet[n].vz = planet[n].vz+planet[n].az*h*0.5; 
    }


    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(600,600);
    glutCreateWindow("Feynman 0.81");
    glutDisplayFunc(display_3D);
    glutKeyboardFunc(process_keys);
    glutMotionFunc(process_mouse_motion);
    glutMouseFunc(process_mouse);
    glutMainLoop();

  
    return 0;
}


