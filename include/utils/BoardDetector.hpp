/*
 * BoardDetector.hpp
 *
 *  Created on: Dec 13, 2016
 *      Author: nima
 */

#ifndef SRC_BOARDDETECTOR_HPP_
#define SRC_BOARDDETECTOR_HPP_

#include <opencv2/core.hpp>
#include <opencv2/aruco.hpp>

struct ArucoBoard {
    int Width;
    int Height;

    float MarkerLength;
    float MarkerSeparation;

    int DictionaryID;
    bool draw_axes = false;
};

struct CameraIntrinsics {
    cv::Mat camMatrix;
    cv::Mat distCoeffs;
};

class BoardDetector {
public:

    BoardDetector(ArucoBoard board, CameraIntrinsics camera, double n_avg);

    void Detect(cv::InputOutputArray &image);

    void DrawAxis(cv::InputOutputArray &image);

    void DrawDetectedMarkers(cv::InputOutputArray &image);

    void DetectBoardAndDrawAxis(cv::InputOutputArray &image);

    /**
     * @brief
     *  To avoid jitter in the position and orientation we smooth them over the last several frames.
     *  This means that for the first frames the results may be wonky.
     * */
    bool Detected() const { return board_detected_; }

private:
    // coppied from Aruco just to add anti-aliasing
    void drawAxisAntiAliased(
        cv::InputOutputArray _image, cv::InputArray camera_Matrix, cv::InputArray _distCoeffs,
        cv::InputArray _rvec, cv::InputArray _tvec, float length);


public:
    //! Information about the actual board we are using.
    ArucoBoard board;

    // number of averaging points to prevent board frame oscillation
    // zero means no averaging
    double n_avg_steps;

    cv::Vec3d rvec, tvec;


private:

    struct ArucoParams {
        cv::Ptr<cv::aruco::DetectorParameters> DetectorParams;
        cv::Ptr<cv::aruco::Dictionary> Dictionary;
        cv::Ptr<cv::aruco::GridBoard> Gridboard;

        //! Alias of #BoardDetector::_gridboard
        cv::Ptr<cv::aruco::Board> Board;

        //! Valid corners and rejected points found in the #BoardDetector::Detect step
        std::vector<std::vector<cv::Point2f>> DetectedCorners, RejectedCorners;

        //! Ids of the markers find in the #BoardDetector::Detect step
        std::vector<int> DetectedMarkerIds;

        //! Should we try to find markers that were not detected in the cv::aruco::detectMarkers step
        //! with an extra call to cv::aruco::refineDetectedMarkers ?
        bool RefindStrategy = 0;


    };

    ArucoParams aruco_;

    CameraIntrinsics camera_;

    //! Number of frames processed since start.
    unsigned int frame_counter_ = 0;

    //! Do we have a valid pose estimation ?
    bool board_detected_ = false;

};


#endif /* SRC_BOARDDETECTOR_HPP_ */

