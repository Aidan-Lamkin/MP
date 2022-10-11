#include "MPEngine.hpp"

#include <CSCI441/objects.hpp>
#include <iostream>

//*************************************************************************************
//
// Helper Functions

#ifndef M_PI
#define M_PI 3.14159265
#endif

/// \desc Simple helper function to return a random number between 0.0f and 1.0f.
GLfloat getRand() {
    return (GLfloat)rand() / (GLfloat)RAND_MAX;
}

//*************************************************************************************
//
// Public Interface

MPEngine::MPEngine()
         : CSCI441::OpenGLEngine(4, 1,
                                 640, 480,
                                 "A3: The Cabin in the Woods") {

    for(auto& _key : _keys) _key = GL_FALSE;

    _mousePosition = glm::vec2(MOUSE_UNINITIALIZED, MOUSE_UNINITIALIZED );
    _leftMouseButtonState = GLFW_RELEASE;
    _modelChoice = 0;
}

MPEngine::~MPEngine() {
    delete _arcballCam;
    delete _freeCam;
}

void MPEngine::handleKeyEvent(GLint key, GLint action) {
    if(key != GLFW_KEY_UNKNOWN)
        _keys[key] = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));

    if(action == GLFW_PRESS) {
        switch( key ) {
            // quit!
            case GLFW_KEY_Q:
            case GLFW_KEY_ESCAPE:
                setWindowShouldClose();
                break;
            case GLFW_KEY_LEFT_SHIFT:
            case GLFW_KEY_RIGHT_SHIFT:
                _shiftButtonState = GLFW_PRESS;
                break;
            case GLFW_KEY_UP:
                _changeCamera(true);
                break;
            case GLFW_KEY_DOWN:
                _changeCamera(false);
                break;
            case GLFW_KEY_1:
                _modelChoice = 0;
                _arcballCam->setPosition(_motorcycle->getPosition());
                _arcballCam->setLookAtPoint(_motorcycle->getPosition());
                _arcballCam->recomputeOrientation();
                break;
            case GLFW_KEY_2:
                _modelChoice = 1;
                _arcballCam->setPosition(_bobomb->getPosition());
                _arcballCam->setLookAtPoint(_bobomb->getPosition());
                _arcballCam->recomputeOrientation();
                break;
            case GLFW_KEY_3:
                _modelChoice = 2;
                _arcballCam->setPosition(_robot->getPosition()+_robot->cameraOffset());
                _arcballCam->setLookAtPoint(_robot->getPosition()+_robot->cameraOffset());
                _arcballCam->recomputeOrientation();
                break;
            case GLFW_KEY_4:
                firstPersonOn = !firstPersonOn;
                break;
            default: break; // suppress CLion warning
        }
    }
    if(action == GLFW_RELEASE){
        switch(key) {
            case GLFW_KEY_LEFT_SHIFT:
            case GLFW_KEY_RIGHT_SHIFT:
                _shiftButtonState = GLFW_RELEASE;
                break;
            default:
                break;
        }
    }
}

void MPEngine::handleMouseButtonEvent(GLint button, GLint action) {
    // if the event is for the left mouse button
    if( button == GLFW_MOUSE_BUTTON_LEFT ) {
        // update the left mouse button's state
        _leftMouseButtonState = action;
    }
}

void MPEngine::handleCursorPositionEvent(glm::vec2 currMousePosition) {
    // if mouse hasn't moved in the window, prevent camera from flipping out
    if(_mousePosition.x == MOUSE_UNINITIALIZED) {
        _mousePosition = currMousePosition;
    }

    // if the left mouse button is being held down while the mouse is moving
    if(_leftMouseButtonState == GLFW_PRESS) {
        if (_shiftButtonState != GLFW_PRESS) {
            // rotate the camera by the distance the mouse moved
            switch (_cameraIndex) {
                case (0):
                    _arcballCam->rotate((currMousePosition.x - _mousePosition.x) * 0.005f,
                                        (_mousePosition.y - currMousePosition.y) * 0.005f);
                    break;
                case (1):
                    _freeCam->rotate((currMousePosition.x - _mousePosition.x) * 0.005f,
                                     (_mousePosition.y - currMousePosition.y) * 0.005f);
                    break;
            }
        } else {
            _arcballCam->zoom((_mousePosition.y - currMousePosition.y) * 0.005f);
        }
    }

    // update the mouse position
    _mousePosition = currMousePosition;
}

