//
// Created by Aidan Lamkin on 10/4/22.
//

#ifndef LAB05_MOTORCYCLE_HPP
#define LAB05_MOTORCYCLE_HPP


#include <GL/glew.h>

#include <glm/glm.hpp>

class Motorcycle {
public:
    Motorcycle( GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint normalMtxUniformLocation, GLint materialColorUniformLocation, GLint modelMtxUniformLocation);

    void drawMotorcycle(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx);

    //movement Methods
    void driveForward();

    void driveBackward();

    void rotate(GLfloat dTheta);

    glm::vec3 getPosition();
    glm::vec3 getCameraOffset();
    GLfloat getAngle();

    //check to see if you went off grid
    void _checkBounds(GLfloat worldSize);

private:
    GLfloat _wheelAngle;
    GLfloat _wheelRotationSpeed;
    GLuint _shaderProgramHandle;

    GLfloat _movementSpeed;

    glm::vec3 _position;

    glm::vec3 _cameraOffset;


    //uniforms
    struct ShaderProgramUniformLocations {
        GLint mvpMtx;
        GLint normalMtx;
        GLint materialColor;
        GLint modelMtx;
    } _shaderProgramUniformLocations;

    //drawing info
    GLfloat _rotateMotorcycleAngle;

    glm::vec3 _colorBody;
    glm::vec3 _scaleBody;
    glm::vec3 _transBody;

    glm::vec3 _colorWheel;
    glm::vec3 _scaleWheel;
    glm::vec3 _transWheel;

    //draw methods
    void _drawMotorcycleBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;

    void _drawMotorcycleWheel(bool isFrontWheel, glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx );

    void _computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const;



};
#endif //LAB05_MOTORCYCLE_HPP
