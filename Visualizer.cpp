#include "Visualizer.h"
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <algorithm>
#include <cstdlib>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

const Visualizer::Color Visualizer::kBlue(255, 0, 0);
const Visualizer::Color Visualizer::kRed(0, 0, 255);
const Visualizer::Color Visualizer::kGreen(0, 255, 0);
const Visualizer::Color Visualizer::kYellow(0, 255, 255);
const Visualizer::Color Visualizer::kPurple(240, 32, 160);
const Visualizer::Color Visualizer::kPathColor(kGreen);
const Visualizer::Color Visualizer::kWhite(255, 255, 255);

const Visualizer::Color Visualizer::kGrey(137, 137, 139);

Visualizer::Visualizer(const Chip& chp, const char* windowName)
	: chip(chp),
	w_(chip.m * 2 * radix),
	h_(chip.n * 2 * radix),
	img_(h_, w_, CV_8UC3, kWhite), window_name(windowName) {
}

void Visualizer::drawGrid(int m, int n) const {
	double stepX = 1.0 * w_ / m;
	double stepY = 1.0 * h_ / n;
	double x = 0, y = 0;
	for (int i = 0; i < m; ++i, x += stepX)
		cv::line(img_, cv::Point(x, 0), cv::Point(x, h_), kGrey);
	for (int i = 0; i < n; ++i, y += stepY)
		cv::line(img_, cv::Point(0, y), cv::Point(w_, y), kGrey);
}

void Visualizer::drawLiquid(int x, int y, int concentration, const Color & color) const {
	drawLiquid(Point(x, y), concentration, color);
}

void Visualizer::drawLiquid(const Point & p, int concentration, const Color & color) const {
	cv::Point center(p2co(p));
	char buff[10];
	itoa(concentration, buff, 10);
	cv::String sConcentration(buff);
	cv::circle(img_, center, radix, color, CV_FILLED);
	double font_scale = 0.8;
	int thickness = 2;
	int font_face = cv::FONT_HERSHEY_DUPLEX;
	int baseline;
	cv::Size text_size = cv::getTextSize(sConcentration, font_face, font_scale, 2, &baseline);
	cv::Point strCenter;
	strCenter.x = center.x - text_size.width / 2;
	strCenter.y = center.y + text_size.height / 2;
	cv::putText(img_, sConcentration, strCenter, font_face, font_scale, kWhite, thickness);
}

void Visualizer::deleteLiquid(const Point & p) const {
	cv::Point center(p2co(p));
	cv::circle(img_, center, radix, kWhite, CV_FILLED);
}

void Visualizer::deleteLiquid(int x, int y) const {
	cv::Point center(p2co(Point(x, y)));
	cv::circle(img_, center, radix, kWhite, CV_FILLED);
}

void Visualizer::drawPath(const Path & path, const Color& color) const {
	for (int i = 1; i < path.size(); ++i) {
		cv::line(img_, p2co(path[i - 1]), p2co(path[i]), color, 2);
	}
}

void Visualizer::deletePath(const Path & path) const {
	for (int i = 1; i < path.size(); ++i) {
		cv::line(img_, p2co(path[i - 1]), p2co(path[i]), kWhite, 2);
	}
}

void Visualizer::refresh() const {
	drawGrid(chip.m, chip.n);
	for (int i = 1; i <= chip.m; ++i) {
		for (int j = 1; j <= chip.n; ++j) {
			if (chip.map[i][j] >= 0) {
				drawLiquid(i, j, chip.map[i][j], kBlue);
			}
		}
	}
}

cv::Point Visualizer::p2co(const Point & point) const {
	double xx = radix + (point.x - 1)*radix * 2;
	double yy = radix + (point.y - 1)*radix * 2;
	return cv::Point(xx, yy);
}

// TODO: delay
void Visualizer::draw() const {
	refresh();
	if (chip.getB) {
		// The color of buffer is purple.
		drawLiquid(1, chip.n, 0, kPurple);
		drawPath(chip.BRoute);
		deleteLiquid(1, chip.n);
		Point dest = chip.BRoute.back();
		drawLiquid(dest.x, dest.y, 0, kBlue);
		deletePath(chip.BRoute);
	}
	if (chip.getS) {
		// The color of sample is yellow.
		drawLiquid(1, 1, 1 << chip.d, kYellow);
		drawPath(chip.SRoute);
		deleteLiquid(1, 1);
		Point dest = chip.SRoute.back();
		drawLiquid(dest.x, dest.y, 1 << chip.d, kBlue);
		deletePath(chip.SRoute);
	}
	// Mix:
	{
		const int delay = 2000;
		Point src1 = chip.route[0].front();
		Point src2 = chip.route[1].front();
		Point dest1 = chip.route[0].back();
		Point dest2 = chip.route[1].back();
		int c1 = chip.map[src1.x][src1.y], c2 = chip.map[src2.x][src2.y];


		deleteLiquid(src1);
		drawLiquid(src1, c1, kRed);
		deleteLiquid(src2);
		drawLiquid(src2, c2, kRed);
		show(delay);

		drawPath(chip.route[0]);
		drawPath(chip.route[1]);
		show(delay);

		deleteLiquid(src1);
		deleteLiquid(src2);
		drawLiquid(dest1, c1, kRed);
		drawLiquid(dest2, c2, kRed);
		show(delay);

		deleteLiquid(dest1);
		deleteLiquid(dest2);

		int c = ((c1 + c2) >> 1);
		drawLiquid(dest1, c, kBlue);
		drawLiquid(dest2, c, kBlue);
		show(delay);

		deleteLiquid(dest1);
		deleteLiquid(dest2);
		drawLiquid(src1, c, kBlue);
		drawLiquid(src2, c, kBlue);
		show(delay);

		deletePath(chip.route[0]);
		deletePath(chip.route[1]);
		refresh();
		show(delay);
	}
	if (chip.discard) {
		Point src = chip.discardRoute.front();
		Point waste = chip.discardRoute.back();
		int c = chip.map[src.x][src.y];
		deleteLiquid(src);
		drawLiquid(src, c, kRed);
		drawPath(chip.discardRoute);
		deleteLiquid(src);
		drawLiquid(waste, c, kRed);
		deletePath(chip.discardRoute);
		deleteLiquid(waste);
	}
}

void Visualizer::show(int delay) const {
	cv::namedWindow(window_name, CV_WINDOW_AUTOSIZE);
	cv::imshow(window_name, img_);
	cv::waitKey(delay);
}

void Visualizer::save(const char* filename) const {
	cv::imwrite(filename, img_);
}
