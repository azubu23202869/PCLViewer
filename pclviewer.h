#pragma once
#pragma execution_character_set("utf-8")


// mybase
#include "ui_pclviewer.h"
#include "QHeightRampDlg.h"



// Qt
#include <QtWidgets/QMainWindow>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QColorDialog>
#include <QtWidgets/QApplication>
#include <QTextCodec>
#include <QDebug>
#include <QTreeWidget> 

// Point Cloud Library
#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/common/common.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/filters/passthrough.h>
#include <boost/thread/thread.hpp>

// Visualization Toolkit (VTK)
#include <vtkAutoInit.h> 
#include <vtkRenderWindow.h>
#include <vtkOutputWindow.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);

//realsense
#include <librealsense2/rs.hpp>

// include OpenCV header file
#include <opencv2/opencv.hpp>
=========
>>>>>>>>> Temporary merge branch 2



// Qt
#include <QtWidgets/QMainWindow>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QColorDialog>
#include <QtWidgets/QApplication>
#include <QTextCodec>
#include <QDebug>
#include <QTreeWidget> 

// Point Cloud Library
#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/common/common.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/filters/passthrough.h>
#include <boost/thread/thread.hpp>

// Visualization Toolkit (VTK)
#include <vtkAutoInit.h> 
#include <vtkRenderWindow.h>
#include <vtkOutputWindow.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);

//realsense
#include <librealsense2/rs.hpp>

// include OpenCV header file
#include <opencv2/opencv.hpp>
=========
>>>>>>>>> Temporary merge branch 2



// Qt
#include <QtWidgets/QMainWindow>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QColorDialog>
#include <QtWidgets/QApplication>
#include <QTextCodec>
#include <QDebug>
#include <QTreeWidget> 

// Point Cloud Library
#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/common/common.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/filters/passthrough.h>
#include <boost/thread/thread.hpp>

// Visualization Toolkit (VTK)
#include <vtkAutoInit.h> 
#include <vtkRenderWindow.h>
#include <vtkOutputWindow.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);

//realsense
#include <librealsense2/rs.hpp>

// include OpenCV header file
#include <opencv2/opencv.hpp>
=========
>>>>>>>>> Temporary merge branch 2

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

<<<<<<<<< Temporary merge branch 1
	
=========
>>>>>>>>> Temporary merge branch 2
	void on_action_frontIso_triggered();

	void on_action_backIso_triggered();

	void on_action_setColor_triggered();

	void on_action_heightRamp_triggered();

	void on_btn2D_clicked();

	void on_btn3D_clicked();

	void on_btnPlay_clicked();

	void on_btnPause_clicked();

	void on_treeWidgetFilelist_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);

	void on_treeWidget_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);

	void setHeightRamp(int, double);

	void receiveFrame(QImage rgb, QImage depth);
protected:
	// �I���ŧi
	//----------------------------------------------------------------
	PointCloudT::Ptr m_currentCloud;

	QList<PointCloudT::Ptr> m_heightCloudList;

	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer;

	// ��L�ŧi
	//----------------------------------------------------------------

	PointT p_min, p_max;

	QHeightRampDlg heightRampDlg;

	QString FileName;

<<<<<<<<< Temporary merge branch 1
=========

>>>>>>>>> Temporary merge branch 2
	double maxLen;

	enum treeItemType { itTopItem = 1001, itGroupItem, itImageItem };
	enum treeColNum { colItem = 0 };


	// 初始化函數
	//----------------------------------------------------------------
<<<<<<<<< Temporary merge branch 1
	void init();

	void iniTree();  

	// 功能函數
	//----------------------------------------------------------------
	void ReadPclFile(const QString& fullPathName);
	QFileInfoList allfile(QTreeWidgetItem* root, QString path);



=========
	void initPCL();

	void initTreeWidget();
	
	void TreeWidgetclr();

	// 功能函數
	//----------------------------------------------------------------
	void ReadPclFile(const QString& fullPathName);
	QFileInfoList AllFile(QTreeWidgetItem* root, const QString& path);
>>>>>>>>> Temporary merge branch 2
};