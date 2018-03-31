#ifndef SLIDE_H
#define SLIDE_H

#include <QPointF>
#include <QGraphicsScene>
#include <QObject>
#include <QMap>
#include "kmpinkelement.h"

struct InkDData{
    QPointF prePoint;
    std::vector<QGraphicsItem*> TempDrawingItem;
    KMPInkElement* Element;
    InkDData(){
        this->Element = Q_NULLPTR;
        this->TempDrawingItem .clear();
    }
};

class Slide : public QGraphicsScene
{
public:
    Slide(QObject* parent = 0 );

    void setColor(const QColor& color ){this->_inkColor = color;}
    void setThickness(int w ){this->_inkThickness = w ;}
    void onDeviceDown( const QPointF& pt ,int id = 0 );
    void onDeviceMove( const QPointF& pt , int id = 0 );
    void onDeviceUp( int id = 0 );

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void drawStart( InkDData* dt );
    void drawTo( InkDData* dt , const QPointF& to );
private:
    QMap< int , InkDData* > _itemMap;
    QColor _inkColor;
    int _inkThickness;
    bool _isDrawing ;
};

#endif // SLIDE_H
