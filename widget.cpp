#include "widget.h"
#include "slide.h"

#include <QTouchEvent>
#include <QEvent>

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
    this->setSceneRect(0,0,600,600);
    this->resize( 600,600);

    slide->setColor(Qt::green);
    slide->setThickness( 8 );

    this->_currentSlide = slide;
}

Widget::~Widget()
{

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
