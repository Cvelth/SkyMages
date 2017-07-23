#include "GameWindow.hpp"
#include "GameLogicEngine\GameCamera.hpp"
#include "Exceptions\WindowExceptions.hpp"

#ifdef OPENGL_ENGINE_USED
#include "Graphics\OpenGLGraphicsEngine\OpenGLGraphicsEngine.hpp"
#endif
#ifdef VULKAN_ENGINE_USED
#include "Graphics\VulkanGraphicsEngine\VulkanGraphicsEngine.hpp"
#endif

void GameWindow::initialize() {
	//Does nothing.
}

void GameWindow::clean() {
	//Does nothing.
}

GameWindow::GameWindow(char* title, size_t width, size_t height, bool isFullscreen)
		: isMapInserted(false){
	
#ifdef OPENGL_ENGINE_USED
	m_graphics = new OpenGLGraphicsEngine();
#endif
#ifdef VULKAN_ENGINE_USED
	m_graphics = new VulkanGraphicsEngine();
#endif

	initialize();
	m_graphics->createWindow(title, width, height, isFullscreen);
	m_graphics->initialize();
}

void GameWindow::insertMap(GameMap* map) {
	m_camera = new GameCamera(map, float(m_graphics->width()) / m_graphics->height());
	isMapInserted = true;
}

GameWindow::~GameWindow() {
	if (isMapInserted && m_camera) delete m_camera;

	m_graphics->clean();
	m_graphics->destroyWindow();
	clean();
}

int GameWindow::loop() {
	m_graphics->initializeMapRendering(m_camera->map());
	//m_graphics->initializeRenderProcess();
	while (!m_graphics->isWindowClosed()) {
		m_graphics->renderMap(m_camera);
		//m_graphics->renderProcess();
		m_graphics->update();
	}
	m_graphics->cleanMapRendering(m_camera->map());
	//m_graphics->clearRenderProcess();
	return 0;
} 