#ifndef KMPINKELEMENT_H
#define KMPINKELEMENT_H

#include <QGraphicsPathItem>
#include <vector>
#include <QPointF>
#include "kmp.h"

class KMPInkElement : public QGraphicsPathItem
{
public:
    KMPInkElement( QGraphicsItem* parent = 0 );

    void setColor( const QColor& color );
    void setThickness( int thickcness );
    void setLayer( qreal zvalue );

    QColor color(void)const{return this->data(KMP_::EPT_Color).value<QColor>();}
    int thickness(void)const{return this->data(KMP_::EPT_Thickness).toInt();}

    void render(void);
    std::vector<QPointF> points(void){return this->_listPoints;}
    void addPoint( const QPointF& pt ){
        this->_listPoints.push_back(pt);
    }

private:
    std::vector<QPointF> _listPoints;

    void initialize(void);
};

#endif // KMPINKELEMENT_H
