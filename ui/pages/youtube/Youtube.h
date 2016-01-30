#ifndef YOUTUBE_H
#define YOUTUBE_H

#include <QWidget>
#include <QWebView>
#include <QPushButton>
#include <QProcess>

class Youtube : public QWidget
{
    Q_OBJECT
public:
    explicit Youtube(QWidget *parent = 0);
    ~Youtube();
    void refresh(void);

signals:

private slots:
    void slot_showKeyboard(void);
    void slot_hideKeyboard(void);

private:
    QWebView *m_browser;

    QPushButton *m_showKeyboard;
    QPushButton *m_hideKeyboard;
    QProcess *m_keyboardProcess;
};

#endif // YOUTUBE_H
