#ifndef QTOBJECT_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define QTOBJECT_H
#pragma once
#pragma execution_character_set("utf-8")


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


#endif