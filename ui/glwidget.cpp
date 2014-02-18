#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{

    setFormat(QGLFormat (QGL::DoubleBuffer | QGL::DepthBuffer));
    translateZ = -1.0;
    translateX = 0.0;
    translateY = 0.0;
    rotationX =0.0;
    rotationY =0.0;
    rotationZ =0.0;
    xMax = 0;
    yMax = 0;
    zMax = 0;
    faceColors[0] = Qt::red;
    faceColors[1] = Qt::green;
    faceColors[2] = Qt::blue;
    faceColors[3] = Qt::yellow;

    oiBackgroundColor = QColor::fromCmykF(0.59,0.40,0.10,0.10);

     QTimer::singleShot(20, this, SLOT(update()));
}


void GLWidget::initializeGL(){
    qglClearColor(oiBackgroundColor.lighter());
    //glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_CULL_FACE);
    glEnable(GL_SMOOTH);

    static GLfloat lightPosition[4] = {0.0,0.0,0.0,1.0};
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0,GL_POSITION,lightPosition);

    GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

void GLWidget::paintGL(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    draw();

}

void GLWidget::resizeGL(int w, int h){

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPerspective(70, w / h, 0.01, 1000);
#ifdef QT_OPENGL_ES_1
    glOrthof(-3.0, +3.0, -3.0, +3.0, 0.1, 15.0);
#else
    glOrtho(-3.0, +3.0, -3.0, +3.0, 0.1, 15.0);
#endif
    glMatrixMode(GL_MODELVIEW);

}

void GLWidget::draw(){

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    //TODO sicht auf schwerpunkt zentrieren
    glTranslatef(translateX, translateY, translateZ);
    glRotatef(rotationX, 1.0, 0.0 ,0.0);
    glRotatef(rotationY, 0.0, 1.0 ,0.0);
    glRotatef(rotationZ, 0.0, 0.0 ,1.0);



    qglColor(Qt::red);
    if(features->size() > 0){

        for(int i =0; i< features->size(); i++){
            OiGraphix::drawFeature(features->at(i));
        }


    }


   /* Sphere *s =  new Sphere();
    s->xyz.setAt(0,1.5);
    s->xyz.setAt(1,1.5);
    s->xyz.setAt(2,1.0);
    s->radius = 1.0;

    FeatureWrapper *f = new FeatureWrapper();
    f->setSphere(s);

    OiGraphix::drawFeature(f);*/



    glPopMatrix();
    update();


}

int GLWidget::faceAtPosition(const QPoint &pos){
    return 0;
}

void GLWidget::mousePressEvent(QMouseEvent *event){
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event){
    GLfloat dx = GLfloat(event->x() - lastPos.x()) / width();
    GLfloat dy = GLfloat(event->y() - lastPos.y()) / height();

    if(event->buttons() & Qt::LeftButton) {
        rotationX += 180 *dy;
        rotationY += 180 *dx;
        updateGL();
    } else if(event->buttons() & Qt::RightButton){
        //rotationX += 180 * dy;
        //rotationZ += 180 * dx;
        translateX += 10*dx;
        translateY += 10*dy;
        updateGL();
    }
    lastPos = event->pos();
}

void GLWidget::mouseDoubleClickEvent(QMouseEvent *event){
    //TODO bei doppel klick koordinaten abgreifen
}

void GLWidget::wheelEvent(QWheelEvent *event){

    translateZ += 0.01*event->delta();
    event->accept();
    updateGL();
}

void GLWidget::focusOnFeature(double x, double y, double z){
    //TODO auf actives feature springen
    /*translateX = x;
    translateY = y;
    translateZ = z-1;*/

}
