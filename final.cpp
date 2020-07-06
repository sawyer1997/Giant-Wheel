#include<stdlib.h>
#include<GL/glut.h>
#include<stdio.h>
#include<iostream>
#include<math.h>
using namespace std;
void display();
void draw_gwheel();
void draw_wagon();
void draw_seat();
void draw_gate();
void place_camera(int);
void set_material(int material);

const int SKY_FRONT=0,SKY_RIGHT=1,SKY_LEFT=2,SKY_BACK=3,SKY_UP=4,SKY_DOWN=5, GWHEEL_RING=3,GWHEEL_TROLLEY=4, GWHEEL_TOP=5;
int sp=-1,incr=0,gate=0,dir=0,change=0,dirchange=0,num=12,num2=8,prevx=0,camw=0, background=0, gw=0, gwheel_ring_color=0, gwheel_trolley_color=0,ground = 0;
float tx=0;
GLint skybox[6], grass,help=0,x_r=0, y_r=0, z_r=0;
GLfloat viewer[3] = {1.0f, 0.0f, 0.0f},camera[3] = {0.0f, 0.0, 0.0};
GLdouble curr=0, prev=0,gw_spin = 0.0,angle=0.0,gw_width = 8.0, gw_radius=45.0,gw_x=-180.0, gw_y=50.0, gw_z=220.0,gy=0,movcord[3]={-150,-10,200};

void draw_cyl(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions);

void set_material(int m)  // To Set Color and light to objects
{
	if(m==0)
	{
		float materialGrey[]={0.8,0.8,0.8},materialWhite[]={0.2,0.2,0.2};
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,materialGrey);
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,materialWhite);
	}

	if(m==GWHEEL_RING)
	{	float materialColours[][3]={{0.8,0.8,0.0},{0.33,0.33,0.33}, {0.9,0.4,0.4} ,{0.5450,0.27,0}, {0.36,0.36,0.36}},materialLightBr[][3]={{0,0,0},{0.3,0.3,0.3},{0,0,0},{0,0,0},{0.0,0.0,0.0}};
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,materialColours[gwheel_ring_color]);
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,materialLightBr[gwheel_ring_color]);
	}
	if(m==GWHEEL_TROLLEY)
	{	float materialColours[][3]={{0.2,0.2,0.8},{0.8,0.8,0.0},{0.33,0.33,0.33}, {0.9,0.4,0.4} ,{0.5450,0.27,0}},materialLightBr[]={0.1,0.1,0.1};
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,materialColours[gwheel_trolley_color]);
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,materialLightBr);
	}

}

GLuint LoadBMP(const char *fileName)  // Funtion To load image into a GLuint
{
	FILE *file;
	unsigned char header[54],*data;
	unsigned int dataPos,size,width, height;
	file = fopen(fileName, "rb");
	fread(header, 1, 54, file);				//Windows BMP begin with 54 byte header
	dataPos		= *(int*)&(header[0x0A]);	//dec10, Actual BMP data
	size		= *(int*)&(header[0x22]);	//dec34, BMP Size
	width		= *(int*)&(header[0x12]);	//dec18, Image Width
	height		= *(int*)&(header[0x16]);	//dec22, Image Height
	if (size == NULL)
		size = width * height * 3;
	if (dataPos == NULL)
		dataPos = 54;
	data = new unsigned char[size];
	fread(data, 1, size, file);
	fclose(file);
	GLuint texture;
	glGenTextures(1, &texture);				//Generate (allocate) 1 texture name
	glBindTexture(GL_TEXTURE_2D, texture);	//Bind the 2D texture

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	//MAG filter
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	//MIN filter

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data); //target, level, internalFormat, width, height,border, format, type, data 
	return texture;
}

