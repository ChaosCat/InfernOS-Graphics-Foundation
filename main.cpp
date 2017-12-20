#include <iostream>
#include "Apps/apps.h"

const char* ERR_UNRECOGNIZED_APP = "Error: Unrecognized app number ";
const char* MSG_PROMPT = "Please enter an OpenGL app ID: ";
unsigned int prompt();

int main()
{
	unsigned int ans = 0;
	while (!ans) ans = prompt();
	switch (ans)
	{
	case 1:
		HelloWindow::appMain();
		break;
	case 2:
		HelloTriangle::appMain();
		break;
	case 3:
		HelloShaders::appMain();
		break;
	case 4:
		HelloTextures::appMain();
		break;
	case 5:
		HelloTransformations::appMain();
		break;
	default:
		std::cout << ERR_UNRECOGNIZED_APP << ans << std::endl;
		return -1;
	}
	return 0;
}

unsigned int prompt()
{
	int ans = 0;
	std::cout << MSG_PROMPT << std::endl;
	std::cin >> ans;
	if (std::cin.fail()) return 0;
	return ans;
}