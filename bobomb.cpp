//
// Created by xxhij on 10/8/2022.
//

#include "bobomb.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <CSCI441/objects.hpp>
#include <CSCI441/OpenGLUtils.hpp>
#include <CSCI441/OpenGLEngine.hpp>

#ifndef M_PI
#define M_PI 3.14159265
#endif


Bobomb::Bobomb( GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint normalMtxUniformLocation, GLint materialColorUniformLocation ) {

    // initializing values in constructor
    _isFlicker = false;
    _flickerCheck = 0;

    _wheelAngle = 0.0f;
    _wheelAngleRotationSpeed = M_PI / 16.0f;

    _shaderProgramHandle                            = shaderProgramHandle;
    _shaderProgramUniformLocations.mvpMtx           = mvpMtxUniformLocation;
    _shaderProgramUniformLocations.normalMtx        = normalMtxUniformLocation;
    _shaderProgramUniformLocations.materialColor    = materialColorUniformLocation;

    _bobombDirection =  0.0f;
    _bobombDirectionRotationSpeed = M_PI / 24.0f;

    _colorBody = glm::vec3( 0.0f, 0.0f, 1.0f );
    _scaleBody = glm::vec3( 1.0f, 1.0f, 1.0f );
    _colorEye = glm::vec3( 1.0f, 1.0f, 1.0f );
    _scaleEye = glm::vec3( 0.05f, 0.1f, 0.05f );

    _colorFuse = glm::vec3( 0.5f, 0.5f, 0.5f );
    _transFuse = glm::vec3(0.0f,0.5f,0.0f);

    _colorBoot = glm::vec3(0.3f, 0.8f, 0.2f);
    _transBootA = glm::vec3(0.0f,-1.3f,0.0f);
    _transBootB = glm::vec3(-0.2f,-0.9f,0.7f);
    _transBootC = glm::vec3(-0.2f,-1.3f,0.7f);

    _colorWheel = glm::vec3( 0.55f, 0.27f, 0.07f );

    _colorFlicker = glm::vec3( 1.0f, 1.0f, 0.0f );
    _colorFlickerEx = glm::vec3( 1.0f, 0.0f, 0.0f );

}

void Bobomb::drawBobomb( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) {
    glUseProgram( _shaderProgramHandle );

    // apply transformations to entire model
    modelMtx = glm::rotate(modelMtx,glm::radians(14.0f),CSCI441::Y_AXIS);
    modelMtx = glm::translate(modelMtx,glm::vec3(0.0f,0.7f,0.0f));
    modelMtx = glm::scale(modelMtx,glm::vec3(0.5f,0.5f,0.5f));

    // draw each part of model, passing modelMtx between each.
    _drawBobombBody(modelMtx, viewMtx, projMtx);        // the body of our bobomb
    _drawBobombEye(true, modelMtx, viewMtx, projMtx);  // the left eye
    _drawBobombEye(false, modelMtx, viewMtx, projMtx); // the right eye
    _drawBobombFuse(modelMtx, viewMtx, projMtx);        // the fuse
    _drawBobombFlicker(modelMtx, viewMtx, projMtx);       // the flicker
    _drawBobombBoot(modelMtx, viewMtx, projMtx);   // the boot
    _drawBobombWheels(modelMtx, viewMtx, projMtx);        // the wheels
}
// moving forward function
void Bobomb::driveForward(GLfloat worldSize) {
    // update wheel angle to animate with movement
    _wheelAngle += _wheelAngleRotationSpeed;
    if( _wheelAngle > 2.0f * M_PI ) _wheelAngle -= 2.0f * M_PI;

    // create a new value by converting direction angle theta and a step size of 0.15 from polar
    // to linear coordinates; then add to the current position.
    glm::vec3 nPosit = glm::vec3(-0.15*sin(-_bobombDirection),0.0f, 0.15*cos(-_bobombDirection));
    _bobombPosition = _bobombPosition + nPosit;
    // bounds checking; if current position is outside the world, undo the movement just done. This functionally locks
    // down the edges of the map.
    if(_bobombPosition.x > worldSize || _bobombPosition.z > worldSize || _bobombPosition.x < -worldSize || _bobombPosition.z < -worldSize){
        _bobombPosition -= nPosit;
    }

}
// backwards moving function; very similar to the forwards movement one,
// with the relative sign of the calculated value negated before being added to the position.
// Bounds checking functionality inverted to account for the inverted direction.
void Bobomb::driveBackward(GLfloat worldSize) {
    _wheelAngle -= _wheelAngleRotationSpeed;
    if( _wheelAngle < 0.0f ) _wheelAngle += 2.0f * M_PI;

    glm::vec3 nPosit = glm::vec3(-0.15f*sin(-_bobombDirection),0.0f,0.15*cos(-_bobombDirection));
    _bobombPosition = _bobombPosition - nPosit;
    if(_bobombPosition.x > worldSize || _bobombPosition.z > worldSize || _bobombPosition.x < -worldSize || _bobombPosition.z < -worldSize){
        _bobombPosition += nPosit;
    }
}
// rotation function; takes a keypress as input,
// and depending on which key is inputted, the stored
// angle for our bobomb direction is updated appropriately.
void Bobomb::rotateBobomb(GLuint key) {
    if(key == GLFW_KEY_A) {
        _bobombDirection += _bobombDirectionRotationSpeed;
        // angle bounds checking; keeps value between 0 and 2*pi.
        if( _bobombDirection > 2.0f * M_PI ) _bobombDirection -= 2.0f * M_PI;
    } else {
        _bobombDirection -= _bobombDirectionRotationSpeed;
        if( _bobombDirection < 0 ) _bobombDirection = 2.0f * M_PI;
    }
}

