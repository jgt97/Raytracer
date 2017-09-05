#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_COLOUR_NONE
#include "catch.hpp"

#include "parser.hpp"
#include "raytrace.hpp"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

TEST_CASE("Test JSON parsing", "[parser]")
{
	QJsonObject camCenter{ { "x", 0 },{ "y", 0 },{ "z", 0 } };
	QJsonObject normal{ { "x", 0 },{ "y", 0 },{ "z", 1 } };
	QJsonArray resolution = { 0.01, 0.01 };
	REQUIRE(resolution[0] == 0.01);
	double x = resolution[0].toDouble();
	QJsonArray size = { 256, 256 };
	QJsonObject camera
	{
		{"center", camCenter},
		{"focus", 10},
		{"normal", normal},
		{"resolution", resolution},
		{"size", size}
	};

	QJsonObject location{ {"x", 5}, {"y", -5}, {"z", 0} };
	QJsonObject light1{ { "intensity", 1 },{ "location", location } };
	QJsonArray lights
	{
		light1
	};

	QJsonObject ob1Center{ { "x", 0 },{ "y", 0 },{ "z", 5 } };
	QJsonObject color{ { "b", 200 },{ "g", 200 },{ "r", 200 } };
	QJsonObject object1{ { "center", ob1Center },{ "color", color }, {"lambert", 1}, {"radius", 1}, {"type", "sphere"} };
	QJsonArray objects
	{
		object1
	};

	QJsonObject scene
	{
		{"camera", camera},
		{"lights", lights},
		{"objects", objects}
	};
	QJsonDocument doc(scene);
	QFile file("testScene1.json");
	file.open(QIODevice::WriteOnly);
	file.write(doc.toJson());
	file.close();

	Parser p("testScene1.json");
	p.parse();
	Scene s = p.getParsedScene();
	REQUIRE(s.camera.focus == 10);
}

TEST_CASE("Test raytace", "[raytrace]")
{
	QJsonObject camCenter{ { "x", 0 },{ "y", 0 },{ "z", 0 } };
	QJsonObject normal{ { "x", 0 },{ "y", 0 },{ "z", 1 } };
	QJsonArray resolution = { 0.01, 0.01 };
	double x = resolution[0].toDouble();
	QJsonArray size = { 256, 256 };
	QJsonObject camera
	{
		{ "center", camCenter },
		{ "focus", 10 },
		{ "normal", normal },
		{ "resolution", resolution },
		{ "size", size }
	};

	QJsonObject location1{ { "x", 5 },{ "y", -5 },{ "z", 0 } };
	QJsonObject light1{ { "intensity", 1 },{ "location", location1 } };
	QJsonArray lights
	{
		light1
	};

	QJsonObject ob1Center{ { "x", 0 },{ "y", 0 },{ "z", 5 } };
	QJsonObject color1{ { "b", 0 },{ "g", 0 },{ "r", 255 } };
	QJsonObject object1{ { "center", ob1Center },{ "color", color1 },{ "lambert", 1 },{ "radius", 1 },{ "type", "sphere" } };

	QJsonObject ob2Center{ { "x", -5 },{ "y", 0 },{ "z", 5 } };
	QJsonObject color2{ { "b", 0 },{ "g", 0 },{ "r", 255 } };
	QJsonObject object2{ { "center", ob2Center },{ "color", color2 },{ "lambert", 1 },{ "radius", 1 },{ "type", "sphere" } };

	QJsonObject ob3Center{ { "x", 0 },{ "y", 0 },{ "z", 0 } };
	QJsonObject color3{ { "b", 255 },{ "g", 0 },{ "r", 255 } };
	QJsonObject normal3{ { "x", 0 },{ "y", 0 },{ "z", -1 } };
	QJsonObject object3{ { "center", ob3Center },{ "color", color3 },{ "lambert", 1 },{ "normal", normal3 },{ "type", "plane" } };

	QJsonArray objects
	{
		object1,
		object2,
		object3

	};

	QJsonObject scene
	{
		{ "camera", camera },
		{ "lights", lights },
		{ "objects", objects }
	};
	QJsonDocument doc(scene);
	QFile file("testScene2.json");
	file.open(QIODevice::WriteOnly);
	file.write(doc.toJson());
	file.close();

	Parser p("testScene2.json");
	p.parse();
	
	Scene test = p.getParsedScene();
	REQUIRE(test.camera.center.x == 0);
	REQUIRE(test.camera.center.y == 0);
	REQUIRE(test.camera.center.z == 0);

	REQUIRE(test.camera.focus == 10);
	REQUIRE(test.camera.normal.dX == 0);
	REQUIRE(test.camera.normal.dY == 0);
	REQUIRE(test.camera.normal.dZ == 1);



	RayTrace rt(p);
	rt.castPrimaryRays(1, 1);

}

TEST_CASE("Test ray", "[ray]")
{
	Ray r;
	Point p(0, 0, 0);
	Point e(1, 1, 1);
	r.setPoints(p, e);
	REQUIRE(r.dX == 1);
	REQUIRE(r.dY == 1);
	REQUIRE(r.dZ == 1);
}
