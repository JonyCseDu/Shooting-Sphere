#include <windows.h>
#include <GL/glut.h>
#include <bits/stdc++.h>

char title[] = "Shooting Sphere";
using namespace std;

int refreshMills = 15;
void initGL() {
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

double mouse_x=0, mouse_y=0, mouse_z = -50, val = 1;
double cx=0, cy=-1.9, cz=-5.0;

bool isPause = false;

struct bullet{
    double x=cx, y=cy, z=cz, r=0.5;
    double ix, iy, iz;
    double mv = 0.5;
    bullet(double px, double py, double pz){
        px -= cx;
        py -= cy;
        pz -= cz;
        double val = sqrt(px*px + py*py + pz*pz);
        ix = px/val;
        iy = py/val;
        iz = pz/val;
        //printf("%lf %lf %lf %lf\n", ix, iy, iz);
    }
    bool next(){
        if(!isPause){
            x += ix*mv;
            y += iy*mv;
            z += iz*mv;
        }

        if(z>105 || fabs(x) > 100 || fabs(y) > 100) return false;
        else{
            glColor3f(0, 1, 0);
            glLoadIdentity();
            glTranslatef(x, y, z);
            glPushMatrix();
                glutSolidSphere(r,100,100);
            glPopMatrix();
            return true;
        }
    }
};

double ang = 0.001;

struct ball{
    double rad=2;
    double x, y, z;
    double cx, cy, cz;
    int rot;

    ball(double tx, double ty, double tz, double px, double py, double pz){
        x = tx, y = ty, z = tz;
        cx = px, cy = py, cz = pz;
    }

    void scale(double sx, double sy, double sz){
        x += sx;
        y += sy;
        z += sz;
    }

    void rotate_z(){
        double p1 = cos(ang), p2 = sin(ang), tx = x, ty = y;
        x = p1*tx - p2*ty;
        y = p2*tx + p1*ty;
    }

    void rotate_x(){
        double p1 = cos(ang), p2 = sin(ang), tz = z, ty = y;
        y = p1*ty - p2*tz;
        z = p2*ty + p1*tz;
    }
    void rotate_y(){
        double p1 = cos(ang), p2 = sin(ang), tz = z, tx = x;
        x = p1*tx + p2*tz;
        z = -p2*tx + p1*tz;
    }

    bool next(){
        if(!isPause){
            scale(-cx, -cy, -cz);
            if(rot&(1<<0)){
                rotate_x();
            }
            if(rot&(1<<1)){
                rotate_y();
            }
            if(rot&(1<<2)){
                rotate_z();
            }
            scale(cx, cy, cz);
        }


//        printf("%lf %lf %lf\n", x, y, z);
//        getchar();
        if(fabs(z)>105 || fabs(x) > 100 || fabs(y) > 100) return false;
        else{
            glColor3f(0, 0, 1);
            glLoadIdentity();
            glTranslatef(x, y, z);

            glPushMatrix();
                glutSolidSphere(rad, 100, 100);
            glPopMatrix();
            return true;
        }
    }
};


void drawPoint(void){
    // draw cross
    glLoadIdentity();
    glColor3f(0.8,0,0);
    glLineWidth(2);

    glBegin(GL_LINES);
        glVertex3f(mouse_x-val, mouse_y, mouse_z);
        glVertex3f(mouse_x+val, mouse_y, mouse_z);
    glEnd();

    glBegin(GL_LINES);
        glVertex3f(mouse_x, mouse_y-val, mouse_z);
        glVertex3f(mouse_x, mouse_y+val, mouse_z);
    glEnd();

    //glBegin(GL_SPHERE_MAP)
}

vector<bullet> vbullet;
vector<ball> vball;

void drawBullets(){
    for(int i=0; i<vbullet.size(); i++){
        if(i>=0){
            if(vbullet[i].next() == false){
                vbullet.erase(vbullet.begin() + i);
                i--;
            }
        }
    }
}

inline double dis(ball &b1, bullet &b2){
    double d1=b1.x-b2.x, d2=b1.y-b2.y, d3=b1.z-b2.z;
    return sqrt(d1*d1 + d2*d2 + d3*d3);
}

int cnt_crt = 0, cnt_des = 0;
bool isShoot(ball &tmp){
    for(int i=0; i<vbullet.size(); i++){
        if(dis(tmp, vbullet[i]) < tmp.rad + vbullet[i].r){
            cnt_des++;
            //printf("destroy %d\n", cnt_des);
            return true;
        }
    }
    return false;
}

void drawBalls(){
    for(int i=0; i<vball.size(); i++){
        if(i>=0){
            if(vball[i].next() == false || isShoot(vball[i]) == true){
                vball.erase(vball.begin() + i);
                i--;
            }
            //else printf("alive");
        }
    }
}

void destroy(){
    vball.clear();
    vbullet.clear();
}

ball create_ball(){
    cnt_crt++;
    double x=10, y=6, z=-80;
    if(cnt_crt == 1) x = 0;
    else if(cnt_crt == 2) y = 0;
    else if(cnt_crt == 4) swap(x, y);

    else if(cnt_crt == 3) x = -x;
    else if(cnt_crt == 5) y = -y;
    else if(cnt_crt == 6) x = -x , y = -y;

    ball tball(x, y, z, 0, 0, -60); //19 15 50
    tball.rot = cnt_crt;

	return tball;
}



double angle = 0;
int cnt = 0;

void drawBitmapText(string str, float x, float y, float z)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
	glRasterPos3f(x, y, z);

	for (int i=0; i<str.size(); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, str[i]);
	}
	glutSwapBuffers();
}

