//
// Created by Aidan Lamkin on 10/4/22.
//

#include "motorcycle.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include <CSCI441/objects.hpp>
#include <CSCI441/OpenGLUtils.hpp>

//constructor
Motorcycle::Motorcycle(GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint normalMtxUniformLocation,GLint materialColorUniformLocation) {

        _wheelAngle = 0.0f;
        _wheelRotationSpeed = M_PI / 16.0f;

        _shaderProgramHandle = shaderProgramHandle;
        _shaderProgramUniformLocations.mvpMtx = mvpMtxUniformLocation;
        _shaderProgramUniformLocations.normalMtx = normalMtxUniformLocation;
        _shaderProgramUniformLocations.materialColor = materialColorUniformLocation;

        _rotateMotorcycleAngle = 3 * M_PI / 2.0f;

        _colorBody = glm::vec3(1.0f,1.0f,1.0f);
        _scaleBody = glm::vec3(5.0f, 0.5f, .5f);
        _transBody = glm::vec3(0.0, .12f, 0.0f);

        _colorWheel = glm::vec3(0.0f, 1.0f, 1.0f);
        _scaleWheel = glm::vec3(1.0f,1.0f,1.0f);
        _transWheel = glm::vec3(0.45f, 0,0);

        _movementSpeed = 0.25f;

        _position = glm::vec3(0,0.1,0);
        _cameraOffset = glm::vec3(0, .5, 0);

}

//moves forward, rotates wheels and checks to bounds of grid
void Motorcycle::driveForward() {
    _wheelAngle -= _wheelRotationSpeed;
    if(_wheelAngle > 2.0f * M_PI){
        _wheelAngle -= 2.0f * M_PI;
    }
    _position.x += cos(-_rotateMotorcycleAngle) * _movementSpeed;
    _position.z += sin(-_rotateMotorcycleAngle) * _movementSpeed;

}

//moves backward, rotates wheels and checks to bounds of grid
void Motorcycle::driveBackward() {
    _wheelAngle -= _wheelRotationSpeed;
    if(_wheelAngle < 0.0f){
        _wheelAngle += 2.0f * M_PI;
    }
    _position.x -= cos(-_rotateMotorcycleAngle) * _movementSpeed;
    _position.z -= sin(-_rotateMotorcycleAngle) * _movementSpeed;
}

//send matrix info to GPU
void Motorcycle::_computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;
    glProgramUniformMatrix4fv( _shaderProgramHandle, _shaderProgramUniformLocations.mvpMtx, 1, GL_FALSE, &mvpMtx[0][0] );
    glm::mat3 normalMtx = glm::mat3( glm::transpose( glm::inverse( modelMtx )));
    glProgramUniformMatrix3fv( _shaderProgramHandle, _shaderProgramUniformLocations.normalMtx, 1, GL_FALSE, &normalMtx[0][0] );
}

//high level draw that calls separate parts
void Motorcycle::drawMotorcycle(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    glUseProgram(_shaderProgramHandle);
    modelMtx = glm::rotate( modelMtx, _rotateMotorcycleAngle, CSCI441::Y_AXIS );
    _drawMotorcycleBody(modelMtx, viewMtx, projMtx);
    _drawMotorcycleWheel(true, modelMtx, viewMtx, projMtx);
    _drawMotorcycleWheel(false, modelMtx, viewMtx, projMtx);

}

void Motorcycle::_drawMotorcycleBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    modelMtx = glm::translate(modelMtx, _transBody);
    modelMtx = glm::scale( modelMtx, _scaleBody );
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);
    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_colorBody[0]);
    CSCI441::drawSolidCube( 0.2 );
}

void Motorcycle::_drawMotorcycleWheel(bool isFrontWheel, glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    if(!isFrontWheel){
        modelMtx = glm::translate(modelMtx,-_transWheel);
        _colorWheel = glm::vec3(0.0f,1.0f,1.0f);
    }
    else{
        modelMtx = glm::translate(modelMtx, _transWheel);
        _colorWheel = glm::vec3(1.0f,0.0f,0.0f);
    }
    modelMtx = glm::rotate(modelMtx, static_cast<GLfloat>(M_PI / 2.0f) + _wheelAngle, CSCI441::Z_AXIS );
    modelMtx = glm::rotate(modelMtx,static_cast<GLfloat>(M_PI / 2.0f), CSCI441::Z_AXIS );
    modelMtx = glm::scale(modelMtx, _scaleWheel);

    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);
    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_colorWheel[0]);

    CSCI441::drawSolidTorus(.05,.08,20,10);
}

//rotates motorcycle
void Motorcycle::rotate(GLfloat dTheta) {
    _rotateMotorcycleAngle -=  dTheta * M_PI / 24.0f;
    if(_rotateMotorcycleAngle > 2 * M_PI) _rotateMotorcycleAngle -= 2 * M_PI;
    if(_rotateMotorcycleAngle < 0) _rotateMotorcycleAngle += 2 * M_PI;
}

glm::vec3 Motorcycle::getPosition() {
    return _position;
}

//makes sure motorcycle does not go off world
void Motorcycle::_checkBounds(GLfloat WorldSize) {
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

GLfloat Motorcycle::getAngle() {
    return _rotateMotorcycleAngle;
}

glm::vec3 Motorcycle::getCameraOffset() {
    return _cameraOffset;
}

