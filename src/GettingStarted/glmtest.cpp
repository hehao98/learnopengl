//
// Have a simple test on the GLM library
//
// Created by hehao on 18-1-24.
//

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

inline ostream &operator<<(ostream &os, glm::vec4 vec)
{
    os << "(" << vec.x << "," << vec.y << "," << vec.z << "," << vec.w << ")";
    return os;
}

int main()
{
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f);

    // A transformation that moves the vector by (1,1,1)
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 1.0f));
    vec = trans * vec;
    // Now vec is (2,1,1)
    cout << vec << endl;

    // Setting trans to identity matrix
    trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
    cout << trans * vec << endl;
    return 0;
}