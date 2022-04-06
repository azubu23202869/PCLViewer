#pragma warning(disable:4996)
#include "pclviewer.h"


namespace fs = std::experimental::filesystem;

// 取得兩個點平行於坐標軸的最短距離
double getMinValue(PointT p1, PointT p2);
// 取得兩個點平行於座標軸的最長距離
double getMaxValue(PointT p1, PointT p2);


PCLViewer::PCLViewer(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::PCLViewer)
{
	ui->setupUi(this);
	path = "./3Dparty/3DTestFile/";
	imgpath = ". / image / gk.png";

	//初始化
	//----------------------------------------------------------------
	setWindowIcon(QIcon(imgpath));
	initPCL();
	initTreeWidgetFileList();
	// connect(ui->action_play, &QAction::triggered, [=]() { QThreadPool::globalInstance()->start(camera); });
	//----------------------------------------------------------------
}

void PCLViewer::initPCL()
{
	// 點雲初始化
	m_currentCloud.reset(new PointCloudT);

	// 可視化初始化
	viewer.reset(new pcl::visualization::PCLVisualizer("viewer", false));

	ui->qvtkWidget->SetRenderWindow(viewer->getRenderWindow());
	viewer->setupInteractor(ui->qvtkWidget->GetInteractor(), ui->qvtkWidget->GetRenderWindow());
	viewer->setBackgroundColor(255, 255, 255);
	viewer->addCoordinateSystem(1.0);
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_(new pcl::PointCloud<pcl::PointXYZRGB>);
	viewer->registerAreaPickingCallback(&PCLViewer::pp_callback_AreaSelect, *this);
	this->cloud = cloud_;

	while (!viewer->wasStopped())
	{
		viewer->spinOnce(100);
		boost::this_thread::sleep(boost::posix_time::microseconds(100000));
	}

	// 事件
	connect(&heightRampDlg, SIGNAL(setHeightRamp(int, double)), this, SLOT(setHeightRamp(int, double)));

	ui->qvtkWidget->update();
}

void PCLViewer::on_action_open_triggered()
{
	// 取得點雲路徑
	QString fileName = QFileDialog::getOpenFileName(this, "選取點雲文件", ".//", "點雲文件(*.pcd *.ply);;所有文件(*.*)");

	if (!fileName.isEmpty())
	{
		ReadPclFile(fileName);
	}
}

void PCLViewer::on_actionaction_paint_triggered()
{
	QMessageBox::about(NULL, "特徵提取", "框選點雲並保存快捷鍵:<font color='red', size='4'> X </font>");
}


//----------------------------------------------------------------
// PCL讀取單一點雲
//----------------------------------------------------------------
void PCLViewer::pp_callback_AreaSelect(const pcl::visualization::AreaPickingEvent& event, void* args)				//new
{
	if (event.getPointsIndices(indices) == -1)
		return;

	for (int i = 0; i < indices.size(); ++i)
	{
		cloud->points.push_back(m_currentCloud->points.at(indices[i]));
	}

	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZRGB>red(cloud, 255, 0, 0);

	std::stringstream ss;
	std::string cloudname;
	num++;
	ss << num;
	ss >> cloudname;
	cloudname += "_cloudname";
	viewer->addPointCloud(cloud, red, cloudname);
	viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 10, cloudname);

	QMessageBox message(QMessageBox::NoIcon, "存檔", "請問要保存嗎?", QMessageBox::Yes | QMessageBox::No, NULL);
	if (message.exec() == QMessageBox::Yes)
	{
		pcl::io::savePLYFileBinary(".\\3Dparty\\3DTestFile\\" + cloudname + ".ply", *cloud);
		initTreeWidgetFileList();
	}

}