//*************************************************************************************
//
// Engine Setup

void MPEngine::_setupGLFW() {
    CSCI441::OpenGLEngine::_setupGLFW();

    // set our callbacks
    glfwSetKeyCallback(_window, a3_engine_keyboard_callback);
    glfwSetMouseButtonCallback(_window, a3_engine_mouse_button_callback);
    glfwSetCursorPosCallback(_window, a3_engine_cursor_callback);
}

void MPEngine::_setupOpenGL() {
    glEnable( GL_DEPTH_TEST );					                        // enable depth testing
    glDepthFunc( GL_LESS );							                // use less than depth test

    glEnable(GL_BLEND);									            // enable blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	    // use one minus blending equation

    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );	        // clear the frame buffer to black
}

void MPEngine::_setupShaders() {
    _lightingShaderProgram = new CSCI441::ShaderProgram("shaders/lab05.v.glsl", "shaders/lab05.f.glsl" );
    _lightingShaderUniformLocations.mvpMatrix = _lightingShaderProgram->getUniformLocation("mvpMatrix");
    _lightingShaderUniformLocations.materialColor = _lightingShaderProgram->getUniformLocation("materialColor");
    _lightingShaderUniformLocations.lightColor = _lightingShaderProgram->getUniformLocation("lightColor");
    _lightingShaderUniformLocations.lightDirection = _lightingShaderProgram->getUniformLocation("lightDirection");
    _lightingShaderUniformLocations.normalMatrix = _lightingShaderProgram->getUniformLocation("normalMatrix");
    _lightingShaderAttributeLocations.vPos = _lightingShaderProgram->getAttributeLocation("vPos");
    _lightingShaderAttributeLocations.vNormal = _lightingShaderProgram->getAttributeLocation("vNormal");
}

void MPEngine::_setupBuffers() {
    CSCI441::ModelLoader::enableAutoGenerateNormals();
    CSCI441::setVertexAttributeLocations( _lightingShaderAttributeLocations.vPos, _lightingShaderAttributeLocations.vNormal);

    //create motorcycle
    _motorcycle = new Motorcycle(_lightingShaderProgram->getShaderProgramHandle(),
                       _lightingShaderUniformLocations.mvpMatrix,
                       _lightingShaderUniformLocations.normalMatrix,
                       _lightingShaderUniformLocations.materialColor);

    _bobomb = new Bobomb(_lightingShaderProgram->getShaderProgramHandle(),
                         _lightingShaderUniformLocations.mvpMatrix,
                         _lightingShaderUniformLocations.normalMatrix,
                         _lightingShaderUniformLocations.materialColor);
    _robot = new Robot(_lightingShaderProgram->getShaderProgramHandle(),
                         _lightingShaderUniformLocations.mvpMatrix,
                         _lightingShaderUniformLocations.normalMatrix,
                         _lightingShaderUniformLocations.materialColor);
    // initialize bobomb Position
    _bobomb->setPosition(glm::vec3(2.0f,0.0f,0.0f));
    _robot->setPosition(glm::vec3(4.0f,0.0f,0.0f));
    _createGroundBuffers();
    _generateEnvironment();
}

