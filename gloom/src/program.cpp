
#include <cmath>

// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
#include "sphere.hpp"
#include "sceneGraph.hpp"
#include "shapes.hpp"
#include "ip_part.hpp"

#include "glm/glm.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"


// Keep track of held down keys
struct {
	bool forward = false;
	bool back = false;
	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;
	bool speed = false;
	bool viewRight = false;
	bool viewLeft = false;
	bool viewUp = false;
	bool viewDown = false;
} heldKeys;

// Camera position
struct {
	float x = 0;
	float y = 0;
	float z = 20;
	float dirVert = 0;
	float dirHor = 0;
} camPos;

Board board;
int selectedPiece = 0;
float defaultPieceScale = 0.8;
float selectedPieceHeight = 3.0;
float pieceAniSpeed = 5.0; // Move n per second in world coordinates (square is 2.0 wide)
float aniStopDelta = 0.03; // 0.03 should be ok down to ~33 fps
// Keep an indexable list of pieces
std::vector<SceneNode*> pieces;


/**
  * A function which sets up a Vertex Array Object (VAO) containing triangles
  */
unsigned int setupVAO(float* vertices, int v_size, unsigned int* indices, int i_size, float* colours, int c_size) {
    // Set up VAO
    unsigned int arrayID = 0;
    glGenVertexArrays(1, &arrayID);
    glBindVertexArray(arrayID);

    // Set up VBO, bind to VAO and add vertices
    unsigned int bufferID = 0;
    glGenBuffers(1, &bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*v_size, vertices, GL_STATIC_DRAW);
    /*
    void glVertexAttribPointer(
        unsigned int index,
        int size,
        enum type,
        bool normalised,
        size_t stride,
        void* pointer
    );
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // Add colour VBO
    unsigned int colourBufferID = 0;
    glGenBuffers(1, &colourBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, colourBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*c_size, colours, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    // Set up index buffer
    unsigned int i_bufferID = 0;
    glGenBuffers(1, &i_bufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_bufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*i_size, indices, GL_STATIC_DRAW);

    // Return VAO ID
    return arrayID;
}


/**
  * A function which constructs and returns a scene graph containing a solar system.
  */
SceneNode* setupSceneGraph() {
	unsigned int slices = 20, layers = 10;
	int indiceCount = slices * layers * 2 * 3; // slices * layers * PRIMITIVES_PER_RECTANGLE * VERTICES_PER_TRIANGLE

	// Center node
	SceneNode* table = createSceneNode();
	VAO_t tableModel = createSlab(colour_t{ 0.4f, 0.25f, 0.2f, 1.0f, 0.0f });
	table->vertexArrayObjectID = tableModel.vaoID;
	table->indexCount = tableModel.indexCount;
	table->rotationSpeedRadians = 0;
	table->orbitSpeedRadians = 0;
	table->rotationDirection = glm::vec3(0.0, 1.0, 0.0);
	table->scaleVector = glm::vec3(10.0, 5.0, 10.0);
	table->y = -0.5;

	// Checkerboard and pieces
	for (int col = 0; col < board.width; col++) {
		for (int row = 0; row < board.height; row++) {
			SceneNode* square = createSceneNode();
			colour_t colour;
			if (col % 2 == row % 2) {
				colour = { 0.7f, 0.0f, 0.0f, 1.0f, 0.0f };
			} else {
				colour = { 0.0f, 0.0f, 0.7f, 1.0f, 0.0f };
			}
			VAO_t squareModel = createSlab(colour);
			square->vertexArrayObjectID = squareModel.vaoID;
			square->indexCount = squareModel.indexCount;

			square->y = 0.6;
			square->x = 2 * col - (float)board.width + 1;
			square->z = 2 * row - (float)board.height + 1;

			addChild(table, square);

			// Pieces
			VAO_t pieceModel;
			switch (board.pieces[col][row]) {
			case PieceShape::NONE:
				continue; // Skip rest of loop if no piece
				break;
			case PieceShape::CIRCLE:
				pieceModel = create34thCircle();
				break;
			case PieceShape::A:
				pieceModel = createA();
				break;
			case PieceShape::HEX:
				pieceModel = createHex();
				break;
			case PieceShape::POGRAM:
				pieceModel = createPoGram();
				break;
			case PieceShape::STAR:
				pieceModel = createStar();
				break;
			case PieceShape::TRIANGLE:
				pieceModel = createTriangle();
				break;
			default:
				continue;
				break;
			}
			SceneNode* piece = createSceneNode();
			piece->vertexArrayObjectID = pieceModel.vaoID;
			piece->indexCount = pieceModel.indexCount;
			piece->y = 0.9;
			piece->x = 2 * col - (float)board.width + 1;
			piece->z = 2 * row - (float)board.height + 1;
			piece->scaleVector = glm::vec3(defaultPieceScale);
			piece->modelType = ModelType::PIECESHAPE;
			piece->pieceGridPos = glm::vec2(col, row);

			pieces.push_back(piece);
			addChild(table, piece);
		}
	}
	// Set height so we can see the default selected piece
	pieces[selectedPiece]->scaleVector[1] = selectedPieceHeight;
	


	

	// planet 2
	SceneNode* planet2 = createSceneNode();
	planet2->vertexArrayObjectID = createCircleVAO(slices, layers, 0.1, 0.2, 0.7, 0.1);
	planet2->indexCount = indiceCount;
	planet2->rotationDirection = glm::vec3(0.0, 1.0, 0.0);
	planet2->rotationSpeedRadians = PI / 20;
	planet2->orbitSpeedRadians = PI / 50;
	planet2->scaleVector = glm::vec3(2.8);
	planet2->x = -10;
	planet2->y = 0;
	planet2->z = -20;

	SceneNode* planet2_moon = createSceneNode();
	planet2_moon->vertexArrayObjectID = createCircleVAO(slices, layers, 0.0, 0.0, 0.4, 0.1);
	planet2_moon->indexCount = indiceCount;
	planet2_moon->rotationDirection = glm::vec3(0.0, 1.0, 0.0);
	planet2_moon->rotationSpeedRadians = PI / 10;
	planet2_moon->orbitSpeedRadians = PI / 10;
	planet2_moon->scaleVector = glm::vec3(0.4);
	planet2_moon->x = 4.5;
	planet2_moon->y = 0;
	planet2_moon->z = 0;

	// planet 3
	SceneNode* planet3 = createSceneNode();
	planet3->vertexArrayObjectID = createCircleVAO(slices, layers, 0.8, 0.3, 0.1, 0.1);
	planet3->indexCount = indiceCount;
	planet3->rotationDirection = glm::vec3(0.0, -1.0, 0.0);
	planet3->rotationSpeedRadians = PI / 60;
	planet3->orbitSpeedRadians = PI / 70;
	planet3->scaleVector = glm::vec3(1.0);
	planet3->x = 14;
	planet3->y = 0;
	planet3->z = -19;

	SceneNode* planet3_moon = createSceneNode();
	planet3_moon->vertexArrayObjectID = createCircleVAO(slices, layers, 0.5, 0.1, 0.0, 0.1);
	planet3_moon->indexCount = indiceCount;
	planet3_moon->rotationDirection = glm::vec3(0.0, -1.0, 0.0);
	planet3_moon->rotationSpeedRadians = PI / 30;
	planet3_moon->orbitSpeedRadians = PI / 15;
	planet3_moon->scaleVector = glm::vec3(0.25);
	planet3_moon->x = 0;
	planet3_moon->y = 0;
	planet3_moon->z = 2.5;

	// planet 4
	SceneNode* planet4 = createSceneNode();
	planet4->vertexArrayObjectID = createCircleVAO(slices, layers, 0.1, 0.5, 0.1, 0.1);
	planet4->indexCount = indiceCount;
	planet4->rotationDirection = glm::vec3(0.0, 1.0, 0.0);
	planet4->rotationSpeedRadians = PI / 90;
	planet4->orbitSpeedRadians = PI / 120;
	planet4->scaleVector = glm::vec3(19.0);
	planet4->x = -29;
	planet4->y = 0;
	planet4->z = -55;

	// planet 5
	SceneNode* planet5 = createSceneNode();
	planet5->vertexArrayObjectID = createCircleVAO(slices, layers, 0.2, 0.3, 0.3, 0.1);
	planet5->indexCount = indiceCount;
	planet5->rotationDirection = glm::vec3(0.0, 1.0, 0.0);
	planet5->rotationSpeedRadians = PI / 40;
	planet5->orbitSpeedRadians = PI / 200;
	planet5->scaleVector = glm::vec3(0.55);
	planet5->x = -30;
	planet5->y = 0;
	planet5->z = 30;

	// Add all children
	addChild(table, planet2);
	addChild(table, planet3);
	addChild(table, planet4);
	addChild(table, planet5);

	addChild(planet2, planet2_moon);
	addChild(planet3, planet3_moon);

	// Return the root node
	return table;
}


/**
  * A function which updates the Scene Graph.
  */
void traverseSceneGraph(SceneNode* sceneGraph, double timeDelta) {
	sceneGraph->rotationY += timeDelta * sceneGraph->orbitSpeedRadians;
	sceneGraph->rotationY = fmod(sceneGraph->rotationY, 2*PI); // Check overflow

	// Update piece position based on its grid position
	if (sceneGraph->modelType == ModelType::PIECESHAPE) {
		int col = sceneGraph->pieceGridPos[0];
		int row = sceneGraph->pieceGridPos[1];
		float xOff = sceneGraph->aniOffset[0];
		float zOff = sceneGraph->aniOffset[1];
		sceneGraph->x = 2 * col - (float)board.width + 1 + xOff;
		sceneGraph->z = 2 * row - (float)board.height + 1 + zOff;

		// Update piece animation
		if (sceneGraph->isAnimating) {
			float deltaMovement = timeDelta * pieceAniSpeed;
			if (deltaMovement > aniStopDelta) deltaMovement = aniStopDelta; // Ensure we don't move to far
			if (sceneGraph->aniOffset[0] > aniStopDelta) {
				sceneGraph->aniOffset[0] -= deltaMovement;
			}
			else if (sceneGraph->aniOffset[0] < -aniStopDelta) {
				sceneGraph->aniOffset[0] += deltaMovement;
			}
			else if (sceneGraph->aniOffset[1] > aniStopDelta) {
				sceneGraph->aniOffset[1] -= deltaMovement;
			}
			else if (sceneGraph->aniOffset[1] < -aniStopDelta) {
				sceneGraph->aniOffset[1] += deltaMovement;
			}
			else { // We ar closer than aniStopDelta to zero so stop animation
				sceneGraph->isAnimating = false;
				sceneGraph->aniOffset[0] = 0;
				sceneGraph->aniOffset[1] = 0;
			}
		}
	}

	// scale, translate, rotate
	//glm::mat4 m0 = glm::rotate((float)PI / 2, glm::vec3(1.0, 0.0, 0.0)); // Rotate all bodies 90 degrees to avoid "the eye"
	//glm::mat4 m1 = glm::scale(glm::vec3(sceneGraph->scaleFactor, sceneGraph->scaleFactor, sceneGraph->scaleFactor));
	glm::mat4 m2 = glm::translate(glm::vec3(sceneGraph->x, sceneGraph->y, sceneGraph->z));
	glm::mat4 m3 = glm::rotate(sceneGraph->rotationY, sceneGraph->rotationDirection);
	sceneGraph->currentTransformationMatrix = m3 * m2;

	for (int i = 0; i < sceneGraph->children.size(); i++) {
		traverseSceneGraph(sceneGraph->children[i], timeDelta);
	}
}


void runProgram(GLFWwindow* window, Board checkerboard)
{
	board = checkerboard;

    // Set GLFW callback mechanism(s)
    glfwSetKeyCallback(window, keyboardCallback);

    // Enable depth (Z) buffer (accept "closest" fragment)
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Configure miscellaneous OpenGL settings
    glEnable(GL_CULL_FACE);

    // Set default colour after clearing the colour buffer
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    //glClearColor(1.0f, 0.46f, 0.098f, 1.0f); // Pumpkin orange

    /////////////////////////////////////////////////////////////
    // Set up your scene here (create Vertex Array Objects, etc.)
    /////////////////////////////////////////////////////////////

    //unsigned int vaoID = createCircleVAO(20, 10, 0.9, 0.9, 0.2, 0.1);
    //int sphereIndiceCount = 20 * 10 * 2 * 3;

	SceneNode* sceneGraph = setupSceneGraph();

    // Load shaders
    Gloom::Shader shader;
    shader.attach("../gloom/shaders/simple.vert");
    shader.attach("../gloom/shaders/simple.frag");
    shader.link();
    shader.activate();

	float vertAngleDegree = 55;
	float vertAngleRad = vertAngleDegree * PI / 180;
	glm::mat4 projection = glm::perspective(vertAngleRad, (float)windowWidth/windowHeight, 0.1f, 100.0f);
    int count = 0; // Frame counter
	float timeCount = 0; // Time counter
	getTimeDeltaSeconds(); // Reset before rendering starts
    /////////////////
    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ///////////////////////
        // Draw your scene here
        ///////////////////////

		double timeDelta = getTimeDeltaSeconds();

		// Handle held keys
		double moveSpeed = 5 * timeDelta; // Speed is n per second
		double viewSpeed = 0.6 * timeDelta;
		if (heldKeys.speed) moveSpeed *= 2; // n times speed with speed modifier

		if (heldKeys.forward) camPos.z -= moveSpeed;
		if (heldKeys.back) camPos.z += moveSpeed;
		if (heldKeys.left) camPos.x -= moveSpeed;
		if (heldKeys.right) camPos.x += moveSpeed;
		if (heldKeys.up) camPos.y += moveSpeed;
		if (heldKeys.down) camPos.y -= moveSpeed;
		if (heldKeys.viewUp) camPos.dirVert -= viewSpeed;
		if (heldKeys.viewDown) camPos.dirVert += viewSpeed;
		if (heldKeys.viewLeft) camPos.dirHor -= viewSpeed;
		if (heldKeys.viewRight) camPos.dirHor += viewSpeed;


		/*
        // Bind our VAO and then draw triangles
        glBindVertexArray(vaoID);

		glm::mat4 model0 = glm::rotate((float)PI/2, glm::vec3(1.0, 0.0, 0.0));
        //glm::mat4 model1 = glm::rotate(sin(count)*(float)PI/2*0.33f, glm::vec3(0.0, 1.0, 0.0)); // wobble scene to better visualize
		glm::mat4 model1 = glm::rotate(count, glm::vec3(0.0, 1.0, 0.0)); // Rotate scene to better visualize
		count += timeDelta * PI/30; // PI per second
		glm::mat4 model = model1 * model0;

        glm::mat4 MVP = projection * view * model;
        glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(MVP));
		*/
		count++; // Count frames
		timeCount += timeDelta; // Count to 1 per second
		traverseSceneGraph(sceneGraph, timeDelta);

        glm::mat4 view0 = glm::translate(glm::vec3(-camPos.x, -camPos.y, -camPos.z)); // Move world in oposite direction of camera
        glm::mat4 view1 = glm::rotate(camPos.dirHor, glm::vec3(0.0, 1.0, 0.0)); // Rotate world horizontally around camera
        glm::mat4 view2 = glm::rotate(camPos.dirVert, glm::vec3(1.0, 0.0, 0.0)); // Rotate world vertically around camera
		glm::mat4 view = view2 * view1 * view0;

		// Render sun, planets and moons
		SceneNode* sun = sceneGraph; // Rename for better readability
		glBindVertexArray(sun->vertexArrayObjectID);
		// The following transformations must be done here to avoid affecting the entire system
		glm::mat4 model0 = glm::rotate((float)PI / 2, glm::vec3(1.0, 0.0, 0.0)); // Rotate body 90 degrees to avoid "the eye"
		glm::mat4 model1 = glm::scale(sun->scaleVector); // Scale here to avoid scaling entire system
		glm::mat4 model2 = glm::rotate(timeCount*sun->rotationSpeedRadians, sun->rotationDirection); // Rotate body around itself

		glm::mat4 sunModel = sun->currentTransformationMatrix;
		glm::mat4 model = sunModel * model2 * model1; // Complete model transformation  * model0
		glm::mat4 MVP = projection * view * model;
		glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(MVP));
		glDrawElements(GL_TRIANGLES, sun->indexCount, GL_UNSIGNED_INT, 0);

		for (int i = 0; i < sun->children.size(); i++) { // Planets
			SceneNode* planet = sun->children[i];
			glBindVertexArray(planet->vertexArrayObjectID);
			model0 = glm::rotate((float)PI / 2, glm::vec3(1.0, 0.0, 0.0));
			model1 = glm::scale(planet->scaleVector);
			model2 = glm::rotate(timeCount*planet->rotationSpeedRadians, planet->rotationDirection);

			glm::mat4 planetModel = planet->currentTransformationMatrix;
			model = sunModel * planetModel * model2 * model1;
			MVP = projection * view * model;
			glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(MVP));
			glDrawElements(GL_TRIANGLES, planet->indexCount, GL_UNSIGNED_INT, 0);

			for (int j = 0; j < planet->children.size(); j++) { // Moons
				SceneNode* moon = planet->children[j];
				glBindVertexArray(moon->vertexArrayObjectID);
				model0 = glm::rotate((float)PI / 2, glm::vec3(1.0, 0.0, 0.0));
				model1 = glm::scale(moon->scaleVector);
				model2 = glm::rotate(timeCount*moon->rotationSpeedRadians, moon->rotationDirection);

				glm::mat4 moonModel = moon->currentTransformationMatrix;
				model = sunModel * planetModel * moonModel * model2 * model1 * model0;
				MVP = projection * view * model;
				glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(MVP));
				glDrawElements(GL_TRIANGLES, moon->indexCount, GL_UNSIGNED_INT, 0);
			}

		}
        

        //glDrawElements(GL_TRIANGLES, sphereIndiceCount, GL_UNSIGNED_INT, 0);


        //////////////////////
        // Handle other events
        glfwPollEvents();

        // Flip buffers
        glfwSwapBuffers(window);
    }
	// Calculate and print average frames per second
	float frameRate = count / timeCount;
	printf("Average framerate: %f\n", frameRate);
}


