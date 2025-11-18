//-- !!! docs.gl !!! --// gl4

#include "../dependencies/GLEW/glew.h"
#include "../dependencies/GLFW/include/GLFW/glfw3.h"

#include <math.h>
#include <iostream>

/* Makes triangle produced into a equailateral by giving appropriate height */
float window_geometry(int hyp, int x) {

   float height = (sqrt((hyp*hyp)-(x*x)));	// pythagoreas: sqrt((hyp^2)-adjacent^2) = opposite 
   int round_height = round(height); 		// rounding for easier window computation (?)
   
   return round_height;
}

int main(void) {
   GLFWwindow* window; // Defines the window varaible to a "GLFWwindow*" "datatype" (?)

   /* Initialize glfw */
   if (!glfwInit()) { // If glfw failed to initialze, notify
      std::cout << "Failed to initialize GLFW" << std::endl;
      return -1;
   }

   /* Create a windowed mode window and its OpenGL context */
   window = glfwCreateWindow(640, window_geometry(640, 640/2), "Hello World", NULL, NULL); // window function(x, y, name, NULL, NULL)
 
   if (!window) { // If the window failed to initialize, terminates the glfw proccess
      glfwTerminate();
      return -1;
   }
   /* Make the window's context current */
   glfwMakeContextCurrent(window); // Selects window that's going to be edited(?)

   /* Initialze glew */
   if (!glewInit() != GLEW_OK) { // If glew failed to initialize, notify
      std::cout << "Failed to initialize GLEW";
   }
   
   float triangle_coordinates[6] = { // coordinates of each vertex of the triangle. -1.0f, -1.0f is bottom-left, 1.0f, 1.0f is top-right
      -1.0f, -1.0f,	// vertex 1: x: -1.0f, y: -1.0f
       0.0f,  1.0f,	// vertex 2: x:  1.0f, y:  1.0f
       1.0f, -1.0f	// vertex 3: x:  1.0f, y: -1.0f
   };
   
   unsigned int triangle_buffer;											// unsigned int needed
   glGenBuffers(1,&triangle_buffer); 			// Generates 'n' amount of buffers assigned to an uint		// https://docs.gl/gl4/glGenBuffers
   glBindBuffer(GL_ARRAY_BUFFER, triangle_buffer);	// Bound buffer is the one future commands will edit!!		// https://docs.gl/gl4/glBindBuffer
   glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_coordinates) * sizeof(float), triangle_coordinates, GL_STATIC_DRAW);	// https://docs.gl/gl4/glBufferData
   
   glEnableVertexAttribArray(0);					// https://docs.gl/gl4/glVertexAttribPointer
   glVertexAttribPointer(0,2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);  // https://docs.gl/gl4/glEnableVertexAttribArray

   /* Loop until the user closes the window */
   while (!glfwWindowShouldClose(window))
   {
      /* Render here */
      glClear(GL_COLOR_BUFFER_BIT);	// https://docs.gl/gl4/glClear
				        // Refreshes back-buffer, allowing us to display info before displaying it
      glDrawArrays(GL_TRIANGLES, 0, sizeof(triangle_coordinates)/2);	// https://docs.gl/gl4/glDrawArrayw
									// divided by 2 due to each vertex having 2 dimensional coordinates

      /* Swap front and back buffers */
      glfwSwapBuffers(window); // swaps the back and front buffer, allowing us to view displayed info in the previous back-buffer

      /* Poll for and process events */
      glfwPollEvents(); // Detects events, and is most likely just an even handler (?)
   }
   glfwTerminate(); // Terminates glfw process
   return 0;
}
