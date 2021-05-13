#ifndef QTOBJECT_H    
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
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <map>
#include <functional>
#include <string>
#include <cstdint>
#include <iomanip>
#include <ctime>
#include<Windows.h>
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
#include <QThread> 
#include <QImage>
#include <QLabel>
#include <QProgressBar>
#include <QGridLayout>
#include <QProcess>

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
#include <vtkRenderer.h>
#include <vtkImageViewer.h>
#include "vtkPNGReader.h"
#include <vtkRenderWindow.h>
#include <vtkOutputWindow.h>
#include <QVTKOpenGLWidget.h>
#include <vtkGenericOpenGLRenderWindow.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);


#include <librealsense2/rs.hpp>
#include <Python.h>

#endif