#ifndef KL2DVIEW_H
#define KL2DVIEW_H

#include <QWidget>

class KLMBase;

class KL2DView : public QWidget
{
    Q_OBJECT
public:
    explicit KL2DView(QWidget *parent = 0);
    void setModule(KLMBase* module);

protected:
    void paintEvent(QPaintEvent* event);

signals:

public slots:

private:
    KLMBase* module;

};

#endif // KL2DVIEW_H