void initSky()  // function to load the background image of sky and ground image of grass into an array
{
	glShadeModel(GL_SMOOTH);							
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				
	glClearDepth(1.0f);									
	glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	
	if(background==0 and ground == 0)                          // 1st background
	{	skybox[SKY_DOWN] = LoadBMP("BMP11/down.bmp");
		skybox[SKY_FRONT] = LoadBMP("BMP11/front.bmp");
		skybox[SKY_BACK] = LoadBMP("BMP11/back.bmp");
		skybox[SKY_RIGHT] = LoadBMP("BMP11/right.bmp");
		skybox[SKY_LEFT] = LoadBMP("BMP11/left.bmp");
		skybox[SKY_UP] = LoadBMP("BMP11/up.bmp");
		grass=LoadBMP("BMP11/grass_1.bmp");
	}
	else if(background == 0 and ground == 1)
	{
		skybox[SKY_DOWN] = LoadBMP("BMP11/down.bmp");
		skybox[SKY_FRONT] = LoadBMP("BMP11/front.bmp");
		skybox[SKY_BACK] = LoadBMP("BMP11/back.bmp");
		skybox[SKY_RIGHT] = LoadBMP("BMP11/right.bmp");
		skybox[SKY_LEFT] = LoadBMP("BMP11/left.bmp");
		skybox[SKY_UP] = LoadBMP("BMP11/up.bmp");
		grass=LoadBMP("BMP11/grass_3.bmp");
	}
	else if(background==1 and ground == 0)       // 2nd background
	{
		skybox[SKY_DOWN] = LoadBMP("BMP12/down.bmp");
		skybox[SKY_FRONT] = LoadBMP("BMP12/front.bmp");
		skybox[SKY_BACK] = LoadBMP("BMP12/back.bmp");
		skybox[SKY_RIGHT] = LoadBMP("BMP12/right.bmp");
		skybox[SKY_LEFT] = LoadBMP("BMP12/left.bmp");
		skybox[SKY_UP] = LoadBMP("BMP12/up.bmp");
		grass=LoadBMP("BMP12/grass_1.bmp");
	}
	else if(background == 1 and ground == 1)
	{
		skybox[SKY_DOWN] = LoadBMP("BMP12/down.bmp");
		skybox[SKY_FRONT] = LoadBMP("BMP12/front.bmp");
		skybox[SKY_BACK] = LoadBMP("BMP12/back.bmp");
		skybox[SKY_RIGHT] = LoadBMP("BMP12/right.bmp");
		skybox[SKY_LEFT] = LoadBMP("BMP12/left.bmp");
		skybox[SKY_UP] = LoadBMP("BMP12/up.bmp");
		grass=LoadBMP("BMP12/grass_3.bmp");
	}
}

void initLights()    // To set lights
{

	GLfloat whiteSpecularMaterial[] = {1.0,1.0,1.0},light_post0[]={0.0,0.0,10.0,1.0},whiteSpecularLight[] = {1.0, 1.0, 1.0},blackAmbientLight[] = {0.3, 0.3, 0.3},whiteDiffuseLight[] = {1.0, 1.0, 1.0},mShininess[] = {50},twoModel[]={GL_TRUE}; 
	glEnable (GL_DEPTH_TEST);
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, whiteSpecularLight);
   	glLightfv(GL_LIGHT0, GL_AMBIENT, blackAmbientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteDiffuseLight);
	glLightfv(GL_LIGHT0, GL_POSITION, light_post0);
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, twoModel);
 	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whiteSpecularMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mShininess);
}

void initLightsforGW(){

	GLfloat light_post1[]={0.0,50.0,0.0,1.0},whiteSpecularLight[] = {0.5, 0.5, 0.5},blackAmbientLight[] = {0.3, 0.3, 0.3}, whiteDiffuseLight[] = {0.6, 0.6, 0.6}, mShininess[] = {128},twoModel[]={GL_TRUE};
	glEnable (GL_DEPTH_TEST);
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, whiteSpecularLight);
   	glLightfv(GL_LIGHT1, GL_AMBIENT, blackAmbientLight);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, whiteDiffuseLight);
	glLightfv(GL_LIGHT1, GL_POSITION, light_post1);
}

void Draw_Skybox(float x, float y, float z, float width, float height, float length){  // function to map images of sky stored in skybox array to distant quadrilaterals 

	glDisable(GL_DEPTH_TEST);
	x = x - width  / 2;
	y = y - height / 2;
	z = z - length / 2;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,skybox[SKY_UP]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y+height, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y+height, z+length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height,	z+length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z); 
	glEnd();

	glBindTexture(GL_TEXTURE_2D,skybox[SKY_FRONT]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z+length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height,	z+length); 
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height,	z);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,skybox[SKY_BACK]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height,	z);	
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z+length); 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z+length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z);		
	glEnd();

	glBindTexture(GL_TEXTURE_2D,skybox[SKY_RIGHT]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height, z); 
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z);
	glEnd();
 
	glBindTexture(GL_TEXTURE_2D,skybox[SKY_LEFT]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z+length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height, z+length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height, z+length); 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z+length);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,skybox[SKY_DOWN]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z+length); 
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y,		z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y,		z+length);
	glEnd();
 	glDisable(GL_TEXTURE_2D);

}



