#ifndef SHAPES_HPP
#define SHAPES_HPP
#pragma once


typedef struct colour_t {
	float red;
	float green;
	float blue;
	float alpha;
	float variation_max;
};

typedef struct VAO_t {
	unsigned int vaoID;
	int indexCount;
};


VAO_t createHex(colour_t colour);
VAO_t createStar(colour_t colour);
VAO_t create34thCircle(colour_t colour);
VAO_t createA(colour_t colour);
VAO_t createTriangle(colour_t colour);
VAO_t createPoGram(colour_t colour);


#endif