// Visualize selected piece by increasing height of model
void changeSelectedPiece() {
	SceneNode* currentSelPiece = pieces[selectedPiece];
	selectedPiece = ++selectedPiece % pieces.size();
	SceneNode* nextSelPiece = pieces[selectedPiece];

	// Set y scale
	currentSelPiece->scaleVector[1] = defaultPieceScale;
	nextSelPiece->scaleVector[1] = selectedPieceHeight;
}


// Check if 2 pieces collide or if pos outside board
bool checkPieceCollision(glm::vec2 pos) {
	// Check offboard position
	if (pos[0] < 0 || pos[0] > board.width-1 || pos[1] < 0 || pos[1] > board.height-1) {
		return true;
	}
	// Check piece collision
	for (int i = 0; i < pieces.size(); i++) {
		if (pieces[i]->pieceGridPos == pos) {
			return true;
		}
	}
	return false;
}


// Move the selected piece
void movePiece(int dCol, int dRow) {
	SceneNode* selPiece = pieces[selectedPiece];
	if (selPiece->isAnimating) return; // Do not allow movement if animating
	glm::vec2 oldPos = selPiece->pieceGridPos;
	glm::vec2 newPos = glm::vec2(oldPos[0] + dCol, oldPos[1] + dRow);
	// If collision, do not move
	if (checkPieceCollision(newPos)) {
		return;
	} else {
		selPiece->pieceGridPos = newPos;
		selPiece->isAnimating = true;
		selPiece->aniOffset[0] = -2.0f * dCol;
		selPiece->aniOffset[1] = -2.0f * dRow;
	}
}