void draw_ground()   // function to map image of grass to ground 
{		
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,grass);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(5000,-10,5000);
	glTexCoord2f(800.0f, 0.0f); glVertex3f(5000,-10,-5000);
	glTexCoord2f(800.0f, 800.0f); glVertex3f(-5000,-10,-5000);
	glTexCoord2f(0.0f, 800.0f); glVertex3f(-5000,-10,5000);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glLineWidth(5.0);
	glTranslatef(0.0, -2, 0.0);
	glTranslatef(0.0, 2, 0.0);
}


void drawSnowMan() {

	// glScalef(10.0,10.0,10.0);
	glColor3f(1.0f, 1.0f, 1.0f);


	// Draw Body
	glTranslatef(0.0f ,0.75f, 0.0f);
	glutSolidSphere(0.75f,20,20);

	// Draw Head
	glTranslatef(0.0f, 1.0f, 0.0f);
	glutSolidSphere(0.25f,20,20);

	// Draw Eyes
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.0f);
	glTranslatef(0.05f, 0.10f, 0.18f);
	glutSolidSphere(0.05f,10,10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.05f,10,10);
	glPopMatrix();

	// Draw Nose
	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidCone(0.08f,0.5f,10,2);
}

void draw_gwheel()  // function to draw Giant Wheel
{
	//num=12;
	GLUquadricObj *quadric=gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);
	
	glPushMatrix();
	
	initLightsforGW();
	// Drawing two supports

	GLfloat twoModel[]={GL_FALSE};
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, twoModel);
	set_material(GWHEEL_RING);
	glTranslatef(0.0,0.0, gw_width);
	glRotatef(-gw_spin,0,0,1);
	draw_cyl(0,0,0,0,-55,20,1.5,8);
	glTranslatef(0.0,0.0, -gw_width*2);
	draw_cyl(0,0,0,0,-55,-20,1.5,8);
	glTranslatef(0.0,0.0, gw_width*2);
	glRotatef(gw_spin,0,0,1);
	glutSolidTorus(1.0, 35.0, 16, 64);

	twoModel[0]=GL_TRUE;
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, twoModel);
	gluDisk(quadric,0.0, 10.0, 10.0, 1);
	glTranslatef(0.0,0.0, -gw_width*2);
	glutSolidTorus(1.0, 35.0, 16, 64);
	gluDisk(quadric,0.0, 10.0, 10.0, 1);
	glTranslatef(0.0,0.0, gw_width); 

	twoModel[0]=GL_FALSE;
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, twoModel);

	for(int i=0; i<num; i++)     // function for each trolly
	{
		glEnable(GL_LIGHTING);	
		glPushMatrix();
		glRotatef(360*i/num, 0.0, 0.0, 1.0);
		glEnable(GL_DEPTH_TEST);
		glTranslatef(0.0,0.0, gw_width); 
		draw_cyl(0.0, 45.0, 0.0, 0.0, 45.0, -5.0, 1.0, 12);
		draw_cyl(0.0, 45.0, 0.0, 0.0, 2.0, 0.0, 1.0, 12);
		glTranslatef(0.0,0.0, -gw_width*2); 
		draw_cyl(0.0, 45.0, 0.0, 0.0, 2.0, 0.0, 1.0, 12);
		draw_cyl(0.0, 45.0, 0.0, 0.0, 45.0, 5.0, 1.0, 12);
		glTranslatef(0.0,0.0, gw_width); 
		glDisable(GL_LIGHTING);
		glTranslatef(0.0, -45.0, 0.0);
		glRotatef(-gw_spin-(360*i/num)-sin(gw_spin/10.0)*10.0, 0.0, 0.0, 1.0);
		set_material(GWHEEL_TROLLEY);
		draw_wagon();
		set_material(GWHEEL_RING);
		glTranslatef(0.0, 45.0, 0.0);
		glPopMatrix();

	}
	set_material(0);
	glPopMatrix();
	glDisable(GL_LIGHT1);
}

