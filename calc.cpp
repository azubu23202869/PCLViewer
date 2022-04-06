
#pragma once
#pragma warning(disable:4996)
#include "calc.h"


auto ParametersCalculator::calc(const string& PlyName) {
	string infile = PlyName;

	pcl::PolygonMesh mesh;

	if (pcl::io::loadPLYFile(infile, mesh))
	{
		std::cout << "error";
	}
	//ParametersCalculator ParametersCalculator;
	pcl::PointCloud<pcl::PointXYZ>::Ptr SRC_cloud(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr SRC_RGB_cloud(new pcl::PointCloud<pcl::PointXYZRGB>);

	fromPCLPointCloud2(mesh.cloud, *SRC_RGB_cloud);

	//--------------程X Y Z畒夹 //程X Y Z畒夹------------//
	pcl::PointXYZ min_sp, max_sp;
	pcl::getMinMax3D(*SRC_cloud, min_sp, max_sp);

	//--------------逼Z禸璸衡щ甮瞏ノ-----------------///

	vector<float> SortedZ;
	for (int i = 0; i < SRC_RGB_cloud->points.size(); i++) {
		SortedZ.push_back(SRC_RGB_cloud->points[i].z);
	}

	sort(SortedZ.begin(), SortedZ.end());
	//----------弄呼戈

		//à呼翴(A B C)畒夹(x,y,z)
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

	//盢–à呼(A,B,C)畒夹翴(x,y,z)皚い
	//pcl::PointCloud<pcl::PointXYZRGB>::Ptr Plant_PointCloud(new pcl::PointCloud<pcl::PointXYZRGB>);//块翴冻
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

	//璸衡翴冻秖タ砏て
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


	//璸衡キА猭秖
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
		//
	vector<Parameter> Plant_AllParameters;
	//------------从ネ蔼------------//
	string GrowthHeight = "pGH";
	float Height;
	Height = SortedZ[SortedZ.size() - 1] - SortedZ[0];

	//------------从把计_щ甮瞏------------//

	int size_10 = floor(SortedZ.size() * 0.1);//10%计秖

	vector<float> minZ_10;//程10%Z
	vector<float> maxZ_10;//程10%Z
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

	//----------从把计_呼溃キ縩----------//
	vector<float> MeshArea;	//呼縩
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

		//tla羆腑縩
		float area = sqrt(pow(cross.x(), 2) + pow(cross.y(), 2) + pow(cross.z(), 2)) / 2;
		MeshArea.push_back(area);
		LeafArea += area;


		if (sqrt(pow(cross.x(), 2) + pow(cross.y(), 2) + pow(cross.z(), 2)) == 0) {
			continue;
		}
		float cos = abs(cross.z() / sqrt(pow(cross.x(), 2) + pow(cross.y(), 2) + pow(cross.z(), 2)));
		ProjectedArea += area * cos;

	}


	//--------------------------从把计_从把计_腑キАà----------//
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

	//--------------------------从把计_縩

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

	//--------------------------
	pcl::PolygonMesh Convex_mesh;
	pcl::ConvexHull<pcl::PointXYZRGB> chull;

	chull.setInputCloud(ProjectedPointCloud);
	chull.setComputeAreaVolume(true);

	float CHA = ProjectedPointCloud->size() * 1.0f;


	//--------------------------从把计_щ紇縩

	//--------------------------从把计_从候龟ProjectedArea//ㄈキ
	float sc = ProjectedArea / CHA;

	//--------------------------从把计_砰縩把计
	float fDB = Height * ProjectedArea;
	//--------------------------从把计_从蛾畖
	float fCD = sqrt(ProjectedArea / M_PI) * 2;


	//--------------------------从把计_从借み


	Eigen::Vector4f centroid;
	pcl::compute3DCentroid(*SRC_RGB_cloud, centroid);
	Eigen::Vector3f CD = centroid.head<3>();
	Eigen::Vector3f PlantCentroid = CD;

	//--------------------------从把计_程糴
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

	//--------------------------从把计_熬み瞯
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


	return Height, lpd, LeafArea, laa, CHA, ProjectedArea, sc, fDB, fCD, PlantCentroid, mw, avg_e;

}

