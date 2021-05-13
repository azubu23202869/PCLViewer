#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "QTObject.h"

class Camera : public QObject, public QRunnable
{
    Q_OBJECT
public:

    void run ();

    void stop() { camera_running = false; }

signals:

    void framesReady(QImage frameRGB, QImage frameDepth);


private:
    rs2::config cfg;

    rs2::pipeline pipe;

    rs2::frameset frames;

    std::string motion_matrix_path;

    bool camera_running = true;


};
QImage realsenseFrameToQImage(const rs2::frame& f);


#endif // CAMERA_H