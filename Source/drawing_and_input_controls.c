/* Feynman  - A simple astronomy simulator written in the C programming language
 * Copyright (C) 2016-2018 Nuno Ferreira and contributors
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
#include "energy_calculation.h"
#include "momentum_calculation.h"
#include "update_motion.h"
#include "check_collisions.h"

#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>


extern Keyboard_control Keyboard;
extern Mouse_control Mouse;


void init_3D(void) 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { -10.0, -10.0, -10.0, 0.0 };
    glShadeModel (GL_SMOOTH);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);    
    glMatrixMode(GL_PROJECTION);
}



void draw_3D(void) 
{
    register int n;
    float x0;
    float y0;
    float z0;

    for (n = 0; n < N_planets; n++)
    {
        if (Keyboard.O_key_pressed == 0)
	{
	    x0 = (float)(Mouse.move_right + planet[n].x) / (Mouse.zoom_in_or_out*AU*10); /* the viewport does not move, what 'moves' are the coordinates of CB's */
            y0 = (float)(Mouse.move_up + planet[n].y) / (Mouse.zoom_in_or_out*AU*10);
            z0 = (float)(Mouse.move_up + planet[n].z) / (Mouse.zoom_in_or_out*AU*10);
        }
	else
	{
            x0 = (float)(planet[n].x - planet[Keyboard.O_key_pressed].x) / (Mouse.zoom_in_or_out*AU*10); /* the view gets centered on the planet with index O_key_pressed */
            y0 = (float)(planet[n].y - planet[Keyboard.O_key_pressed].y) / (Mouse.zoom_in_or_out*AU*10);
            z0 = (float)(planet[n].z - planet[Keyboard.O_key_pressed].z) / (Mouse.zoom_in_or_out*AU*10);
	}
        
	float a0 = 0.003 + sqrt(planet[n].m / 3.285E23) / 1500000;   // Radius of CB's is sqrt() of their mass
        float b0 = 0.002 + sqrt(planet[n].m / 3.285E23) / 2250000;
        glLoadIdentity();
        gluPerspective(100,1,0.1,100);
        gluLookAt (-0.0, -0.10, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); /* viewing transformation  */
        glTranslatef(x0, y0, z0);
        glutSolidSphere (0.02, 30, 30);
    }
    
    glFlush();
    glutSwapBuffers();   
}



void draw(void) 
{
    register int n;
    float x0;
    float y0;
	
    for (n = 0; n < N_planets; n++)
    {
        if (Keyboard.O_key_pressed == 0)
	{
	    x0 = (float)(Mouse.move_right + planet[n].x) / (Mouse.zoom_in_or_out*AU*10); //the viewport does not move, what 'moves' are the coordinates of CB's
            y0 = (float)(Mouse.move_up + planet[n].y) / (Mouse.zoom_in_or_out*AU*10);
        }
	else
	{
            x0 = (float)(planet[n].x - planet[Keyboard.O_key_pressed].x) / (Mouse.zoom_in_or_out*AU*10); //the view gets centered on the planet with index O_key_pressed
            y0 = (float)(planet[n].y - planet[Keyboard.O_key_pressed].y) / (Mouse.zoom_in_or_out*AU*10);
	}
        
	float a0 = 0.003+sqrt(planet[n].m / 3.285E23) / 1500000;   // Radius of CB's is sqrt() of their mass
        float b0 = 0.002+sqrt(planet[n].m / 3.285E23) / 2250000;
	
	glBegin(GL_POLYGON); 	            
        glColor3f(planet[n].color.red, planet[n].color.green, planet[n].color.blue); /* Color of the planet */      
        glVertex2f( x0+a0, y0);  
        glVertex2f( x0+b0,y0+b0);
        glVertex2f( x0,y0+a0);
        glVertex2f( x0-b0,y0+b0);
        glVertex2f( x0-a0,y0);
        glVertex2f( x0-b0,y0-b0);
        glVertex2f( x0,y0-a0);
        glVertex2f( x0+b0,y0-b0);
        glVertex2f( x0+a0,y0);
        glEnd();
    } 
}




void display(void) 
{
    static int dummy_k = 0;
    Energy system_Energy;
    Momentum system_Momentum;
	 
    system_Energy.KE = 0;
    system_Energy.PE = 0;
    system_Momentum.linear_x = 0;
    system_Momentum.linear_y = 0;
    system_Momentum.linear_z = 0;
	
    if (Keyboard.Pause == 0)
    {
        iteration++;
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
        
        if (iteration == 1)
        {		
            glClear(GL_COLOR_BUFFER_BIT);
        }
        
        if (iteration-dummy_k == 200)
        {
            calculate_energies(&system_Energy);
            calculate_momentum(&system_Momentum);     
            draw();
            glFlush();  
            /* Output time (1 year = 31557600 seconds), number of celestial bodies and kinetic, potential, total energies and several linear and angular momenta */
            printf("Time(Years)= %.4f, Number of CB's = %i,  K(J)= %.2E,  P(J)= %.2E, K+P(J)= %.2E\n",iteration*h/31557600, N_planets,system_Energy.KE,system_Energy.PE,system_Energy.KE+system_Energy.PE);
            printf("LMx = %.2E, LMy = %.2E, LMz = %.2E, AMx = %.2E, AMy = %.2E, AMz = %.2E\n",system_Momentum.linear_x,system_Momentum.linear_y,system_Momentum.linear_z,system_Momentum.angular_x,system_Momentum.angular_y,system_Momentum.angular_z);
            printf("\n");
	    dummy_k = iteration;
            	 
            if (Keyboard.P_key_pressed == 0)
            {
	        glClear(GL_COLOR_BUFFER_BIT);
            }   
        }
   
        switch(Number_of_threads)
        {
	    case 4:	   
                update_motion_parallel();
                break;
            case 1:
                update_motion();
                break;    
        }
        check_collisions();
    }
    glutPostRedisplay();
}


