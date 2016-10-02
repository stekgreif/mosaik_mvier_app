#ifndef TOOLBOXMUTE_H
#define TOOLBOXMUTE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>


class ToolBoxMute : public QWidget
{
	Q_OBJECT

	public:
		ToolBoxMute(QWidget *uiParent = 0);
		~ToolBoxMute();

	protected:
		void paintEvent(QPaintEvent *event);

	private:
		QLabel *m_testLabel;
		QPushButton *m_buttonMute;

};

#endif // TOOLBOXMUTE_H
