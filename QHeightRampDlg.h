#pragma once
#pragma execution_character_set("utf-8")

#include "QTObject.h"
#include "ui_QHeightRampDlg.h"

namespace Ui{
	class QHeightRampDlg;
}

class QHeightRampDlg : public QWidget
{
	Q_OBJECT

public:
	explicit QHeightRampDlg(QWidget* parent = 0);
	~QHeightRampDlg();

private:
	Ui::QHeightRampDlg *ui;

public Q_SLOTS:
	void on_btn_ok_clicked();

	void on_btn_cancel_clicked();

signals:
	void setHeightRamp(int dir, double height);

};
