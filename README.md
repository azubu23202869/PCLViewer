# PCLViewer
環境
```
使用版本:
	PCL:1.11.1
	VTK:8.2.0
	QT:5.15.2
	
建置參照:https://blog.csdn.net/qq_21095573/article/details/107505585
```

屬性檔
```
C++ 環境建置 VC++目錄中:
QT_VTK.props:
	include目錄:
		C:\Program Files\PCL 1.11.1\include\pcl-1.11
		C:\Program Files\PCL 1.11.1\3rdParty\Boost\include\boost-1_74
		C:\Program Files\PCL 1.11.1\3rdParty\Eigen\eigen3
		C:\Program Files\PCL 1.11.1\3rdParty\FLANN\include
		C:\Program Files\PCL 1.11.1\3rdParty\Qhull\include
		C:\Program Files\PCL 1.11.1\3rdParty\VTK\include\vtk-8.2
		C:\Program Files\OpenNI2\Include

	程式庫目錄:
		C:\Program Files\PCL 1.11.1\lib
		C:\Program Files\PCL 1.11.1\3rdParty\Qhull\lib
		C:\Program Files\PCL 1.11.1\3rdParty\FLANN\lib
		C:\Program Files\PCL 1.11.1\3rdParty\Boost\lib
		C:\Program Files\PCL 1.11.1\3rdParty\VTK\lib
		C:\Program Files\OpenNI2\Lib
		
OpenCV.props:
	include目錄:
		C:\...\Intel RealSense SDK 2.0\include
		C:\...\Intel RealSense SDK 2.0\bin\x64
		C:\...\OpenCV\build\include
	程式庫目錄:
		C:\...\Intel RealSense SDK 2.0\lib\x64
		C:\...\OpenCV\build\lib\Release
	連結器>輸入:
		opencv_world452.lib
		realsense2.lib


C/C++:
	前置處理器添加:
		_SCL_SECURE_NO_WARNINGS
		_CRT_SECURE_NO_WARNINGS
		BOOST_USE_WINDOWS_H
		NOMINMAX
		_SILENCE_FPOS_SEEKPOS_DEPRECATION_WARNING
		_SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
	所有選項:SDL檢查>否
```
