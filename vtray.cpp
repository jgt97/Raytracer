#include "parser.hpp"
#include "raytrace.hpp"

#include <iostream>
#include <chrono>

#include <thread>
#include <QImage>
#include <QImageWriter>
#include <QPoint>
#include <QColor>

int main(int argc, char* argv[])
{
	std::string inFile;
	std::string outputFile;
	unsigned int numThreads;
	if (argc == 5) {
		std::string threadString = argv[2];
		numThreads = std::stoi(threadString);
		inFile = argv[3];
		outputFile = argv[4];
	}
	else {
		numThreads = 1;
		inFile = argv[1];
		outputFile = argv[2];
	}
	Parser p(inFile);
	p.parse();

	RayTrace rt(p);

	std::vector<std::thread> threadVec(numThreads);
	for (unsigned int k = 1; k <= numThreads; ++k) {
		threadVec[k - 1] = std::thread(&RayTrace::castPrimaryRays, &rt, numThreads, k);
	}

	for (auto it = threadVec.begin(); it != threadVec.end(); ++it) {
		it->join();
	}
	
	QString qOutFile = QString::fromStdString(outputFile);
	QImageWriter imageIO(qOutFile);
	QImage image(rt.scene.camera.size[0], rt.scene.camera.size[1], QImage::Format_RGBX8888);

	int count = 0;
	while (!rt.colorList.empty()){
		locAndCol curr;
		rt.colorList.try_pop(curr);
		Color col = curr.cl;
		int x = curr.x;
		int y = curr.y;

		double max;
		if (rt.rMax >= rt.gMax && rt.rMax >= rt.bMax) {
			max = rt.rMax;
		}
		else if (rt.gMax >= rt.rMax && rt.gMax >= rt.bMax) {
			max = rt.gMax;
		}
		else {
			max = rt.bMax;
		}

		col.r = (col.r / max) * 255;
		col.g = (col.g / max) * 255;
		col.b = (col.b / max) * 255;

		int r = col.r;
		int g = col.g;
		int b = col.b;

		QColor color(r, g, b);

		image.setPixel(x, y, color.rgb());

		count++;

	}
	imageIO.write(image);
		
	return 0;
}
