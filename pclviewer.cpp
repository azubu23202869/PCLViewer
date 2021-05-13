#pragma warning(disable:4996)
#include "pclviewer.h"


namespace fs = std::experimental::filesystem;

// 取得兩個點平行於坐標軸的最短距離
double getMinValue(PointT1 p1, PointT1 p2);
// 取得兩個點平行於座標軸的最長距離
double getMaxValue(PointT1 p1, PointT1 p2);

QStringList datastr = { "高度" ,"光投射深度","面積","平均角度","凸包","投影面積","緊實度","體積參數","植物圓直徑","質心", "最大高度","偏心率" };
 // Camera* camera = new Camera();

PCLViewer::PCLViewer(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::PCLViewer)
{
	ui->setupUi(this);


	//初始化
	//----------------------------------------------------------------
	initPCL();
	initTreeWidget();
	initTreeWidgetFileList();
	initTreeWidgetParameter();
	ui->toolBar_Right->setEnabled(0);
	ui->action_getimage->setEnabled(0);
	ui->action_play->setEnabled(0);
	ui->action_delete->setEnabled(0);
	ui->action_update->setEnabled(0);
	// connect(camera, &Camera::framesReady, this, &PCLViewer::receiveFrame);
	// connect(ui->action_play, &QAction::triggered, [=]() { QThreadPool::globalInstance()->start(camera); });
	//----------------------------------------------------------------
}

void PCLViewer::initPCL()
{
	// 點雲初始化
	m_currentCloud.reset(new PointCloudT1);
	// 可視化初始化
	viewer.reset(new pcl::visualization::PCLVisualizer("viewer", false));
	// 設置VTK可視化窗口指針
	ui->qvtkWidget->SetRenderWindow(viewer->getRenderWindow());
	// 設置視窗互動
	viewer->setupInteractor(ui->qvtkWidget->GetInteractor(), ui->qvtkWidget->GetRenderWindow());
	// 添加坐標軸
	viewer->addCoordinateSystem(1.0);

	// 事件
	connect(&heightRampDlg, SIGNAL(setHeightRamp(int, double)), this, SLOT(setHeightRamp(int, double)));
}

void PCLViewer::on_action_open_triggered()
{
	// 取得點雲路徑
	QString fileName = QFileDialog::getOpenFileName(this, "選取點雲文件", ".//", "點雲文件(*.ply);;所有文件(*.*)");

	if (!fileName.isEmpty())
	{
		ReadPclFile(fileName);
	}
}

