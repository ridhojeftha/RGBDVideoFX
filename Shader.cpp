#include "Shader.h"

Shader::Shader(const char *fragFile) :
programID(0) {

    //create the program object
    programID = glCreateProgram();
    if (programID == 0)
        throw std::runtime_error("glCreateProgram failed");

    //Load shaders
    GLuint vert = loadShaderFile("passcoords.vert", GL_VERTEX_SHADER);
    GLuint frag = loadShaderFile(fragFile, GL_FRAGMENT_SHADER);

    //attach all the shaders
    glAttachShader(programID, vert);
    glAttachShader(programID, frag);

    //link the shaders together
    glLinkProgram(programID);

    //detach shaders
    glDetachShader(programID, vert);
    glDetachShader(programID, frag);

    //throw exception if linking failed
    GLint status;
    glGetProgramiv(programID, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        std::string msg("Program linking failure: ");

        GLint infoLogLength;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
        char* strInfoLog = new char[infoLogLength + 1];
        glGetProgramInfoLog(programID, infoLogLength, NULL, strInfoLog);
        msg += strInfoLog;
        delete[] strInfoLog;

        glDeleteProgram(programID);
        programID = 0;
        throw std::runtime_error(msg);
    }

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

    // connect the xyz to the "pos" attribute of the vertex shader
    glEnableVertexAttribArray(attrib("pos"));
    glVertexAttribPointer(attrib("pos"), 3, GL_FLOAT, GL_FALSE, 5 * sizeof (GLfloat), NULL);


    glVertexAttribPointer(attrib("coord"), 2, GL_FLOAT, GL_TRUE, 5 * sizeof (GLfloat), (const GLvoid*) (3 * sizeof (GLfloat)));
    glEnableVertexAttribArray(attrib("coord"));

    // unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

Shader::~Shader() {
    if (programID != 0) glDeleteProgram(programID);
}

//this function return the programID

GLuint Shader::id() const {
    return programID;
}

//this function return the handle of arrribute variable

GLint Shader::attrib(const GLchar* attribName) const {
    if (!attribName){
       // std::cerr << "attribName was NULL\n";
    }

    GLint attrib = glGetAttribLocation(programID, attribName);
    if (attrib == -1){
   // std::cerr << "Program attribute not found: " << attribName << "\n";
    }

    return attrib;
}

//this function return the handle of uniform variable

GLint Shader::uniform(const GLchar* uniformName) const {
    if (!uniformName){
       // std::cerr << "uniformName was NULL\n";
    }

    GLint uniform = glGetUniformLocation(programID, uniformName);
    if (uniform == -1){
       // std::cerr << "Program uniform not found: " << uniformName << "\n";
    }

    return uniform;
}

GLuint Shader::loadShaderFile(const char *shaderFile, GLenum type) {
    std::ifstream in(shaderFile);
    std::string src = "";
    std::string line = "";

    while (std::getline(in, line))
        src += line+"\n";

    //src.resize(src.length()-1);//POSSIBLE FIX (Remove last \n)

    GLuint shaderID = glCreateShader(type);

    const char* source = src.c_str();
    glCompileShader(shaderID);


    GLint status;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
        std::string msg("Compile failure in shader: ");
        msg.append(shaderFile);
        msg.append("\n");

        GLint infoLogLength;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
        char* strInfoLog = new char[infoLogLength + 1];
        glGetShaderInfoLog(shaderID, infoLogLength, NULL, strInfoLog);
        msg += strInfoLog;
        delete[] strInfoLog;

        glDeleteShader(shaderID);
        shaderID = 0;
        throw std::runtime_error(msg);
    }



    return shaderID;
}
