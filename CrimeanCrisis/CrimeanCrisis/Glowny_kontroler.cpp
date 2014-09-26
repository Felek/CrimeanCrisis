#include "Glowny_kontroler.h"

Glowny_kontroler::Glowny_kontroler()
{
	_sound = new Sound();
	_sound->LoadSounds();
	_objects = new std::list<GraphicObject>();
}


Glowny_kontroler::~Glowny_kontroler()
{
	delete _sound;
	_objects->clear();
	delete _objects;
}

void Glowny_kontroler::Start() 
{
	GraphicObject obj;	
	obj.loadOBJ("models/megahuge_t1.obj", "grafiki/tex1.bmp");
	//system("SERVER.exe");
	RendController::init(_objects);
	obj.SetDisplay();
	_objects->push_back(obj);
		

	//static const GLfloat g_vertex_buffer_data[] = {
	//	-100.0f, -100.0f, 10.0f,
	//	100.0f, 100.0f, 10.0f,
	//	-100.0f, 100.0f, 10.0f,
	//	-100.0f, -100.0f, 10.0f,
	//	100.0f, 100.0f, 10.0f,
	//	100.0f, -100.0f, 10.0f,
	//};
	//
	//GLuint vertexbuffer;
	//glGenBuffers(1, &vertexbuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	//glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle
	//glDisableVertexAttribArray(0);
	glutMainLoop();


	//bool the_end = false;
	//GameServer server;
	//Klient klient(_objects);
	////poniższy kod odpowiada za uruchomienie serwera i klienta
	////std::thread first(&GameServer::Start, server);
	////std::thread second(&Klient::Start, klient);
	////first.join();
	////second.join();
}