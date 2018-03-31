#include "slide.h"

Slide::Slide(QObject *parent)
    :QGraphicsScene(parent)
{
    this->_isDrawing =false;
}

void Slide::drawStart(InkDData *dt){
    double x = dt->prePoint.x() - (double(_inkThickness)/2.f);
    double y = dt->prePoint.y() -(double(_inkThickness)/2.f);

    QGraphicsEllipseItem* el = addEllipse(QRect(x,y,_inkThickness,_inkThickness),
                                          QPen(_inkColor,1 ) , QBrush(_inkColor));

    dt->TempDrawingItem.push_back(el);
}

void Slide::drawTo(InkDData *dt, const QPointF &to){
    QGraphicsLineItem* li = addLine(dt->prePoint.x(),
                                    dt->prePoint.y(),
                                    to.x(),to.y(),
                                    QPen(QBrush(_inkColor),_inkThickness,Qt::SolidLine,
                                         Qt::RoundCap,Qt::RoundJoin ));

    dt->TempDrawingItem.push_back(li);
}

void Slide::onDeviceDown(const QPointF &pt, int id){
    if(_itemMap.keys().contains(id))
    {
        InkDData* dt = _itemMap.value(id);
        if(dt->Element)
            QGraphicsScene::removeItem(dt->Element);
        if(dt->TempDrawingItem.size() >= 1 ){
            for(size_t i = 0; i < dt->TempDrawingItem.size() ;i++)
            {
                QGraphicsScene::removeItem( dt->TempDrawingItem[i]);
            }
        }

        dt->TempDrawingItem.clear();
        delete dt;

         _itemMap.remove( id );

    }

    InkDData* dt =  new InkDData;
    dt->prePoint = pt;//scene point;
    dt->Element  = new KMPInkElement;
    dt->Element->addPoint(pt);
    _itemMap.insert( id , dt );

    this->drawStart( dt );

}

void Slide::onDeviceMove(const QPointF &pt, int id){
    if(_itemMap.keys().contains(id)){
        InkDData* dt = _itemMap.value(id);
        QPointF to = pt;

        if(dt->Element)
            dt->Element->addPoint( to );

        this->drawTo( dt , to );
        dt->prePoint = to;

    }
}

void Slide::onDeviceUp(int id){
    if(_itemMap.keys().contains(id)){
        InkDData* dt = _itemMap.value(id);
        for(size_t i = 0; i < dt->TempDrawingItem.size() ;i++)
        {
            QGraphicsScene::removeItem( dt->TempDrawingItem[i]);
        }

        dt->TempDrawingItem.clear();
        dt->Element->setColor( _inkColor );
        dt->Element->setThickness( _inkThickness );
        dt->Element->render();
        QGraphicsScene::addItem(  dt->Element );

        _itemMap.remove( id );
    }
}

#include <QGraphicsSceneMouseEvent>
void Slide::mousePressEvent(QGraphicsSceneMouseEvent *event){
    this->_isDrawing = true;
    this->onDeviceDown(event->scenePos());
}
void Slide::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    if(this->_isDrawing ){
        this->onDeviceMove( event->scenePos() );
    }
}

void Slide::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if(this->_isDrawing){
        this->_isDrawing = false;
        this->onDeviceUp();
    }
}