void draw_gwheel2()  // function to draw Giant Wheel
{
	//num=12;
	GLUquadricObj *quadric=gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);
	
	glPushMatrix();
	
	initLightsforGW();
	// Drawing two supports

	GLfloat twoModel[]={GL_FALSE};
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, twoModel);
	set_material(GWHEEL_RING);

	glTranslatef(0.0,0.0, gw_width);
	glRotatef(-gw_spin,0,0,1);
	draw_cyl(0,0,0,0,-55,20,1.5,8);

	glTranslatef(0.0,0.0, -gw_width*2);
	draw_cyl(0,0,0,0,-55,-20,1.5,8);
	glTranslatef(0.0,0.0, gw_width*2);

	glRotatef(gw_spin,0,0,1);

	glutSolidTorus(1.0, 35.0, 16, 64);

	twoModel[0]=GL_TRUE;
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, twoModel);
	gluDisk(quadric,0.0, 10.0, 10.0, 1);
	glTranslatef(0.0,0.0, -gw_width*2);
	glutSolidTorus(1.0, 35.0, 16, 64);
	gluDisk(quadric,0.0, 10.0, 10.0, 1);
	glTranslatef(0.0,0.0, gw_width); 

	twoModel[0]=GL_FALSE;
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, twoModel);

	for(int i=0; i<num2; i++)     // function for each trolly
	{
		glEnable(GL_LIGHTING);	
		glPushMatrix();
		glRotatef(360*i/num2, 0.0, 0.0, 1.0);
		glEnable(GL_DEPTH_TEST);
		glTranslatef(0.0,0.0, gw_width); 
		draw_cyl(0.0, 45.0, 0.0, 0.0, 45.0, -5.0, 1.0, 12);
		draw_cyl(0.0, 45.0, 0.0, 0.0, 2.0, 0.0, 1.0, 12);
		glTranslatef(0.0,0.0, -gw_width*2); 
		draw_cyl(0.0, 45.0, 0.0, 0.0, 2.0, 0.0, 1.0, 12);
		draw_cyl(0.0, 45.0, 0.0, 0.0, 45.0, 5.0, 1.0, 12);
		glTranslatef(0.0,0.0, gw_width); 
		glDisable(GL_LIGHTING);
		glTranslatef(0.0, -45.0, 0.0);
		glRotatef(-gw_spin-(360*i/num2)-sin(gw_spin/10.0)*10.0, 0.0, 0.0, 1.0);
		set_material(GWHEEL_TROLLEY);
		draw_wagon();
		set_material(GWHEEL_RING);
		glTranslatef(0.0, 45.0, 0.0);
		glPopMatrix();

	}
	set_material(0);
	glPopMatrix();
	glDisable(GL_LIGHT1);
}

