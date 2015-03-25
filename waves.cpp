/* waves.cpp
*/

#include <iostream>
#include <cstdio>
#include <string>
#include <cmath>
#include <armadillo>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "2d_sin_series.cpp"

using namespace std;
using namespace arma;

void arma_to_cv(mat & arma_mat, cv::Mat & cv_mat)
{
    //check if size do match!!!
    int xsize = arma_mat.n_rows;
    int ysize = arma_mat.n_cols;

    cv::Size cv_size = cv_mat.size();

    if ((cv_size.height != ysize) or (cv_size.width != xsize)) {
        printf("arma_to_cv: size mismatch!\n");
    }

    float greyscale;

    for (int x = 0; x < xsize; ++x) {
        for (int y = 0; y < ysize; ++y) {
            cv::Vec3b& rgb = cv_mat.at<cv::Vec3b>(y, x); //cv May has inverted x <-> y

            greyscale = 255.0 * fabs(arma_mat(x, y));
            
            if (greyscale > 255.0) {
                //printf("grey >255! %f\n", greyscale);
                greyscale = 255;
            }

            double sign;
            if (arma_mat(x, y) > 0) {
                rgb[0] = 0;
                rgb[1] = 0;
                rgb[2] = greyscale;
            } else {
                rgb[0] = greyscale;
                rgb[1] = 0;
                rgb[2] = 0;
            }

        }
    }
}

double profile(double x, double y) //2d func that gives initial profile
{
    return 0;
}


int main(int argc, char** argv)
{
    const char * out_vid_name = "waves.mpg";


    if (argc != 6) {
        printf("usage: [maxorder, x size, y size, fps, speed scale]\n");
        return 1;
    }

    int max_ord = atoi(argv[1]);
    int xsize = atoi(argv[2]);
    int ysize = atoi(argv[3]);
    int fps = atoi(argv[4]);
    double speed_scale = atof(argv[5]);

    if (max_ord >= 100) {
        printf("n = %i? are you nuts?\n", max_ord);
        return 1;
    }

    printf("max order = %i\n", max_ord);
    printf("resolution = %ix%i\n", xsize, ysize);
    printf("fps = %i\n", fps);
    printf("speed scale = %.2f\n", speed_scale);
    printf("**********************\n");

    sin_series_2d wave_exp(max_ord, xsize, ysize, &profile);
    mat ser_sum;

    // opencv video init
    cv::Mat frame(ysize, xsize, CV_8UC3); //inverted x,y for cv::Mat vars

    cv::VideoWriter out_vid(out_vid_name,
                            CV_FOURCC('P','I','M','1'),
                            fps, cv::Size(xsize, ysize),
                            true);

    if (!out_vid.isOpened())
    {
        printf("output vid can't be opened\n");
        return 1;
    }

    // time and speed constants

    // set c speed so that mode (0,0) oscillates once every second
    // (when speed_scale = 1.0)
    double k0 = sqrt( pow(1.0/xsize, 2) +
                      pow(1.0/ysize, 2));

    double c_speed = speed_scale * 2.0 * M_PI / k0;

    // render one second of video
    double time = 0;
    double timestep = 1.0 / double(fps);

    int tot_frames_n = int(fps / speed_scale);
    printf("rendering %i frames...\n", tot_frames_n);


    // normalize so that abs(max) = 1.0;
	double frame_abs_max = fabs(wave_exp.sum().max());
	double frame_abs_min = fabs(wave_exp.sum().min());

    double norm = 1.0 / max(frame_abs_max, frame_abs_min);

    for (int nf = 0; nf < tot_frames_n; nf++) {

        // get a cv::Mat frame
        ser_sum = wave_exp.sum_at(time, c_speed) * norm;

        //printf("frame max %f\n", ser_sum.max());
        arma_to_cv(ser_sum, frame);

        out_vid.write(frame);

        time += timestep;
        printf("frame %i,\ttime %.3f, \t prog %.2f%%\r",
               nf, time, 100.0 * float(nf)/float(tot_frames_n));
        fflush(stdout);
    }

    printf ("video written to %s!                         \n\n",
            out_vid_name);

    out_vid.release();
    cv::imwrite( "frame.jpg", frame);

    return 0;
}












