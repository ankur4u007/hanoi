
#include <GL/glut.h>
#include <stdio.h>
#include<conio.h>
#include<stdlib.h>
double WIDTH = 1280;
double HEIGHT = 800;
int frun=0;
char printmsg[5];
    
GLboolean motion = GL_TRUE;
GLboolean back_wall = GL_TRUE;
//GLint xangle = 0, yangle = 25;
GLint xangle = -1, yangle = -1;
GLint xlangle = 0, ylangle = 25;

float rotx=0,roty=0;
#define other(i,j) (6-(i+j))
#define wallz -(WIDTH/2)
#define DISK_HEIGHT 20
int NUM_DISKS =7;
int SPEED=2;
int SHIFTY=350;
float SHIFTZ=1;
int xdisk[10],newx[10];
int ydisk[10],newy[10];
int pause=0;
int pending=0;
int a,b,c,stop,countrpt;


#define CONE NUM_DISKS+1
#define WALL CONE + 1
#define HANOI_SOLVE 0
#define HANOI_QUIT 1
#define HANOI_LIGHTING 2
#define HANOI_WALL 3
#define HANOI_FOG 4
#define HANOI_SPEEDUP 5
#define HANOI_SPEEDDOWN 6

GLfloat lightOneDirection[] =
{0, 0, -1};
GLfloat lightOnePosition[] =
{320, 450, 480, 1};
GLfloat lightOneColor[] =
{1.0, 1.0, 0.5, 1.0};

GLfloat lightTwoDirection[] =
{0, 0, -1};
GLfloat lightTwoPosition[] =
{620, 450, 480, 1};
GLfloat lightTwoColor[] =
{ 0.8, 0.3 ,.5 ,1.0};

GLfloat lightthreeDirection[] =
{0, 0, -1};
GLfloat lightthreePosition[] =
{970, 450, 480, 1};
GLfloat lightthreeColor[] =
{1.0, 1.0, 0.5, 1.0};

GLfloat lightZeroPosition[] =
{400, 450, 580, 1};
GLfloat lightZeroColor[] =
{.5, .5, .5, .3};

GLfloat diskColor0[] = {1.0, 0.0, 0.0, .9}; 
GLfloat diskColor1[] = {0.0, 1.0, 0.0, .9}; 
GLfloat diskColor2[] = {0.0, 0.0, 1.0, .9};
GLfloat diskColor3[] = {1.0, 1.0, 1.0, .9};
GLfloat diskColor4[] = {1.0, 1.0, 0.0, .9};
GLfloat diskColor5[] = {0.0, 1.0, 1.0, .9};
GLfloat diskColor6[] = {1.0, 0.0, 1.0, .9};
GLfloat diskColor7[] = {0.0, 0.0, 0.0, .9};

GLfloat poleColor[] = {1.0, 0.0, 1.0, .8};

typedef struct stack_node {
  int size;
  struct stack_node *next;
} stack_node;

typedef struct stack {
  struct stack_node *head;
  int depth;
} stack;

stack poles[4];

void 
push(int which, int size)
{
  stack_node *new1 = new stack_node;
  if (!new1) {
    fprintf(stderr, "out of memory!\n");
    exit(-1);
  }
  
  new1->size = size;
  new1->next = poles[which].head;
  poles[which].head = new1;
  poles[which].depth++;
}

int 
pop(int which)
{
  int retval = poles[which].head->size;
  stack_node *temp = poles[which].head;
  poles[which].head = poles[which].head->next;
  poles[which].depth--;
  free(temp);
  return retval;
}

typedef struct move_node {
  int t, f;
  struct move_node *next;
  struct move_node *prev;
} move_node;

typedef struct move_stack {
  int depth;
  struct move_node *head, *tail;
} move_stack;

move_stack moves;

void 
init()
{
  int i;
  
  for (i = 0; i < 4; i++) {
    poles[i].head = NULL;
    poles[i].depth = 0;
  }
  moves.head = NULL;
  moves.tail = NULL;
  moves.depth = 0;


 
  for (i = 1; i <= NUM_DISKS; i++) {
    glNewList(i, GL_COMPILE);
    
    {
  
      glutSolidTorus(DISK_HEIGHT / 2, 5 * i, 15, 15);
       
    }
    glEndList();
  }
  glNewList(CONE, GL_COMPILE);
  {
    glutSolidCone(10, (NUM_DISKS + 1) * DISK_HEIGHT, 20, 20);
  }
  glEndList();
}

