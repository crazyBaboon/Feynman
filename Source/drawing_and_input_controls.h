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

#ifndef _DRAWING_AND_INPUT_CONTROLS_H_
#define _DRAWING_AND_INPUT_CONTROLS_H_

void draw(void); 

void draw_3D(void);

void display(void);

void display_3D(void);

void process_keys(unsigned char key, int abba, int baba);

void process_mouse(int button, int state, int x, int y);

void process_mouse_motion(int x, int y);

/* Asturias */


#endif

