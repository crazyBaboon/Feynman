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

/* EVERY 'EXTERN' VARIABLE DECLARED IN THIS FILE IS ACCESSIBLE TO ANY FILE THAT #include THIS FILE */


#ifndef _MAIN_H_
#define _MAIN_H_

#define Number_of_threads 4

/* Here are all the global variables used in Feynman */
extern int N_planets;    /* number of celestial bodies (CB's) */
extern const double AU;  /* Astronomical unit of distance */
extern const double G;   /* Universal gravitation constant */
extern double h;  /* Time resolution in seconds */
extern int iteration;  /* variable used to set the time marching algorithm */
extern double **r; /* matrix of the distance between CB's. */


/* Definition of all structures and Typedef's */
struct RGB{
    float red,green,blue;  //this will describe the planet colour
};

struct PLANET{
    double x,y,z,vx,vy,vz,ax,ay,az,m;  //Variables of motion distance, velocity, acceleration and mass
    struct RGB color; //Colour of the planet (I honestly prefer Color as it better matches the phonetics)
    int planet_id;
};

typedef struct PLANET Planet; 

extern Planet *planet;


/* Energy structure that comprises potential and kinetic energy */
struct ENERGY{
    double KE; //Kinetic energy of the system
    double PE; //Kinetic energy of the system
};

/* Momentum structure that comprises linear and angular momentum */
struct MOMENTUM{
    double linear_x; //Linear Momentum of the system in the x-direction
    double linear_y; //Linear Momentum of the system in the y-direction
    double linear_z; //Linear Momentum of the system in the z-direction
    double angular_x; //Angular Momentum of the system
    double angular_y; //Angular Momentum of the system
    double angular_z; //Angular Momentum of the system
};

typedef struct ENERGY Energy; 
typedef struct MOMENTUM Momentum; 

/* Structure that comprises keyboard control flags */
struct KEYBOARD_CONTROL{
    short int P_key_pressed; 
	short int O_key_pressed;   
	short int Pause; 
};

typedef struct KEYBOARD_CONTROL Keyboard_control; 

/* Structure that comprises mouse control flags */
struct MOUSE_CONTROL{
    float zoom_in_or_out; 
    float move_right;
    float move_up;
    int xtemp; 
    int ytemp;
};

typedef struct MOUSE_CONTROL Mouse_control; 


#endif
