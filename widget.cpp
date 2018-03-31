#include "widget.h"
#include "slide.h"

#include <QTouchEvent>
#include <QEvent>
#include <QDebug>
#include <QMenu>

Widget::Widget(QWidget *parent)
    : QGraphicsView(parent)
{
    this->setInteractive(true);
    this->setOptimizationFlag(QGraphicsView::IndirectPainting);
    this->setCacheMode(QGraphicsView::CacheBackground);

    //for touch
    this->setAttribute(Qt::WA_AcceptTouchEvents);
    this->viewport()->setAttribute(Qt::WA_AcceptTouchEvents);

    //antialiasing
    setRenderHint(QPainter::Antialiasing,true );

    //init scene
    Slide* slide = new Slide;
    this->setScene(slide);
//    this->setSceneRect(0,0,600,600);
//    this->resize( 600,600);
    this->setWindowFlags(Qt::Window);
    this->showFullScreen();
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    slide->setColor(Qt::green);
    slide->setThickness( 1 );

    this->_currentSlide = slide;

    m_pActExit = new QAction(tr("EXIT"), this);
    connect(m_pActExit, SIGNAL(triggered()),this, SLOT(actExitHandle()));
}

Widget::~Widget()
{
    delete _currentSlide;
    delete m_pActExit;
    qDebug("~Widget\r\n");
}

void Widget::resizeEvent(QResizeEvent *event){
    Q_UNUSED(event);
    this->setSceneRect(0,0,this->width(),this->height());
    this->centerOn(this->width()/2 , this->height() /2 );
}

void Widget::setScene(Slide *scene){
    Q_EMIT slideChangedBefore();
    QGraphicsView::setScene( scene );
    this->_currentSlide = scene;

    Q_EMIT slideChangedAfter();
}

bool Widget::viewportEvent(QEvent *event){
    // 处理touch事件
    QEvent::Type evType = event->type();
    if(evType==QEvent::TouchBegin || evType == QEvent::TouchUpdate || evType == QEvent::TouchEnd )
    {
        QTouchEvent* touchEvent = static_cast<QTouchEvent*>(event);
        QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();

        foreach( const QTouchEvent::TouchPoint tp , touchPoints ){
            //不考虑pad
            QPoint touchPos  = QPoint( tp.pos().x() , tp.pos().y() );
            if(tp.id() == 0 ){
                if( tp.state() == Qt::TouchPointPressed )
                    this->_isTouchMode = true;
                else
                    this->_isTouchMode = false;
            }

            QPointF scenepos = this->mapToScene(touchPos.x() , touchPos.y() );
            switch( tp.state() ){
            case Qt::TouchPointPressed:
                this->_currentSlide->onDeviceDown(scenepos, tp.id());
                break;
            case Qt::TouchPointMoved:
                this->_currentSlide->onDeviceMove(scenepos,tp.id());
                break;
            case Qt::TouchPointReleased:
                this->_currentSlide->onDeviceUp(tp.id());
                break;
            }

        }

        if(evType == QEvent::TouchEnd ){
            // to do
        }

        return true;
    }

    return QGraphicsView::viewportEvent(event);
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape){
        exit(0);
    }
    return QGraphicsView::keyPressEvent(event);
}

void Widget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = new QMenu(this);
    menu->addAction(m_pActExit);
    menu->move(cursor().pos());
    menu->show();
}

void Widget::actExitHandle()
{
    exit(0);
}
