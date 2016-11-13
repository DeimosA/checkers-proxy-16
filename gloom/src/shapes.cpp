
#define _USE_MATH_DEFINES
#include <cmath>

#include "shapes.hpp"
//#include "sceneGraph.hpp"
#include "program.hpp"




/* Create hexagonal piece */
VAO_t createHex(colour_t colour) {

	int sideCount = 6;
	int vertexCount = 12 * 3;
	int colourCount = 12 * 4;
	int indexCount = (2 * 4 + sideCount * 2) * 3;

	float* vertices = new float[vertexCount];
	float* colours = new float[colourCount];
	//unsigned int* indices = new unsigned int[indexCount];

	float degreesPerCorner = 2 * M_PI / sideCount;
	float x, z;

	for (int i = 0; i < sideCount; i++) {
		x = cos(degreesPerCorner*i);
		//printf("x is: %f\n", x);
		z = sin(degreesPerCorner*i);
		//printf("z is: %f\n", z);

		// Top hex
		vertices[i + 0] = x;
		vertices[i + 1] = 0.2;
		vertices[i + 2] = z;

		colours[i + 0] = colour.red;
		colours[i + 1] = colour.green;
		colours[i + 2] = colour.blue;
		colours[i + 3] = colour.alpha;

		// Bottom hex
		int offset = i * 3 + vertexCount/2;
		vertices[offset + 0] = x;
		vertices[offset + 1] = -0.2;
		vertices[offset + 2] = z;

		offset = i * 4 + colourCount/2;
		colours[offset + 0] = colour.red;
		colours[offset + 1] = colour.green;
		colours[offset + 2] = colour.blue;
		colours[offset + 3] = colour.alpha;
	}

	unsigned int indices[] = {
		0, 4, 2, // Top
		0, 5, 4,
		4, 3, 2,
		2, 1, 0,
		6, 8, 10, // Bottom
		6, 7, 8,
		8, 9, 10,
		10, 11, 6,
		0, 7, 6, // Side 1
		0, 1, 7,
		1, 8, 7, // Side 2
		1, 2, 8,
		2, 9, 8, // Side 3
		2, 3, 9,
		3, 10, 9, // Side 4
		3, 4, 10,
		4, 11, 10, // Side 5
		4, 5, 11,
		5, 6, 11, // Side 6
		5, 0, 6
	};

	unsigned int vaoID = setupVAO(vertices, vertexCount, indices, indexCount, colours, colourCount);

	delete vertices;
	delete colours;
	//delete indices;

	return VAO_t{vaoID, indexCount};
}


/* Create five pointed star piece */
VAO_t createStar(colour_t colour) {
	return VAO_t{0, 0};
}


/* Create 3/4th circle piece */
VAO_t create34thCircle(colour_t colour) {
	return VAO_t{ 0, 0 };
}


/* Create A-like piece sans middle bar */
VAO_t createA(colour_t colour) {
	return VAO_t{ 0, 0 };
}


/* Create triangle piece */
VAO_t createTriangle(colour_t colour) {
	return VAO_t{ 0, 0 };
}


/* Create parallelogram piece */
VAO_t createPoGram(colour_t colour) {
	return VAO_t{ 0, 0 };
}
