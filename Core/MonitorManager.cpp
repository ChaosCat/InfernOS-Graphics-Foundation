#include "../Dependencies/glad/glad.h"
#include "../Dependencies/GLFW/glfw3.h"
#include "MonitorManager.h"

MonitorManager::MonitorManager()
{
	this->updateMonitors();
	if (this->m_monitorCount > 0)
		this->m_activeMonitor = glfwGetPrimaryMonitor();
}

MonitorManager::MonitorManager (GLFWmonitorfun cb)
{
	this->updateMonitors();
	if (this->m_monitorCount > 0)
		this->m_activeMonitor = glfwGetPrimaryMonitor();
	glfwSetMonitorCallback(cb);
}

MonitorManager::~MonitorManager() {}

GLvoid MonitorManager::updateMonitors() { this->m_monitors = glfwGetMonitors(&this->m_monitorCount); }

const GLint MonitorManager::getMonitorCount() { return this->m_monitorCount; }

const GLFWvidmode* MonitorManager::getActiveMonitorVidMode() { return glfwGetVideoMode(this->m_activeMonitor); }

GLint MonitorManager::getActiveMonitorWidth() { return this->MonitorManager::getActiveMonitorVidMode()->width; }

GLint MonitorManager::getActiveMonitorHeight() { return this->MonitorManager::getActiveMonitorVidMode()->height; }

GLFWmonitor* MonitorManager::getActiveMonitor() { return this->m_activeMonitor; }

GLvoid MonitorManager::setActiveMonitor(GLint n)
{
	this->updateMonitors();
	GLint count = this->getMonitorCount();
	if (n < 0 || n >= count) return;
	else this->m_activeMonitor = this->m_monitors[n];
}
