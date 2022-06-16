
#define GLEW_STATIC  
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


/*
	1.�������飺ָ��ÿ����������ԣ�������Ӧ�ó����ַ�ռ䣨OpenGL ES ��Ϊ�ͻ��ռ䣩 �Ļ�������
	    �Ƕ��㻺��������Ļ������ṩָ��������������

		����������glVertexAttribPointer �� glVertexAttribIPointer����ָ����

	2.�����Լ��洢�������Եķ�����
		�ṹ���飺��һ���������洢
		����ṹ���ڵ����Ļ������洢
		�ٶ�ÿ��������4���������ԣ�λ�á����ߺ������������ꡣ ��Щ����һ�𱣴���Ϊ���ж�������һ���������С�

		����λ�����ԣ� ��3����������������x��y��z�� ����ʽָ����
		���㷨�ߣ� Ҳ��3����������������x��y��z�� ����ʽָ����
		ÿ���������꣺ ��2����������ɵ�������s��t������ʽָ����

		�������Ŀ��Ϊ��ɶ�����������Ե��ܴ�С����һ���������10������������40���ֽڣ�12���ֽ�����λ�á�12���ֽ����ڷ��ߣ�
		8���ֽ�����Tex0��8���ֽ�����Tex1��
	
	3.�ṹ�����Ч�ʻ���ߣ�ԭ����ÿ��������������ݿ���˳���ȡ������
	�����Ҫ����ĳ���ض�������ʱ���ͻ�Ƚ��鷳�����Խ���̬�Ķ������Ա����ڵ����Ļ���������Ч�ʵ���

	4.glEnableVertexAttribArray��glDisableVertexAttribArray����ֱ��������úͽ���ͨ�ö����������顣


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
	glBindBuffer(GL_ARRAY_BUFFER,VBO);//����һ��VBO���󣬲����к�GL_ARRAY_BUFFER���ͽ��а�
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);//��֮ǰ����Ķ������ݸ��Ƶ�������ڴ���4

	//��ʱ�Ѿ������ݴ洢���Կ����ڴ��У���������Ҫ����һ��������ɫ����Ƭ����ɫ���������Ĵ�����Щ��ɫ

	/*
		������ɫ����GLSL����
		ÿһ��������ɫ������ʼ��һ���汾����
		����ʹ��in�ؼ��֣��ڶ�����ɫ�����������еĶ����������ԡ�

		���δ���������c��Ϊ������OpenGl�����������У���Ҫ���ж�̬�ı���
	*/
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);//����һ��������ɫ��
	//����ɫ��Դ�븽�ŵ���ɫ�������ϣ�Ȼ�����
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//����Ҫ�������ɫ��������Ϊ��һ���������ڶ��������Ǵ��ݵ�Դ���ַ����������������������Ƕ�����ɫ��������Դ��
	glCompileShader(vertexShader);//�����Ƿ�ɹ�

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);//����Ƿ�ɹ�
	if (!success) {
		glGetShaderInfoLog(vertexShader,512,NULL,infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	
	/*
		�ô��ڱ�������״̬����Ⱦѭ����ֱ��������Ϊ�Ĳ���֮��

	*/
	while (!glfwWindowShouldClose(window))
	{
		
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//״̬���ú���
		glClear(GL_COLOR_BUFFER_BIT);//״̬ʹ�ú�����ʹ�õ�ǰ��״̬��ȡӦ��

		
		glfwSwapBuffers(window);//������ɫ���壬��Ϊ�����ʾ����Ļ��
		glfwPollEvents();//����Ƿ����ʲô�¼������´��ڵ�״̬
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
	
