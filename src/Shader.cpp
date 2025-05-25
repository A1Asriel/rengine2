#include "Shader.h"

#include <glad/glad.h>

#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>

#include "Logging.h"

REngine::Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;

    if (vertexPath != NULL && fragmentPath != NULL) {
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } else {
        WARN("Falling back to built-in shader");
        vertexCode = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normalMatrix;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    Normal = mat3(normalMatrix) * aNormal;
    TexCoord = aTexCoord;
    FragPos = vec3(model * vec4(aPos, 1.0));
}
        )";
        fragmentCode = R"(
#version 330 core
in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;

out vec4 FragColor;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform bool distort;
uniform float u_time;
uniform vec3 u_camera_position;
uniform Light light;
uniform Material material;

vec3 random3(vec3 p) {
    return fract(
        sin(vec3(
            dot(p, vec3(127.1, 311.7, 74.7)),
            dot(p, vec3(269.5, 183.3, 246.1)),
            dot(p, vec3(113.5, 271.9, 124.6))
        )) * 43758.5453
    );
}

float noise(vec3 p) {
    vec3 i = floor(p);
    vec3 f = fract(p);
    f = f*f*(3.0-2.0*f);

    return mix(
        mix(mix(dot(random3(i), f), 
                dot(random3(i + vec3(1.0,0.0,0.0)),f - vec3(1.0,0.0,0.0)),f.x),
            mix(dot(random3(i + vec3(0.0,1.0,0.0)),f - vec3(0.0,1.0,0.0)),
                dot(random3(i + vec3(1.0,1.0,0.0)),f - vec3(1.0,1.0,0.0)),f.x),f.y),
        mix(mix(dot(random3(i + vec3(0.0,0.0,1.0)),f - vec3(0.0,0.0,1.0)),
                dot(random3(i + vec3(1.0,0.0,1.0)),f - vec3(1.0,0.0,1.0)),f.x),
            mix(dot(random3(i + vec3(0.0,1.0,1.0)),f - vec3(0.0,1.0,1.0)),
                dot(random3(i + vec3(1.0,1.0,1.0)),f - vec3(1.0,1.0,1.0)),f.x),f.y),f.z);
}

float perlin(vec3 p) {
    float total = 0.0;
    float frequency = 1.0;
    float amplitude = 1.0;

    for(int i=0; i<4; i++) {
        total += noise(p * frequency) * amplitude;
        frequency *= 2.0;
        amplitude *= 0.5;
    }

    return total;
}

void main() {
    if (!gl_FrontFacing) {
        discard;
        return;
    }

    vec2 ourUV;

    if (distort) {
        vec3 noise_coord = vec3(TexCoord * 5.0, u_time * 0.1);
        float noise_value = perlin(noise_coord) * 0.1;
        ourUV = TexCoord + vec2(noise_value);
    } else {
        ourUV = TexCoord;
    }

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, ourUV));

    vec3 norm = normalize(Normal);
    vec3 light_direction = normalize(light.position - FragPos);
    float diff = max(dot(norm, light_direction), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, ourUV));

    vec3 viewDir = normalize(u_camera_position - FragPos);
    vec3 reflectDir = reflect(-light_direction, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * vec3(texture(material.specular, ourUV)));

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
        )";
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void REngine::Shader::use() {
    glUseProgram(ID);
}

void REngine::Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void REngine::Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void REngine::Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void REngine::Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void REngine::Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void REngine::Shader::checkCompileErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            ERROR("Shader compilation error: " << infoLog);
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            ERROR("Shader linking error: " << infoLog);
        }
    }
}