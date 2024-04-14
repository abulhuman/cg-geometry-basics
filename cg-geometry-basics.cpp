/*
	Team Members					ID
	1.Amanuel Almayheu           	ETS0105/11
	2.Adem Mohammed                 ETS0082/11
	3.Adonias Wondwosen             ETS0082/11
	4.Amanuel Abay                  ETS 0104/11
*/

#include <iostream>
#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

GLint r, xCenter, yCenter;

namespace CGLA {
	GLint x1, x2, y1, y2; // using namespaced global variables - x1, x2, y1, y2 - since they are already defined in math.h library
	void setPixel(int x, int y){
		glBegin(GL_POINTS);
		glVertex2i(x,y);
		glEnd();
		}
}
/** Draws x and y axes to show where we plot out lines or circles */
void drawCoordinates(void){	
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.8, 1.0, 0.8);
	glBegin(GL_LINES);
	glVertex3f(0, -100, 0.0);
	glVertex3f(0, 100, 0.25);
	glEnd();
	glFlush();
	glBegin(GL_LINES);
	glVertex3f(-100, 0, 0.0);
	glVertex3f(100, 0, 0.25);
	glEnd();
	glFlush();
}

// 
void directLine(void){
	drawCoordinates();
	// Calculate slope(m) and y-intercept(b) to find the line equation y = mx + b
	GLint m = (CGLA::y2 - CGLA::y1) / (CGLA::x2 - CGLA::x1), 
		  b = CGLA::y1 - m * CGLA::x1;
	GLint x ,y, y_, x_;
	
	// if the slope is negative, i.e., a declining line
	if(m <= 1){
		// for every integer value of x between and excluding CGLA::x1 (hence x = CGLA::x1+1) and CGLA::x2
		for(x = CGLA::x1 + 1; x < CGLA::x2; x++){
			// the correspondig value of y=m*x+b
			y = m * x + b;
			
			// perform simple calcualtion to avoid floating point
			y_ = floor(y + 0.5);
			
			// plot the point with the new coordinate
			CGLA::setPixel(x, y_);
			}
		} else {
			// if the slope is positive i.e., an inclining line
			// for every value of y between and excluding CGLA::y1 (hence y = CGLA::y1+1) and CGLA::y2
			for(y = CGLA::y1 + 1; y < CGLA::y2; y++){
				// the corresponding value of x from the equation y=mx+b; becomes x=(y-b)/m
				x = (y - b) / m;
				
				// perform simple calcualtion to avoid floating point
				x_= floor(x + 0.5);
				
				// plot the point with the new coordinate
				CGLA::setPixel(x_, y);
				}
			}
	glFlush();

}

void DDALine(void){
	drawCoordinates();
	GLint dy, dx, step, x, y, k, XIncrement, YIncrement;
	
	// horizontal difference between the points
	dx = CGLA::x2 - CGLA::x1;
	
	// vertical difference between the points
	dy = CGLA::y2 - CGLA::y1;
	
	// the difference(horizontal or vertical) with the greater 
	// magnitude determines the value of parameter `step`
	if(abs(dx) > abs(dy))
		 step = abs(dx);
	else step = abs(dy);
	
	// starting with (CGLA::x1, CGLA::y1)
	x = CGLA::x1;
	y = CGLA::y1;
	CGLA::setPixel(x, y);
	
	// XIncrement and YIncrement represent the offset needed at each step 
	// to generate the next pixel position along the line path
	XIncrement = dx / (GLint) step;
	YIncrement = dy / (GLint) step;
	
	// we loop through the above process `step` times
	// then plot the pixel
	for(k = 1; k <= step; k++) {
		x += XIncrement;
		y += YIncrement;	
		CGLA::setPixel(x, y);
	}
	
	glFlush();
}

void bresenhamsLine(void){
	drawCoordinates();
	GLint dx, dy, x, y, p;
	
	// horizontal difference between the points
	dx = CGLA::x2 - CGLA::x1;
	
	// vertical difference between the points
	dy = CGLA::y2 - CGLA::y1;

	x = CGLA::x1;
	y = CGLA::y1;
	
	// plot the the first pixel on (x1, y1)
	CGLA::setPixel(x, y);
	
	//  starting value of the decision parameter
	p = (2 * dy) - dx;
	
	// loop from 0 up to dx
	for(int k = 0; k < dx; k++){
		// if the decision variable is < 0, 
		// increment x, and plot the pixel with the new x-value and,
		// set the next decision variable to p(k+1) = P(k) + 2dy
		if(p < 0){
			x++;
			CGLA::setPixel(x, y);
			p += 2*dy;
		} else {
			// if the decision variable is >=0,
			// increment both x and y, plot the pixel with the new coordiantes
			// set the next decision variable to p(k+1) = p(k) + 2dy - 2dx
			x++; y++;
			CGLA::setPixel(x, y);
			p += (2*dy) - (2*dx);
		}
	}
	glFlush();
}

// reflect the initial points (pixels) drawn from 90° to 45° along
// all the 8-axes of symmetry around each 45° axis given 
// the initial points coordinates and the circle's center coordinates
void drawCircle(int xCenter, int yCenter, int x, int y)
{
    CGLA::setPixel(xCenter + x, yCenter + y);
    CGLA::setPixel(xCenter - x, yCenter + y);
    CGLA::setPixel(xCenter + x, yCenter - y);
    CGLA::setPixel(xCenter - x, yCenter - y);
    CGLA::setPixel(xCenter + y, yCenter + x);
    CGLA::setPixel(xCenter - y, yCenter + x);
    CGLA::setPixel(xCenter + y, yCenter - x);
    CGLA::setPixel(xCenter - y, yCenter - x);
}

