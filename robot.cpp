//
// Created by Ethan Ko on 10/9/2022.
//

#include "robot.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <CSCI441/objects.hpp>
#include <CSCI441/OpenGLUtils.hpp>
#include <iostream>
#include <cmath>

//constructor
Robot::Robot(GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint normalMtxUniformLocation,GLint materialColorUniformLocation, GLint modelMtxUniformLocation, GLint vPosAttributeLocation, GLint vNormalAttributeLocation) {
    CSCI441::ModelLoader::enableAutoGenerateNormals();
    _shaderProgramHandle = shaderProgramHandle;
    _shaderProgramUniformLocations.mvpMtx = mvpMtxUniformLocation;
    _shaderProgramUniformLocations.modelMtx = modelMtxUniformLocation;
    _shaderProgramUniformLocations.normalMtx = normalMtxUniformLocation;
    _shaderProgramUniformLocations.materialColor = materialColorUniformLocation;
    _shaderProgramAttributeLocations.vPos = vPosAttributeLocation;
    _shaderProgramAttributeLocations.vNormal = vNormalAttributeLocation;
    /*
     * Switch Robot with Cube for fast loading model
     * Switch back for detailed model
     * Switch scaling down below
    */
    _modelBody = new CSCI441::ModelLoader();
    _modelBody->loadModelFile( "models/Robot.obj" );
    _modelBody->setAttributeLocations(_shaderProgramAttributeLocations.vPos, _shaderProgramAttributeLocations.vNormal);





    _modelCube = new CSCI441::ModelLoader();
    _modelCube->loadModelFile( "models/Cube.obj" );
    _modelCube->setAttributeLocations(_shaderProgramAttributeLocations.vPos, _shaderProgramAttributeLocations.vNormal);

    _position = glm::vec3(0.0f,0.0f,0.0f);
    _boxX = 0.29;
    _boxZ = 0.8;
    _idleMotion = 0.0;
    _rotation = 0.0;
    _speed = 0.1;
}


//Draws the whole robot
void Robot::drawRobot(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    glUseProgram(_shaderProgramHandle);
    modelMtx = glm::mat4(1.0f);
    modelMtx = glm::translate(modelMtx, _position);
    modelMtx = glm::translate( modelMtx, glm::vec3(0.4,0.0,0.456) );
    modelMtx = glm::rotate( modelMtx, _rotation, glm::vec3(0.0,1.0,0.0) );
    modelMtx = glm::translate( modelMtx, glm::vec3(-0.4,0.0,-0.456) );
    _drawBody(modelMtx, viewMtx, projMtx);
    //_drawCubeStack(modelMtx, viewMtx, projMtx);
}

void Robot::_drawBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    modelMtx = glm::translate( modelMtx, glm::vec3(0.0,-0.01,0.0) );

    /*
     * Change from 0.03 to 0.001 for models/Robot.obj
    */
     modelMtx = glm::scale( modelMtx, glm::vec3(0.001,0.001,0.001) );
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);

    glm::vec3 modelColor = glm::vec3(1.0,1.0,1.0);
    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &modelColor[0]);

    _modelBody->draw(_shaderProgramHandle);
}

void Robot::_drawCubeStack(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    modelMtx = glm::translate( modelMtx, glm::vec3(_boxX,0.125,_boxZ+_idleMotion) );
    modelMtx = glm::scale( modelMtx, glm::vec3(0.01,0.01,0.01) );
    glm::vec3 modelColor = glm::vec3(0.92,0.85,0.2);
    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &modelColor[0]);

    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);
    _modelCube->draw(_shaderProgramHandle);
}

glm::vec3 Robot::getPosition(){
    return _position;
}

glm::vec3 Robot::cameraOffset(){
    return glm::vec3(0.4,0.2,0.456);
}

void Robot::setPosition(glm::vec3 newPosition){
    _position = newPosition;
}

void Robot::rotate(float radians){
    _rotation+=radians;
}

float Robot::getAngle(){
    return _rotation;
}

void Robot::moveForward(GLfloat worldSize){
//    _boxX+=_speed*sin(_rotation);
//    _boxZ+=_speed*cos(_rotation);
//    std::cout << "x: " << _boxX << "    z: " << _boxZ << std::endl;
    _position.x+=_speed*sin(_rotation);
    _position.z+=_speed*cos(_rotation);
    _checkBounds(worldSize);
}
void Robot::moveBackwards(GLfloat worldSize){
//    _boxX-=_speed*sin(_rotation);
//    _boxZ-=_speed*cos(_rotation);
//    std::cout << "x: " << _boxX << "    z: " << _boxZ << std::endl;
    _position.x-=_speed*sin(_rotation);
    _position.z-=_speed*cos(_rotation);
    _checkBounds(worldSize);
}

//makes sure motorcycle does not go off world
void Robot::_checkBounds(GLfloat WorldSize) {
    if(_position.x > WorldSize){
        _position.x = WorldSize;
    }
    else if(_position.x < -WorldSize){
        _position.x = -WorldSize;
    }
    if(_position.z > WorldSize){
        _position.z = WorldSize;
    }
    else if(_position.z < -WorldSize){
        _position.z = -WorldSize;
    }
}

void Robot::idleMotion(){
    _idleMotion = 0.02*sin(glfwGetTime());
}

void Robot::_computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const{
    glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;
    glProgramUniformMatrix4fv( _shaderProgramHandle, _shaderProgramUniformLocations.mvpMtx, 1, GL_FALSE, &mvpMtx[0][0] );
    glProgramUniformMatrix4fv( _shaderProgramHandle, _shaderProgramUniformLocations.modelMtx, 1, GL_FALSE, &modelMtx[0][0] );
    glm::mat3 normalMtx = glm::mat3( glm::transpose( glm::inverse( modelMtx )));
    glProgramUniformMatrix3fv( _shaderProgramHandle, _shaderProgramUniformLocations.normalMtx, 1, GL_FALSE, &normalMtx[0][0] );
}

