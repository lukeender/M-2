#include <string>
#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>
#include <ceres/ceres.h>
#include <thread>
namespace fs = std::filesystem;
using namespace std;
/*

*/
struct GaussianCostFunction {
    GaussianCostFunction(double x, double y)
        : x_(x), y_(y) {}

    template <typename T>
    bool operator()(const T* const params, T* residual) const {
        T mean = params[2];
        T width = params[3];
        T amplitude = params[1];
        T offset = params[0];
        residual[0] = offset + amplitude*exp(-T(2) * pow((x_ - mean)/width,T(2))) //1D gaussian function
                      - y_;

        return true;
    }

private:
    const double x_;
    const double y_;
};
/*

*/
class ImageProcessor
{
    public:
        static inline cv::Mat darkimage; // the dark (calibration) image is the same
                                         // for every image, so we treat it as static
        static inline bool darkimageinit = false;
        
        ImageProcessor(string filename,string darkfilename) : _filename(filename)
        {if (!darkimageinit) {darkimage = cv::imread(darkfilename,cv::IMREAD_GRAYSCALE);
                              darkimageinit = true;}}
        void run()
        {
            image = cv::imread(_filename,cv::IMREAD_GRAYSCALE) - darkimage;
            cv::GaussianBlur(image,image, cv::Size(0,0),1);
            vector<uchar> y = {};
            findCenterOfIntensity();
            image.row(center.x).copyTo(y);
            uchar minval, maxval;
            calcMinMax(y,minval,maxval);
            double params[4] = {(double)minval, (double)maxval, (double)center.y, 100.0};
            ceres::Problem problem;
            for (int i = 0; i < y.size(); ++i) {
                problem.AddResidualBlock(
                    new ceres::AutoDiffCostFunction<GaussianCostFunction, 1, 4>(
                        new GaussianCostFunction((double)i, (double)y[i])
                    ),
                    nullptr,
                    params
                );
            }

            // Set solver options
            ceres::Solver::Options options;
            options.num_threads = std::thread::hardware_concurrency();
            //options.minimizer_progress_to_stdout = true;

            // Solve the problem
            ceres::Solver::Summary summary;
            ceres::Solve(options, &problem, &summary);
            res = vector<double>(params, params + 4);
        }
        vector<double> getres() {return res;}

    private:
        //finds the min and max values in a vector 
        //and stores the values in "minval" and "maxval"
        void calcMinMax(vector<uchar> vec, uchar& minval, uchar& maxval)
        {
            uchar mi = std::numeric_limits<uchar>::max();
            uchar ma = 0;
            for (uchar val : vec)
            {
                ma = std::max(ma,val);
                mi = std::min(mi,val);
            }
            minval = mi;
            maxval = ma;
        }

        //estimates the location of the center of intensity in image
        //via a linearly weighted sum. Stores result in "center"
        void findCenterOfIntensity()
        {
            double col = 0, row = 0, sum = 0;
            double temp;
            for (int i = 0; i < image.cols; i++)
            {
                for (int j = 0; j < image.rows; j++)
                {
                    temp = image.at<uchar>(j,i);
                    if (temp > 10) // throw out small (<4% of dyanimc range) values
                    {
                        col += temp*i;
                        row += temp*j;
                        sum += temp;
                    }


                }
            }
            center = cv::Point(row / sum, col / sum);
        }
        string _filename; //the filename of the image to load and analyze
        cv::Mat image; //the image data to analyze
        cv::Point center; //the approximate center of the gaussian
        vector<double> res; //the output parameters of the optimization
};





