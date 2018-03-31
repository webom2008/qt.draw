#include "kmpinkelement.h"
#include <QUuid>
#include <QBrush>
#include <QPen>

KMPInkElement::KMPInkElement(QGraphicsItem *parent)
    :QGraphicsPathItem(parent)
{
    initialize();
}

void KMPInkElement::initialize()
{
    QVariant v;
    v.setValue<QColor>(Qt::green);
    this->setData( KMP_::EPT_Color,v);
    this->setData( KMP_::EPT_Id,QVariant(QUuid::createUuid().toString()));
    this->setData( KMP_::EPT_Thickness,QVariant(4));
    this->setData( KMP_::EPT_Locked,QVariant(false));
    this->setData( KMP_::EPT_Visibiled,QVariant(false ));

}

void KMPInkElement::setColor(const QColor &color){
    QVariant v;
    v.setValue<QColor>(color);
    this->setData( KMP_::EPT_Color,v);
}

void KMPInkElement::setThickness(int thickcness){
    this->setData( KMP_::EPT_Thickness,QVariant(thickcness));
}

void KMPInkElement::setLayer(qreal zvalue){
    this->setData( KMP_::EPT_Layer,QVariant(zvalue));
    this->setZValue( zvalue );
}

void KMPInkElement::render()
{
    if(this->_listPoints.size() < 1 ) return ;

    QPainterPath path;
    path.moveTo( this->_listPoints[0]);
    for( size_t i = 1 ; i < this->_listPoints.size() ; i++ ){
        path.lineTo( this->_listPoints[i]);
    }

    this->setPen( QPen(QBrush(color()), (qreal)thickness() , Qt::SolidLine , Qt::RoundCap,Qt::RoundJoin) );
    this->setPath( path );
}
