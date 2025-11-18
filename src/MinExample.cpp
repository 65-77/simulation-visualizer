#include <GLFW/glfw3.h>
int main(void) {
   GLFWwindow* window;
   if(!glfwInit())
      return -1;
   window = glfwCreateWindow(640, 640, "w", NULL, NULL);
   if (!window) {
      glfwTerminate();
      return 0;
   }
   glfwMakeContextCurrent(window);
   while (!glfwWindowShouldClose(window)) {
      glfwSwapBuffers(window);
   }
   glfwTerminate();
   return 0;
}
