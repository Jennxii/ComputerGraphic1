#include "app.hpp"
#include "cg\cg_window.hpp"

int main()
{
	if (!glfwInit())
		return -1;

	std::shared_ptr<cg::CgWindow> window = cg::CgWindow::create<App>("the end is near", true, 800, 800, 20, 30, 4, 5, 4);

	if (window == nullptr)
	{
		glfwTerminate();
		return -1;
	}

	while (1)
	{
		if (window->shouldClose())
		{
			window.reset();
			break;
		}
		else
		{
			window->onNewFrame();
		}

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}