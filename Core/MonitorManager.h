#pragma once
#include "../Dependencies/glad/glad.h"
#include "../Dependencies/GLFW/glfw3.h"

class MonitorManager
{
public:
	MonitorManager();
	MonitorManager(GLFWmonitorfun);
	~MonitorManager();
	GLFWmonitor* getActiveMonitor();
	const GLint getMonitorCount();
	const GLFWvidmode* getActiveMonitorVidMode();
	GLint getActiveMonitorWidth();
	GLint getActiveMonitorHeight();
	GLvoid setActiveMonitor(GLint);
private:
	GLvoid updateMonitors();
	GLint m_monitorCount;
	GLFWmonitor* m_activeMonitor;
	GLFWmonitor** m_monitors;
};