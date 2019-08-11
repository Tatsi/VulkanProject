#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <vector>
#include <algorithm>

const int WIDTH = 800;
const int HEIGHT = 600;

class HelloTriangleApplication {
public:
	void run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

private:
	void initVulkan() {
		createInstance();
	}
	void createInstance() {
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Vulkan project";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		// Get supported instance level extensions
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
		std::cout << "available extensions:" << std::endl;

		for (const auto& extension : extensions) {
			std::cout << "\t" << extension.extensionName << std::endl;
		}

		// Get extensions that are required for WSI
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		// Check that all instance level extensions required by GLFW are supported
		for (uint32_t i = 0; i < glfwExtensionCount; ++i)
		{
			const std::string extensionName = std::string(glfwExtensions[i]);
			if (std::find_if(extensions.begin(), extensions.end(), [extensionName](const VkExtensionProperties& p) {
				return p.extensionName == extensionName;
				}) == extensions.end())
			{
				std::cerr << "GLFW requires extension " << extensionName << ", but it's not supported!" << std::endl;
				std::exit(EXIT_FAILURE);
			}
		}

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;
		createInfo.enabledLayerCount = 0; // Don't enable any validation layers now

		VkResult result = vkCreateInstance(&createInfo, nullptr, &m_instance);
	}

	void initWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		m_window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan project", nullptr, nullptr);
	}

	void mainLoop() {
		while (!glfwWindowShouldClose(m_window)) {
			glfwPollEvents();
		}
	}

	void cleanup() {
		vkDestroyInstance(m_instance, nullptr);
		glfwDestroyWindow(m_window);
		glfwTerminate();

	}
	GLFWwindow* m_window;
	VkInstance m_instance;
};

int main() {
	HelloTriangleApplication app;

	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}