#pragma once

class LogicObject
{
public:
	LogicObject();
	virtual ~LogicObject();

private:
	int id;
	int x, y;		// po�o�enie na mapie
	int dx, dy;		// po�o�enie docelowe (je�li jednostka w ruchu, je�li stoi to dx == x, dy == y)
};

