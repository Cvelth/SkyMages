#pragma once
#include "MGL_AdditionalStructures.hpp"

class MGLWindow;
class GameCamera;
class GameControllerInterface;
class RenderQueue;
class AbstractGameObject;

class MyGraphicsLibraryEngine {
private:
	MGLWindow *m_window;
	GameCamera *m_camera;
	RenderQueue *m_queue;

	MapProgram m_map_program;
	QueueProgram m_queue_program;
public:
	MyGraphicsLibraryEngine();
	~MyGraphicsLibraryEngine();

	virtual void initialize();
	virtual void clean();

	virtual void initializeWindow(const char* title, size_t width, size_t height, bool isFullscreen);
	virtual void destroyWindow();
	virtual bool isWindowClosed();

	virtual size_t width() const;
	virtual size_t height() const;

	virtual void clearWindow();
	virtual void update();
	virtual void pollEvents();
	void waitEvents();

	virtual void insertController(GameControllerInterface* controller);
	virtual void initializeQueue();
	virtual void addToRenderQueue(AbstractGameObject* go);

	virtual void recalculateCamera();
	virtual void recalculateProjection();
	virtual void recalculateInstancing();

	virtual void initializeMapRendering(GameCamera* camera);
	virtual void renderMap();
	virtual void cleanMapRendering();

	virtual void initializeQueueRendering();
	virtual void renderQueue();
	virtual void cleanQueueRendering();
};