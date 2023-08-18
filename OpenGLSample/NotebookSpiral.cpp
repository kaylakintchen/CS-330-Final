#define _USE_MATH_DEFINES // for C++
#include <cmath>

#include "notebookSpiral.h"

using namespace std; // Standard namespace


vector<GLfloat>* NotebookSpiral::drawNotebookSpirals()
{
    vector<GLfloat>* vec = new vector<GLfloat>{
	
	-0.5f, 0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, 0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, 0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

	 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	  0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,

	 -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	  0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	  0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
	  0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
	
    };
    return vec;
}


