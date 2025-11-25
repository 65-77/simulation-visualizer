//-- !!! docs.gl !!! --// gl4

#include "../dependencies/GLEW/glew.h"
#include "../dependencies/GLFW/include/GLFW/glfw3.h"

#include <math.h> // math
#include <iostream> // input/output stream
#include <fstream> // file stream
#include <string> // strings!
#include <sstream> //string stream
#include <signal.h> // Error handler

#define	ASSERT(x) do { if (!(x)) raise(SIGTRAP); } while(0)
#define GLCall(x) GLClearError();\
   x;\
   ASSERT(GLLogCall(#x, __FILE__, __LINE__))

/* :sparkles: error handler :sparkles:*/
static void GLClearError() {
   while (glGetError() != GL_NO_ERROR);	// https://docs.gl/gl4/glGetError
}

static bool GLLogCall(const char* function, const char* file, int line) {
   while (GLenum error = glGetError()) {
      std::cout << "[OpenGL Error]: ( " << error << " )" <<
	 "\n\t\033[35mFunction: \033[37m" << function <<
	 "\n\t\033[35mFile: \033[37m" << file <<
	 "\n\t\033[35mLine: \033[37m" << line <<
	 std::endl;

      if (error == GL_INVALID_ENUM) {
	 std::cout << "\t\033[35mInvalid: \033[37menum" << std::endl;
      } else if (error == GL_INVALID_VALUE) {
	 std::cout << "\t\033[35mInvalid: \033[37mvalue" << std::endl;
      } else if (error == GL_INVALID_OPERATION) {
	 std::cout << "\t\033[35mInvalid: \033[37moperation" << std::endl;
      } else if (error == GL_INVALID_FRAMEBUFFER_OPERATION) {
	 std::cout << "\t\033[35mInvalid: \033[37mframebuffer operation" << std::endl;
      } else if (error == GL_OUT_OF_MEMORY) {
	 std::cout << "\tOut of memory" << std::endl;
      } else if (error == GL_STACK_UNDERFLOW) {
	 std::cout << "\tStack underflow" << std::endl;
      } else if (error == GL_STACK_OVERFLOW) {
	 std::cout << "\tStack overflow" << std::endl;
      } else {
	 std::cout << "\tGood luck with this one ;w;" << std::endl;
      }

      std::cout << "\033[37m " << std::endl;
      return false;
   }

   return true;
}

struct ShaderProgramSource {
   std::string VertexSource;
   std::string FragmentSource;
};

/* Seperates vertex and fragment shader when reading shader file ( primary.shader ) */
static ShaderProgramSource ParseShader(const std::string& shaderFilePath) {
   std::ifstream stream(shaderFilePath); // takes in input file
   if (!stream.is_open()) {
      std::cout << "Shader file not open " << shaderFilePath << std::endl;
   }

   enum class ShaderType {
      NONE = -1, VERTEX = 0, FRAGMENT = 1
   };

   std::stringstream ss[2]; // one for vertex, one for fragments
   std::string currentLine;
   ShaderType type = ShaderType::NONE;

   while(getline(stream, currentLine)) { // Loops through content until it condition met
      if (currentLine.find("#shader") != std::string::npos) {
	 if (currentLine.find("vertex") != std::string::npos) { // checks for vertex, to specify vertex
	    type = ShaderType::VERTEX;
	 } else if (currentLine.find("fragment") != std::string::npos) { // checks for fragment to specify fragment
	    type = ShaderType::FRAGMENT;
	 }
      } else {
	 ss[(int)type] << currentLine << '\n'; 
      }
   }

   return { ss[0].str(), ss[1].str() };
}

static unsigned int compileShader(unsigned int type, const std::string& source) {
   GLCall(unsigned int id = glCreateShader(type));			// https://docs.gl/gl4/glCreateShader

   GLCall(const char* src = source.c_str());

   GLCall(glShaderSource(id, 1, &src, nullptr));			// https://docs.gl/gl4/glShaderSource
   GLCall(glCompileShader(id));						// https://docs.gl/gl4/glCompileShader	

   int shaderResult;

   GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &shaderResult));		// https://docs.gl/gl4/glGetShader	

   if (shaderResult == GL_FALSE){
      int logLength;
      GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength));	// https://docs.gl/gl4/glGetShader

      char* errorMessage = (char*)alloca(logLength*sizeof(char));	// https://www.geeksforgeeks.org/c/pointer-arithmetics-in-c-with-examples/
									// https://www.man7.org/linux/man-pages/man3/alloca.3.html
      GLCall(glGetShaderInfoLog(id, logLength, &logLength, errorMessage));	// https://docs.gl/gl4/glGetShaderInfoLog

      std::cout << "CompileShader:\n\tshaderResult " <<
	 (type == GL_VERTEX_SHADER ? "vertex: " : "fragment ")		// checks shader type
	 <<"= GL_FALSE" << std::endl;
      std::cout << errorMessage << std::endl;

      GLCall(glDeleteShader(id));					// https://docs.gl/gl4/glDeleteShader

      return 0;
   }

   return id;
}

