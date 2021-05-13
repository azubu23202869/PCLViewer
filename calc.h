#pragma once
#pragma warning(disable:4996)
#include <string>
#include <vector>

// Point Cloud Library
#include <boost/make_shared.hpp>              //boost指针相关头文件
#include <pcl/io/pcd_io.h>					//PCL的PCD格式文件的输入输出头文件
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>					//PCL对各种格式的点的支持头文件
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/common/transforms.h>
#include <pcl/point_representation.h>         //点表示相关的头文件
#include <pcl/filters/voxel_grid.h>           //用于体素网格化的滤波类头文件 
#include <pcl/filters/filter.h>             //滤波相关头文件
#include <pcl/features/normal_3d.h>         //法线特征头文件
#include <pcl/registration/icp.h>           //ICP类相关头文件
#include <pcl/registration/icp_nl.h>        //非线性ICP 相关头文件
#include <pcl/registration/transforms.h>      //变换矩阵类头文件
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/surface/gp3.h>
// Visualization Toolkit (VTK)
#include <vtkRenderWindow.h>
/*过滤背景*/
#include <pcl/surface/mls.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/passthrough.h>
#include <pcl/features/normal_3d_omp.h>
#include <pcl/filters/radius_outlier_removal.h>
#include <pcl/filters/conditional_removal.h> 
#include <pcl/features/fpfh.h>
#include <pcl/features/fpfh_omp.h>
#include <pcl/registration/icp.h>
#include <pcl/registration/sample_consensus_prerejective.h>
#include <pcl/registration/ia_ransac.h>
#include <pcl/filters/project_inliers.h>
#include<iostream>
#include <pcl/registration/ia_fpcs.h>

#include <pcl/surface/convex_hull.h>

#include <pcl/PolygonMesh.h>
#include <pcl/io/vtk_lib_io.h>
#include <pcl/features/moment_of_inertia_estimation.h>
//点云需要的头文件
#include <pcl/io/ply_io.h>
#include <thread>//多线程
#include <regex> 
//添加需要的头文件

#include <pcl/point_types.h>  
#include <pcl/io/pcd_io.h>  

#include <pcl/io/obj_io.h>
#include <pcl/PolygonMesh.h>
#include <pcl/io/vtk_lib_io.h>
#include <pcl/kdtree/kdtree_flann.h>  
#include <pcl/features/normal_3d.h>  
#include <pcl/surface/gp3.h>  
#include <pcl/visualization/pcl_visualizer.h>  
#include <pcl/features/from_meshes.h>
#include <boost/thread/thread.hpp>  
#include <fstream>  
#include <iostream>  
#include <stdio.h>  
#include <string.h>  
#include <pcl/common/common.h>
#include "pcl/features/normal_3d.h"
#include "pcl/Vertices.h"
using namespace Eigen;
using namespace std;
using namespace pcl::io;
using namespace pcl::console;
using namespace pcl::search;
using namespace pcl::visualization;
#include <stdio.h>
#include <boost/shared_ptr.hpp>

typedef pcl::PointXYZRGB PointT; //：：声明函数变量   前面类名称，后面则是成员
typedef pcl::PointCloud<PointT> PointCloudT;

typedef pcl::Normal sufaceNormals;

typedef pcl::PointCloud<sufaceNormals> NormalCloud; //法向量点云

typedef pcl::PointNormal NormalT;
typedef pcl::PointCloud<NormalT> NormalCloudT; //法向量点云

typedef pcl::search::KdTree<pcl::PointXYZRGB> SearchMethod;//搜寻算法
typedef pcl::FPFHSignature33 FeatureT; //定义FPFH特征类型，33维向量
typedef pcl::PointCloud<FeatureT> FeatureCloudT; //FPFH特征点云

typedef pcl::FPFHEstimation<PointT, NormalCloudT, FeatureT> FeatureEstimationT; //点云特征匹配对