// idle animation function;
// update this value on every rendered frame.
// change the color of the light on the
// bob-omb fuse over time; providing idle animation
// regardless of movement.
void Bobomb::_updateFlicker() {
    _flickerCheck += 1;
    if(_flickerCheck > 1e8) _flickerCheck = 0;
    if(_flickerCheck % 30 == 0 && !(_isFlicker))_isFlicker = true;
    else if(_flickerCheck % 30 == 0) _isFlicker = false;
}
// beginning of several draw functions; all
// iteratively draw different parts of the model and pass
// the modelMtx along to the next as it goes.
void Bobomb::_drawBobombBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const {
    modelMtx = glm::translate(modelMtx, glm::vec3(0.0f, 0.0f, 0.0f));
    modelMtx = glm::scale( modelMtx, _scaleBody );
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);

    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_colorBody[0]);

    CSCI441::drawSolidSphere(0.5,glm::degrees(2*M_PI),glm::degrees(2*M_PI));
}
// functionality derived from isLeftWing function from lab05 plane class.
void Bobomb::_drawBobombEye(bool isLeftEye, glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const {
    if( isLeftEye ) {
        modelMtx = glm::translate(modelMtx, glm::vec3(0.0f, 0.0f, 0.5f));

    } else modelMtx = glm::translate(modelMtx, glm::vec3(-0.3f, 0.0f, 0.4f));

    modelMtx = glm::scale( modelMtx, _scaleEye );

    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);

    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_colorEye[0]);

    CSCI441::drawSolidSphere(0.5,glm::degrees(2*M_PI),glm::degrees(2*M_PI));
}
void Bobomb::_drawBobombFuse(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const {
    modelMtx = glm::translate( modelMtx, _transFuse);

    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);

    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_colorFuse[0]);

    CSCI441::drawSolidCube( 0.15 );
}
void Bobomb::_drawBobombFlicker(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const {
    modelMtx = glm::translate( modelMtx, glm::vec3(0.0f,0.6f,0.0f));
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);
    // here is where we utilize the _isFlicker bool to choose a color for the flicker.
    if(!_isFlicker) glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_colorFlicker[0]);
    else glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_colorFlickerEx[0]);

    CSCI441::drawSolidCube( 0.05 );
}
void Bobomb::_drawBobombBoot(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const {

    glm::mat4 modelMtx1 = glm::translate( modelMtx, _transBootA );
    _computeAndSendMatrixUniforms(modelMtx1, viewMtx, projMtx);

    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_colorBoot[0]);

    CSCI441::drawSolidCylinder(0.5f,0.5f,1.1f,glm::degrees(2*M_PI),glm::degrees(2*M_PI));

    glm::mat4 modelMtx2 = glm::translate( modelMtx, _transBootB );
    _computeAndSendMatrixUniforms(modelMtx2, viewMtx, projMtx);

    CSCI441::drawSolidSphere(0.45,glm::degrees(2*M_PI),glm::degrees(2*M_PI));

    glm::mat4 modelMtx3 = glm::translate( modelMtx, _transBootC );
    _computeAndSendMatrixUniforms(modelMtx3, viewMtx, projMtx);

    CSCI441::drawSolidCylinder(0.4f,0.4f,0.3f,glm::degrees(2*M_PI),glm::degrees(2*M_PI));

}

