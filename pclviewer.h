#pragma once
#pragma execution_character_set("utf-8")
// mybase
#include "ui_pclviewer.h"
#include "QHeightRampDlg.h"
#include "Camera.h"
#include "calc.h"

// QT
#include "QTObject.h"

typedef pcl::PointXYZRGB PointT1;
typedef pcl::PointCloud<PointT1> PointCloudT1;

typedef pcl::PointXYZRGB PointT2;
typedef pcl::PointCloud<PointT2> PointCloudT2;

namespace Ui {
	class PCLViewer;
}

class PCLViewer : public QMainWindow
{
	Q_OBJECT
	QThread workThread;

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

	void on_action_getimage_triggered();

	void on_action_play_triggered();

	void on_action_delete_triggered();


	void on_action_update_triggered();
	void on_treeWidgetFilelist_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);

	void setHeightRamp(int, double);

	void receiveFrame(QImage rgb, QImage depth);
protected:
	// 點雲宣告
	//----------------------------------------------------------------
	PointCloudT1::Ptr m_currentCloud;

	QList<PointCloudT1::Ptr> m_heightCloudList;

	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer;
	//----------------------------------------------------------------

	// 其他宣告
	//----------------------------------------------------------------

	PointT1 p_min, p_max;

	QHeightRampDlg heightRampDlg;

	QString FileName;

	rs2::config cfg;

	rs2::pipeline pipe;

	rs2::frameset frames;

	QThreadPool* pool;

	bool camera_running = true;

	double maxLen;

	enum treeItemType { itTopItem = 1001, itGroupItem, itImageItem };
	enum treeColNum { colItem = 0, colItem1 = 1 };


	// 初始化函數
	//----------------------------------------------------------------
	void initPCL();

	void initTreeWidget();

	void initTreeWidgetFileList();

	void initTreeWidgetParameter();

	void uiEnable();

	void uiDisable();

	// 功能函數
	//----------------------------------------------------------------
	void ReadPclFile(const QString& fullPathName);
	void ReadPara(const QString& Plypath);
	QFileInfoList AllFile(QTreeWidgetItem* root, const QString& path);
	void tokenize(string& str, char delim, vector<string>& out);
	stringstream Readcalc(const QString& Plypath);
};