void display_3D(void) 
{    
    init_3D();
    static int dummy_k = 0;	
    Energy system_Energy;
    Momentum system_Momentum;
			
    system_Energy.KE = 0;
    system_Energy.PE = 0;
    system_Momentum.linear_x = 0;
    system_Momentum.linear_y = 0;
    system_Momentum.linear_z = 0;
	
    if (Keyboard.Pause == 0)
    {
        iteration++;
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
        
        if (iteration == 1)
	{		
            glClear(GL_COLOR_BUFFER_BIT);
        }
  
        if (iteration-dummy_k == 500)
	{
            calculate_energies(&system_Energy);
            calculate_momentum(&system_Momentum);
            draw_3D();
            glFlush();  
            //Output time (1 year = 31557600 seconds), number of celestial bodies and kinetic, potential and total energies
            printf("Time(Years)= %.4f, Number of CB's = %i,  K(J)= %.2E,  P(J)= %.2E, K+P(J)= %.2E\n",iteration*h/31557600, N_planets,system_Energy.KE,system_Energy.PE,system_Energy.KE+system_Energy.PE);
            printf("LMx = %.2E, LMy = %.2E, LMz = %.2E, AMx = %.2E, AMy = %.2E, AMz = %.2E\n",system_Momentum.linear_x,system_Momentum.linear_y,system_Momentum.linear_z,system_Momentum.angular_x,system_Momentum.angular_y,system_Momentum.angular_z);
            printf("\n");
         
            dummy_k = iteration;	  
            if (Keyboard.P_key_pressed == 0)
	    {
                glClear(GL_COLOR_BUFFER_BIT);
	    }   
        }
   
        switch(Number_of_threads)
	{
	    case 4:	   
                update_motion_parallel();
                break;
            case 1:
                update_motion();
                break;    
        }	    
        check_collisions();
    }	
    glutPostRedisplay();
}




void process_keys(unsigned char key, int abba, int baba) 
{
    switch (key)
    {    
        case 27 :      
            break;
        case 112 : /* 'p' key - enter Path mode */
            if (Keyboard.P_key_pressed == 1)
	    {
                Keyboard.P_key_pressed = 0;
	    }
            else
	    {
                Keyboard.P_key_pressed = 1;
	    } 
            break;
       
       case 111: /* 'o' key - enter Origin mode */
           glClear(GL_COLOR_BUFFER_BIT);
           if (Keyboard.O_key_pressed < N_planets)
	   {
               Keyboard.O_key_pressed++;
	   }
           else
	   {
               Keyboard.O_key_pressed = 0;
	   } 
           break;
       
       case 105 : /* 'i' key - zoom in */
           glClear(GL_COLOR_BUFFER_BIT);
           Mouse.zoom_in_or_out = Mouse.zoom_in_or_out / 1.5;
           h = h / 2; /* Changes time step */
           break;
       
       case 117 : /* 'u' key - zoom out */
           glClear(GL_COLOR_BUFFER_BIT);
           Mouse.zoom_in_or_out = Mouse.zoom_in_or_out*1.5;
           h = h * 2; /* Changes time step */
           break;
       
       case 32 : /* 'spacebar' key - pause simulation */
           if (Keyboard.Pause == 1)
	   {
               Keyboard.Pause = 0;
	   }
           else
	   {
               Keyboard.Pause = 1;
	   }
           break;
    }
}


void process_mouse(int button, int state, int x, int y)
{  
    /* This section is for translation. the coordinates x,y are converted to captured by the mouse at the clicks are converted to distance to offset the graphics */
    if ((state == GLUT_DOWN) && (button == GLUT_MIDDLE_BUTTON))
    {
        if (Keyboard.O_key_pressed == 0) /* Execute the mouse translation only if we are not in 'fixed view' mode */
	{    
	    glClear(GL_COLOR_BUFFER_BIT);
            Mouse.xtemp = x;
            Mouse.ytemp = y;
	}
	else
	{
            printf("Exit 'Fixed view mode' by pressing the 'o' key \n");
	}
    }
    else if ((state == GLUT_UP) && (button == GLUT_MIDDLE_BUTTON))
    {
        if (Keyboard.O_key_pressed == 0)
	{   
	    glClear(GL_COLOR_BUFFER_BIT);
	    Mouse.move_right = Mouse.move_right - 0.02*(float)(Mouse.xtemp - x)*AU*Mouse.zoom_in_or_out;
	    Mouse.move_up = Mouse.move_up + (0.02*(float)(Mouse.ytemp - y))*AU*Mouse.zoom_in_or_out;
	}
    }		   
	//next section is for zoom in and zoom out
    else if (button == 3)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        Mouse.zoom_in_or_out=Mouse.zoom_in_or_out / 1.1;
    }
    else if(button == 4)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        Mouse.zoom_in_or_out=Mouse.zoom_in_or_out * 1.1;
    }
}


void process_mouse_motion(int x, int y)
{ 
    if (Keyboard.O_key_pressed == 0)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        Mouse.move_right = Mouse.move_right - 0.02*(float)(Mouse.xtemp-x)*AU*Mouse.zoom_in_or_out;
        Mouse.move_up = Mouse.move_up + 0.02*(float)(Mouse.ytemp-y)*AU*Mouse.zoom_in_or_out;
        Mouse.xtemp = x;
        Mouse.ytemp = y;
    }
}