void keyboardCallback(GLFWwindow* window, int key, int scancode,
                      int action, int mods)
{
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE: // Use escape key for terminating the GLFW window
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
			case GLFW_KEY_TAB: // Switch selected piece
				changeSelectedPiece();
				break;
			// Move selected piece
			case GLFW_KEY_LEFT:
				movePiece(-1, 0);
				break;
			case GLFW_KEY_RIGHT:
				movePiece(1, 0);
				break;
			case GLFW_KEY_UP:
				movePiece(0, -1);
				break;
			case GLFW_KEY_DOWN:
				movePiece(0, 1);
				break;

            case GLFW_KEY_A: // Go left
				heldKeys.left = true;
                break;
            case GLFW_KEY_D: // Go right
				heldKeys.right = true;
                break;
            case GLFW_KEY_W: // Go forward
				heldKeys.forward = true;
                break;
            case GLFW_KEY_S: // Go backward
				heldKeys.back = true;
                break;
            case GLFW_KEY_LEFT_SHIFT: // Go up
				heldKeys.up = true;
                break;
            case GLFW_KEY_SPACE: // Go down
				heldKeys.down = true;
                break;
			case GLFW_KEY_LEFT_CONTROL: // Speed modifier
				heldKeys.speed = true;
				break;
            case GLFW_KEY_L: // Rotate right
				heldKeys.viewRight = true;
                break;
            case GLFW_KEY_J: // Rotate left
				heldKeys.viewLeft = true;
                break;
            case GLFW_KEY_I: // Rotate up
				heldKeys.viewUp = true;
                break;
            case GLFW_KEY_K: // Rotate down
				heldKeys.viewDown = true;
                break;
        }

	}
	else if (action == GLFW_RELEASE) {
		switch (key) {
		case GLFW_KEY_A:
			heldKeys.left = false;
			break;
		case GLFW_KEY_D:
			heldKeys.right = false;
			break;
		case GLFW_KEY_W:
			heldKeys.forward = false;
			break;
		case GLFW_KEY_S:
			heldKeys.back = false;
			break;
		case GLFW_KEY_LEFT_SHIFT:
			heldKeys.up = false;
			break;
		case GLFW_KEY_SPACE:
			heldKeys.down = false;
			break;
		case GLFW_KEY_LEFT_CONTROL:
			heldKeys.speed = false;
			break;
		case GLFW_KEY_L: // Rotate right
			heldKeys.viewRight = false;
			break;
		case GLFW_KEY_J: // Rotate left
			heldKeys.viewLeft = false;
			break;
		case GLFW_KEY_I: // Rotate up
			heldKeys.viewUp = false;
			break;
		case GLFW_KEY_K: // Rotate down
			heldKeys.viewDown = false;
			break;
		}
	}
}
