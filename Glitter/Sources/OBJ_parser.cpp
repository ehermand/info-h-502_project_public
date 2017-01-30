#include <iostream>
#include <vector>
#include <string>

#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct OBJ {
    std::string name;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvcoords;
    std::vector<glm::vec3> normals;
    std::vector<GLuint> vertexIndices;
};

OBJ loadOBJ(const GLchar* path) {

    std::ifstream fileStream;
    std::string line,dataType;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvcoords;
    std::vector<glm::vec3> normals;
    std::vector<GLuint> vertexIndices,uvIndices,normalIndices;
    std::string name;

    fileStream.open(path);

    if (fileStream.fail()) {
        std::cout << "OBJ file could not be read!" << std::endl;
    } else {
        while(std::getline(fileStream,line)) {
            std::stringstream lineStream(line);
            //lineStream << line;
            //std::cout << line << std::endl;
            //std::getline(lineStream,dataType,' ');
            lineStream >> dataType;
            //std::cout << dataType << std::endl;
            if (dataType == "v") {
                GLfloat x,y,z;
                lineStream >> x >> y >> z;
                glm::vec3 vertex = glm::vec3(x,y,z);
                vertices.push_back(vertex);
            } else if (dataType == "vt") {
                GLfloat u,v;
                lineStream >> u >> v;
                glm::vec2 uvcoord = glm::vec2(u,v);
                uvcoords.push_back(uvcoord);
            } else if (dataType == "vn") {
                GLfloat x,y,z;
                lineStream >> x >> y >> z;
                glm::vec3 normal = glm::vec3(x,y,z);
                normals.push_back(normal);
            } else if (dataType == "f") {
                GLuint vi,vti,vni;
                while (lineStream.good()) {
                    lineStream >> vi; vertexIndices.push_back(vi-1);
                    //std::cout << vi << std::endl;
                    //std::cin.get();
                    if (lineStream.peek() == '/') {
                        lineStream.ignore();
                        if (lineStream.peek() == '/') {
                            lineStream.ignore();
                            lineStream >> vni; normalIndices.push_back(vni-1);
                        } else {
                            lineStream >> vti; uvIndices.push_back(vti-1);
                        }
                    }
                }
            } else if (dataType == "s"){
                lineStream >> name;
            }
        }
        std::vector<glm::vec2> uvcoords_remap(vertices.size());
        for (int i=0;i<uvIndices.size();i++) {
            std::cout << i << std::endl;
            uvcoords_remap[vertexIndices[i]] = uvcoords[uvIndices[i]];
        }
        std::vector<glm::vec3> normals_remap(vertices.size());
        for (int i=0;i<normalIndices.size();i++) {
            normals_remap[vertexIndices[i]] = normals[normalIndices[i]];
        }
        OBJ out = {name,vertices,uvcoords_remap,normals_remap,vertexIndices};
        return out;
    }
}