//----------------------------------------------------------------
// 讀取3D檔案
//----------------------------------------------------------------
void PCLViewer::ReadPclFile(const QString& fullPathName) {

	//清空點雲
	m_currentCloud->clear();
	viewer->removeAllPointClouds();
	viewer->removeAllCoordinateSystems();

	QFileInfo fi(fullPathName);
	NowReadFileName = fi.baseName().toStdString();

	// 讀取點雲數據
	if (fullPathName.endsWith(".pcd", Qt::CaseInsensitive))
		pcl::io::loadPCDFile(fullPathName.toStdString(), *m_currentCloud);
	else
		pcl::io::loadPLYFile(fullPathName.toStdString(), *m_currentCloud);

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


void PCLViewer::initTreeWidgetFileList() {
	ui->treeWidgetFilelist->clear();
	//創建頂層
	QStringList columnTitle;
	columnTitle.append("檔案");
	fs::create_directory("./3Dparty");
	ui->treeWidgetFilelist->setHeaderLabels(columnTitle);
	QIcon icon;
	icon.addFile("./image/open.png");    //設置icon
	QTreeWidgetItem* item = new QTreeWidgetItem(PCLViewer::itTopItem);
	item->setText(colItem, path);
	item->setIcon(colItem, QIcon(icon));
	ui->treeWidgetFilelist->addTopLevelItem(item);
	AllFile(item, path);
}

//----------------------------------------------------------------
// 開啟資料夾
//----------------------------------------------------------------
void PCLViewer::on_action_opendirectory_triggered() 
{
	path = nullptr;
	QString strpath = QFileDialog::getExistingDirectory(this, "開啟資料夾", "./", QFileDialog::ShowDirsOnly);
	path = QDir::fromNativeSeparators(strpath) + "/";

	ui->treeWidgetFilelist->clear();
	QIcon icon;
	icon.addFile("./image/open.png");    //設置icon
	QTreeWidgetItem* item = new QTreeWidgetItem(PCLViewer::itTopItem);
	item->setText(colItem, path);
	item->setIcon(colItem, QIcon(icon));
	ui->treeWidgetFilelist->addTopLevelItem(item);
	AllFile(item, path);
}

//----------------------------------------------------------------
// 讀取當前目錄
//----------------------------------------------------------------

QFileInfoList PCLViewer::AllFile(QTreeWidgetItem* root, const QString& path) {
	/*添加path路徑文件*/
	QDir dir(path);          //遍歷各子目錄
	QDir dir_file(path);    //遍歷子目錄中所有文件
	dir_file.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);        //取得當前所有文件
	dir_file.setSorting(QDir::Size | QDir::Reversed);
	QFileInfoList list_file = dir_file.entryInfoList();
	for (int i = 0; i < list_file.size(); ++i) {       //將當前目錄中所有文件添加到treewidget中
		QFileInfo fileInfo = list_file.at(i);
		QString name2 = fileInfo.fileName();
		QTreeWidgetItem* child = new QTreeWidgetItem(PCLViewer::itImageItem);
		child->setText(colItem, name2);
		child->setIcon(0, QIcon("./image/file.png"));
		child->setCheckState(1, Qt::Checked);
		root->addChild(child);
	}
	QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);   //獲取當前所有目錄
	for (int i = 0; i != folder_list.size(); i++)         //自動遞迴添加到各目錄到上一級目錄
	{
		QString namepath = folder_list.at(i).absoluteFilePath();    //取得路徑
		QFileInfo folderinfo = folder_list.at(i);
		QString name = folderinfo.fileName();      //取得目錄名
		QTreeWidgetItem* childroot = new QTreeWidgetItem(PCLViewer::itGroupItem);
		childroot->setText(colItem, name);
		childroot->setIcon(0, QIcon("./image/open.png"));
		childroot->setCheckState(1, Qt::Checked);
		root->addChild(childroot);              //將當前目錄添加成path的子項目
		QFileInfoList child_file_list = AllFile(childroot, namepath);          //進行遞迴
		file_list.append(child_file_list);
		file_list.append(name);
	}
	return file_list;
}



//----------------------------------------------------------------
//Call CMD
//----------------------------------------------------------------
void PCLViewer::on_treeWidgetFilelist_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
	QString filename;
	QString dir = path;
	qDebug() << dir;
	NowReadFileName = "";
	//列表響應
	if (current == Q_NULLPTR) return;

	int var = current->type();
	switch (var) {
	case itTopItem:
		break;

	case itGroupItem:
		break;

	case itImageItem:
		filename = current->text(current->data(colItem, Qt::UserRole).toInt());
		ReadPclFile(dir + filename);
		break;
	}
}