void 
mpop(void)
{
  move_node *temp = moves.head;
  moves.head = moves.head->next;
  free(temp);
  moves.depth--;
}

void 
mpush(int t, int f)
{
  move_node *new1 = new move_node;
  if (!new1) {
    fprintf(stderr, "Out of memory!\n");
    exit(-1);
  }
  new1->t = t;
  new1->f = f;
  new1->next = NULL;
  new1->prev = moves.tail;
  if (moves.tail)
    moves.tail->next = new1;
  moves.tail = new1;
  if (!moves.head)

    moves.head = moves.tail;
  moves.depth++;
}

/* ARGSUSED1 */
void 
keyboard(unsigned char key, int x, int y)
{
  switch (key) 
  {
  case 27:             /* ESC */
  case 'q':
  case 'Q':
    exit(0);
  
  case 'i':
  case 'I':
  SPEED+=2;
  break;
  
  case 'd':
  case 'D':
       if(SPEED>2)
       SPEED-=2;
       break;
  case 'm':
  case 'M':
       SHIFTZ+=.05;
       break;
  case 'n':
  case 'N':
       if(SHIFTZ>1.05)
       SHIFTZ-=.05;
       break;
 }
}

void 
update(void)
{
    
  glutPostRedisplay();
}

void 
DrawPost(int xcenter)
{
  glPushMatrix();
  {
    glTranslatef(xcenter, SHIFTY, 0);
    glRotatef(90, -1, 0, 0);
    glCallList(CONE);
  } glPopMatrix();
}
GLfloat wallcolor[] =
{0, .3, 1, 1};
GLfloat wallcolor1[] =
{0, .3, 1, 1};
GLfloat wallcolor2[] =
{.1,.1 , .1, 1};
GLfloat wallcolor3[] =
{0, 1, 1, 1};

/*void 
DrawWall(void)
{
  int i;
  int j,z;
 // glColor3fv(wallcolor);
  for (i = 0; i < WIDTH; i += 10) {
      glColor3fv(wallcolor3);
    for (j = 0; j < HEIGHT; j += 10) {
        glColor3fv(wallcolor2);
  //   for ( z= 100; z < 120; z += 10) {
    //    glColor3fv(wallcolor1);
      glBegin(GL_POLYGON);
      {
        glNormal3f(0, 0, 1);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, wallcolor1);
        glVertex3f(i + 10, j, wallz);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, wallcolor2);
        glVertex3f(i + 10, j + 10, wallz);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, wallcolor3);
        glVertex3f(i, j + 10, wallz);
       // glVertex3f(i, j, wallz);
      }
      glEnd();
    //  }
    }
  }
}

*/


void 
DrawWall(void)
{
  int i;
  int j,z;
 // glColor3fv(wallcolor);
  for (i = 120; i < 1150; i += 10) {
      glColor3fv(wallcolor3);
  //  for (j = 120; j < 340; j += 10) {
     //   glColor3fv(wallcolor2);
     for ( z= -400; z < 400; z += 10) {
        glColor3fv(wallcolor1);
      glBegin(GL_POLYGON);
      {
        glNormal3f(0, 0, 1);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, wallcolor1);
        glVertex3f(i, 350, z);
        glMaterialfv(GL_FRONT, GL_AMBIENT, wallcolor2);
        glVertex3f(i,350, z+10);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, wallcolor3);
        glVertex3f(i+10,350,z);
       // glVertex3f(i, j, wallz);
      }
      glEnd();
    //  }
      }
  }
}

void 
DrawPosts(void)
{
  glColor3fv(poleColor);
  glLineWidth(10);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, poleColor);
  DrawPost(WIDTH / 4);
  DrawPost(2 * WIDTH / 4);
  DrawPost(3 * WIDTH / 4);
}

void timer() {
	glutPostRedisplay();
}


void 
hanoi_menu(int value)
{
  switch (value) {
  case HANOI_SOLVE:
    motion = !motion;
    if(motion) {
      glutIdleFunc(update);
   //   timer();
    } else {
      glutIdleFunc(NULL);
    }
    break;
  case HANOI_LIGHTING:
    if (glIsEnabled(GL_LIGHTING))
      glDisable(GL_LIGHTING);
    else
      glEnable(GL_LIGHTING);
    break;
  case HANOI_WALL:
    back_wall = !back_wall;
    break;
  case HANOI_FOG:
    if (glIsEnabled(GL_FOG))
      glDisable(GL_FOG);
    else {
      glEnable(GL_FOG);
      glFogi(GL_FOG_MODE, GL_EXP);
      glFogf(GL_FOG_DENSITY, .01);
    }
    break;
  case HANOI_SPEEDUP:
       SPEED+=2;
       break;
  case HANOI_SPEEDDOWN:
       if(SPEED>2)
       SPEED-=2;
       break;
  case HANOI_QUIT:
    exit(0);
    break;
  }
  glutPostRedisplay();
}

