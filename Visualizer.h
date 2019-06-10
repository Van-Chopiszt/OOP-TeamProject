#ifndef VISUALIZER_H
#define VISUALIZER_H
#include <algorithm>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "Chip.h"
class Visualizer {
   private:
    const Chip& chip;
    typedef cv::Scalar Color;
    static const Color kBlue /* (0, 0, 255) */;
    static const Color kRed /* (255,0,0) */;
    static const Color kGreen /* (0,255,0) */;
    static const Color kYellow /* (255, 255, 0) */;
    static const Color kPurple /* (160,32,240) */;
    static const Color kPathColor /* kGreen */;
    static const Color kWhite /* (255,255,255) */;
    static const Color kGrey /* (139, 137, 137) */;
	const char* window_name;
   public:
    const int radix = 30;
    int w_, h_;
    mutable cv::Mat img_;
    void drawGrid(int m, int n) const;
    void drawLiquid(int x, int y, int concentration, const Color& color) const;
    void drawLiquid(const Point& p, int concentration,
                    const Color& color) const;
    void deleteLiquid(const Point& p) const;
    void deleteLiquid(int x, int y) const;
    void drawPath(const Path& path, const Color& color = kGreen) const;
    void deletePath(const Path& path) const;
	void refresh() const;
	cv::Point p2co(const Point& point) const;
   public:
    Visualizer(const Chip& chp, const char* window_name);
    void draw() const;
    void show(int delay = 0) const;
    void save(const char* filename) const;
};

#endif