// take the coordinates of the circle's center and the radius from the global variables
void bresenhamsCircle(void){
	drawCoordinates();
	GLint x = 0, y = r, d = 3 - (2*r);
	
	while(x <= y){
		// first plot a pixel whose position has
		// already been determined starting with (0, r)
		drawCircle(xCenter, yCenter, x, y);
		
		// test the decision variable to update it properly
		if(d < 0)
		// if d < 0, pixel T is chosen and d is updated accordingly
			d += ((4 * x)+ 6);
		else {
		// if d >= 0, pixel S is chosen and d is updated accordingly and
		// y is decremented, as we are on a decline (90° to 45°)
			d += (4 * (x - y) + 10);
			y--;
		}
		// increment x, as we are on a decline (90° to 45°)
		x++;
	}
	glFlush();
}

// take the coordinates of the circle's center and the radius from the global variables
void midPointCircle(void){
	drawCoordinates();
	// calculate the initial decision parameter from the radius
	int x = 0, y = r, p = 1 - r;
	
	// first plot a pixel whose position has
	// already been determined starting with (0, r)
	drawCircle(xCenter, yCenter, x, y);
	while(x <= y){
		// increment x, as we are on a decline (90° to 45°)
		x++;
		
		// if p < 0, pixel T is chosen and p is updated accordingly
		if(p < 0)
			p+= 2 * x + 3;
		else {
			// if p >= 0, pixel S is chosen and p is updated accordingly and
			// y is decremented, as we are on a decline (90° to 45°)
			y--;
			p += 2 * (x - y) + 5;
		}
		drawCircle(xCenter, yCenter, x, y);
	}
	glFlush();
}

void myInit() {
	glClearColor(.2, .2, .2, 0.2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
	gluOrtho2D(-100,100,-100,100);

}

int main(int iArgc, char** cppArgv) {
	glutInit(&iArgc, cppArgv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(1080, 100);
	glutCreateWindow("Computer Graphics Lab Assignment");
	myInit();
	
	top:
	
	std::cout << "____________________________________________________________________________\n\n";
	std::cout << "This program plots a line or a circle \nbased on a set of given scan conversion algorithms of your choosing.\n";
	std::cout << "____________________________________________________________________________\n";
	std::cout << "Choose\n 1. Line\n 2. Circle\n____________________________________________________________________________\n\n";
	std::cout << "\tType the number corresponding to your choice to select it.\n";
	std::cout << "____________________________________________________________________________\nChoice: ";
	int choiceFlag = 0;
	std::cin >> choiceFlag;
	switch(choiceFlag){
		case 1:
			lineTop:
			system("cls");
			std::cout << "What algorithm do you want to use?" << std::endl;
			std::cout << "Choose\n 1. Direct Line Algorithm\n 2. DDA Algorithm\n 3. Bresenham\'s Line Algorithm";
			std::cout << "\n____________________________________________________________________________\n\n";
			std::cout << "\tType the number corresponding to your choice to select it.\n";
			std::cout << "____________________________________________________________________________\nChoice: ";
			std::cin >> choiceFlag;
			switch(choiceFlag){
				case 1:
					std::cout << "(x1, y1) = "; std::cin >> CGLA::x1 >> CGLA::y1;
					std::cout << "(x2, y2) = "; std::cin >> CGLA::x2 >> CGLA::y2;
					glutDisplayFunc(directLine);
					glutMainLoop();
					break;
				case 2:
					std::cout << "(x1, y1) = "; std::cin >> CGLA::x1 >> CGLA::y1;
					std::cout << "(x2, y2) = "; std::cin >> CGLA::x2 >> CGLA::y2;
					glutDisplayFunc(DDALine);
					glutMainLoop();
					break;
				case 3:
					std::cout << "(x1, y1) = "; std::cin >> CGLA::x1 >> CGLA::y1;
					std::cout << "(x2, y2) = "; std::cin >> CGLA::x2 >> CGLA::y2;
					glutDisplayFunc(bresenhamsLine);
					glutMainLoop();
					break;
				default:
					std::cout << "\nInvalid Choice :). Please Try Again.\n";
					system("timeout 3");
					goto lineTop;
			}
			break;
		case 2:
			circleTop:
			system("cls");
			std::cout << "What algorithm do you want to use?" << std::endl;
			printf("Choose\n 1. Bresenham\'s Circle Algorithm\n 2. Mid-point Circle Algorithm");
			printf("\n____________________________________________________________________________\n\n");
			printf("\tType the number corresponding to your choice to select it.\n");
			printf("____________________________________________________________________________\nChoice: ");
			std::cin >> choiceFlag;
			switch(choiceFlag){
				case 1:
					std::cout << "(xCenter, yCenter) = "; std::cin >> xCenter >> yCenter;
					std::cout << "radius(r) = "; std::cin >> r;
					glutDisplayFunc(bresenhamsCircle);
					glutMainLoop();
					break;
				case 2:
					std::cout << "(xCenter, yCenter) = "; std::cin >> xCenter >> yCenter;
					std::cout << "radius(r) = "; std::cin >> r;
					glutDisplayFunc(midPointCircle);
					glutMainLoop();
					break;
				default:
					std::cout << "\nInvalid Choice :). Please Try Again.\n";
					system("timeout 3");
					goto circleTop;
			}
			break;
		default:
			system("cls");
			std::cout << "Invalid Choice :). Please Try Again.\n";
			goto top;
	}
}