int oldx, oldy;

GLboolean leftb = GL_FALSE, middleb = GL_FALSE;

void 
hanoi_mouse(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON) {
    oldx = x;
    oldy = y;
    if (state == GLUT_DOWN)
      leftb = GL_TRUE;
    else
      leftb = GL_FALSE;
    if(leftb==GL_TRUE)
     printf("leftb is true and cordinates are %d,%d\n",x,y);
  }
  if (button == GLUT_MIDDLE_BUTTON) {
    oldx = x;
    oldy = y;
    if (state == GLUT_DOWN)
      middleb = GL_TRUE;
    else
      middleb = GL_FALSE;
  }
}



void 
hanoi_visibility(int state)
{
  if (state == GLUT_VISIBLE && motion) {
        // glutTimerFunc( 1, &timer, 0 ); 
    //     timer();   
    glutIdleFunc(update);
  } else {
    glutIdleFunc(NULL);
  }
}

void 
hanoi_motion(int x, int y)
{
  if (leftb) {
    xangle += (x - oldx);
    yangle += (y - oldy);
    printf("\n Xangle is -->%d\n Yangle is -->%d\n",xangle,yangle);
  }
  if (middleb) {
    xlangle -= (x - oldx);
    ylangle -= (y - oldy);
  }
  oldx = x;
  oldy = y;
  glutPostRedisplay();
}




void 
DrawDisk(int xcenter, int ycenter, int size,int count)
{
   //const float b = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
 int i;
 if(count==NUM_DISKS&&frun==1)
 {
   pending=1;
     for(count=1;count<=NUM_DISKS;count++)
     {
     if(xdisk[count]!=newx[count])
       break;
     }
     if(count<=NUM_DISKS)
     {
        a=ydisk[count];
        b=xdisk[count];
        c=(SHIFTY+((NUM_DISKS + 1) * DISK_HEIGHT));
        stop=0;
     }
     countrpt=count;
 }
 if(frun==0){
 glPushMatrix();
  {
  
    glTranslatef(xcenter, ycenter, 0);
    glRotatef(90, 1, 0, 0);
    glCallList(size);
  } glPopMatrix();
}
  if(frun==0)
  {
  xdisk[size]=xcenter;
  ydisk[size]=ycenter;
  }
 if(size==NUM_DISKS)
 frun=1;
  
}
void colordisk(int szcnt)
{
     glEnable (GL_BLEND);
     glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
               if(szcnt==1||szcnt%8==1){
                glColor3fv(diskColor0);
                 glMaterialfv(GL_FRONT, GL_DIFFUSE, diskColor0);}
               else if(szcnt==2||szcnt%8==2){
                glColor3fv(diskColor1);
              glMaterialfv(GL_FRONT, GL_DIFFUSE, diskColor1);}
            else    if(szcnt==3||szcnt%8==3) {
                glColor3fv(diskColor2);
              glMaterialfv(GL_FRONT, GL_DIFFUSE, diskColor2);}
            else    if(szcnt==4||szcnt%8==4){
                glColor3fv(diskColor3);
              glMaterialfv(GL_FRONT, GL_DIFFUSE, diskColor3);}
             else   if(szcnt==5||szcnt%8==5){
                glColor3fv(diskColor4);
              glMaterialfv(GL_FRONT, GL_DIFFUSE, diskColor4);}
             else   if(szcnt==6||szcnt%8==6){
                glColor3fv(diskColor5);
              glMaterialfv(GL_FRONT, GL_DIFFUSE, diskColor5);}
             else   if(szcnt==7||szcnt%8==7){
                glColor3fv(diskColor6);
              glMaterialfv(GL_FRONT, GL_DIFFUSE, diskColor6);}
             else   if(szcnt==8||szcnt%8==0){
                glColor3fv(diskColor7);
              glMaterialfv(GL_FRONT, GL_DIFFUSE, diskColor7);}
     //Disable (GL_BLEND);
}