void PCLViewer::ReadPclFile(const QString& fullPathName) {
	//清空點雲
	m_currentCloud->clear();
	viewer->removeAllPointClouds();
	viewer->removeAllCoordinateSystems();

	// 讀取參數
	ReadPara(fullPathName);

	// 讀取點雲數據
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



void PCLViewer::initTreeWidget()
{
	QStringList headerList;
	headerList.append(tr("檔案名稱"));
	ui->treeWidgetFilelist->setHeaderLabels(headerList);
	headerList.clear();
	headerList.append(tr("參數詳情"));
	headerList.append(tr(""));
	ui->treeWidgetParameter->setHeaderLabels(headerList);

	//自動調整行寬
	ui->treeWidgetFilelist->header()->setSectionResizeMode(QHeaderView::ResizeToContents); 
	ui->treeWidgetParameter->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui->treeWidgetParameter->setWordWrap(true);

	fs::create_directories("./PointCloudFile/PLYFile");
	fs::create_directories("./PointCloudFile/GetJPG");
	fs::create_directories("./PointCloudFile/GetPNG");

	// item->setForeground(column,QBrush(color));
	// ui->treeWidgetFilelist->setStyleSheet("background-color:rgb(99, 99, 99);color:white");
	/*
	QTreeWidgetItem* item = new QTreeWidgetItem(PCLViewer::itTopItem);
	item->setIcon(PCLViewer::colItem, icon);        //第一行圖標
	item->setText(PCLViewer::colItem, "PCD文件");  //第一列的文字
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
	item->setCheckState(colItem, Qt::Checked);
	item->setData(PCLViewer::colItem, Qt::UserRole, QVariant(dataStr));
	ui->treeWidget->setHeaderItem(item);           //添加頂層節點
	*/
}




void PCLViewer::initTreeWidgetParameter() {
	ui->treeWidgetParameter->clear();
	for each (auto num in datastr) {
		QTreeWidgetItem* item1 = new QTreeWidgetItem();
		item1->setText(colItem, num);
		ui->treeWidgetParameter->addTopLevelItem(item1);
	}
}

void PCLViewer::ReadPara(const QString& Plypath) {
	ui->treeWidgetParameter->clear();
	string tmp;
	Readcalc(Plypath) >> tmp;
	// qDebug() << QString::fromStdString(tmp);
	vector<string> ans;
	char splitsymbo = ',';
	tokenize(tmp, splitsymbo, ans);
	// for (auto it : ans) qDebug() << it.c_str();
	QStringList Para;
	auto count = 0;
	tmp.clear();
	tmp = ans[9] + ", \n" + ans[10] + ", \n" + ans[11];
	for each (auto num in datastr) {
		Para << num;
		if (count == 9) {
			Para << tmp.c_str();
			QTreeWidgetItem* item1 = new QTreeWidgetItem(Para);
			ui->treeWidgetParameter->addTopLevelItem(item1);
			count = 12;
		}
		else
		{
			Para.append(ans[count].c_str());
			QTreeWidgetItem* item1 = new QTreeWidgetItem(Para);
			ui->treeWidgetParameter->addTopLevelItem(item1);
			count++;
		}
		Para.clear();
	}
}

void PCLViewer::initTreeWidgetFileList() {
	//創建頂層
	QString path = "./PointCloudFile/";
	QIcon icon;
	icon.addFile("./image/open.png");    //設置icon
	QTreeWidgetItem* item = new QTreeWidgetItem(PCLViewer::itTopItem);
	item->setText(colItem, path);
	item->setIcon(colItem, QIcon(icon));
	item->setCheckState(1, Qt::Checked);
	ui->treeWidgetFilelist->addTopLevelItem(item);
	AllFile(item, path);
}
// 讀取當前目錄
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




void PCLViewer::on_treeWidgetFilelist_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
	QString filename;
	QString dir = "./PointCloudFile/PLYFile/";

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


// 收到Frame
void PCLViewer::receiveFrame(QImage rgb, QImage depth)
{	

	ui->rgb_label->setPixmap(QPixmap::fromImage(rgb));
	ui->depth_label->setPixmap(QPixmap::fromImage(depth));

}



// 切換頁面
//----------------------------------------------------------------
void PCLViewer::on_action_getimage_triggered() {

	ui->stackedWidget->setCurrentIndex(1);

}

//----------------------------------------------------------------
// 頁面1
//----------------------------------------------------------------







//----------------------------------------------------------------
// 頁面2
//----------------------------------------------------------------


void PCLViewer::on_action_play_triggered() {





}

void PCLViewer::on_action_delete_triggered() {
	



}


//----------------------------------------------------------------


void PCLViewer::uiDisable() {




}


void PCLViewer::uiEnable() {





}



void PCLViewer::on_action_update_triggered() {
	QTreeWidgetItemIterator it(ui->treeWidgetParameter);

	vector<string> myTreeStr;
	while (*it) {
		qDebug() << (*it)->text(0) << "," << (*it)->text(1);
		++it;
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
	pcl::visualization::PointCloudColorHandlerCustom<PointT1> singelColor(m_currentCloud, color.red(), color.green(), color.blue());
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
		PointCloudT1::Ptr cloudTemp(new PointCloudT1());

		pcl::PassThrough<PointT1> pass;			//直通濾波器對象
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
		pcl::visualization::PointCloudColorHandlerGenericField<PointT1> fieldColor(m_heightCloudList.at(j), field);
		std::string index = std::to_string(j);
		viewer->addPointCloud(m_heightCloudList.at(j), fieldColor, index);
	}

}

double getMinValue(PointT1 p1, PointT1 p2)
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


double getMaxValue(PointT1 p1, PointT1 p2)
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



stringstream PCLViewer::Readcalc(const QString& Plypath) {

	string infile = Plypath.toStdString();

	pcl::PolygonMesh mesh;

	if (pcl::io::loadPLYFile(infile, mesh))
	{
		std::cout << "error";
	}
	//ParametersCalculator ParametersCalculator;
	pcl::PointCloud<pcl::PointXYZ>::Ptr SRC_cloud(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr SRC_RGB_cloud(new pcl::PointCloud<pcl::PointXYZRGB>);

	fromPCLPointCloud2(mesh.cloud, *SRC_RGB_cloud);

	//--------------最小X Y Z座標 //最大X Y Z座標------------//
	pcl::PointXYZ min_sp, max_sp;
	pcl::getMinMax3D(*SRC_cloud, min_sp, max_sp);

	//--------------排序Z軸，計算光投射深度用-----------------///

	vector<float> SortedZ;
	for (int i = 0; i < SRC_RGB_cloud->points.size(); i++) {
		SortedZ.push_back(SRC_RGB_cloud->points[i].z);
	}

	sort(SortedZ.begin(), SortedZ.end());
	//----------讀取網格資料

		//三角網格的三個點(A B C)座標(x,y,z)
	vector<float> Ax;
	vector<float> Ay;
	vector<float> Az;
	vector<float> Bx;
	vector<float> By;
	vector<float> Bz;
	vector<float> Cx;
	vector<float> Cy;
	vector<float> Cz;

	pcl::PolygonMesh Plant_MeshTriangle = mesh;

	//將每個三角網格(A,B,C)的三個座標點(x,y,z)存入各別的陣列中
	//pcl::PointCloud<pcl::PointXYZRGB>::Ptr Plant_PointCloud(new pcl::PointCloud<pcl::PointXYZRGB>);//輸入的點雲
	for (int i = 0; i < Plant_MeshTriangle.polygons.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			switch (j)
			{
			case 0:
				Ax.push_back(SRC_RGB_cloud->points[Plant_MeshTriangle.polygons[i].vertices[j]].x);
				Ay.push_back(SRC_RGB_cloud->points[Plant_MeshTriangle.polygons[i].vertices[j]].y);
				Az.push_back(SRC_RGB_cloud->points[Plant_MeshTriangle.polygons[i].vertices[j]].z);

				break;
			case 1:
				Bx.push_back(SRC_RGB_cloud->points[Plant_MeshTriangle.polygons[i].vertices[j]].x);
				By.push_back(SRC_RGB_cloud->points[Plant_MeshTriangle.polygons[i].vertices[j]].y);
				Bz.push_back(SRC_RGB_cloud->points[Plant_MeshTriangle.polygons[i].vertices[j]].z);

				break;
			case 2:
				Cx.push_back(SRC_RGB_cloud->points[Plant_MeshTriangle.polygons[i].vertices[j]].x);
				Cy.push_back(SRC_RGB_cloud->points[Plant_MeshTriangle.polygons[i].vertices[j]].y);
				Cz.push_back(SRC_RGB_cloud->points[Plant_MeshTriangle.polygons[i].vertices[j]].z);

				break;
			}
		}
	}

	//計算點雲向量，並正規化
	NormalCloud::Ptr OriginalNormals(new NormalCloud);
	NormalCloud::Ptr MeshNormals(new NormalCloud);
	NormalCloud::Ptr AvgNormals(new NormalCloud);
	NormalCloudT::Ptr normal(new NormalCloudT);


	pcl::features::computeApproximateNormals(*SRC_RGB_cloud, mesh.polygons, *OriginalNormals);



	for (int i = 0; i < mesh.polygons.size(); i++) {
		float len = sqrt(
			pow(OriginalNormals->points[mesh.polygons[i].vertices[0]].normal[0], 2) +
			pow(OriginalNormals->points[mesh.polygons[i].vertices[0]].normal[1], 2) +
			pow(OriginalNormals->points[mesh.polygons[i].vertices[0]].normal[2], 2));
		if (len == 0)
			continue;
		if (OriginalNormals->points[mesh.polygons[i].vertices[0]].normal[2] < 0)
		{
			OriginalNormals->points[mesh.polygons[i].vertices[0]].normal[0] *= -1;
			OriginalNormals->points[mesh.polygons[i].vertices[0]].normal[1] *= -1;
			OriginalNormals->points[mesh.polygons[i].vertices[0]].normal[2] *= -1;
		}

		pcl::Normal N_tmp;
		N_tmp.normal_x = OriginalNormals->points[mesh.polygons[i].vertices[0]].normal[0] / len;
		N_tmp.normal_y = OriginalNormals->points[mesh.polygons[i].vertices[0]].normal[1] / len;
		N_tmp.normal_z = OriginalNormals->points[mesh.polygons[i].vertices[0]].normal[2] / len;

		MeshNormals->push_back(N_tmp);
	}


	//計算平均法向量
	double avg_Normal_x = 0;
	double avg_Normal_y = 0;
	double avg_Normal_z = 0;
	for (int i = 0; i < MeshNormals->size(); i++) {
		avg_Normal_x += MeshNormals->points[i].normal_x;
		avg_Normal_y += MeshNormals->points[i].normal_y;
		avg_Normal_z += MeshNormals->points[i].normal_z;
	}
	avg_Normal_x /= MeshNormals->size();
	avg_Normal_y /= MeshNormals->size();
	avg_Normal_z /= MeshNormals->size();

	pcl::Normal tmp;
	tmp.normal_x = avg_Normal_x;
	tmp.normal_y = avg_Normal_y;
	tmp.normal_z = avg_Normal_z;
	AvgNormals->push_back(tmp);

	//	cout << "AvgNormals" << AvgNormals << endl;
		//存放的
	vector<Parameter> Plant_AllParameters;
	//------------植物生長高度------------//
	string GrowthHeight = "pGH";
	float Height;
	Height = SortedZ[SortedZ.size() - 1] - SortedZ[0];

	//------------植物參數_光投射深度------------//

	int size_10 = floor(SortedZ.size() * 0.1);//10%數量

	vector<float> minZ_10;//最小10%Z值
	vector<float> maxZ_10;//最大10%Z值
	for (int i = 0; i < size_10; i++)
	{
		minZ_10.push_back(SortedZ[i]);
		maxZ_10.push_back(SortedZ[SortedZ.size() - i - 1]);
	}

	float sum_minZ = 0;
	float sum_maxZ = 0;
	for (int i = 0; i < size_10; i++)
	{
		sum_minZ += minZ_10[i];
		sum_maxZ += maxZ_10[i];
	}
	float avg_maxZ = sum_maxZ / size_10;
	float avg_minZ = sum_minZ / size_10;

	float lpd = avg_maxZ - avg_minZ;

	//----------植物參數_網格壓平後面積----------//
	vector<float> MeshArea;	//各網格面積
	float LeafArea = 0;
	float ProjectedArea = 0;
	for (int i = 0; i < MeshNormals->size(); i++)
	{
		Eigen::Vector3f p1(Ax[i], Ay[i], Az[i]);
		Eigen::Vector3f p2(Bx[i], By[i], Bz[i]);
		Eigen::Vector3f p3(Cx[i], Cy[i], Cz[i]);
		Eigen::Vector3f vector_p1p2 = p2 - p1;
		Eigen::Vector3f vector_p1p3 = p3 - p1;
		Eigen::Vector3f cross = vector_p1p2.cross(vector_p1p3);

		//tla總葉面積
		float area = sqrt(pow(cross.x(), 2) + pow(cross.y(), 2) + pow(cross.z(), 2)) / 2;
		MeshArea.push_back(area);
		LeafArea += area;


		if (sqrt(pow(cross.x(), 2) + pow(cross.y(), 2) + pow(cross.z(), 2)) == 0) {
			continue;
		}
		float cos = abs(cross.z() / sqrt(pow(cross.x(), 2) + pow(cross.y(), 2) + pow(cross.z(), 2)));
		ProjectedArea += area * cos;

	}


	//--------------------------植物參數_植物參數_葉平均角----------//
	float laa;

	float NavX = 0;
	float NavY = 0;
	float NavZ = 0;

	for (int i = 0; i < MeshNormals->size(); i++)
	{
		float Vector_NavX = MeshArea[i] * MeshNormals->points[i].normal_x;
		float Vector_NavY = MeshArea[i] * MeshNormals->points[i].normal_y;
		float Vector_NavZ = MeshArea[i] * MeshNormals->points[i].normal_z;

		NavX += Vector_NavX;
		NavY += Vector_NavY;
		NavZ += Vector_NavZ;
	}
	NavX /= MeshNormals->size();
	NavY /= MeshNormals->size();
	NavZ /= MeshNormals->size();

	float len_vectorNav = sqrt(pow(NavX, 2) + pow(NavY, 2) + pow(NavZ, 2));
	NavX /= len_vectorNav;
	NavY /= len_vectorNav;
	NavZ /= len_vectorNav;

	float normalizatedLen = sqrt(pow(NavX, 2) + pow(NavY, 2) + pow(NavZ, 2));
	laa = acos((NavX * 0 + NavY * 0 + NavZ * 1) / normalizatedLen);

	laa = laa / M_PI * 180;
	if (laa < 0)
		laa += 360;

	//--------------------------植物參數_凸包面積

	//std::vector<Vertices> Vert_chullMesh;
	pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients());
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr ProjectedPointCloud(new pcl::PointCloud<pcl::PointXYZRGB>);


	coefficients->values.resize(4);
	coefficients->values[0] = coefficients->values[1] = 0;
	coefficients->values[2] = 1.0;
	coefficients->values[3] = 0;

	pcl::ProjectInliers<pcl::PointXYZRGB> proj;
	proj.setModelType(pcl::SACMODEL_PLANE);
	proj.setInputCloud(SRC_RGB_cloud);
	proj.setModelCoefficients(coefficients);
	proj.filter(*ProjectedPointCloud);

	//savePCDFile("project.pcd", *ProjectedPointCloud);

	//--------------------------凸包
	pcl::PolygonMesh Convex_mesh;
	pcl::ConvexHull<pcl::PointXYZRGB> chull;

	chull.setInputCloud(ProjectedPointCloud);
	chull.setComputeAreaVolume(true);

	float CHA = ProjectedPointCloud->size() * 1.0f;


	//--------------------------植物參數_投影面積

	//--------------------------植物參數_植物緊實度ProjectedArea//亞平
	float sc = ProjectedArea / CHA;

	//--------------------------植物參數_體積參數
	float fDB = Height * ProjectedArea;
	//--------------------------植物參數_植物圓直徑
	float fCD = sqrt(ProjectedArea / M_PI) * 2;


	//--------------------------植物參數_植物質心


	Eigen::Vector4f centroid;
	pcl::compute3DCentroid(*SRC_RGB_cloud, centroid);
	Eigen::Vector3f CD = centroid.head<3>();
	Eigen::Vector3f PlantCentroid = CD;

	//--------------------------植物參數_最大寬度
	float mw = 0;

	pcl::PointCloud<pcl::PointXYZRGB>::Ptr Convex_cloud(new pcl::PointCloud<pcl::PointXYZRGB>);
	pcl::ConvexHull<pcl::PointXYZRGB> dep_chull;
	dep_chull.setInputCloud(ProjectedPointCloud);
	dep_chull.setComputeAreaVolume(true);
	dep_chull.reconstruct(*Convex_cloud);

	for (int i = 0; i < Convex_cloud->size(); i++) {
		for (int j = 0; j < Convex_cloud->size(); j++) {
			float tmp_len = hypot(
				Convex_cloud->points[j].x - Convex_cloud->points[i].x,
				Convex_cloud->points[j].y - Convex_cloud->points[i].y);
			if (tmp_len > mw)
				mw = tmp_len;
		}
	}

	//--------------------------植物參數_偏心率
	float avg_e = 0;

	vector<float> eccentricity;

	pcl::MomentOfInertiaEstimation<pcl::PointXYZ> feature_extractor;
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_xyz(new pcl::PointCloud<pcl::PointXYZ>);
	cloud_xyz->points.resize(SRC_RGB_cloud->size());
	for (int i = 0; i < SRC_RGB_cloud->size(); i++)
	{
		cloud_xyz->points[i].x = SRC_RGB_cloud->points[i].x;
		cloud_xyz->points[i].y = SRC_RGB_cloud->points[i].y;
		cloud_xyz->points[i].z = SRC_RGB_cloud->points[i].z;
	}

	feature_extractor.setInputCloud(cloud_xyz);
	feature_extractor.compute();
	feature_extractor.getEccentricity(eccentricity);

	for (int i = 0; i < eccentricity.size(); i++)
		avg_e += eccentricity[i];
	avg_e /= eccentricity.size();

	stringstream str; 
	str <<	Height				<< "," 
		<<	lpd					<< "," 
		<<	LeafArea			<< "," 
		<<	laa					<< "," 
		<<	CHA					<< ","
		<<	ProjectedArea		<< "," 
		<<	sc					<< "," 
		<<	fDB					<< "," 
		<<	fCD					<< "," 
		<<	PlantCentroid[0]	<< "," 
		<<	PlantCentroid[1]	<< ","
		<<	PlantCentroid[2]	<< ","
		<<	mw					<< "," 
		<<	avg_e;
	return str;
}


void PCLViewer::tokenize(string& str, char delim, vector<string>& out)
{
	size_t start;
	size_t end = 0;

	while ((start = str.find_first_not_of(delim, end)) != string::npos)
	{
		end = str.find(delim, start);
		out.push_back(str.substr(start, end - start));
	}
}







PCLViewer::~PCLViewer()
{
	delete ui;
}