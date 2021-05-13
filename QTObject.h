#ifndef QTOBJECT_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define QTOBJECT_H
#pragma once
#pragma execution_character_set("utf-8")
#pragma warning(disable:4996)

// base
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <experimental/filesystem>
#include <string>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <map>
#include <functional>
#include <string>


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
#include <QWidget>
#include <QMessageBox>
#include <QThread> 
#include <QImage>
#include <QLabel>
#include <QRunnable>
#include <QThreadPool>


// Point Cloud Library
#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/surface/mls.h>
#include <pcl/common/common.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/filters/passthrough.h>
#include <boost/thread/thread.hpp>
#include <pcl/filters/voxel_grid.h>
#include <pcl/search/kdtree.h>
#include <pcl/search/search.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/ModelCoefficients.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/filters/conditional_removal.h>

// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

// Visualization Toolkit (VTK)
#include <vtkAutoInit.h> 
#include <vtkRenderWindow.h>
#include <vtkOutputWindow.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);


// MongoDB
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>


#include <librealsense2/rs.hpp>

#endif