void 
DrawDooDads()
{
  int i,count,szcnt=0;
  stack_node *temp;
  int xcenter, ycenter;
  for (i = 1,count =1; i <= 3; i++) 
  {
    xcenter = i * WIDTH / 4;
    
    for (temp = poles[i].head, ycenter = SHIFTY+(DISK_HEIGHT * poles[i].depth - DISK_HEIGHT / 2); temp; temp = temp->next, ycenter -= DISK_HEIGHT,count++) {
    szcnt=temp->size;
     colordisk(i);
     glDisable (GL_BLEND);
     newx[temp->size]=xcenter;newy[temp->size]=ycenter;
      DrawDisk(xcenter, ycenter, temp->size,count);
      
    }
  }
}

#define MOVE(t,f) mpush((t),(f))

static void
mov(int n, int f, int t)
{
  int o;

  if (n == 1) {
    MOVE(t, f);
    return;
  }
  o = other(f, t);
  mov(n - 1, f, o);
  mov(1, f, t);
  mov(n - 1, o, t);
}

void light()
{
  glDisable(GL_LIGHTING); 
  glDisable(GL_LIGHT1);
  glDisable(GL_LIGHT2);
  glDisable(GL_LIGHT3);
  glDisable(GL_LIGHT0);
  
   glLightfv(GL_LIGHT1, GL_POSITION, lightOnePosition);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, lightOneColor);
  glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 10);
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lightOneDirection);
  glEnable(GL_LIGHT1);

  glLightfv(GL_LIGHT2, GL_POSITION, lightTwoPosition);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, lightTwoColor);
/*  glLightf(GL_LIGHT2,GL_LINEAR_ATTENUATION,.005); */
  glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 10);
  glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, lightTwoDirection);
  glEnable(GL_LIGHT2);
  
  glLightfv(GL_LIGHT3, GL_POSITION, lightthreePosition);
  glLightfv(GL_LIGHT3, GL_DIFFUSE, lightthreeColor);
  glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 10);
  glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, lightthreeDirection);
  glEnable(GL_LIGHT3);
  
  
  //glEnable (GL_BLEND);
  //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);
  glEnable(GL_LIGHT0);
 // glDisable (GL_BLEND);
  
  glEnable(GL_LIGHTING);    
}

void draw1()
{
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     int i;
     if(frun==1) 
     { 
      for(i=1;i<=NUM_DISKS;i++)
     {
     xdisk[i]=newx[i];
     ydisk[i]=newy[i];
     }  
     }
     
     for(i=1;i<NUM_DISKS;i++)
     {
     if(newx[i]!=(3 * WIDTH / 4))
     break;
     }
     if(i==NUM_DISKS) {
     MessageBox(NULL,"This is just a Beginning... :)\n\n\tBY- Ank & Arpit","Show Over!",MB_OK | MB_ICONINFORMATION);
     exit(1);
     }
              
      int t, f;
    //    pause=0;
        
      DrawDooDads();
       
    /*  if (motion && moves.depth) {
        t = moves.head->t;
        f = moves.head->f;
        push(t, pop(f));
        mpop();
      }*/ 
      
                     //Move back to origin  //zoom */
                    glMatrixMode(GL_MODELVIEW);
                    glLoadIdentity();
                    gluLookAt(0,50,100,0,0,0,0,1,0);
                  
               glPushMatrix();
                      {     
                    float centerX = WIDTH * 0.5f;
                    float centerY = HEIGHT * 0.3f;
                //    glMatrixMode(GL_MODELVIEW);
                  //   glLoadIdentity();
                    glTranslatef(centerX,centerY,0);
                       //Move to center of scene
                    glScalef(SHIFTZ,SHIFTZ,SHIFTZ);          //Scale at center
                    glTranslatef(-centerX,-centerY,0);  
                      glPushMatrix();
                      {
                        glTranslatef(WIDTH / 2, HEIGHT / 2, 0);
                        glRotatef(xlangle, 0, 1, 0);
                        glRotatef(ylangle, 1, 0, 0);
                        glTranslatef(-WIDTH / 2, -HEIGHT / 2, 0);
                        glLightfv(GL_LIGHT0, GL_POSITION, lightZeroPosition);
                        
                      }
                 glPopMatrix();
                 
                 if(rotx>=360)rotx=0;             
                 glPushMatrix();
                 {
                     
                       glTranslatef(WIDTH / 2, HEIGHT / 2, 0);
                      glRotatef(rotx, 0,1 , 0);
                      rotx+=.1;
                        glTranslatef(-WIDTH / 2, -HEIGHT / 2, 0);
                         
                      glPushMatrix();
                      {
                        glTranslatef(WIDTH / 2, HEIGHT / 2, 0);
                        glRotatef(xangle, 0, 1, 0);
                        glRotatef(yangle, 1, 0, 0);
                        glTranslatef(-WIDTH / 2, -HEIGHT / 2, 0);
                        light();
                        if (back_wall) {
                        glMaterialfv(GL_FRONT, GL_DIFFUSE, wallcolor);
                        DrawWall();
                      }
                        DrawPosts();
                        for(int i=1;i<=NUM_DISKS;i++)
                        {
                             
                              colordisk(i);
                              
                               glPushMatrix();
                                  {
                                     
                                    glTranslatef(xdisk[i], ydisk[i], 0);
                                    glRotatef(90, 1, 0, 0);
                                    glCallList(i);
                                  } glPopMatrix();
                               glDisable (GL_BLEND);
                        } 
                      }
                      glPopMatrix();
                      }
                 glPopMatrix();
                 }
                 glPopMatrix();
      glutSwapBuffers();
     
}