void delay(long long x){
    x *= 20000000;
    for(long long i=0; i<x; i++);
}

void restart_mission(){
    destroy();
    drawBitmapText("RESTART MISSION" ,0, 0, -2);
    delay(50);
    cnt_des = cnt_crt = 0;
}
int level = 0;
char str[30];
void new_mission(){
    destroy();
    sprintf(str, "LEVEL %d PASSED\n", level);
    drawBitmapText(str, 0, 0, -2);
    delay(50);
    cnt_des = cnt_crt = 0;
    ang += 0.002;
    level++;
}

void exit_mission(){

    sprintf(str, "YOU PASSED %d LEVELS\n", level);
    drawBitmapText(str ,0, 0, -2);
    delay(50);
    printf("%s", str);
    exit(0);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
    bool flag = true;
    if(cnt % 100==0){
        if(cnt_crt < 7){
            vball.push_back(create_ball());
            cnt = 0;
        }
        else if(cnt/200 > 10 || cnt_des == 7){
            cnt = 0;
            if(cnt_des < 7){
                //restart_mission();
                exit_mission();
            }
            else{
                new_mission();
            }
            flag = false;
        }
    }
    if(flag){
        drawPoint();
        drawBullets();
        drawBalls();
        glutSwapBuffers();
    }

    cnt++;

}

/* Called back when timer expired [NEW] */
void timer(int value) {
   glutPostRedisplay();      // Post re-paint request to activate display()
   glutTimerFunc(refreshMills, timer, 0); // next timer call milliseconds later
}

GLdouble lt = -1, bt=-1;

void reshape(GLsizei width, GLsizei height) {
    if (height == 0) height = 1;
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //glFrustum(lt, lt+2, bt, bt+2, 2, 100.0);
    gluPerspective(50.0f, aspect, 0.1f, 100.0f);
}
double pi = 3, pre_x=0, pre_y=0;


void My_mouse_routine(int x, int y)
{
    mouse_x = (x/600.0 - 0.5) * pi*50;
    mouse_y = ((600-y)/600.0  - 0.5) * pi*50;

//    mouse_x = (x/300.0 - 1)*pi;
//    mouse_y = ((600-y)/300.0  - 1)*pi;
//
//    lt += (mouse_x - pre_x)/pi;
//    bt += (mouse_y - pre_y)/pi;
//
//    printf("%lf %lf\n", lt, bt);
//
//    pre_x = mouse_x, pre_y = mouse_y;

}

void mouse_click(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        vbullet.push_back(bullet(mouse_x, mouse_y, mouse_z));
    }
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
        case 'q':
            exit_mission();
            break;
        case ' ':
            isPause ^= true;
            break;
        case 'i':
            ang += 0.001;
            break;
        case 'd':
            ang -= 0.001;
            break;
        default:
            break;
   }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(50, 50);

    glutCreateWindow(title);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    initGL();
    glutTimerFunc(0, timer, 0);

   // mouse event handlers
    glutMouseFunc(mouse_click);
    glutPassiveMotionFunc( My_mouse_routine );
    glutSetCursor(GLUT_CURSOR_NONE);

    glutKeyboardFunc(keyboard);

    // added
    glEnable(GL_CULL_FACE);
//    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //glEnable(GL_COLOR_MATERIAL);


    glutMainLoop();                 // Enter the infinite event-processing loop
    return 0;
}
