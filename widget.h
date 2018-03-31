#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsView>

class Slide;
class Widget : public QGraphicsView
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    void setInkColor(const QColor* color );
    void setInkThickness(int thickness );

    Slide* currentSlide(void)const{return this->_currentSlide; }
    void setScene(Slide *scene);
protected:
    void resizeEvent(QResizeEvent *event);


    //
    virtual bool viewportEvent(QEvent *event);
private:
    Slide* _currentSlide;
    bool _isTouchMode;
Q_SIGNALS:
    void slideChangedBefore(void);
    void slideChangedAfter(void);
};

#endif // WIDGET_H
