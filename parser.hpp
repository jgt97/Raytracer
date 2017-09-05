#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "scene.hpp"

#include <QString>
#include <QFile>
#include <string>
#include <QJsonObject>

class Parser
{
public:
	Parser(std::string inFile);
	void parse();
	void parseCamera();
	void parseLights();
	void parseObjects();
	Scene getParsedScene() const;
private:
	Scene scene;

	QString fileName;
	QFile jsonFile;
	QJsonObject Qscene;
};

#endif