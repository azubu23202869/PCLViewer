#include "pclviewer.h"

PCLViewer::PCLViewer(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::PCLViewer)
{
	ui->setupUi(this);

	//初始化
	init();
}

// 取得兩個點平行於坐標軸的最短距離
double getMinValue(PointT p1, PointT p2);
// 取得兩個點平行於座標軸的最長距離
double getMaxValue(PointT p1, PointT p2);


void PCLViewer::init()
{
	// 點雲初始化
	m_currentCloud.reset(new PointCloudT);
	// 可視化初始化
	viewer.reset(new pcl::visualization::PCLVisualizer("viewer", false));
	// 設置VTK可視化窗口指針
	ui->qvtkWidget->SetRenderWindow(viewer->getRenderWindow());
	// 設置視窗互動
	viewer->setupInteractor(ui->qvtkWidget->GetInteractor(), ui->qvtkWidget->GetRenderWindow());
	// 添加坐標軸
	viewer->addCoordinateSystem(1.0);

	QStringList headerList;
	headerList.append(tr("文件名"));
	headerList.append(tr("文件大小"));
	headerList.append(tr("文件路徑"));
	ui->treeWidget->setHeaderLabels(headerList);
	ui->treeWidget->setColumnWidth(1, 150);
	// 事件
	connect(&heightRampDlg, SIGNAL(setHeightRamp(int, double)), this, SLOT(setHeightRamp(int, double)));
}

void PCLViewer::on_action_open_triggered()
{
	// 取得點雲路徑
	QString fileName = QFileDialog::getOpenFileName(this,"選取點雲文件",".//","點雲文件(*.pcd);;所有文件(*.*)");
	
	if (!fileName.isEmpty())
	{
		//清空點雲
		m_currentCloud->clear();
		viewer->removeAllPointClouds();
		viewer->removeAllCoordinateSystems();

		// 讀取點雲數據
		pcl::io::loadPCDFile(fileName.toStdString(), *m_currentCloud);
	}
	//添加到視窗
	viewer->addPointCloud(m_currentCloud);

	pcl::getMinMax3D(*m_currentCloud, p_min, p_max);

	double scale = getMinValue(p_max, p_min);
	maxLen = getMaxValue(p_max, p_min);

	viewer->addCoordinateSystem(scale * 0.3);

	// 重設視角
	viewer->resetCamera();

	//刷新窗口
	ui->qvtkWidget->update();
}

// 取得影像
void PCLViewer::on_action_getimage_triggered() {
	imu_renderer::get_camara_image;
}



// 讀取當前資料夾的檔案
void PCLViewer::on_pushButton_clicked() {
	QDir dir("D:/face"); //路徑
	dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	dir.setSorting(QDir::Time);

	QFileInfoList list = dir.entryInfoList();
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		QTreeWidgetItem* newItem = new QTreeWidgetItem();
		newItem->setText(0, fileInfo.fileName());
		newItem->setText(1, QString::number(fileInfo.size()));
		newItem->setText(2, fileInfo.absoluteFilePath());
		ui->treeWidget->addTopLevelItem(newItem);
	}
}



//重設視角
void PCLViewer::on_action_reset_triggered()
{
	if (!m_currentCloud->empty())
	{
		viewer->resetCamera();
		ui->qvtkWidget->update();
	}
}

// 俯視圖
void PCLViewer::on_action_up_triggered()
{
	if (!m_currentCloud->empty())
	{
		viewer->setCameraPosition(0.5 * (p_min.x + p_max.x), 0.5 * (p_min.y + p_max.y), p_max.z + 2 * maxLen, 0.5 * (p_min.x + p_max.x), 0.5 * (p_min.y + p_max.y), 0, 0, 1, 0);
		ui->qvtkWidget->update();
	}
}

// 前視圖
void PCLViewer::on_action_front_triggered()
{
	if (!m_currentCloud->empty())
	{
		viewer->setCameraPosition(0.5 * (p_min.x + p_max.x), p_min.y - 2 * maxLen, 0.5 * (p_min.z + p_max.z), 0.5 * (p_min.x + p_max.x), 0, 0.5 * (p_min.z + p_max.z), 0, 0, 1);
		ui->qvtkWidget->update();
	}
}

//左視圖
void PCLViewer::on_action_left_triggered()
{
	if (!m_currentCloud->empty())
	{
		viewer->setCameraPosition(p_min.x - 2 * maxLen, 0.5 * (p_min.y + p_max.y), 0.5 * (p_min.z + p_max.z), 0, 0.5 * (p_min.y + p_max.y), 0.5 * (p_min.z + p_max.z), 0, 0, 1);
		ui->qvtkWidget->update();
	}
}

//後視圖
void PCLViewer::on_action_back_triggered()
{
	if (!m_currentCloud->empty())
	{
		viewer->setCameraPosition(0.5 * (p_min.x + p_max.x), p_max.y + 2 * maxLen, 0.5 * (p_min.z + p_max.z), 0.5 * (p_min.x + p_max.x), 0, 0.5 * (p_min.z + p_max.z), 0, 0, 1);
		ui->qvtkWidget->update();
	}
}

