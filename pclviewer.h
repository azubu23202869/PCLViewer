#pragma once
#pragma execution_character_set("utf-8")


// mybase
#include "ui_pclviewer.h"
#include "QHeightRampDlg.h"

//QT
#include "QTObject.h"

typedef pcl::PointXYZ PointT;
typedef pcl::PointCloud<PointT> PointCloudT;

namespace Ui {
	class PCLViewer;
}

class PCLViewer : public QMainWindow
{
	Q_OBJECT

public:
	explicit PCLViewer(QWidget* parent = 0);
	~PCLViewer();

private:
	Ui::PCLViewer* ui;

public Q_SLOTS:

	void on_action_open_triggered();

	void on_action_reset_triggered();

	void on_action_up_triggered();

	void on_action_front_triggered();

	void on_action_left_triggered();

	void on_action_back_triggered();

	void on_action_right_triggered();

	void on_action_bottom_triggered();

	void on_action_frontIso_triggered();

	void on_action_backIso_triggered();

	void on_action_setColor_triggered();

	void on_action_heightRamp_triggered();

	void on_btn2D_clicked();

	void on_btn3D_clicked();

	void on_btnPlay_clicked();

	void on_btnPause_clicked();

	void on_treeWidgetFilelist_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);

	void setHeightRamp(int, double);

	void receiveFrame(QImage rgb, QImage depth);
protected:
	// 點雲宣告
	//----------------------------------------------------------------
	PointCloudT::Ptr m_currentCloud;

	QList<PointCloudT::Ptr> m_heightCloudList;

	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer;
	//----------------------------------------------------------------

	// 其他宣告
	//----------------------------------------------------------------

	PointT p_min, p_max;

	QHeightRampDlg heightRampDlg;

	QString FileName;


	double maxLen;

	enum treeItemType { itTopItem = 1001, itGroupItem, itImageItem };
	enum treeColNum { colItem = 0 };


	// 初始化函數
	//----------------------------------------------------------------
	void initPCL();

	void initTreeWidget();

	void TreeWidgetclr();

	// 功能函數
	//----------------------------------------------------------------
	void ReadPclFile(const QString& fullPathName);
	QFileInfoList AllFile(QTreeWidgetItem* root, const QString& path);
};