void draw_wagon(){     // function to make wagon of giant wheel
	
	GLdouble wagon_size=5.0;
	double eqn[]={0.0, -1.0, 0.0, -3.5};
	glPushMatrix();
	glTranslatef(0.0, -5.0, 0.0);
	glEnable(GL_CLIP_PLANE0);
	glClipPlane(GL_CLIP_PLANE0, eqn);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glDisable(GL_CLIP_PLANE0);
	for(int c=0;c<4;c++)
	{
		// GLdouble eqn0[4] = {-2, 0.0, -3.0, 0.5};
		// glClipPlane(GL_CLIP_PLANE0, eqn0);

		// if (c ==0 || c==1  ) glEnable(GL_CLIP_PLANE0); // Clip points s.t. z > 0.25.
  //  		else glDisable(GL_CLIP_PLANE0);
		
		glRotatef((90/4.0), 0.0, 1.0, 0.0);
		glNormal3f(1.0,0.0,0.0);
		glutWireCube(wagon_size*2.0);

		// if (c == 0 || c ==3) glDisable(GL_CLIP_PLANE0);
	}
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	glScalef(2.0, 3.0, 2.0);
	set_material(0);
	draw_seat();
	draw_gate();
	set_material(GWHEEL_TROLLEY);
	glScalef(1/2.0,1/3.0,1/2.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glPopMatrix();
	glTranslatef(0.0, wagon_size, 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glutSolidCone(5.0, 5.0, 6,6);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, -2*wagon_size, 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	GLUquadricObj *quadric=gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluDisk(quadric, 0.0, 6.0, 16.0, 1.0);
	gluDeleteQuadric(quadric);
	glDisable(GL_LIGHTING);
	glDisable(GL_NORMALIZE);
	glPopMatrix();

}

void idle()
{
	if(dirchange == 1) {
		if(gw == 1) gw = -1;
		else if(gw == -1) gw = 1;
		dirchange = 0;
		}

	if(gw == 0) dir=0;
	if(sp == 1) {dir == 1 ? gw_spin += 1 : gw_spin -= 1; gate=1;}  // to change speed
	if(sp == 0) {gw_spin = 0;}
	if(sp == 2) {dir == 1 ? gw_spin += 2 : gw_spin -= 2;gate=1;}
	if(sp == 3) {dir == 1 ? gw_spin += 3 : gw_spin -= 3; gate=1;}
	

	if(gw == 1)
	{
		dir=1;
		gw_spin+=0.25;
		if(camw == 1)
		{
			movcord[0]=-gw_x +(gw_radius*sin(gw_spin*3.14/180.0)) + sin(gw_spin/10.0)-2;
		movcord[2]=gw_z;
		
		movcord[1]=-gw_y-(gw_radius*cos(gw_spin*3.14/180.0))+6;
		}
		else if(camw==2)
		{
		movcord[0]=-2*gw_x +(gw_radius*sin(gw_spin*3.14/180.0)) + sin(gw_spin/10.0)-2;
		movcord[2]=2*gw_z;
		
		movcord[1]=-gw_y-(gw_radius*cos(gw_spin*3.14/180.0))+6;
		}
	}
	if(gw == -1)
	{
		dir=2;
		gw_spin-=0.25;
		if(camw == 1)
		{
			movcord[0]=-gw_x +(gw_radius*sin(gw_spin*3.14/180.0)) + sin(gw_spin/10.0)-2;
		movcord[2]=gw_z;
		
		movcord[1]=-gw_y-(gw_radius*cos(gw_spin*3.14/180.0))+6;
		}
		else if(camw==2)
		{
		movcord[0]=-2*gw_x +(gw_radius*sin(gw_spin*3.14/180.0)) + sin(gw_spin/10.0)-2;
		movcord[2]=2*gw_z;
		
		movcord[1]=-gw_y-(gw_radius*cos(gw_spin*3.14/180.0))+6;
		}
	}
	if(change == 1 )
	{
		draw_gwheel();
		draw_gwheel2();
		change = 0;
		incr=0;
	}

	display();
}

void draw_gate() {  // function to draw gate of each trolley of giant wheel 

	if(gate == 1) {           // when giant wheel is moving gate must be closed
	glPushMatrix();
	//glEnable(GL_LIGHTING);
	glTranslatef(-2.0, 0.0, -2.0);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	glTranslatef(0.0, 0.5, 0.75);
	glTranslatef(1.25,0.5,0.0);
	glNormal3f(0.0,1.0,0.0);
	glScalef(1.0, 0.1, 1.5);
	glColor3f(1.0, 0.0, 0.0);
	glutWireCube(1.0);
	glRotatef(0.0, 90.0, 0.0, 2.0);
	glTranslatef(-1.0, 2.50, 0.0);
	glutWireCube(1.0);
	glTranslatef(-1.0, -2.50, 0.0);
	glutWireCube(1.0);

	}
	else if(gate == 0){  // when giant wheel is stoped gates are opened
	glPushMatrix();
	//glEnable(GL_LIGHTING);
	glTranslatef(-2.0, 0.0, -2.0);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	glTranslatef(0.0, 0.5, 0.75);
	glTranslatef(1.25,0.5,0.0);
	glNormal3f(0.0,1.0,0.0);
	glScalef(1.0, 0.1, 1.5);
	glColor3f(1.0, 0.0, 0.0);
	glutWireCube(1.0);
	glRotatef(90.0, 0.0, 0.0, 2.0);
	glTranslatef(-1.0, 2.50, 0.0);
	glutWireCube(1.0);
	glTranslatef(-1.0, -2.50, 0.0);
	glutWireCube(1.0);

	}

	glPopMatrix();

}
void draw_object()
{
	glPushMatrix();
	glTranslatef(-0.0,0.0,-1.0);
	glDisable(GL_LIGHTING);
	glColor3f(1.0,0.0,0.0);
	glutSolidCube(1.0);
	glTranslatef(0.0,0.0,1.25);
	glColor3f(.96,.80,0.69);
	glutSolidSphere(0.75,50.0,50.0);
	glTranslatef(0.0,0.0,-1.75);
	draw_cyl(0.0,0.0,0.0,0.5,0.0,0.0,1.0,80);
	glTranslatef(0.0,0.0,-1.75);
	glEnable(GL_LIGHTING);


	glPopMatrix();
}
void draw_seat()  //function to draw seat and object inside each trolley 
{

	glPushMatrix();
	glEnable(GL_LIGHTING);
	glTranslatef(1.0, 0.0, -1.0);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	glTranslatef(0.0, 0.5, 0.0);
	draw_cyl(-2.5,0.0,0.0,2.5,0.0,0.0,0.05,6);
	glTranslatef(0.0,-0.5,0.0);
	glNormal3f(0.0,1.0,0.0);
	glScalef(4.0, 0.1, 1.0);
	glutSolidCube(1.0);
	glScalef(1/4.0, 1/0.1, 1.0);
	glTranslatef(0.0, 0.5, -1.0);
	glRotatef(80.0, 1.0, 0.0, 0.0);
	glScalef(4.0, 0.1, 1.0);
	glNormal3f(0.0,1.0,0.0);
	glutSolidCube(1.0);
	glScalef(1/4.0, 1/0.1, 1.0);
	glPopMatrix();


	if(sp !=3)
	draw_object();
	

}


void drawText(char *string,float x,float y,float z) 
{  
	char *c;
	glRasterPos3f(x, y,z);
	for (c=string; *c != '\0'; c++) 
	{
		if(*c=='\n')
		glRasterPos3f(x, y-0.05,z);
		else
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
	}
}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor4f(1.0,1.0,1.0,1.0);
	glLoadIdentity();	
	gluLookAt(viewer[0], viewer[1], viewer[2],camera[0], camera[1], camera[2],0, 1, 0);
	if(help)
	{
		glPushMatrix();
		glColor4f(1.0,1.0,0.0,1.0);
		glDisable(GL_LIGHTING);
		drawText("Amusement Park", 0, 0.55, 0.15);
		drawText("Welcome to the amusement park. \nUse the following keys for movement and action controls:", 0,0.4,0.82);
		drawText("* LEFT and RIGHT arrow: Look Around 360 degrees", 0,0.30,0.82);
		drawText("* UP and DOWN arrow: Move forward and backward", 0,0.25,0.82);
		drawText("* b : Change background", 0,0.2,0.82);
		drawText("* h : Show/hide help menu", 0,0.1,0.82);
		drawText("* g : Start/stop Giant Wheel", 0,0.05,0.82);
		drawText("* w/s : Change camera position", 0,-0.05,0.82);
		drawText("Above actions can also be performed from the right-click context menu", 0,-0.25,0.82);
		glEnable(GL_LIGHTING);
		glPopMatrix();
	}
	else
	{
	    // glScalef(1.5f, 1.5f, 1.0f);  For Zooming

		// Drawing Sky
		glRotatef(x_r, 0, 1, 0);
		Draw_Skybox(viewer[0]+(0.05*movcord[0]),viewer[1]+(0.05*movcord[1]),viewer[2]+(0.05*movcord[2]),250,250,250);
		
		// Drawing Ground
		glTranslatef(movcord[0],movcord[1],movcord[2]);
		draw_ground();

		// Drawing Giant Wheel
		glPushMatrix();
		glTranslatef(gw_x, gw_y, -gw_z);
		glRotatef(gw_spin, 0.0, 0.0, 1.0);
		draw_gwheel();
		glPopMatrix();

		// Drawing 2nd Giant Wheel

		glPushMatrix();
		glTranslatef(2*gw_x, gw_y, 2*-gw_z);
		glRotatef(gw_spin, 0.0, 0.0, 1.0);
		draw_gwheel2();
		glPopMatrix();

		if(sp == 3)
		{
			for(int i = 0 ; i < num + num2 ; i++)
			{
						int j = i%5;
						glPushMatrix();
						glTranslatef(10*i-0.0,0.0,i*j-1.0);
						// glDisable(GL_LIGHTING);
						glColor3f(1.0,0.0,0.0);
						glutSolidCube(1.0);
						glTranslatef(0.0,0.0,1.25);
						glColor3f(.96,.80,0.69);
						glutSolidSphere(0.75,50.0,50.0);
						glTranslatef(0.0,0.0,-1.75);
						draw_cyl(0.0,0.0,0.0,0.5,0.0,0.0,1.0,80);
						glTranslatef(0.0,0.0,-1.75);
						// glEnable(GL_LIGHTING);
			
			
						glPopMatrix();
			}
		}
	}
	glutSwapBuffers();

}

void displayReshape(int width,int height)
{
	glViewport(0,0,width,height);						
	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();
	gluPerspective(65,(GLfloat)width/(GLfloat)height,0.01f,1000.0f);
	glMatrixMode(GL_MODELVIEW);						
	glLoadIdentity();			
}


void renderCylinder(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions,GLUquadricObj *quadric)
{

	float vx = x2-x1,vy = y2-y1,vz = z2-z1;
	if(vz == 0) vz = .0001;
	float v = sqrt( vx*vx + vy*vy + vz*vz );
	float ax = 57.2957795*acos( vz/v );
	if ( vz < 0.0 )
	    ax = -ax;
	float rx = -vy*vz;
	float ry = vx*vz;
	glPushMatrix();
	glTranslatef( x1,y1,z1 );
	glRotatef(ax, rx, ry, 0.0);
	gluQuadricOrientation(quadric,GLU_OUTSIDE);
	gluCylinder(quadric, radius, radius, v, subdivisions, 1);
	gluQuadricOrientation(quadric,GLU_INSIDE);
	gluDisk( quadric, 0.0, radius, subdivisions, 1);
	glTranslatef( 0,0,v );
	gluQuadricOrientation(quadric,GLU_OUTSIDE);
	gluDisk( quadric, 0.0, radius, subdivisions, 1);
	glPopMatrix();

}
void draw_cyl(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions)
{
	GLUquadricObj *quadric=gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);
	renderCylinder(x1,y1,z1,x2,y2,z2,radius,subdivisions,quadric);
	gluDeleteQuadric(quadric);
}