//----------------------------------------------------------------
// 取像
//----------------------------------------------------------------
void PCLViewer::on_action_play_triggered() 
{
	QMessageBox* pMsgBox = new QMessageBox;
	pMsgBox->setStandardButtons(QMessageBox::NoButton);  //把預設的 OK button 移除
	QLabel* pMsgLabel = new QLabel;  //用 QLabel 是因為 layout 時比較好對齊
	pMsgLabel->setText("取像中, 請稍等...");
	QProgressBar* pProgressBar = new QProgressBar;
	pProgressBar->setValue(24);  //隨便設個初始值當範例
	QGridLayout* pLayout = new QGridLayout;
	pLayout = static_cast<QGridLayout*>(pMsgBox->layout());
	pLayout->addWidget(pMsgLabel, 0, 0);
	pLayout->addWidget(pProgressBar, 1, 0);
	pMsgBox->show();
	mySystem("CppPythonCallback.exe", ".\\3Dparty\\Open3D\\examples\\python\\reconstruction_system\\sensors");
	pProgressBar->setValue(100);  
	pMsgBox->setStandardButtons(QMessageBox::Ok);
}

//----------------------------------------------------------------
//重構
//----------------------------------------------------------------
void PCLViewer::on_action_reconstruction_triggered()
{
	QMessageBox message1(QMessageBox::NoIcon, "重構", "是否要重構?", QMessageBox::Yes | QMessageBox::No, NULL);
	if (message1.exec() == QMessageBox::Yes)
	{
		system("cd .\\3Dparty\\Open3D\\examples\\python\\reconstruction_system && python run_system.py config/realsense.json --make --register --refine --integrate && /k");
	}

	QMessageBox message(QMessageBox::NoIcon, "開啟","已經重構完成是否要轉換?", QMessageBox::Yes | QMessageBox::No, NULL);
	
	if (message.exec() == QMessageBox::Yes)
	{
		if (fs::exists(".\\3Dparty\\Open3D\\examples\\python\\reconstruction_system\\dataset\\realsense\\scene\\integrated.ply")) {
			if (!fs::exists(".\\3Dparty\\3DTestFile\\integrated.ply")) {
				fs::copy_file(".\\3Dparty\\Open3D\\examples\\python\\reconstruction_system\\dataset\\realsense\\scene\\integrated.ply", ".\\3Dparty\\3DTestFile\\integrated.ply");
			}
			else
			{
				fs::remove(".\\3Dparty\\3DTestFile\\integrated.ply");
				fs::copy_file(".\\3Dparty\\Open3D\\examples\\python\\reconstruction_system\\dataset\\realsense\\scene\\integrated.ply", ".\\3Dparty\\3DTestFile\\integrated.ply");
			}
			system("cd .\\3Dparty\\3DTestFile\\ && python transform.py && /c");
			QMessageBox::about(NULL, "完成", "轉換完成");
			initTreeWidgetFileList();
		}
		else
		{
			QMessageBox::about(NULL, "失敗", "<font color='red'> 未找到PLY檔案!! </font>");
		}
	}

}


//----------------------------------------------------------------
//Call CMD
//----------------------------------------------------------------
void PCLViewer::mySystem(const std::string& cmd,const std::string& dir)
{
	std::wstring stemp = std::wstring(cmd.begin(), cmd.end());
	LPCWSTR _cmd = stemp.c_str();
	std::wstring sstemp = std::wstring(dir.begin(), dir.end());
	LPCWSTR _dir = sstemp.c_str();

	SHELLEXECUTEINFO ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = _cmd;
	ShExecInfo.lpParameters = NULL;
	ShExecInfo.lpDirectory = _dir;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
	CloseHandle(ShExecInfo.hProcess);

}

//----------------------------------------------------------------
//重設視角
//----------------------------------------------------------------
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

	for (double i = min_range - 1; i < max_range + height1;)
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