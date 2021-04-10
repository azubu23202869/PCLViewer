


int main(int argc, char** argv) try
{
    string motion_matrix_path = "D:\\123\\motion_matrix.text";
    FILE* fp;
    fp = fopen(motion_matrix_path.c_str(), "w");
    // judge whether devices is exist or not 
    rs2::context ctx;
    auto list = ctx.query_devices(); // Get a snapshot of currently connected devices
    if (list.size() == 0)
        throw std::runtime_error("No device detected. Is it plugged in?");
    rs2::device dev = list.front();

    //
    rs2::frameset frames;
    //Contruct a pipeline which abstracts the device
    rs2::pipeline pipe;//Create a communication pipeline//https://baike.so.com/doc/1559953-1649001.html pipeline explanation
    //Create a configuration for configuring the pipeline with a non default profile
    rs2::config cfg;//Create a configuration with a non-default configuration to configure the pipeline
    //Add desired streams to configuration
    cfg.enable_stream(RS2_STREAM_COLOR, width, height, RS2_FORMAT_BGR8, fps);//Add the required flow to the configuration
    cfg.enable_stream(RS2_STREAM_DEPTH, width, height, RS2_FORMAT_Z16, fps);
    cfg.enable_stream(RS2_STREAM_ACCEL, RS2_FORMAT_MOTION_XYZ32F);
    cfg.enable_stream(RS2_STREAM_GYRO, RS2_FORMAT_MOTION_XYZ32F);

    // get depth scale 
    // float depth_scale = get_depth_scale(profile.get_device());

    // start stream 
    pipe.start(cfg);//Instruct the pipeline to start the flow using the requested configuration
    int countImge = 0;
    while (waitKey(1) < 0) { // 若有按鍵則結束顯示串流影像

        frames = pipe.wait_for_frames();//Wait for all configured stream generation framework

            // Align to depth 
        rs2::align align_to_depth(RS2_STREAM_DEPTH);
        frames = align_to_depth.process(frames);

        // Get imu data
        if (rs2::motion_frame accel_frame = frames.first_or_default(RS2_STREAM_ACCEL))
        {
            rs2_vector accel_sample = accel_frame.get_motion_data();
            fprintf(fp, "%f ", accel_sample.x);
            fprintf(fp, "%f ", accel_sample.x);
            fprintf(fp, "%f\n", accel_sample.x);

            std::cout << "Accel:" << accel_sample.x << ", " << accel_sample.y << ", " << accel_sample.z << std::endl;

        }

        if (rs2::motion_frame gyro_frame = frames.first_or_default(RS2_STREAM_GYRO))
        {
            rs2_vector gyro_sample = gyro_frame.get_motion_data();
            fprintf(fp, "%f ", gyro_sample.x);
            fprintf(fp, "%f ", gyro_sample.x);
            fprintf(fp, "%f\n", gyro_sample.x);
            fprintf(fp, "\n");
            std::cout << "Gyro:" << gyro_sample.x << ", " << gyro_sample.y << ", " << gyro_sample.z << std::endl;
        }


        //Get each frame
        rs2::frame color_frame = frames.get_color_frame();
        rs2::frame depth_frame = frames.get_depth_frame();



        // Creating OpenCV Matrix from a color image
        Mat color(Size(width, height), CV_8UC3, (void*)color_frame.get_data(), Mat::AUTO_STEP);
        Mat pic_depth(Size(width, height), CV_16U, (void*)depth_frame.get_data(), Mat::AUTO_STEP);

        // Display in a GUI
        namedWindow("Display Image", WINDOW_AUTOSIZE);
        string colorpath = "D:/123/4/" + to_string(countImge) + ".jpg";
        imwrite(colorpath, color);
        imshow("Display Image", color);
        string depthpath = "D:/123/5/" + to_string(countImge) + ".png";
        imwrite(depthpath, pic_depth);
        imshow("Display depth", pic_depth * 15);
        countImge++;

    }
    fclose(fp);
    return 0;
}

// error
catch (const rs2::error& e)
{
    std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
    return EXIT_FAILURE;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}