void windowSpecial(int key,int x,int y)
{
	
	if(key==GLUT_KEY_UP)
	{
		movcord[0]+=5*cos(-1*x_r*3.14/180.0);
		movcord[2]+=5*sin(1*x_r*3.14/180.0);
	}
	if(key== GLUT_KEY_DOWN)
	{
		movcord[0]-=5*cos(-1*x_r*3.14/180.0);
		movcord[2]-=5*sin(1*x_r*3.14/180.0);
	}
	if(key==GLUT_KEY_RIGHT) x_r+=3;
	if(key==GLUT_KEY_LEFT) x_r-=3;
	display();

}


void  kb(unsigned char key, int x, int y)
{   
	if(key=='w') movcord[1]--;
	if(key=='s'){
		if(movcord[1] < 9 ) 
			{movcord[1]++;}
	}
	display();
}

void handleMouse(int x,int y)
{
	if((x-prevx)>=0){
		incr=1;
		if(sp==0)sp=-1;
		if(dir == 0){
			gw = 1;
			gate = 1;
			change = 1;
			tx=1;
		}
		else if(dir == 1){
			gw_spin = gw_spin + 1.25;
			tx = 1.25;
		}
		else if(dir == 2){
			dirchange=1;
			gw_spin = gw_spin + 1.25;
			tx = 1.25;
		}
	}
	else{
		if(sp==0)sp=-1;
		incr=1;
		if(dir == 0){
			gw = -1;
			gate = 1;
			change =1;
			tx = 1;
		}
		else if(dir == 1){
			dirchange=1;
			gw_spin = gw_spin - 1.25;
			tx = -1.25;
		}
		else if(dir == 2){
			gw_spin = gw_spin - 1.25;
			tx = -1.25;
		}
	}
	prevx=x;
}


