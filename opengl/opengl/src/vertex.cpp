
#define GLEW_STATIC  
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


/*
	1.顶点数组：指定每个顶点的属性，保存在应用程序地址空间（OpenGL ES 称为客户空间） 的缓冲区。
	    是顶点缓冲区对象的基础，提供指定顶点属性数据

		顶点数组用glVertexAttribPointer 或 glVertexAttribIPointer函数指定。

	2.分配以及存储顶点属性的方法：
		结构数组：在一个缓冲区存储
		数组结构：在单独的缓冲区存储
		假定每个顶点有4个顶点属性：位置、法线和两个纹理坐标。 这些属性一起保存在为所有顶点分配的一个缓冲区中。

		顶点位置属性： 以3个浮点数的向量（x，y，z） 的形式指定。
		顶点法线： 也以3个浮点数的向量（x，y，z） 的形式指定。
		每个纹理坐标： 以2个浮点数组成的向量（s，t）的形式指定。

		缓冲区的跨距为组成顶点的所有属性的总大小。（一个顶点等于10个浮点数或者40个字节：12个字节用于位置、12个字节用于法线，
		8个字节用于Tex0，8个字节用于Tex1）
	
	3.结构数组的效率会更高，原因是每个顶点的属性数据可以顺序读取，但是
	如果需要更改某个特定的属性时，就会比较麻烦，可以将动态的顶点属性保存在单独的缓冲区避免效率低下

	4.glEnableVertexAttribArray和glDisableVertexAttribArray命令分别用于启用和禁用通用顶点属性数组。


*/




void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(800,600,"LearnOpenGl",NULL,NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;

	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, *framebuffer_size_callback);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		printf("Error\n");

	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);//生成一个VBO对象，并进行和GL_ARRAY_BUFFER类型进行绑定
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);//将之前定义的顶点数据复制到缓冲的内存中4

	//此时已经将数据存储在显卡的内存中，接下来需要创建一个顶点着色器和片段着色器，真正的处理这些颜色

	/*
		顶点着色器：GLSL语言
		每一个顶点着色器都起始于一个版本声明
		我们使用in关键字，在顶点着色器中声明所有的顶点输入属性。

		本段代码类似于c，为了能在OpenGl中正常的运行，需要进行动态的编译
	*/
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);//创建一个顶点着色器
	//将着色器源码附着到着色器对象上，然后编译
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//将需要编译的着色器对象，作为第一个参数，第二个参数是传递的源码字符串的数量，第三个参数是顶点着色器真正的源码
	glCompileShader(vertexShader);//编译是否成功

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);//检查是否成功
	if (!success) {
		glGetShaderInfoLog(vertexShader,512,NULL,infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	
	/*
		让窗口保持运行状态，渲染循环，直到我们认为的操作之后。

	*/
	while (!glfwWindowShouldClose(window))
	{
		
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//状态设置函数
		glClear(GL_COLOR_BUFFER_BIT);//状态使用函数，使用当前的状态获取应该

		
		glfwSwapBuffers(window);//交换颜色缓冲，作为输出显示在屏幕上
		glfwPollEvents();//检查是否出发什么事件，更新窗口的状态
	}

	glfwTerminate();
	return 0;
}




	void processInput(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}


	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}
	