typedef pcl::SampleConsensusInitialAlignment<PointT, PointT, FeatureT>FPFHPAlignmentT;//
typedef pcl::registration::FPCSInitialAlignment<pcl::PointXYZRGB, pcl::PointXYZRGB>AlignmentT; //4pcs
typedef pcl::IterativeClosestPoint<pcl::PointXYZRGB, pcl::PointXYZRGB>ICPAlignmentT;
//pcl::registration::FPCSInitialAlignment<pcl::PointXYZRGB, pcl::PointXYZRGB> fpcs;
typedef pcl::search::KdTree<pcl::PointXYZRGB> SearchT; //搜索方法


struct PlantParametersName_S {	//儲存整株參數簡寫
	string SolidityCompactness = "pSC";	//string 參數英文名 = "簡寫";
	string CircleDiameter = "pCD";
	string Eccentricity = "pE";
	string MaxWidth = "pMW";
	string Centroid = "pC";
	string LightPenetrationDepth = "pLPD";
	string DigitalBiomass = "pDB";
	string LeafAverageAngle = "pLAA";
	string LeafMaxInclinationAngle = "pLMIA";
	string TotalLeafArea = "pTLA";
	string ConvexHullArea = "pCHA";
	string ProjectedLeafArea = "pPLA";
	string LeafAreaIndex = "pLAI";
	string GrowthHeight = "pGH";
};
struct LeafParametersName_S {	//儲存單葉參數簡寫
	string LeafAlive = "Alive";
	string LeafBladeLength = "iL";
	string LeafWidth = "iW";
	string LeafHighestPoint = "iHP";
	string Perimeter = "iP";
	string MediumLeafWidthIndex = "iMLWI";
	string SlendernessOfLeaves = "iSOL";
	string Roundness = "iR";
	string LeafArea = "iLA";
	string LeafProjectedArea = "iLPA";
	string LeafInclinationAngle = "iLIA";
	string LeafAzimuthAngle = "iLAA";
	string LeafInclination = "iLI";
};
struct Parameter {	//參數結構

	bool isPoint;	//是否為三維點（用於將參數寫入json時進行判斷）
	bool isBool;	//是否為布林值（用於將參數寫入json時進行判斷）
	string name;	//參數名稱簡寫
	float value;	//參數數值
	bool bValue;	//參數布林值
	Eigen::Vector3f TDPoint;	//參數三維點座標ax

};

class ParametersCalculator {
public:
	ParametersCalculator();
	~ParametersCalculator();
	auto calc(const string& PlyName);

private:
	int k;

	PointCloudT::Ptr& Plant_PointCloud;	//輸入的點雲
	PointCloudT::Ptr& ProjectedPointCloud;	//壓平後的點雲
	NormalCloudT::Ptr& MeshNormals;	//所有網格的法線向量
	NormalCloudT::Ptr& AvgNormals;	//平均法線向量
	const char* test_word;
	pcl::PolygonMesh Plant_MeshTriangle;	//mesh後的網格
	pcl::PointXYZ Min_Point;	//最小X Y Z座標
	pcl::PointXYZ Max_Point;	//最大X Y Z座標

	Eigen::Vector3f PlantCentroid;	//整株質心座標，用於計算葉片方位角
	vector<float> MeshArea;	//各網格面積
	float Height;	//植物生長高度
	float TLA;	//總葉面積
	float CHA;	//凸包面積
	float PLA;	//投影面積
	float LeafInclinationAngle;	//葉方位角

	Parameter Parameter_tmp;	//參數暫存
	PlantParametersName_S pPName;	//整株參數簡寫名稱
	LeafParametersName_S lPName;	//單葉參數簡寫名稱
	vector<Parameter> Plant_AllParameters;	//存放計算後的所有整株參數數值的陣列
	vector<Parameter> Leaf_AllParameters;	//存放計算後的所有單葉參數數值的陣列

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

	//排序後的Z軸
	vector<float> SortedZ;
};
