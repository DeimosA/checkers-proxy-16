#ifndef IP_PART_HPP
#define IP_PART_HPP


// Allowed piece types
enum class PieceShape {
	NONE,
	CIRCLE,
	A,
	HEX,
	POGRAM,
	STAR,
	TRIANGLE
};

struct Board {
	// Board size
	static const int width = 8;
	static const int height = 5;

	PieceShape pieces[width][height] = {};
};


Board ip_main();


#endif // !IP_PART_HPP