static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader) { // Taking in source code of shaders as strings
   GLCall(unsigned int program = glCreateProgram());	// https://docs.gl/gl4/glCreateProgram

   GLCall(unsigned int vtxShader = compileShader(GL_VERTEX_SHADER, vertexShader));
   GLCall(unsigned int frgShader = compileShader(GL_FRAGMENT_SHADER, fragmentShader));

   GLCall(glAttachShader(program, vtxShader));		// https://docs.gl/gl4/glAttachShader	
   GLCall(glAttachShader(program, frgShader));

   GLCall(glLinkProgram(program));			// https://docs.gl/gl4/glLinkProgram
   GLCall(glValidateProgram(program));			// https://docs.gl/gl4/glValidateProgram 

   GLCall(glDeleteShader(vtxShader));			// https://docs.gl/gl4/glDeleteShader
   GLCall(glDeleteShader(frgShader));
   return program; 
}

/* increments the colors in our little transition thingy */
float colorIncrementor(float color, float &increment) {
   color += increment; // increments by original value

   /* rounds value if higher than max */
   if (color >= 1.0f) {
      color = 1.0f;
      increment = -increment;
   /* rounds value if lower than max */
   } else if (color <=0.0f) {
      color = 0.0f;
      increment = -increment;
   }

   return color;
}

int main(void) {
   GLCall(GLFWwindow* window); // Defines the window varaible to a "GLFWwindow*" "datatype" (?)

   /* Initialize glfw */
   if (!glfwInit()) { // If glfw failed to initialze, notify
      std::cout << "Failed to initialize GLFW" << std::endl;
      return -1;
   }

   /* Create a windowed mode window and its OpenGL context */
   GLCall(window = glfwCreateWindow(1980, 1120, "Hello World", NULL, NULL)); // window function(x, y, name, NULL, NULL

   if (!window) { // If the window failed to initialize, terminates the glfw proccess
      GLCall(glfwTerminate());
      return -1;
   }
   /* Make the window's context current */
   GLCall(glfwMakeContextCurrent(window)); // Selects window that's going to be edited(?)

   /* Initialze glew */
   if (glewInit() != GLEW_OK) { // If glew failed to initialize, notify
      std::cout << "Failed to initialize GLEW" << std::endl;
   }

   float triangle_coordinates[] = { // coordinates of each vertex of the triangle. -1.0f, -1.0f is bottom-left, 1.0f, 1.0f is top-right
      -1.0f, -1.0f,	// vertex 0: x: -0.5f, y: -0.5f
      1.0f,  1.0f,	// vertex 1: x:  0.0f, y:  0.5f
      1.0f, -1.0f,	// vertex 2: x:  0.5f, y: -0.5f
      -1.0f,  1.0f,	// vertex 3: x: -0.5f, y:  0.5f
   };

   /* Index buffer */
   unsigned int triangle_indices[] = { // Allows us to reuse coordinates from memory
      0, 1, 2,		// triangle: 1: vertex: 0, 1 and 2
      0, 1, 3		// traingle: 2: vertex: 2, 3 and 0
   };

   unsigned int triangle_buffer;				// unsigned int needed
   GLCall(glGenBuffers(1,&triangle_buffer)); 			// Generates 'n' amount of buffers assigned to an uint	// https://docs.gl/gl4/glGenBuffers
   GLCall(glBindBuffer(GL_ARRAY_BUFFER, triangle_buffer));	// Bound buffer is the one future commands will edit!!	// https://docs.gl/gl4/glBindBuffer
   GLCall(glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), triangle_coordinates, GL_STATIC_DRAW)); // data to gpu?	// https://docs.gl/gl4/glBufferData

   GLCall(glEnableVertexAttribArray(0));					// https://docs.gl/gl4/glVertexAttribPointer
   GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));  // https://docs.gl/gl4/glEnableVertexAttribArray

   unsigned int ibo;				// Index buffer object
   GLCall(glGenBuffers(1, &ibo));			// Generates a new buffer
   GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo)); 	// Binds it to an Element array buffer
   GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), triangle_indices, GL_STATIC_DRAW));

   ShaderProgramSource source = ParseShader("../res/shaders/primary.shader");

   unsigned int shader = createShader(source.VertexSource, source.FragmentSource);
   GLCall(glUseProgram(shader));

   GLCall(int location = glGetUniformLocation(shader, "u_Color"));
   ASSERT(location != -1);
   // GLCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f));

   /* RGB */
   float r, g, b, a;
   r = 0.0f;
   g = 0.0f;
   b = 1.0f;
   a = 1.0f;

   /* Increment */
   float basei = 0.1f;  

   ///------------///   
   ///- MAINLOOP -///
   ///------------///

   /* Loop until the user closes the window */
   while (!glfwWindowShouldClose(window)) {
      /* Render here */
      GLCall(glClear(GL_COLOR_BUFFER_BIT));	// https://docs.gl/gl4/glClear

      // Refreshes back-buffer, allowing us to display info before displaying it
      // glDrawArrays(GL_TRIANGLES, 0, sizeof(triangle_coordinates)/2);	// https://docs.gl/gl4/glDrawArrayw
      // divided by 2 due to each vertex having 2 dimensional coordinates per item
      r = colorIncrementor(r, basei);
      g = colorIncrementor(g, basei);
      b = colorIncrementor(b, basei);

      GLCall(glUniform4f(location, r, g, b, a));

      GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));	// https://docs.gl/gl4/glDrawElements



      /* Swap front and back buffers */
      GLCall(glfwSwapBuffers(window)); // swaps the back and front buffer, allowing us to view displayed info in the previous back-buffer

      /* Poll for and process events */
      GLCall(glfwPollEvents()); // Detects events, and is most likely just an even handler (?)
   }

   glDeleteProgram(shader);	// https://docs.gl/gl4/glDeleteProgram

   glfwTerminate(); // Terminates glfw process
   return 0;
}
