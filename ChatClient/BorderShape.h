#pragma once

#include "Shape.h"

class BorderShape final : public Shape
{
public:
	BorderShape(const COORD& size);
	BorderShape(const BorderShape& other);
	~BorderShape() = default;

	BorderShape& operator=(const BorderShape& other);
private:
	void CreateWcharBorder();
};