// 右視圖
void PCLViewer::on_action_right_triggered()
{
	if (!m_currentCloud->empty())
	{
		viewer->setCameraPosition(p_max.x + 2 * maxLen, 0.5 * (p_min.y + p_max.y), 0.5 * (p_min.z + p_max.z), 0, 0.5 * (p_min.y + p_max.y), 0.5 * (p_min.z + p_max.z), 0, 0, 1);
		ui->qvtkWidget->update();
	}
}

// 底視圖
void PCLViewer::on_action_bottom_triggered()
{
	if (!m_currentCloud->empty())
	{
		viewer->setCameraPosition(0.5 * (p_min.x + p_max.x), 0.5 * (p_min.y + p_max.y), p_min.z - 2 * maxLen, 0.5 * (p_min.x + p_max.x), 0.5 * (p_min.y + p_max.y), 0, 0, 1, 0);
		ui->qvtkWidget->update();
	}
}

// 前軸測
void PCLViewer::on_action_frontIso_triggered()
{
	if (!m_currentCloud->empty())
	{
		viewer->setCameraPosition(p_min.x - 2 * maxLen, p_min.y - 2 * maxLen, p_max.z + 2 * maxLen, 0.5 * (p_min.x + p_max.x), 0.5 * (p_min.y + p_max.y), 0.5 * (p_min.z + p_max.z), 1, 1, 0);
		ui->qvtkWidget->update();
	}
}

// 後軸測
void PCLViewer::on_action_backIso_triggered()
{
	viewer->setCameraPosition(p_max.x + 2 * maxLen, p_max.y + 2 * maxLen, p_max.z + 2 * maxLen, 0.5 * (p_min.x + p_max.x), 0.5 * (p_min.y + p_max.y), 0.5 * (p_min.z + p_max.z), -1, -1, 0);
	ui->qvtkWidget->update();
}

// 設置點雲顏色
void PCLViewer::on_action_setColor_triggered()
{
	QColor color = QColorDialog::getColor(Qt::white, this, "設置點雲顏色", QColorDialog::ShowAlphaChannel);

	viewer->removeAllPointClouds();
	pcl::visualization::PointCloudColorHandlerCustom<PointT> singelColor(m_currentCloud, color.red(), color.green(), color.blue());
	viewer->addPointCloud(m_currentCloud, singelColor, "myCloud", 0);
	viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY, color.alpha() * 1.0 / 255, "myCloud");

	ui->qvtkWidget->update();

}

// 設置高度渲染
void PCLViewer::on_action_heightRamp_triggered()
{
	heightRampDlg.show();
}

// 進行高度渲染
void PCLViewer::setHeightRamp(int dir, double height1)
{
	//清空點雲
	viewer->removeAllPointClouds();
	m_heightCloudList.clear();

	double min_range = 0;
	double max_range = 0;
	std::string field = "x";

	switch (dir)
	{
	case 0:
		min_range = p_min.x;
		max_range = p_max.x;
		field = "x";
		break;

	case 1:
		min_range = p_min.y;
		max_range = p_max.y;
		field = "y";
		break;

	case 2:
		min_range = p_min.z;
		max_range = p_max.z;
		field = "z";
		break;
	default:
		break;
	}

	for (double i = min_range - 1; i < max_range + height;)
	{
		PointCloudT::Ptr cloudTemp(new PointCloudT());

		pcl::PassThrough<PointT> pass;			//直通濾波器對象
		pass.setInputCloud(m_currentCloud);		//輸入點雲
		pass.setFilterFieldName(field);			//設置過濾字段
		pass.setFilterLimits(i, i + height1);	//設置過濾範圍
		pass.setFilterLimitsNegative(false);	//設置保留字段
		pass.filter(*cloudTemp);				//執行濾波

		i += height1;

		m_heightCloudList.append(cloudTemp);
	}

	//分段渲染
	for (int j = 0; j < m_heightCloudList.size(); j++)
	{
		pcl::visualization::PointCloudColorHandlerGenericField<PointT> fieldColor(m_heightCloudList.at(j), field);
		std::string index = std::to_string(j);
		viewer->addPointCloud(m_heightCloudList.at(j), fieldColor, index);
	}

}

double getMinValue(PointT p1, PointT p2)
{
	double min = 0;

	if (p1.x - p2.x > p1.y - p2.y)
	{
		min = p1.y - p2.y;
	}
	else
	{
		min = p1.x - p2.x;
	}

	if (min > p1.z - p2.z)
	{
		min = p1.z - p2.z;
	}

	return min;
}


double getMaxValue(PointT p1, PointT p2)
{
	double max = 0;

	if (p1.x - p2.x > p1.y - p2.y)
	{
		max = p1.x - p2.x;

	}
	else
	{
		max = p1.y - p2.y;
	}

	if (max < p1.z - p2.z)
	{
		max = p1.z - p2.z;
	}

	return max;
}

PCLViewer::~PCLViewer()
{
	delete ui;
}