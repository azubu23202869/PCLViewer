# PCLViewer
![投影片1](https://user-images.githubusercontent.com/43975713/162055204-814ea64a-cebf-4c04-a4b8-a122c0a033ba.JPG)
環境
```
使用版本:
	PCL:1.11.1
	VTK:8.2.0
	QT:5.15.2
	Open3D:0.13
```
[建置參照](https://blog.csdn.net/qq_21095573/article/details/107505585)

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
使用方式

需先下載[Open3D](https://github.com/isl-org/Open3D/releases/tag/v0.15.1)複製到3Dparty

![螢幕擷取畫面 2022-04-07 034744](https://user-images.githubusercontent.com/43975713/162058793-fdb64566-27a7-43dd-81f0-99f55b61571c.png)

接著把CppPythonCallback.exe複製到"3Dparty\Open3D\examples\python\reconstruction_system\sensors"

Demo
![投影片4](https://user-images.githubusercontent.com/43975713/162055233-6bf9fbc9-6a96-4802-ac7b-c184c271b34f.JPG)
![投影片6](https://user-images.githubusercontent.com/43975713/162055580-78f4b2ba-06a1-4f98-836d-284498dfd394.JPG)
![投影片7](https://user-images.githubusercontent.com/43975713/162055612-2e40894f-010d-483c-9515-aa97a88c8ff7.JPG)
![投影片8](https://user-images.githubusercontent.com/43975713/162055638-488272d2-10e7-4539-ae12-3625f4165aa9.JPG)
![投影片9](https://user-images.githubusercontent.com/43975713/162055698-e7a7d4ca-7d79-4f2f-a789-3c26f66c438b.JPG)
![投影片10](https://user-images.githubusercontent.com/43975713/162055729-df807a4a-b85e-409b-a514-f59637efec01.JPG)
![投影片11](https://user-images.githubusercontent.com/43975713/162055755-693b91e5-7beb-4b15-9fae-0ca811450bd0.JPG)
![投影片12](https://user-images.githubusercontent.com/43975713/162055780-64c5a1a5-1848-4755-a8d7-ea4e179f4626.JPG)
![投影片13](https://user-images.githubusercontent.com/43975713/162055805-bd3368f6-d10f-430f-b759-c0cd20fb36e2.JPG)
![投影片14](https://user-images.githubusercontent.com/43975713/162055845-3584d134-dbb9-4c46-8c24-f94fb6635867.JPG)
![投影片15](https://user-images.githubusercontent.com/43975713/162055869-3232502d-42b0-4c3e-b5b8-a2c0f4f14377.JPG)
![投影片16](https://user-images.githubusercontent.com/43975713/162055896-955f54a7-7e03-41a8-8d70-e435bda03260.JPG)

