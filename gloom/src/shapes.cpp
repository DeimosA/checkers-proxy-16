
#define _USE_MATH_DEFINES
#include <cmath>

#include "shapes.hpp"
//#include "sceneGraph.hpp"
#include "program.hpp"




/* Create hexagonal piece */
VAO_t createHex(colour_t colour) {

	int sideCount = 6;
	int vertexSize = 2 * sideCount * 3;
	int colourSize = 2 * sideCount * 4;
	int indexCount = (2 * 4 + 2 * sideCount) * 3;

	float* vertices = new float[vertexSize];
	float* colours = new float[colourSize];

	float degreesPerCorner = 2 * M_PI / sideCount;
	float x, y, z;
	y = 0.2;

	for (int i = 0; i < sideCount; i++) {
		// Calculate coordinates based on angle
		x = cos(degreesPerCorner*i);
		z = sin(degreesPerCorner*i);

		// Top hex
		int offset = 3 * i;
		vertices[offset + 0] = x;
		vertices[offset + 1] = y;
		vertices[offset + 2] = z;

		offset = 4 * i;
		colours[offset + 0] = colour.red;
		colours[offset + 1] = colour.green;
		colours[offset + 2] = colour.blue;
		colours[offset + 3] = colour.alpha;

		// Bottom hex
		offset = 3 * i + vertexSize/2;
		vertices[offset + 0] = x;
		vertices[offset + 1] = -y;
		vertices[offset + 2] = z;

		offset = i * 4 + colourSize/2;
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

	unsigned int vaoID = setupVAO(vertices, vertexSize, indices, indexCount, colours, colourSize);
	delete vertices;
	delete colours;
	return VAO_t{ vaoID, indexCount };
}


/* Create five pointed star piece */
VAO_t createStar(colour_t colour) {

	int sideCount = 2 * 5;
	int vertexSize = 2 * sideCount * 3;
	int colourSize = 2 * sideCount * 4;
	int indexCount = (2 * 8 + 2 * sideCount) * 3;

	float* vertices = new float[vertexSize];
	float* colours = new float[colourSize];

	float degreesPerCorner = 2 * M_PI / sideCount;
	float innerCornerScalar = 0.5;
	float x, y, z;
	y = 0.2;

	for (int i = 0; i < sideCount; i++) {
		// Calculate coordinates based on angle
		x = cos(i*degreesPerCorner + 3*M_PI/2); // Add 270 degrees to "point" towards z-
		z = sin(i*degreesPerCorner + 3*M_PI/2);
		if (i % 2 != 0) { // We have an inner corner
			x *= innerCornerScalar;
			z *= innerCornerScalar;
		}

		// Top
		int offset = 3 * i;
		vertices[offset + 0] = x;
		vertices[offset + 1] = y;
		vertices[offset + 2] = z;

		offset = 4 * i;
		colours[offset + 0] = colour.red;
		colours[offset + 1] = colour.green;
		colours[offset + 2] = colour.blue;
		colours[offset + 3] = colour.alpha;

		// Bottom
		offset = 3 * i + vertexSize / 2;
		vertices[offset + 0] = x;
		vertices[offset + 1] = -y;
		vertices[offset + 2] = z;

		offset = i * 4 + colourSize / 2;
		colours[offset + 0] = colour.red;
		colours[offset + 1] = colour.green;
		colours[offset + 2] = colour.blue;
		colours[offset + 3] = colour.alpha;
	}
	unsigned int indices[] = {
		0, 9, 1, // Top
		2, 1, 3,
		4, 3, 5,
		6, 5, 7,
		8, 7, 9,
		1, 9, 5,
		1, 5, 3,
		9, 7, 5,
		10, 11, 19, // Bottom
		12, 13, 11,
		14, 15, 13,
		16, 17, 15,
		18, 19, 17,
		11, 15, 19,
		11, 13, 15,
		19, 15, 17,
		0, 11, 10, // Side 1
		0, 1, 11,
		1, 12, 11, // Side 2
		1, 2, 12,
		2, 13, 12, // Side 3
		2, 3, 13,
		3, 14, 13, // Side 4
		3, 4, 14,
		4, 15, 14, // Side 5
		4, 5, 15,
		5, 16, 15, // Side 6
		5, 6, 16,
		6, 17, 16, // Side 7
		6, 7, 17,
		7, 18, 17, // Side 8
		7, 8, 18,
		8, 19, 18, // Side 9
		8, 9, 19,
		9, 10, 19, // Side 10
		9, 0, 10
	};

	unsigned int vaoID = setupVAO(vertices, vertexSize, indices, indexCount, colours, colourSize);
	delete vertices;
	delete colours;
	return VAO_t{ vaoID, indexCount };
}


/* Create 3/4th circle piece */
VAO_t create34thCircle(colour_t colour) {

	int sideCount = 15 + 2;
	int vertexSize = 2 * sideCount * 3;
	int colourSize = 2 * sideCount * 4;
	int indexCount = (2 * 15 + 2 * sideCount) * 3;

	float* vertices = new float[vertexSize];
	float* colours = new float[colourSize];

	float degreesPerCorner = (3 * M_PI/2) / (sideCount-2);
	float x, y, z;
	y = 0.2;

	for (int i = 0; i < sideCount-1; i++) {
		// Calculate coordinates based on angle
		x = cos(i*degreesPerCorner + M_PI); // Add 180 degrees to "point" towards z-
		z = sin(i*degreesPerCorner + M_PI);

		// Top
		int offset = 3 * i;
		vertices[offset + 0] = x;
		vertices[offset + 1] = y;
		vertices[offset + 2] = z;

		offset = 4 * i;
		colours[offset + 0] = colour.red;
		colours[offset + 1] = colour.green;
		colours[offset + 2] = colour.blue;
		colours[offset + 3] = colour.alpha;

		// Bottom
		offset = 3 * i + vertexSize / 2;
		vertices[offset + 0] = x;
		vertices[offset + 1] = -y;
		vertices[offset + 2] = z;

		offset = i * 4 + colourSize / 2;
		colours[offset + 0] = colour.red;
		colours[offset + 1] = colour.green;
		colours[offset + 2] = colour.blue;
		colours[offset + 3] = colour.alpha;
	}
	// Center vertices
	// Top
	x = 0;
	z = 0;
	int i = sideCount - 1; // Start where we left off in the loop
	int offset = 3 * i;
	vertices[offset + 0] = x;
	vertices[offset + 1] = y;
	vertices[offset + 2] = z;

	offset = 4 * i;
	colours[offset + 0] = colour.red;
	colours[offset + 1] = colour.green;
	colours[offset + 2] = colour.blue;
	colours[offset + 3] = colour.alpha;

	// Bottom
	offset = 3 * i + vertexSize / 2;
	vertices[offset + 0] = x;
	vertices[offset + 1] = -y;
	vertices[offset + 2] = z;

	offset = i * 4 + colourSize / 2;
	colours[offset + 0] = colour.red;
	colours[offset + 1] = colour.green;
	colours[offset + 2] = colour.blue;
	colours[offset + 3] = colour.alpha;

	unsigned int indices[] = {
		0, 16, 1, // Top
		1, 16, 2,
		2, 16, 3,
		3, 16, 4,
		4, 16, 5,
		5, 16, 6,
		6, 16, 7,
		7, 16, 8,
		8, 16, 9,
		9, 16, 10,
		10, 16, 11,
		11, 16, 12,
		12, 16, 13,
		13, 16, 14,
		14, 16, 15,
		17, 18, 33, // Bottom
		18, 19, 33,
		19, 20, 33,
		20, 21, 33,
		21, 22, 33,
		22, 23, 33,
		23, 24, 33,
		24, 25, 33,
		25, 26, 33,
		26, 27, 33,
		27, 28, 33,
		28, 29, 33,
		29, 30, 33,
		30, 31, 33,
		31, 32, 33,
		0, 18, 17, // Side 1
		0, 1, 18,
		1, 19, 18, // Side 2
		1, 2, 19,
		2, 20, 19, // Side 3
		2, 3, 20,
		3, 21, 20, // Side 4
		3, 4, 21,
		4, 22, 21, // Side 5
		4, 5, 22,
		5, 23, 22, // Side 6
		5, 6, 23,
		6, 24, 23, // Side 7
		6, 7, 24,
		7, 25, 24, // Side 8
		7, 8, 25,
		8, 26, 25, // Side 9
		8, 9, 26,
		9, 27, 26, // Side 10
		9, 10, 27,
		10, 28, 27, // Side 11
		10, 11, 28,
		11, 29, 28, // Side 12
		11, 12, 29,
		12, 30, 29, // Side 13
		12, 13, 30,
		13, 31, 30, // Side 14
		13, 14, 31,
		14, 32, 31, // Side 15
		14, 15, 32,
		15, 33, 32, // Inner side 1
		15, 16, 33,
		16, 17, 33, // Inner side 2
		16, 0, 17
	};

	unsigned int vaoID = setupVAO(vertices, vertexSize, indices, indexCount, colours, colourSize);
	delete vertices;
	delete colours;
	return VAO_t{ vaoID, indexCount };
}


/* Create A-like piece sans middle bar */
VAO_t createA(colour_t colour) {

	int sideCount = 6;
	int vertexSize = 2 * sideCount * 3;
	int colourSize = 2 * sideCount * 4;
	int indexCount = (2 * 4 + 2 * sideCount) * 3;

	float sideLength = 2.0f;
	float outerRadius = sideLength / sqrt(3.0f); //  Radius of the circumscribed circle
	float innerRadius = outerRadius / 2; // Radius of the inscribed circle
	float y = 0.2;
	float insideWidth = sideLength - sideLength / 3;
	float height = outerRadius + innerRadius;
	float insideHeight = insideWidth * sqrt(3.0f) / 2;
	float insideZ = outerRadius - (height - insideHeight);

	float vertices[] = {
		0, y, -outerRadius, // Top
		sideLength/2, y, innerRadius,
		insideWidth/2, y, innerRadius,
		0, y, -insideZ,
		-insideWidth/2, y, innerRadius,
		-sideLength/2, y, innerRadius,

		0, -y, -outerRadius, // Bottom
		sideLength/2, -y, innerRadius,
		insideWidth/2, -y, innerRadius,
		0, -y, -insideZ,
		-insideWidth/2, -y, innerRadius,
		-sideLength/2, -y, innerRadius
	};
	float colours[] = {
		colour.red, colour.green, colour.blue, colour.alpha,
		colour.red, colour.green, colour.blue, colour.alpha,
		colour.red, colour.green, colour.blue, colour.alpha,
		colour.red, colour.green, colour.blue, colour.alpha,
		colour.red, colour.green, colour.blue, colour.alpha,
		colour.red, colour.green, colour.blue, colour.alpha,
		colour.red, colour.green, colour.blue, colour.alpha,
		colour.red, colour.green, colour.blue, colour.alpha,
		colour.red, colour.green, colour.blue, colour.alpha,
		colour.red, colour.green, colour.blue, colour.alpha,
		colour.red, colour.green, colour.blue, colour.alpha,
		colour.red, colour.green, colour.blue, colour.alpha,
	};
	unsigned int indices[] = {
		0, 3, 1, // Top
		3, 2, 1,
		0, 5, 3,
		3, 5, 4,
		6, 7, 9, // Bottom
		9, 7, 8,
		6, 9, 11,
		9, 10, 11,
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
	unsigned int vaoID = setupVAO(vertices, vertexSize, indices, indexCount, colours, colourSize);
	return VAO_t{ vaoID, indexCount };
}


/* Create triangle piece */
VAO_t createTriangle(colour_t colour) {
	int sideCount = 3;
	int vertexSize = 2 * sideCount * 3;
	int colourSize = 2 * sideCount * 4;
	int indexCount = (2 * 1 + 2 * sideCount) * 3;

	float sideLength = 2.0f;
	float outerRadius = sideLength / sqrt(3.0f); //  Radius of the circumscribed circle
	float innerRadius = outerRadius / 2; // Radius of the inscribed circle
	float y = 0.2;
	float height = outerRadius + innerRadius;

	float vertices[] = {
		0, y, -outerRadius, // Top
		sideLength/2, y, innerRadius,
		-sideLength/2, y, innerRadius,

		0, -y, -outerRadius, // Bottom
		sideLength/2, -y, innerRadius,
		-sideLength/2, -y, innerRadius
	};
	float colours[] = {
		colour.red, colour.green, colour.blue, colour.alpha,
		colour.red, colour.green, colour.blue, colour.alpha,
		colour.red, colour.green, colour.blue, colour.alpha,
		colour.red, colour.green, colour.blue, colour.alpha,
		colour.red, colour.green, colour.blue, colour.alpha,
		colour.red, colour.green, colour.blue, colour.alpha,
	};
	unsigned int indices[] = {
		0, 2, 1, // Top
		3, 4, 5, // Bottom
		0, 4, 3, // Side 1
		0, 1, 4,
		1, 5, 4, // Side 2
		1, 2, 5,
		2, 3, 5, // Side 3
		2, 0, 3
	};
	unsigned int vaoID = setupVAO(vertices, vertexSize, indices, indexCount, colours, colourSize);
	return VAO_t{ vaoID, indexCount };
}


/* Create parallelogram piece */
VAO_t createPoGram(colour_t colour) {
	int sideCount = 4;
	int vertexSize = 2 * sideCount * 3;
	int colourSize = 2 * sideCount * 4;
	int indexCount = (2 * 2 + 2 * sideCount) * 3;
	float y = 0.2;
	float z = 0.9;
	float xNear = 0.35;
	float xFar = 1.0;

	float vertices[] = {
		xFar, y, -z, // Top
		xNear, y, z,
		-xFar, y, z,
		-xNear, y, -z,
		xFar, -y, -z, // Bottom
		xNear, -y, z,
		-xFar, -y, z,
		-xNear, -y, -z,
	};
	float colours[] = {
		colour.red, colour.green, colour.blue, colour.alpha,
		colour.red, colour.green, colour.blue, colour.alpha,
		colour.red, colour.green, colour.blue, colour.alpha,
		colour.red, colour.green, colour.blue, colour.alpha,
		colour.red, colour.green, colour.blue, colour.alpha,
		colour.red, colour.green, colour.blue, colour.alpha,
		colour.red, colour.green, colour.blue, colour.alpha,
		colour.red, colour.green, colour.blue, colour.alpha,
	};
	unsigned int indices[] = {
		0, 3, 1, // Top
		3, 2, 1,
		4, 5, 7, // Bottom
		7, 5, 6,
		0, 5, 4, // Side 1
		0, 1, 5,
		1, 6, 5, // Side 2
		1, 2, 6,
		2, 7, 6, // Side 3
		2, 3, 7,
		3, 4, 7, // Side 4
		3, 0, 4,
	};
	unsigned int vaoID = setupVAO(vertices, vertexSize, indices, indexCount, colours, colourSize);
	return VAO_t{ vaoID, indexCount };
}
