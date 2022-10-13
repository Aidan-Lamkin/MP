//
// Created by Ethan Ko on 10/9/2022.
//

#ifndef MP_ROBOT_HPP
#define MP_ROBOT_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <CSCI441/ModelLoader.hpp>
#include <CSCI441/OpenGLEngine.hpp>

class Robot{
public:
    Robot( GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint normalMtxUniformLocation, GLint materialColorUniformLocation, GLint modelMtxUniformLocation );
    void drawRobot(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx);
    glm::vec3 getPosition();
    void setPosition(glm::vec3 newPosition);
    void _checkBounds(GLfloat worldSize);
    void rotate(float radians);
    float getAngle();
    void moveForward(GLfloat worldSize);
    void moveBackwards(GLfloat worldSize);
    void idleMotion();
    glm::vec3 cameraOffset();
private:
    float _speed;
    float _boxX;
    float _boxZ;
    float _rotation;
    float _idleMotion;
//    float bodyScale;
    glm::vec3 _position;

    CSCI441::ModelLoader* _modelBody;
    CSCI441::ModelLoader* _modelCube;
    GLuint _shaderProgramHandle;
    struct ShaderProgramUniformLocations {
        GLint mvpMtx;
        GLint modelMtx;
        GLint normalMtx;
        GLint materialColor;
    } _shaderProgramUniformLocations;

    struct ShaderProgramAttributeLocations {
        GLint vPos;
        GLint vNormal;
//        GLint vPosCube;
//        GLint vNormalCube;
    } _shaderProgramAttributeLocations;



    //draw methods
    void _drawBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    void _drawCubeStack(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx )const;

    void _computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const;
};


#endif //MP_ROBOT_HPP