void draw()
{
 if(pending==1)
 {
       int i;

     if(countrpt<=NUM_DISKS)
     {
     
                     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                   
                     //Move back to origin
                    
                    
                    
                     glMatrixMode(GL_MODELVIEW);
                    glLoadIdentity();
                    gluLookAt(0,50,100,0,0,0,0,1,0);
                   
                  
                  glPushMatrix();
                      { 
                   
                   float centerX = WIDTH * 0.5f;
                    float centerY = HEIGHT * 0.3f;
                //    glMatrixMode(GL_MODELVIEW);
                  //   glLoadIdentity();
                    glTranslatef(centerX,centerY,0);
                       //Move to center of scene
                    glScalef(SHIFTZ,SHIFTZ,SHIFTZ);          //Scale at center
                    glTranslatef(-centerX,-centerY,0);
                   
                      
                      glPushMatrix();
                      {
                        glTranslatef(WIDTH / 2, HEIGHT / 2, 0);
                        glRotatef(xlangle, 0, 1, 0);
                        glRotatef(ylangle, 1, 0, 0);
                        glTranslatef(-WIDTH / 2, -HEIGHT / 2, 0);
                     //   glLightfv(GL_LIGHT0, GL_POSITION, lightZeroPosition);
                        
                      }
                      glPopMatrix();
                      if(rotx>=360)rotx=0;
                 
                     glPushMatrix();
                    {
                       glTranslatef(WIDTH / 2, HEIGHT / 2, 0);
                        glRotatef(rotx, 0, 1, 0);
                        glTranslatef(-WIDTH / 2, -HEIGHT / 2, 0);
                        rotx+=.1;
                      glPushMatrix();
                      {
                        glTranslatef(WIDTH / 2, HEIGHT / 2, 0);
                        glRotatef(xangle, 0, 1, 0);
                        glRotatef(yangle, 1, 0, 0);
                        glTranslatef(-WIDTH / 2, -HEIGHT / 2, 0);
                       //  glLightfv(GL_LIGHT0, GL_POSITION, lightZeroPosition);
                        light();
                        if (back_wall) {
                        glMaterialfv(GL_FRONT, GL_DIFFUSE, wallcolor);
                        DrawWall();
                      }
                        DrawPosts();
                        for(i=1;i<=NUM_DISKS;i++)
                        {
                              colordisk(i);
                              if(i==countrpt)
                              { 
                                         if(a<=(SHIFTY+((NUM_DISKS + 1) * DISK_HEIGHT)))
                                         {    
                                              a+=SPEED;
                                              glPushMatrix();
                                             {
                                                glTranslatef(xdisk[i], a, 0);
                                                glRotatef(90, 1, 0, 0);
                                                glCallList(i);
                                              } glPopMatrix();
                                              
                                          }
                                        if ((b!=newx[i])&&(a>(SHIFTY+((NUM_DISKS + 1) * DISK_HEIGHT)))) 
                                          {
                                         
                                              glPushMatrix();
                                             {
                                                glTranslatef(b,a, 0);
                                                glRotatef(90, 1, 0, 0);
                                                glCallList(i);
                                              } glPopMatrix();
                                              if(xdisk[i]<newx[i])
                                              {
                                              b+=SPEED;
                                              if(b>newx[i])
                                              b=newx[i];
                                              }
                                              if(xdisk[i]>newx[i])
                                              {
                                              b-=SPEED;
                                              if(b<newx[i])
                                              b=newx[i];
                                              }
                                          }
                                          
                                   else if(c>=newy[i]&&a>(SHIFTY+((NUM_DISKS + 1) * DISK_HEIGHT))) 
                                          {
                                            if(c==newy[i])
                                            {
                                            pause=1;
                                            }
                                          
                                              
                                              glPushMatrix();
                                             {
                                                glTranslatef(b,c, 0);
                                                glRotatef(90, 1, 0, 0);
                                                glCallList(i);
                                              } glPopMatrix();
                                              
                                              c-=SPEED;
                                              if(c<newy[i])
                                              c=newy[i];
                                          
                                          } 
                                               
                                          
                                          
                                          
                              }
                              
                              if(i!=countrpt){
                                glPushMatrix();
                                  {
                                    glTranslatef(xdisk[i], ydisk[i], 0);
                                    glRotatef(90, 1, 0, 0);
                                    glCallList(i);
                                  } glPopMatrix();
                                }
                        } 
                        glDisable (GL_BLEND);
                      }
                      glPopMatrix();
                      }
                      glPopMatrix();
                       }
                      glPopMatrix();
                      glutSwapBuffers();  
                      if(pause==1) 
                    pending=0;
        }    
            
 }
      
if(pending==0)
{
     int i;
     if(frun==1) 
     { 
      for(i=1;i<=NUM_DISKS;i++)
     {
     xdisk[i]=newx[i];
     ydisk[i]=newy[i];
     }  
     }
     
     for(i=1;i<NUM_DISKS;i++)
     {
     if(newx[i]!=(3 * WIDTH / 4))
     break;
     }
     if(i==NUM_DISKS) {
     MessageBox(NULL,"This is just a Beginning... :)\n\n\tBY- Ank & Arpit","Show Over!",MB_OK | MB_ICONINFORMATION);
     exit(1);
     }
              
      int t, f;
        pause=0;
        
    
       DrawDooDads();
       
      if (motion && moves.depth) {
        t = moves.head->t;
        f = moves.head->f;
        push(t, pop(f));
        mpop();
      }
}

}