void MPEngine::_createGroundBuffers() {
    struct Vertex {
        GLfloat x, y, z;
        GLfloat normalX, normalY, normalZ;

    };

    Vertex groundQuad[4] = {
            {-1.0f, 0.0f, -1.0f, 0,1,0},
            { 1.0f, 0.0f, -1.0f,0,1,0},
            {-1.0f, 0.0f,  1.0f,0,1,0},
            { 1.0f, 0.0f,  1.0f,0,1,0}
    };

    GLushort indices[4] = {0,1,2,3};

    _numGroundPoints = 4;

    glGenVertexArrays(1, &_groundVAO);
    glBindVertexArray(_groundVAO);

    GLuint vbods[2];       // 0 - VBO, 1 - IBO
    glGenBuffers(2, vbods);
    glBindBuffer(GL_ARRAY_BUFFER, vbods[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundQuad), groundQuad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(_lightingShaderAttributeLocations.vPos);
    glVertexAttribPointer(_lightingShaderAttributeLocations.vPos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(_lightingShaderAttributeLocations.vNormal);
    glVertexAttribPointer(_lightingShaderAttributeLocations.vNormal, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbods[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void MPEngine::_generateEnvironment() {
    //******************************************************************
    // parameters to make up our grid size and spacing, feel free to
    // play around with this
    const GLfloat GRID_WIDTH = WORLD_SIZE * 1.8f;
    const GLfloat GRID_LENGTH = WORLD_SIZE * 1.8f;
    const GLfloat GRID_SPACING_WIDTH = 1.0f;
    const GLfloat GRID_SPACING_LENGTH = 1.0f;
    // precomputed parameters based on above
    const GLfloat LEFT_END_POINT = -GRID_WIDTH / 2.0f - 5.0f;
    const GLfloat RIGHT_END_POINT = GRID_WIDTH / 2.0f + 5.0f;
    const GLfloat BOTTOM_END_POINT = -GRID_LENGTH / 2.0f - 5.0f;
    const GLfloat TOP_END_POINT = GRID_LENGTH / 2.0f + 5.0f;
    //******************************************************************

    srand( time(0) );                                                   // seed our RNG

    // psych! everything's on a grid.
    for(int i = LEFT_END_POINT; i < RIGHT_END_POINT; i += GRID_SPACING_WIDTH) {
        for(int j = BOTTOM_END_POINT; j < TOP_END_POINT; j += GRID_SPACING_LENGTH) {
            // don't just draw a building ANYWHERE.
            if( i % 6 && j % 6 && getRand() < 0.05f ) {
                // translate to spot
                glm::mat4 transToSpotMtx = glm::translate( glm::mat4(1.0), glm::vec3(i, 0.0f, j) );

                if(getRand() > 0.5f) {
                    // compute random height
                    GLdouble height = powf(getRand(), 2.5) * 10 + 1;
                    // scale to building size
                    glm::mat4 scaleToHeightMtx = glm::scale(glm::mat4(1.0), glm::vec3(1, height, 1));

                    // translate up to grid
                    glm::mat4 transToHeight = glm::translate(glm::mat4(1.0), glm::vec3(0, height / 2.0f, 0));

                    // compute full model matrix
                    glm::mat4 modelMatrix = transToHeight * scaleToHeightMtx * transToSpotMtx;

                    // compute random color
                    glm::vec3 color(.3, .3, .3);
                    // store building properties
                    BuildingData currentBuilding = {modelMatrix, color};
                    _buildings.emplace_back(currentBuilding);
                }
                else{
                    glm::mat4 modelMatrix = transToSpotMtx;
                    GLdouble height = powf(getRand(), 2.5) * 5 + 1;
                    TreeData currentTree = {modelMatrix, glm::vec3(0,height,0), glm::vec3(.6, .3, 0), glm::vec3(0,1,0)};
                    _trees.emplace_back(currentTree);
                }
            }
        }
    }
}

void MPEngine::_setupScene() {
    //initialize arcballcam
    _freeCam = new CSCI441::FreeCam();
    _freeCam->setPosition( glm::vec3(60.0f, 40.0f, 30.0f) );
    _freeCam->setTheta( -M_PI / 3.0f );
    _freeCam->setPhi( M_PI / 2.8f );
    _freeCam->recomputeOrientation();

    _arcballCam = new CSCI441::ArcballCam();
    _arcballCam->setPosition( glm::vec3(0.0f, .4f, 0.0f) );
    _arcballCam->setLookAtPoint(glm::vec3(0,.1f,0.0f));
    _arcballCam->setTheta( 0.0f);
    _arcballCam->setPhi( M_PI / 2.8f + .45 );
    _arcballCam->recomputeOrientation();

    _firstPersonCam = new CSCI441::FreeCam();
    _firstPersonCam->setPhi(M_PI / 2.8f + .80 );
    _firstPersonCam->setPosition(_motorcycle->getPosition() + _motorcycle->getCameraOffset());
    _firstPersonCam->setTheta(0);
    _firstPersonCam->recomputeOrientation();

    glm::vec3 lightColor = glm::vec3(1,1,1);
    glm::vec3 lightDirection = glm::vec3(-1,-1,-1);
    glProgramUniform3fv(_lightingShaderProgram->getShaderProgramHandle(),_lightingShaderUniformLocations.lightColor,1,&lightColor[0]);
    glProgramUniform3fv(_lightingShaderProgram->getShaderProgramHandle(),_lightingShaderUniformLocations.lightDirection,1,&lightDirection[0]);


}

//*************************************************************************************
//
// Engine Cleanup

void MPEngine::_cleanupShaders() {
    fprintf( stdout, "[INFO]: ...deleting Shaders.\n" );
    delete _lightingShaderProgram;
}

void MPEngine::_cleanupBuffers() {
    fprintf( stdout, "[INFO]: ...deleting VAOs....\n" );
    CSCI441::deleteObjectVAOs();
    glDeleteVertexArrays( 1, &_groundVAO );

    fprintf( stdout, "[INFO]: ...deleting VBOs....\n" );
    CSCI441::deleteObjectVBOs();

    fprintf( stdout, "[INFO]: ...deleting models..\n" );
    delete _motorcycle;
}

//*************************************************************************************
//
// Rendering / Drawing Functions - this is where the magic happens!

void MPEngine::_renderScene(glm::mat4 viewMtx, glm::mat4 projMtx) const {
    // use our lighting shader program
    _lightingShaderProgram->useProgram();

    //// BEGIN DRAWING THE GROUND PLANE ////
    // draw the ground plane
    glm::mat4 groundModelMtx = glm::scale( glm::mat4(1.0f), glm::vec3(WORLD_SIZE, 1.0f, WORLD_SIZE));
    _computeAndSendMatrixUniforms(groundModelMtx, viewMtx, projMtx);

    glm::vec3 groundColor(0.3f, 0.8f, 0.2f);
    glUniform3fv(_lightingShaderUniformLocations.materialColor, 1, &groundColor[0]);

    glBindVertexArray(_groundVAO);
    glDrawElements(GL_TRIANGLE_STRIP, _numGroundPoints, GL_UNSIGNED_SHORT, (void*)0);
    //// END DRAWING THE GROUND PLANE ////

    //// BEGIN DRAWING THE BUILDINGS ////
    for( const BuildingData& currentBuilding : _buildings ) {
        _computeAndSendMatrixUniforms(currentBuilding.modelMatrix, viewMtx, projMtx);

        glUniform3fv(_lightingShaderUniformLocations.materialColor, 1, &currentBuilding.color[0]);

        CSCI441::drawSolidCube(1.0);
    }

    for( TreeData currentTree : _trees){
        _computeAndSendMatrixUniforms(currentTree.modelMatrix, viewMtx, projMtx);

        glUniform3fv(_lightingShaderUniformLocations.materialColor, 1, &currentTree.treeColor[0]);
        CSCI441::drawSolidCylinder(0.5f, 0.5f, currentTree.leafTranslate.y, 2, 4);

        currentTree.modelMatrix = glm::translate(currentTree.modelMatrix, currentTree.leafTranslate);
        _computeAndSendMatrixUniforms(currentTree.modelMatrix, viewMtx, projMtx);

        glUniform3fv(_lightingShaderUniformLocations.materialColor, 1, &currentTree.leafColor[0]);
        CSCI441::drawSolidCone(.75f,2,2,4);


    }
    //// END DRAWING THE BUILDINGS AND TREES////
    //// BEGIN DRAWING THE MOTORCYCLE ////
    glm::mat4 modelMtx(1.0f);
    glm::mat4 motorcycleModelMtx(1.0f);

    motorcycleModelMtx = glm::translate(motorcycleModelMtx, _motorcycle->getPosition());
    _motorcycle->drawMotorcycle(motorcycleModelMtx, viewMtx, projMtx);
    //// END DRAWING THE MOTORCYCLE ////
    glm::mat4 modelBobombMtx(1.0f);
    //// BEGIN DRAWING THE HERO ////
    // model translated relative to stored hero position coordinates.
    modelBobombMtx = glm::translate(modelBobombMtx, glm::vec3(_bobomb->getPosition().x, _bobomb->getPosition().y,
                                                  _bobomb->getPosition().z));
    // model rotated relative to stored direction angle
    modelBobombMtx = glm::rotate(modelBobombMtx, _bobomb->getDirection(), CSCI441::Y_AXIS);
    _bobomb->drawBobomb(modelBobombMtx, viewMtx, projMtx);

    //// END DRAWING THE HERO ////

    //// BEGIN DRAWING THE ROBOT ////
    glm::mat4 robotModelMtx(1.0f);
    robotModelMtx = glm::translate(robotModelMtx, _robot->getPosition());
    _robot->drawRobot(robotModelMtx, viewMtx, projMtx);
    //// END DRAWING THE ROBOT ////

}

void MPEngine::_updateScene() {
    _bobomb->_updateFlicker();
    _robot->idleMotion();
    // turn right
    if(_keys[GLFW_KEY_SPACE]){
        switch(_cameraIndex){
            case(0):
                break;
            case(1):
                if( _keys[GLFW_KEY_LEFT_SHIFT] || _keys[GLFW_KEY_RIGHT_SHIFT] ) {
                    _freeCam->moveBackward(.25f);
                }
                // go forward
                else {
                    _freeCam->moveForward(.25f);
                }
                break;
        }
    }
    if( _keys[GLFW_KEY_D] ) {
        switch(_cameraIndex) {
            case(0):
                if(_modelChoice == 0){
                    _motorcycle->rotate(1.0f);
                    if(firstPersonOn){
                        _firstPersonCam->setTheta(-_motorcycle->getAngle() + (5 * M_PI_2 + M_PI_4 - .2));
                        _firstPersonCam->recomputeOrientation();
                    }
                }
                else if(_modelChoice == 1){
                    _bobomb->rotateBobomb(GLFW_KEY_D);
                    if(firstPersonOn){
                        _firstPersonCam->setTheta(-_bobomb->getDirection());
                        _firstPersonCam->recomputeOrientation();
                    }
                }
                else if(_modelChoice == 2){
                    _robot->rotate(-0.0523599);
                    if(firstPersonOn){
                        _firstPersonCam->setTheta(-_robot->getAngle());
                        _firstPersonCam->recomputeOrientation();
                    }
                }
                break;
            case(1):
                _freeCam->rotate(.02f, 0.0f);
                break;

        }
    }
    // turn left
    if( _keys[GLFW_KEY_A] ) {
        switch(_cameraIndex) {
            case(0):
                if(_modelChoice == 0){
                    _motorcycle->rotate(-1.0f);
                    if(firstPersonOn){
                        _firstPersonCam->setTheta(-_motorcycle->getAngle() + (5 * M_PI_2 + M_PI_4 - .15) );
                        _firstPersonCam->recomputeOrientation();
                    }
                }
                else if(_modelChoice == 1){
                    _bobomb->rotateBobomb(GLFW_KEY_A);
                    if(firstPersonOn){
                        _firstPersonCam->setTheta(-_bobomb->getDirection());
                        _firstPersonCam->recomputeOrientation();
                    }
                }
                else if(_modelChoice == 2){
                    _robot->rotate(0.0523599);
                    if(firstPersonOn){
                        _firstPersonCam->setTheta(-_robot->getAngle());
                        _firstPersonCam->recomputeOrientation();
                    }
                }

                break;
            case(1):
                _freeCam->rotate(-.02f, 0.0f);
                break;
        }


    }
    // move forward
    if( _keys[GLFW_KEY_W] ) {

        switch(_cameraIndex) {
            case(0):
                if(_modelChoice == 0) {
                    _motorcycle->driveForward();
                    _motorcycle->_checkBounds(WORLD_SIZE);
                    _arcballCam->setLookAtPoint(_motorcycle->getPosition());
                    if(firstPersonOn){
                        _firstPersonCam->setPosition(_motorcycle->getPosition() + _motorcycle->getCameraOffset());
                        _firstPersonCam->recomputeOrientation();
                    }
                }
                else if(_modelChoice == 1) {
                    _bobomb->driveForward(WORLD_SIZE);
                    _arcballCam->setLookAtPoint(_bobomb->getPosition());
                    if(firstPersonOn){
                        _firstPersonCam->setPosition(_bobomb->getPosition());
                        _firstPersonCam->recomputeOrientation();
                    }
                }
                else if(_modelChoice == 2) {
                    _robot->moveForward(WORLD_SIZE);
                    _arcballCam->setLookAtPoint(_robot->getPosition()+_robot->cameraOffset());
                    if(firstPersonOn){
                        _firstPersonCam->setPosition(_robot->getPosition()+_robot->cameraOffset());
                        _firstPersonCam->recomputeOrientation();
                    }
                }

                _arcballCam->recomputeOrientation();
                break;
            case(1):
                _freeCam->rotate(0.0f, 0.02f);
                break;
        }

    }
    // move backward
    if( _keys[GLFW_KEY_S] ) {
        switch(_cameraIndex) {
            case (0):
                if (_modelChoice == 0) {
                    _motorcycle->driveBackward();
                    _motorcycle->_checkBounds(WORLD_SIZE);
                    _arcballCam->setLookAtPoint(_motorcycle->getPosition());
                    if(firstPersonOn){
                        _firstPersonCam->setPosition(_motorcycle->getPosition() + _motorcycle->getCameraOffset());
                        _firstPersonCam->recomputeOrientation();
                    }
                }
                else if(_modelChoice == 1) {
                    _bobomb->driveBackward(WORLD_SIZE);
                    _arcballCam->setLookAtPoint(_bobomb->getPosition());
                    if(firstPersonOn){
                        _firstPersonCam->setPosition(_bobomb->getPosition());
                        _firstPersonCam->recomputeOrientation();
                    }
                }
                else if(_modelChoice == 2) {
                    _robot->moveBackwards(WORLD_SIZE);
                    _arcballCam->setLookAtPoint(_robot->getPosition()+_robot->cameraOffset());
                    if(firstPersonOn){
                        _firstPersonCam->setPosition(_robot->getPosition()+_robot->cameraOffset());
                        _firstPersonCam->recomputeOrientation();
                    }
                }

                _arcballCam->recomputeOrientation();
                break;
            case(1):
                _freeCam->rotate(0.0f, -0.02f);
                break;
        }
    }
}

void MPEngine::run() {
    //  This is our draw loop - all rendering is done here.  We use a loop to keep the window open
    //	until the user decides to close the window and quit the program.  Without a loop, the
    //	window will display once and then the program exits.
    while( !glfwWindowShouldClose(_window) ) {	        // check if the window was instructed to be closed
        glDrawBuffer( GL_BACK );				        // work with our back frame buffer
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	// clear the current color contents and depth buffer in the window

        // Get the size of our framebuffer.  Ideally this should be the same dimensions as our window, but
        // when using a Retina display the actual window can be larger than the requested window.  Therefore,
        // query what the actual size of the window we are rendering to is.
        GLint framebufferWidth, framebufferHeight;
        glfwGetFramebufferSize( _window, &framebufferWidth, &framebufferHeight );

        // update the viewport - tell OpenGL we want to render to the whole window

        // set the projection matrix based on the window size
        // use a perspective projection that ranges
        // with a FOV of 45 degrees, for our current aspect ratio, and Z ranges from [0.001, 1000].
        glm::mat4 projectionMatrix = glm::perspective( 45.0f, (GLfloat) framebufferWidth / (GLfloat) framebufferHeight, 0.001f, 1000.0f );
        glm::mat4 viewMatrix (1.0f);

        glViewport( 0, 0, framebufferWidth, framebufferHeight );


        // set up our look at matrix to position our camera
        switch(_cameraIndex) {
            case(0):
                viewMatrix = _arcballCam->getViewMatrix();
                break;
            case(1):
                viewMatrix = _freeCam->getViewMatrix();
                break;
        }

        // draw everything to the window
        _renderScene(viewMatrix, projectionMatrix);

        glClear( GL_DEPTH_BUFFER_BIT );	// clear the current color contents and depth buffer in the window

        if(firstPersonOn) {
            viewMatrix = _firstPersonCam->getViewMatrix();
            glViewport(framebufferWidth / (double)3 * 2, framebufferHeight / (double)3 * 2, framebufferWidth, framebufferHeight);
            _drawFirstPerson(viewMatrix, projectionMatrix);
        }




        _updateScene();

        glfwSwapBuffers(_window);                       // flush the OpenGL commands and make sure they get rendered!
        glfwPollEvents();				                // check for any events and signal to redraw screen
    }
}

//*************************************************************************************
//
// Private Helper FUnctions

void MPEngine::_computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    // precompute the Model-View-Projection matrix on the CPU
    glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;
    // then send it to the shader on the GPU to apply to every vertex
    _lightingShaderProgram->setProgramUniform(_lightingShaderUniformLocations.mvpMatrix, mvpMtx);

    glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelMtx)));
    _lightingShaderProgram->setProgramUniform(_lightingShaderUniformLocations.normalMatrix, normalMatrix);
}

void MPEngine::_changeCamera(bool up) {
    if(up){
        if(_cameraIndex == 1){
            _cameraIndex = 0;
        }
        else{
            _cameraIndex++;
        }
    }
    else{
        if(_cameraIndex == 0){
            _cameraIndex = 1;
        }
        else{
            _cameraIndex--;
        }
    }
}

void MPEngine::_drawFirstPerson(glm::mat4 viewMtx, glm::mat4 projMtx) {
    _lightingShaderProgram->useProgram();

    //// BEGIN DRAWING THE GROUND PLANE ////
    // draw the ground plane
    glm::mat4 groundModelMtx = glm::scale( glm::mat4(1.0f), glm::vec3(WORLD_SIZE, 1.0f, WORLD_SIZE));
    _computeAndSendMatrixUniforms(groundModelMtx, viewMtx, projMtx);

    glm::vec3 groundColor(0.3f, 0.8f, 0.2f);
    glUniform3fv(_lightingShaderUniformLocations.materialColor, 1, &groundColor[0]);

    glBindVertexArray(_groundVAO);
    glDrawElements(GL_TRIANGLE_STRIP, _numGroundPoints, GL_UNSIGNED_SHORT, (void*)0);
    //// END DRAWING THE GROUND PLANE ////

    //// BEGIN DRAWING THE BUILDINGS ////
    for( const BuildingData& currentBuilding : _buildings ) {
        _computeAndSendMatrixUniforms(currentBuilding.modelMatrix, viewMtx, projMtx);

        glUniform3fv(_lightingShaderUniformLocations.materialColor, 1, &currentBuilding.color[0]);

        CSCI441::drawSolidCube(1.0);
    }

    for( TreeData currentTree : _trees){
        _computeAndSendMatrixUniforms(currentTree.modelMatrix, viewMtx, projMtx);

        glUniform3fv(_lightingShaderUniformLocations.materialColor, 1, &currentTree.treeColor[0]);
        CSCI441::drawSolidCylinder(0.5f, 0.5f, currentTree.leafTranslate.y, 2, 4);

        currentTree.modelMatrix = glm::translate(currentTree.modelMatrix, currentTree.leafTranslate);
        _computeAndSendMatrixUniforms(currentTree.modelMatrix, viewMtx, projMtx);

        glUniform3fv(_lightingShaderUniformLocations.materialColor, 1, &currentTree.leafColor[0]);
        CSCI441::drawSolidCone(.75f,2,2,4);


    }
    //// END DRAWING THE BUILDINGS AND TREES////
    //// BEGIN DRAWING THE MOTORCYCLE ////
    glm::mat4 modelMtx(1.0f);
    glm::mat4 motorcycleModelMtx(1.0f);

    motorcycleModelMtx = glm::translate(motorcycleModelMtx, _motorcycle->getPosition());
    _motorcycle->drawMotorcycle(motorcycleModelMtx, viewMtx, projMtx);
    //// END DRAWING THE MOTORCYCLE ////
    glm::mat4 modelBobombMtx(1.0f);
    //// BEGIN DRAWING THE HERO ////
    // model translated relative to stored hero position coordinates.
    modelBobombMtx = glm::translate(modelBobombMtx, glm::vec3(_bobomb->getPosition().x, _bobomb->getPosition().y,
                                                              _bobomb->getPosition().z));
    // model rotated relative to stored direction angle
    modelBobombMtx = glm::rotate(modelBobombMtx, _bobomb->getDirection(), CSCI441::Y_AXIS);
    _bobomb->drawBobomb(modelBobombMtx, viewMtx, projMtx);
    //// END DRAWING THE HERO ////

    //// BEGIN DRAWING THE ROBOT ////
    glm::mat4 robotModelMtx(1.0f);
    robotModelMtx = glm::translate(robotModelMtx, _robot->getPosition());
    _robot->drawRobot(robotModelMtx, viewMtx, projMtx);
    //// END DRAWING THE ROBOT ////
}

//*************************************************************************************
//
// Callbacks

void a3_engine_keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods ) {
    auto engine = (MPEngine*) glfwGetWindowUserPointer(window);

    // pass the key and action through to the engine
    engine->handleKeyEvent(key, action);
}

void a3_engine_cursor_callback(GLFWwindow *window, double x, double y ) {
    auto engine = (MPEngine*) glfwGetWindowUserPointer(window);

    // pass the cursor position through to the engine
    engine->handleCursorPositionEvent(glm::vec2(x, y));
}

void a3_engine_mouse_button_callback(GLFWwindow *window, int button, int action, int mods ) {
    auto engine = (MPEngine*) glfwGetWindowUserPointer(window);

    // pass the mouse button and action through to the engine
    engine->handleMouseButtonEvent(button, action);
}