void place_camera(int action)
{

	camw=action;

	if(camw == 1)
	{
		movcord[0]=-gw_x +(gw_radius*sin(gw_spin*3.14/180.0)) + sin(gw_spin/10.0);
		movcord[2]=gw_z;
		movcord[1]=-gw_y-(gw_radius*cos(gw_spin*3.14/180.0))+6;	
	}
	if(camw==2)
	{	

		movcord[0]=-2*gw_x +(gw_radius*sin(gw_spin*3.14/180.0)) + sin(gw_spin/10.0);
		movcord[2]=2*gw_z;
		movcord[1]=-gw_y-(gw_radius*cos(gw_spin*3.14/180.0))+6;
	}
	if(camw==0)
		viewer[1]=viewer[2]=0.0;
	display();
}

void handle_gwheel(int action)
{
	if(action==0){
		if(sp==0)sp=-1;
		if(gw == 0){
			gw=1;
			gate =1;
			change =1;
		}
		else {
			sp=0;
			gw =0;
			gate =0;
 			change =1;
		}

	}
	else if(action == 1) {
		if(sp==0)sp=-1;
		if(gw == 0) {gw =1;gate =1;
			change =1;}
		else if(gw == 1) gw = -1;
		else if(gw == -1) gw = 1;
	}
	else if(action == 2)
	{
		if(num == 12)
		{
			num = 6;
			change = 1;
		}		
		else 
		{
			num = 12;
			change = 1;
		}
	}
	if(action == 3)
	{
		if(sp==-1)sp=0;
		if(sp==0) {sp =1; gate =1;
			change =1;}
		else if(sp ==1) {sp=2; gate =1;
			change =1;}
		else if(sp ==2) {sp=3; gate =1;
			change =1;

		}
		// if(sp == 3)
		// {

		// }
	}
	if(action == 4){
		if(sp==-1){sp=0; gate =0; gw=0;
			change =1;}
		if(sp==2) {sp =1; gate =1;
			change =1;}
		else if(sp ==1) {sp=0;gw=0; gate =0;
			change =1;}
		else if(sp ==3) {sp=2; gate =1;
			change =1;}
	}
	if(action == 5)
	{
		if(num2 == 8)
		{
			num2 = 4;
			change = 1;
		}		
		else 
		{
			num2 = 8;
			change = 1;
		}
	}
}


