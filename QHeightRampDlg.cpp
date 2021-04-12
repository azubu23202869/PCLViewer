#include "QHeightRampDlg.h"


QHeightRampDlg::QHeightRampDlg(QWidget* parent) :
	QWidget(parent),
	ui(new Ui::QHeightRampDlg)
{
	ui->setupUi(this);
	setWindowModality(Qt::ApplicationModal);
}

QHeightRampDlg::~QHeightRampDlg()
{
}

void QHeightRampDlg::on_btn_ok_clicked()
{
	int dir = ui->comboBox_direction->currentIndex();
	double height = ui->spinBox_period->value();
	if (height != NULL) {
		emit setHeightRamp(dir, height);
	}
	else
	{
		QMessageBox::warning(this, "ĵ�i", "�п�J���׶g��");
	}
	close();
}

void QHeightRampDlg::on_btn_cancel_clicked()
{
	close();
}
