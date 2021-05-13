#include "Camera.h"
#define rgb_width 640
#define rgb_height 480
#define depth_width 640
#define depth_height 480
#define fps 30


namespace fs = std::experimental::filesystem;

void Camera::run ()
{
    std::ofstream file;

    fs::create_directories("./data");

    motion_matrix_path = "data\\motion_matrix.text";

    file.open(motion_matrix_path.c_str(), ios::out | ios::trunc);
    
    rs2::context ctx;
    auto list = ctx.query_devices();
    if (list.size() == 0)
        throw std::runtime_error("No device detected. Is it plugged in?");
    rs2::device dev = list.front();

    cfg.enable_stream(RS2_STREAM_DEPTH, depth_width, depth_height, RS2_FORMAT_Z16, fps);
    cfg.enable_stream(RS2_STREAM_COLOR, rgb_width, rgb_height, RS2_FORMAT_RGB8, fps);
    cfg.enable_stream(RS2_STREAM_ACCEL, RS2_FORMAT_MOTION_XYZ32F);
    cfg.enable_stream(RS2_STREAM_GYRO, RS2_FORMAT_MOTION_XYZ32F);

    pipe.start(cfg);
    
    while (camera_running)
    {

        frames = pipe.wait_for_frames();
        
        rs2::align align_to_depth(RS2_STREAM_DEPTH);
        frames = align_to_depth.process(frames);

        // IMU Data
        if (rs2::motion_frame accel_frame = frames.first_or_default(RS2_STREAM_ACCEL))
        {
            rs2_vector accel_sample = accel_frame.get_motion_data();
            file << accel_sample.x << " ";
            file << accel_sample.x << " ";
            file << accel_sample.x << "\n";

        }

        if (rs2::motion_frame gyro_frame = frames.first_or_default(RS2_STREAM_GYRO))
        {
            rs2_vector gyro_sample = gyro_frame.get_motion_data();

            file << gyro_sample.x << " ";
            file << gyro_sample.x << " ";
            file << gyro_sample.x << "\n";
            file << "\n";
        }
        
        rs2::frame rgb = frames.get_color_frame();
        rs2::frame depth = frames.get_depth_frame();

        auto q_rgb = realsenseFrameToQImage(rgb);
        auto q_depth = realsenseFrameToQImage(depth);

        emit framesReady(q_rgb, q_depth);
    }
}



QImage realsenseFrameToQImage(const rs2::frame& f)
{
    using namespace rs2;

    auto vf = f.as<video_frame>();
    const int w = vf.get_width();
    const int h = vf.get_height();

    if (f.get_profile().format() == RS2_FORMAT_RGB8)
    {
        auto r = QImage((uchar*)f.get_data(), w, h, w * 3, QImage::Format_RGB888);
        return r;
    }
    else if (f.get_profile().format() == RS2_FORMAT_Z16)
    {
        // only if you have Qt > 5.13
        auto r = QImage((uchar*)f.get_data(), w, h, w * 2, QImage::Format_Grayscale16);
        return r;
    }

    throw std::runtime_error("Frame format is not supported yet!");
}