void menu(int action)
{
	if(action==0)
	{
		background=(background+1)%2;
		initSky();
	}
	if(action==1) help==1?help=0:help=1;
	if(action==2)	exit(0);
	if(action == 3)
	{
		ground=(ground+1)%2;
		initSky();
	}

}


void handle_gwheel_ring(int action)
{
	gwheel_ring_color=action;
}

void handle_gwheel_trolley(int action)
{
	gwheel_trolley_color=action;
}

void addMenu()
{
	int submenu1,submenu2,submenu21, submenu22;
	submenu1 = glutCreateMenu(place_camera);
	glutAddMenuEntry("Free Movement",0);
	glutAddMenuEntry("Inside Giant Wheel_1",1);
	glutAddMenuEntry("Inside Giant Wheel_2",2);
	submenu21 = glutCreateMenu(handle_gwheel_ring);
	glutAddMenuEntry("Yellow",0);
	glutAddMenuEntry("Silver",1);
	glutAddMenuEntry("Pale red",2);
	glutAddMenuEntry("Brown",3);
	glutAddMenuEntry("Grey",4);
	submenu22 = glutCreateMenu(handle_gwheel_trolley);
	glutAddMenuEntry("Blue",0);
	glutAddMenuEntry("Yellow",1);
	glutAddMenuEntry("Grey",2);
	glutAddMenuEntry("Pale red",3);
	glutAddMenuEntry("Brown",4);
	submenu2 = glutCreateMenu(handle_gwheel);
	glutAddMenuEntry("Stop/Start Giant Wheel",0);
	glutAddMenuEntry("Rotate Giant Wheel in Opposite direction",1);
	glutAddMenuEntry("Make 6/12 Wheels in GW_1",2);
	glutAddMenuEntry("Increase Speed",3);
	glutAddMenuEntry("Decrease Speed",4);
	glutAddMenuEntry("Make 4/8 Wheels in GW_2",5);
	glutAddSubMenu("Giant Wheel frame colour",submenu21);
	glutAddSubMenu("Giant Wheel trolley colour",submenu22);
	glutCreateMenu(menu);
	glutAddSubMenu("Camera Position",submenu1);
	glutAddSubMenu("Giant Wheel",submenu2);
	glutAddMenuEntry("Change Background",0);
	glutAddMenuEntry("Show/hide Help",1);
	glutAddMenuEntry("Quit",2);
	glutAddMenuEntry("Change Ground",3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}

int main(int argc, char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(800,600);
	glutCreateWindow("Amusement Park");
	initLights();
	initSky();
	glutDisplayFunc(display);
 	glutReshapeFunc(displayReshape);
 	glutKeyboardFunc(kb);
	glutMotionFunc(handleMouse);
	glutIdleFunc(idle);
	glutSpecialFunc(windowSpecial);
	addMenu();
	glutMainLoop();
	return 0;
}