int 
main(int argc, char *argv[])
{
  int i;
  glutInit(&argc, argv);
  for(i=1; i<argc; i++) {
    if(!strcmp("-n", argv[i])) {
      i++;
      if(i >= argc) {
	printf("hanoi: number after -n is required\n");
	exit(1);
      }
      NUM_DISKS = atoi(argv[i]);
    }
  }
  printf("enter the following :)\n\n \t 0: auto\n\t 1: manual");
  int ch=getch();
  glutInitWindowSize(WIDTH, HEIGHT);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutGameModeString("640x800:32@60");
  glutEnterGameMode();

 // glutCreateWindow("Hanoi");

  
  glutKeyboardFunc(keyboard);

  glViewport(0, 0, WIDTH, HEIGHT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, WIDTH, 0, HEIGHT, -10000, 10000);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glClearColor(0, 0, 0, 0);
  glClearDepth(1.0);

  //glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

/*  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);  */



  glutMouseFunc(hanoi_mouse);
  glutMotionFunc(hanoi_motion);
  glutVisibilityFunc(hanoi_visibility);

  glutCreateMenu(hanoi_menu);
  glutAddMenuEntry("Solve", HANOI_SOLVE);
  glutAddMenuEntry("SpeedUp", HANOI_SPEEDUP);
  glutAddMenuEntry("SpeedDown", HANOI_SPEEDDOWN);
  glutAddMenuEntry("Lighting", HANOI_LIGHTING);
  glutAddMenuEntry("Back Wall", HANOI_WALL);
  glutAddMenuEntry("Fog", HANOI_FOG);
  glutAddMenuEntry("Quit", HANOI_QUIT);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3d(1,0,0);
  init();
  if(ch=='1')
  glutDisplayFunc(draw1);
  else
  glutDisplayFunc(draw);
  for (i = 0; i < NUM_DISKS; i++)
    push(1, NUM_DISKS - i);
  mov(NUM_DISKS, 1, 3);
  glutMainLoop();

  return 0;             
}

