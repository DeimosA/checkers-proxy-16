
#include <opencv2/opencv.hpp>

#include "ip_part.hpp"

using namespace cv;
std::string windowName = "Checkers Scrutator";
int waitTime = 0;



/* Read an image file or throw errof if no data */
Mat readImage(std::string filename) {
	Mat image = imread(filename, CV_LOAD_IMAGE_UNCHANGED);
	if (!image.data) {
		throw std::runtime_error("No image data for: " + filename);
	}
	return image;
}


/* Process an image */
Board processImage(Mat image) {
	// Create window to show images
	namedWindow(windowName, WINDOW_AUTOSIZE);
	imshow(windowName, image);
	waitKey(waitTime);

	// Preprocess image
	cvtColor(image, image, CV_BGR2GRAY);
	//GaussianBlur(image, image, Size(0, 0), 0.9);
	//imshow("blurred image", image);
	Mat filteredImage;
	bilateralFilter(image, filteredImage, 7, 15.0, 15.0);
	image = filteredImage;
	imshow(windowName, image);
	waitKey(waitTime);

	// Load templates to detect (order matters)
	std::vector<Mat> templates;
	templates.push_back(readImage("../images/templates/34circle.png"));
	templates.push_back(readImage("../images/templates/a.png"));
	templates.push_back(readImage("../images/templates/hex.png"));
	templates.push_back(readImage("../images/templates/pogram.png"));
	templates.push_back(readImage("../images/templates/star.png"));
	templates.push_back(readImage("../images/templates/triangle.png"));

	// Board with grid of detected pieces (c, r)
	Board board;

	Mat currentTemplate, markedImage = image.clone();
	cvtColor(markedImage, markedImage, CV_GRAY2BGR);
	int templateWidth = 92;
	Mat templateImage(100, templates.size() * templateWidth, CV_8UC1); // (y, x)
	// Iterate over template list and try to detect
	for (int i = 0; i < templates.size(); i++) {
		// Prepare template
		cvtColor(templates[i], currentTemplate, CV_BGR2GRAY);
		Canny(currentTemplate, currentTemplate, 50, 100, 3, true);
		// Show cannied templates
		Size templSize = currentTemplate.size();
		currentTemplate.copyTo( Mat(templateImage, Rect(i*templateWidth, 5, templSize.width, templSize.height)) );
		imshow("Template cannies", templateImage);
		waitKey(1);

		// Try to detect template in image and record positions
		Ptr<GeneralizedHoughBallard> ghb = createGeneralizedHoughBallard();
		ghb->setTemplate(currentTemplate);
		ghb->setCannyLowThresh(30);
		ghb->setCannyHighThresh(80);
		ghb->setMinDist(20);
		ghb->setVotesThreshold(75);
		ghb->setDp(4.0);

		std::vector<Vec4f> templPositions;
		ghb->detect(image, templPositions);

		// Iterate over detected positions
		int blue = i * 25 > 255, red = 255 - i * 25; // Marker color variation for each templates
		printf("Detect count: %i \n", templPositions.size());
		for (int j = 0; j < templPositions.size(); j++) {
			// Visualize detection
			Vec4f pos = templPositions[j];
			drawMarker( markedImage, Point(pos[0], pos[1]), Scalar(
				blue > 255 ? 255 : blue,
				0,
				red < 0 ? 0 : red
			) );
			imshow(windowName, markedImage);

			// Calculate board positions
			int r, c;
			c = (int)pos[0] / (image.cols / board.width);
			c = c > board.width ? board.width : c; // May be 1 more than width, maybe (not shure if image is zero indexed)
			r = (int)pos[1] / (image.rows / board.height);
			r = r > board.height ? board.height : r;
			// Check for duplicate detection
			if (board.pieces[c][r] == PieceShape::NONE) {
				board.pieces[c][r] = static_cast<PieceShape>(i + 1);
			}
		}
		waitKey(waitTime);
	}

	printf("\nBoard:\n");
	for (int i = 0; i < board.height; i++) {
		for (int j = 0; j < board.width; j++) {
			printf(" %i", board.pieces[j][i]);
		}
		printf("\n");
	}
	printf("\n");


	// Test stuff
	Mat cannyImage;
	Canny(image, cannyImage, 10, 90, 3, true);
	imshow("Canny & HoughLines", cannyImage);
	waitKey(waitTime);

	std::vector<Vec4i> houghLines;
	HoughLinesP(cannyImage, houghLines, 3, 0.5*CV_PI / 180, 50, 15, 10);
	cvtColor(cannyImage, cannyImage, CV_GRAY2BGR);
	for (int i = 0; i < houghLines.size(); i++)
	{
		Vec4i linea = houghLines[i];
		Point pt1, pt2;
		pt1.x = linea[0];
		pt1.y = linea[1];
		pt2.x = linea[2];
		pt2.y = linea[3];
		line(cannyImage, pt1, pt2, Scalar(0, 0, 255), 1, CV_AA);
	}
	imshow("Canny & HoughLines", cannyImage);

	return board;
	
	/*
	// Split image in checker squares for faster processing
	int squareWidth = image.cols / 8;
	int squareHeigth = image.rows / 5;
	int off = 1;
	Rect squareSize(0, 0, image.cols / 8 - 2, image.rows / 5 - 2);
	std::vector<Mat> squares;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 5; j++) {
			squares.push_back(Mat(image, Rect(i*squareWidth + off, j*squareHeigth + off, squareWidth - off, squareHeigth - off)));
		}
	}

	for (int i = 0; i < squares.size(); i++) {
		ghbHex->detect(squares[i], hexPositions);
	}
	*/

}


// Sample board with each piece-type in arbitrary location
Board createSampleBoard() {
	Board board;
	board.pieces[1][1] = PieceShape::CIRCLE;
	board.pieces[5][1] = PieceShape::A;
	board.pieces[3][0] = PieceShape::HEX;
	board.pieces[7][2] = PieceShape::POGRAM;
	board.pieces[6][2] = PieceShape::STAR;
	board.pieces[1][4] = PieceShape::TRIANGLE;
	return board;
}


/* Start point for image processing part */
Board ip_main() {
	// Select image to process
	std::string fileNames[] = {
		"easy01.png",
		"easy02.png",
		"difficult01.png",
		"difficult02.png"
	};
	std::cout << "\nChoose image:\n"
		<< "[0] " << fileNames[0] << "\n"
		<< "[1] " << fileNames[1] << "\n"
		<< "[2] " << fileNames[2] << "\n"
		<< "[3] " << fileNames[3] << "\n"
		<< "[4] Sample board (No image)\n";
	int fileIndex = 4;
	std::cin >> fileIndex;
	std::cout << "\n";

	// If no valid image is selected, just return sample board
	if (fileIndex > 3 || fileIndex < 0) {
		return createSampleBoard();
	}

	std::string filename = fileNames[fileIndex];
	Mat image = readImage("../images/" + filename);
	Board board = processImage(image);

	waitKey(0); // Wait a while, wait forever
	return board;
}