void Bobomb::_drawBobombWheels(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const {
    glm::mat4 modelMtx1 = glm::translate( modelMtx, glm::vec3(0.15f,-1.2f,0.85f));
    modelMtx1 = glm::rotate( modelMtx1, glm::radians(75.0f),CSCI441::Y_AXIS );
    glm::mat4 modelMtx1a = glm::rotate(modelMtx1,_wheelAngle,CSCI441::Z_AXIS);

    _computeAndSendMatrixUniforms(modelMtx1a, viewMtx, projMtx);

    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_colorWheel[0]);

    CSCI441::drawSolidTorus(0.1f,0.2f,5,5);

    glm::mat4 modelMtx2 = glm::translate( modelMtx1, glm::vec3(0.0f,0.0f,-0.8f));
    glm::mat4 modelMtx2a = glm::rotate(modelMtx2,_wheelAngle,CSCI441::Z_AXIS);

    _computeAndSendMatrixUniforms(modelMtx2a, viewMtx, projMtx);

    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_colorWheel[0]);

    CSCI441::drawSolidTorus(0.1f,0.2f,5,5);

    glm::mat4 modelMtx3 = glm::translate( modelMtx1, glm::vec3(1.0f,0.0f,0.15f));
    glm::mat4 modelMtx3a = glm::rotate(modelMtx3,_wheelAngle,CSCI441::Z_AXIS);

    _computeAndSendMatrixUniforms(modelMtx3a, viewMtx, projMtx);

    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_colorWheel[0]);

    CSCI441::drawSolidTorus(0.1f,0.2f,5,5);

    glm::mat4 modelMtx4 = glm::translate( modelMtx2, glm::vec3(1.0f,0.0f,-0.05f));
    glm::mat4 modelMtx4a = glm::rotate(modelMtx4,_wheelAngle,CSCI441::Z_AXIS);

    _computeAndSendMatrixUniforms(modelMtx4a, viewMtx, projMtx);

    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_colorWheel[0]);

    CSCI441::drawSolidTorus(0.1f,0.2f,5,5);


}

void Bobomb::_computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    // precompute the Model-View-Projection matrix on the CPU
    glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;
    // then send it to the shader on the GPU to apply to every vertex
    glProgramUniformMatrix4fv( _shaderProgramHandle, _shaderProgramUniformLocations.mvpMtx, 1, GL_FALSE, &mvpMtx[0][0] );

    glm::mat3 normalMtx = glm::mat3( glm::transpose( glm::inverse( modelMtx )));
    glProgramUniformMatrix3fv( _shaderProgramHandle, _shaderProgramUniformLocations.normalMtx, 1, GL_FALSE, &normalMtx[0][0] );
}

// getters, setters
glm::vec3 Bobomb::getPosition() {
    return _bobombPosition;
}

void Bobomb::setPosition(glm::vec3 nPosit) {
    _bobombPosition = nPosit;
}

GLfloat Bobomb::getDirection() {
    return _bobombDirection;
}

void Bobomb::setDirection(GLfloat nDirec) {
    _bobombDirection = nDirec;
}

