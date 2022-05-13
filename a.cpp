#include <windows.h>
#include <math.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <bits/stdc++.h>
using namespace std;

vector< pair<int,int> > circle;
vector< pair<int,int> > window;

bool c=false;
int ymini = 1000;
int ymaxi = -1;

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

void init(){
    glClearColor(0,0,0,0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,640,0,480);
}

void drawWindow(){
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glBegin(GL_POLYGON);
        glVertex2f(window[0].first,window[0].second);
        glVertex2f(window[0].first,window[1].second);
        glVertex2f(window[1].first,window[1].second);
        glVertex2f(window[1].first,window[0].second);
    glEnd();
    glFlush();

}

void drawCircle(float r){
    float x = 0, y = r;
    float x_centre = circle[0].first;
    float y_centre = circle[0].second;

    glBegin(GL_POINTS);

    pair<float,float> pr1(x + x_centre,y + y_centre);
    pair<float,float> pr2(x + x_centre,-y + y_centre);
    pair<float,float> pr3(y + x_centre,x + y_centre);
    pair<float,float> pr4(-y + x_centre,x + y_centre);

    circle.push_back(pr1);
    circle.push_back(pr2);
    circle.push_back(pr3);
    circle.push_back(pr4);

    float P = 1 - r;

    while (y > x)
    {
        x++;
        if (P <= 0)
            P = P + 2*x + 1;
        else
        {
            y--;
            P = P - 2*y + 2*x + 1;
        }

        if (x > y)
            break;

        pair<float,float> pr1(x + x_centre,y + y_centre);
        pair<float,float> pr2(-x + x_centre,y + y_centre);
        pair<float,float> pr3(x + x_centre,-y + y_centre);
        pair<float,float> pr4(-x + x_centre,-y + y_centre);
        circle.push_back(pr1);
        circle.push_back(pr2);
        circle.push_back(pr3);
        circle.push_back(pr4);

        if (x != y)
        {
            pair<float,float> pr1(y + x_centre,x + y_centre);
            pair<float,float> pr2(-y + x_centre,x + y_centre);
            pair<float,float> pr3(y + x_centre,-x + y_centre);
            pair<float,float> pr4(-y + x_centre,-x + y_centre);
            circle.push_back(pr1);
            circle.push_back(pr2);
            circle.push_back(pr3);
            circle.push_back(pr4);
        }
    }

    glEnd();
}

void drawCircle()
{
    /*glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glBegin(GL_POLYGON);
    for(int i=0;i<circle.size();i++)
    {
        glVertex2d(circle[i].first,circle[i].second);
    }
    glEnd();
    glFlush();*/
    float x1=circle[0].first;
    float y1=circle[0].second;
    float x2=circle[1].first;
    float y2=circle[1].second;
    cout<<"called\n";
    float r=pow(((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)),0.5);
    cout<<"R : "<<r;
    drawCircle(r);
    //drawCircle(r);
}

float findMin(int x,int y)
{
    if(x<y)
        return x;
    else
        return y;
}

float findMax(int x,int y)
{
    if(x>y)
        return x;
    else
        return y;
}

pair<float,float> intersect(float x1,float y1,float x2,float y2,int scan){
    float m;
    if(x2-x1==0)
    {
        m=0;
        pair<float,float> temp(x2,scan);
        return temp;
    }
    m=(y2-y1)/float(x2-x1);

    float x=x1+(scan-y1)/m;
    pair<float,float> temp(x,scan);
    return temp;
}

void oEdgeList(){
    for(int i=ymini;i<ymaxi;i++)
    {
        vector< pair<float,float> > temp;
        for(int j=0;j<circle.size();j++)
        {
            bool flag=false;
            pair<float,float> p;

            if(j==circle.size()-1)
            {
                if((findMin(circle[j].second,circle[0].second)<=i) && findMax(circle[j].second,circle[0].second)>= i)
                {
                    p=intersect(circle[j].first,circle[j].second,circle[0].first,circle[0].second,i);
                    flag=true;
                }
            }
            else
            {
                if(findMin(circle[j].second,circle[j+1].second)<=i && findMax(circle[j].second,circle[j+1].second)>=i){
                    p=intersect(circle[j].first,circle[j].second,circle[j+1].first,circle[j+1].second,i);
                    flag=true;
                }
            }
            if(flag==true)
            {
                auto m=find(temp.begin(),temp.end(),p);
                if(m==temp.end())
                {
                    temp.push_back(p);
                }
                else
                {
                    if(i-1>=0 && j!=circle.size()-1 && ((circle[j-1].second>=p.second && p.second<=circle[j+1].second)||(circle[j-1].second<=p.second && p.second>=circle[j+1].second)))
                    {
                        temp.push_back(p);
                    }
                }
            }

            sort(temp.begin(),temp.end());
            int loop=temp.size()-1;
            for(int k = 0;k<loop;k++)
            {
                if(k%2==0)
                {
                    glBegin(GL_LINES);
                        glVertex2d(temp[k].first,temp[k].second);
                        glVertex2d(temp[k+1].first,temp[k+1].second);
                    glEnd();
                    glFlush();
                }
            }
        }
    }
}




void mousehandle(int button, int status, int x,int y)
{
    if(button == GLUT_LEFT_BUTTON && status==GLUT_DOWN)
    {
        pair<int,int> temp;
        glBegin(GL_POINTS);
            glVertex2d(x,480-y);
        glEnd();
        glFlush();

        temp.first = x;
        temp.second = 480-y;
        cout<<"x : "<<x<<"  y : "<<temp.second<<"\n";
        ymini = min(ymini,temp.second);
        ymaxi = max(ymaxi,temp.second);

        if(c==false)
        {
            window.push_back(temp);
        }
        else
        {
            circle.push_back(temp);
        }
    }
    if(button == GLUT_RIGHT_BUTTON && status == GLUT_DOWN)
    {
        if(c==false)
        {
            c=true;
            drawWindow();
        }
        else
        {
            drawCircle();
            oEdgeList();
        }
    }
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(50,50);
    glutInitDisplayMode(GLUT_RGB);

    glutCreateWindow("Order Edge List");
    init();

    glutDisplayFunc(display);
    glutMouseFunc(mousehandle);
    glutMainLoop();

    return EXIT_SUCCESS;
}
