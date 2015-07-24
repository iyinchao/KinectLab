#ifndef KL2DVIEW_H
#define KL2DVIEW_H

#include <QWidget>

class KLMBase;

class KL2DView : public QWidget
{
    Q_OBJECT
public:
    explicit KL2DView(int id = 0, QWidget *parent = 0);
    ~KL2DView();
    void setModule(KLMBase* module);
    void setId(int id);
    const int getId();

protected:
    void paintEvent(QPaintEvent* event);

signals:

public slots:
    void h_moduleDestroy();

private:
    KLMBase* module;
    int id;

};

#endif // KL2DVIEW_H
