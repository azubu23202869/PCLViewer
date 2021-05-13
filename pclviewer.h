#pragma once
#pragma execution_character_set("utf-8")
// mybase
#include "ui_pclviewer.h"
#include "QHeightRampDlg.h"

// QT
#include "QTObject.h"

typedef pcl::PointXYZRGB PointT;
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

	void on_action_play_triggered();

	void on_action_opendirectory_triggered();

	void on_action_reconstruction_triggered();

	void on_treeWidgetFilelist_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);

	void on_actionaction_paint_triggered();

	void setHeightRamp(int, double);


protected:
	// 點雲宣告
	//----------------------------------------------------------------
	PointCloudT::Ptr m_currentCloud;

	PointT p_min, p_max;

	QList<PointCloudT::Ptr> m_heightCloudList;

	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer;

	//----------------------------------------------------------------

	// 其他宣告
	//----------------------------------------------------------------

	std::vector<int> indices;
	PointCloudT::Ptr cloud;
	std::vector<PointT>v;
	PointT p;

	QHeightRampDlg heightRampDlg;

	QString path;
	QString imgpath;


	std::string NowReadFileName;

	double maxLen;

	int* __ptr64 p64 = nullptr;

	int num = 0;


	enum treeItemType { itTopItem = 1001, itGroupItem, itImageItem };
	enum treeColNum { colItem = 0, colItem1 = 1 };


	// 初始化函數
	//----------------------------------------------------------------
	void initPCL();

	void initTreeWidgetFileList();


	// 功能函數
	//----------------------------------------------------------------
	void ReadPclFile(const QString& fullPathName);
	QFileInfoList AllFile(QTreeWidgetItem* root, const QString& path);
	void mySystem(const std::string& cmd, const std::string& dir);
	void pp_callback_AreaSelect(const pcl::visualization::AreaPickingEvent& event, void* args);					 //new
};