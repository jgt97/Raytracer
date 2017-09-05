#include "parser.hpp"

#include <QFile>
#include <QString>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <string>
#include <iostream>

Parser::Parser(std::string inFile)
{
	fileName = QString::fromStdString(inFile);
	jsonFile.setFileName(fileName);
	jsonFile.open(QIODevice::ReadOnly);
	QJsonDocument doc = QJsonDocument::fromJson(jsonFile.readAll());
	Qscene = doc.object();
}

void Parser::parse()
{
	parseCamera();
	parseObjects();
	parseLights();
}

void Parser::parseCamera()
{
	QJsonObject Qcamera = Qscene.find("camera")->toObject();
	QJsonObject Qcenter = Qcamera.find("center")->toObject();

	scene.camera.center.x = Qcenter.find("x")->toDouble();
	scene.camera.center.y = Qcenter.find("y")->toDouble();
	scene.camera.center.z = Qcenter.find("z")->toDouble();
	


	scene.camera.focus = Qcamera.find("focus")->toDouble();

	QJsonObject Qnormal = Qcamera.find("normal")->toObject();
	if (Qnormal.length() != 3) {
		std::cerr << "Error: camera normal vector must have 3 components";
		std::exit(EXIT_FAILURE);
	}
	else {
		scene.camera.normal.dX = Qnormal.find("x")->toDouble();
		scene.camera.normal.dY = Qnormal.find("y")->toDouble();
		scene.camera.normal.dZ = Qnormal.find("z")->toDouble();
	}

	QJsonArray Qresolution = Qcamera.find("resolution")->toArray();
	scene.camera.resolution[0] = Qresolution[0].toDouble();
	if (scene.camera.resolution[0] < 0) {
		std::cerr << "Error: x resolution must not be negative";
		std::exit(EXIT_FAILURE);
	}
	scene.camera.resolution[1] = Qresolution[1].toDouble();
	if (scene.camera.resolution[1] < 0) {
		std::cerr << "Error: y resolution must not be negative";
		std::exit(EXIT_FAILURE);
	}

	QJsonArray Qsize = Qcamera.find("size")->toArray();
	scene.camera.size[0] = Qsize[0].toInt();
	if (scene.camera.size[0] < 0) {
		std::cerr << "Error: x size must not be negative";
		std::exit(EXIT_FAILURE);
	}
	scene.camera.size[1] = Qsize[1].toInt();
	if (scene.camera.size[1] < 0) {
		std::cerr << "Error: y size must not be negative";
		std::exit(EXIT_FAILURE);
	}

	object imagePlane;
	imagePlane.center = scene.camera.center;
	imagePlane.normal = scene.camera.normal;
	imagePlane.type = "plane";
	scene.objects.push_back(imagePlane);
}

void Parser::parseLights()
{
	QJsonArray QLightsArray = Qscene.find("lights")->toArray();
	QJsonArray::Iterator it;
	for(it = QLightsArray.begin(); it != QLightsArray.end(); ++it)
	{
		light temp;
		QJsonObject QlightObj = it->toObject();

		temp.intensity = QlightObj.find("intensity")->toDouble();
		if (temp.intensity < 0) {
			std::cerr << "Error: intensity not allowed to be negative";
			std::exit(EXIT_FAILURE);
		}

		QJsonObject QlocationObj = QlightObj.find("location")->toObject();
		temp.location.x = QlocationObj.find("x")->toDouble();
		temp.location.y = QlocationObj.find("y")->toDouble();
		temp.location.z = QlocationObj.find("z")->toDouble();
		scene.lights.push_back(temp);
	}

}

void Parser::parseObjects()
{
	QJsonArray QObjectsArray = Qscene.find("objects")->toArray();
	QJsonArray::Iterator it;
	for (it = QObjectsArray.begin(); it != QObjectsArray.end(); ++it)
	{
		object temp;
		QJsonObject QobjectObj = it->toObject();

		QJsonObject Qcenter = QobjectObj.find("center")->toObject();
		temp.center.x = Qcenter.find("x")->toDouble();
		temp.center.y = Qcenter.find("y")->toDouble();
		temp.center.z = Qcenter.find("z")->toDouble();

		QJsonObject Qcolor = QobjectObj.find("color")->toObject();
		temp.color.b = Qcolor.find("b")->toInt();
		if (temp.color.b < 0 || temp.color.b > 255) {
			std::cerr << "Error: color B channel must be between 0 and 255";
			std::exit(EXIT_FAILURE);
		}
		temp.color.g = Qcolor.find("g")->toInt();
		if (temp.color.g < 0 || temp.color.g > 255) {
			std::cerr << "Error: color G channel must be between 0 and 255";
			std::exit(EXIT_FAILURE);
		}
		temp.color.r = Qcolor.find("r")->toInt();
		if (temp.color.r < 0 || temp.color.r > 255) {
			std::cerr << "Error: color R channel must be between 0 and 255";
			std::exit(EXIT_FAILURE);
		}

		temp.lambert = QobjectObj.find("lambert")->toDouble();
		if (temp.lambert < 0 || temp.lambert > 1) {
			std::cerr << "Error: lambert must be between 0 and 1";
			std::exit(EXIT_FAILURE);
		}
		temp.type = QobjectObj.find("type")->toString().toStdString();
		if (temp.type == "plane") {
			QJsonObject qNormal = QobjectObj.find("normal")->toObject();
			temp.normal.dX = qNormal.find("x")->toDouble();
			temp.normal.dY = qNormal.find("y")->toDouble();
			temp.normal.dZ = qNormal.find("z")->toDouble();
		}
		if (temp.type == "sphere") {
			temp.radius = QobjectObj.find("radius")->toDouble();
			if (temp.radius < 0.0) {
				std::cerr << "Error: negative radius not allowed";
				std::exit(EXIT_FAILURE);
			}
		}

		scene.objects.push_back(temp);
	}
}

Scene Parser::getParsedScene() const
{
	return scene;
}