#ifndef SHAPES_HPP
#define SHAPES_HPP


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


VAO_t createHex(colour_t colour = colour_t{ 0.9f, 0.9f, 0.9f, 1.0f, 0.0f });
VAO_t createStar(colour_t colour = colour_t{ 0.1f, 0.1f, 0.9f, 1.0f, 0.0f });
VAO_t create34thCircle(colour_t colour = colour_t{ 0.9f, 0.0f, 0.0f, 1.0f, 0.0f });
VAO_t createA(colour_t colour = colour_t{ 0.8f, 0.9f, 0.0f, 1.0f, 0.0f });
VAO_t createTriangle(colour_t colour = colour_t{ 0.8f, 0.0f, 0.9f, 1.0f, 0.0f });
VAO_t createPoGram(colour_t colour = colour_t{ 0.0f, 0.9f, 0.0f, 1.0f, 0.0f });
VAO_t createSlab(colour_t colour = colour_t{ 1.0f, 0.0f, 0.0f, 1.0f, 0.0f });


#endif
