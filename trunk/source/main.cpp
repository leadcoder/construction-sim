/****************************************************************************
* This file is part of GASS.                                                *
* See http://code.google.com/p/gass/										*
*                                                                           *
* Copyright (c) 2008-2009 GASS team. See Contributors.txt for details.      *
*                                                                           *
* GASS is free software: you can redistribute it and/or modify              *
* it under the terms of the GNU Lesser General Public License as published  *
* by the Free Software Foundation, either version 3 of the License, or      *
* (at your option) any later version.                                       *
*                                                                           *
* GASS is distributed in the hope that it will be useful,                   *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU Lesser General Public License for more details.                       *
*                                                                           *
* You should have received a copy of the GNU Lesser General Public License  *
* along with GASS. If not, see <http://www.gnu.org/licenses/>.              *
*****************************************************************************/

#include "Sim/GASS.h"
#include "Plugins/Game/GameMessages.h"
#include "Sim/GASSSceneObjectRef.h"
#include "Sim/GASSBaseSceneManager.h"
#include "Core/Utils/GASSHeightmap.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <MyGUI.h>
#include "MyGUI_LastHeader.h"
#ifdef WIN32
#include <conio.h>
#endif

#ifndef WIN32
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

int _getch( ) {
	struct termios oldt,
		newt;
	int            ch;
	tcgetattr( STDIN_FILENO, &oldt );
	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &newt );
	ch = getchar();
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
	return ch;
}

#endif
namespace GASS
{
	class CESim;

	class GUI : public SHARE_CLASS<GUI>, public IMessageListener
	{

	private:
		MyGUI::Window* m_MainMenuWin;
		CESim* m_CESim;
	public:
		GUI(CESim* sim): m_MainMenuWin(0), m_CESim(sim){}
		virtual ~GUI(){}
		void Init()
		{
			SimEngine::Get().GetSimSystemManager()->RegisterForMessage(REG_TMESS(GUI::OnGUILoaded,GUILoadedEvent,0));
		}

		void OnGUILoaded(GUILoadedEventPtr message)
		{
			const MyGUI::IntSize& view = MyGUI::RenderManager::getInstance().getViewSize();
			const MyGUI::IntSize size(450, 450);
			m_MainMenuWin = MyGUI::Gui::getInstance().createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord((800 - size.width) / 2, (600 - size.height) / 2, size.width, size.height), MyGUI::Align::Default, "Main");
			m_MainMenuWin->setMinSize(150, 150);
			m_MainMenuWin->setCaption("ScrollView demo");
			//MyGUI::ScrollView* scroll_view = window->createWidget<MyGUI::ScrollView>("ScrollView", MyGUI::IntCoord(2, 2, window->getClientCoord().width - 2, window->getClientCoord().height - 2), MyGUI::Align::Stretch);
			//const MyGUI::IntSize size(450, 450);
			//MyGUI::TextBox* text = window->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(2, 2, window->getClientCoord().width - 2, window->getClientCoord().height - 2), MyGUI::Align::Default);
			//text->setCaption("TextBox");
			const MyGUI::IntSize bsize(150, 20);
			MyGUI::Button* button = m_MainMenuWin->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(12, 12, 12 + bsize.width, 12 + bsize.height), MyGUI::Align::Default);
			button->setCaption("START!");
			//button->setTextAlign(MyGUI::Align::Center);
			button->eventMouseButtonClick += MyGUI::newDelegate(this, &GUI::OnStart);
			//button->eventMouseButtonClick += MyGUI::newDelegate(this, &State::notifyMouseButtonClick);

			//button->setNeedToolTip(true);
			//button->eventToolTip += MyGUI::newDelegate(this, &WidgetsWindow::notifyToolTip);

		}

		void GUI::OnStart(MyGUI::Widget* _sender)
		{
			m_MainMenuWin->setVisible(false);
		}
	};
	typedef SPTR<GUI> GUIPtr;

	class CESim
	{
	public:

		CESim(){}
		virtual ~CESim(){}

		void Init()
		{
			GASS::SimEngine* engine = new GASS::SimEngine();
			GASS::GUIPtr gui(new GASS::GUI(this));
			gui->Init();

			engine->Init(GASS::FilePath("GASS.xml"));
			GASS::GraphicsSystemPtr gfx_sys = engine->GetSimSystemManager()->GetFirstSystemByClass<GASS::IGraphicsSystem>();
			GASS::RenderWindowPtr win = gfx_sys->CreateRenderWindow("MainWindow",800,600);
			win->CreateViewport("MainViewport", 0, 0, 1, 1);
			GASS::InputSystemPtr input_system = GASS::SimEngine::Get().GetSimSystemManager()->GetFirstSystemByClass<GASS::IInputSystem>();
			input_system->SetMainWindowHandle(win->GetHWND());
			GASS::ScenePtr scene = CreateDemoTerrain();
			
			GASS::SceneObjectPtr vehicle = scene->LoadObjectFromTemplate("Liebherr",scene->GetRootSceneObject());
			vehicle->SendImmediateRequest(GASS::PositionRequestPtr(new GASS::PositionRequest(GASS::Vec3(0,4,-13))));

			//create free camera and set start pos
			GASS::SceneObjectPtr free_obj = scene->LoadObjectFromTemplate("FreeCameraObject",scene->GetRootSceneObject());
			if(free_obj)
			{
				free_obj->SendImmediateRequest(GASS::PositionRequestPtr(new GASS::PositionRequest(GASS::Vec3(0,2,0))));
				GASS::SystemMessagePtr camera_msg(new GASS::ChangeCameraRequest(free_obj->GetFirstComponentByClass<GASS::ICameraComponent>()));
				GASS::SimEngine::GetPtr()->GetSimSystemManager()->PostMessage(camera_msg);
			}
		}

		GASS::ScenePtr CreateDemoTerrain() 
		{
			GASS::ScenePtr scene(GASS::SimEngine::Get().CreateScene("NewScene"));
			{
				GASS::SceneObjectTemplatePtr plane_template (new GASS::SceneObjectTemplate);
				plane_template->SetName("PlaneObject");
				plane_template->AddBaseSceneComponent("LocationComponent");
				plane_template->AddBaseSceneComponent("ManualMeshComponent");
				plane_template->AddBaseSceneComponent("PhysicsPlaneGeometryComponent");
				GASS::ComponentPtr plane_comp  = plane_template->AddBaseSceneComponent("PlaneGeometryComponent");
				plane_comp->SetPropertyByType("Size",GASS::Vec2(100,100));
				GASS::SimEngine::Get().GetSceneObjectTemplateManager()->AddTemplate(plane_template);
			}
			{
				GASS::SceneObjectTemplatePtr light_template (new GASS::SceneObjectTemplate);
				light_template->SetName("LightObject");
				light_template->AddBaseSceneComponent("LocationComponent");
				GASS::BaseSceneComponentPtr light_comp = light_template->AddBaseSceneComponent("LightComponent");
				light_comp->SetPropertyByString("DiffuseColor","0.5 0.5 0.5");
				light_comp->SetPropertyByString("AmbientColor","0.5 0.5 0.5");
				GASS::SimEngine::Get().GetSceneObjectTemplateManager()->AddTemplate(light_template);
			}

			{
				GASS::SceneObjectTemplatePtr box_template (new GASS::SceneObjectTemplate);
				box_template->SetName("BoxObject");
				box_template->AddBaseSceneComponent("LocationComponent");
				GASS::BaseSceneComponentPtr mmc  = box_template->AddBaseSceneComponent("ManualMeshComponent");
				mmc->SetPropertyByType("CastShadow",true);
				box_template->AddBaseSceneComponent("PhysicsBoxGeometryComponent");
				box_template->AddBaseSceneComponent("PhysicsBodyComponent");
				GASS::BaseSceneComponentPtr box_comp  = box_template->AddBaseSceneComponent("BoxGeometryComponent");
				box_comp->SetPropertyByType("Size",GASS::Vec3(1,1,1));
				box_comp->SetPropertyByType("Lines",false);

				GASS::SimEngine::Get().GetSceneObjectTemplateManager()->AddTemplate(box_template);
			}

			{
				GASS::SceneObjectTemplatePtr mesh_template (new GASS::SceneObjectTemplate);
				mesh_template->SetName("MeshObject");
				mesh_template->AddBaseSceneComponent("LocationComponent");
				mesh_template->AddBaseSceneComponent("MeshComponent");
				mesh_template->AddBaseSceneComponent("PhysicsBoxGeometryComponent");
				mesh_template->AddBaseSceneComponent("PhysicsBodyComponent");
				GASS::SimEngine::Get().GetSceneObjectTemplateManager()->AddTemplate(mesh_template);
			}

			GASS::SceneObjectPtr terrain_obj = scene->LoadObjectFromTemplate("PlaneObject",scene->GetRootSceneObject());
			GASS::SceneObjectPtr light_obj = scene->LoadObjectFromTemplate("LightObject",scene->GetRootSceneObject());
			light_obj->SendImmediateRequest(GASS::RotationRequestPtr(new GASS::RotationRequest(GASS::Vec3(40,32,0))));
			return scene;
		}

		void Run()
		{

			while(true)
			{
				SimEngine::Get().Update();
				static bool key_down=false;
				if(GetAsyncKeyState(VK_SPACE))
				{
					/*if(!key_down)
					{
					key_down = true;
					GASS::Vec3 pos = free_obj->GetFirstComponentByClass<GASS::ILocationComponent>()->GetPosition();
					GASS::Quaternion rot = free_obj->GetFirstComponentByClass<GASS::ILocationComponent>()->GetRotation();
					GASS::Mat4 rot_mat;
					rot_mat.Identity();
					rot.ToRotationMatrix(rot_mat);
					GASS::Vec3 vel = rot_mat.GetViewDirVector()*-2500;
					GASS::Vec3 torq(0,0,2000);
					torq = rot_mat * torq;
					GASS::SceneObjectPtr box_obj = scene->LoadObjectFromTemplate("BoxObject",scene->GetRootSceneObject());
					box_obj->SendImmediateRequest(GASS::PositionRequestPtr(new GASS::PositionRequest(pos)));
					box_obj->SendImmediateRequest(GASS::RotationRequestPtr(new GASS::RotationRequest(rot)));
					box_obj->SendImmediateRequest(GASS::PhysicsBodyAddForceRequestPtr(new GASS::PhysicsBodyAddForceRequest(vel)));
					box_obj->SendImmediateRequest(GASS::PhysicsBodyAddTorqueRequestPtr(new GASS::PhysicsBodyAddTorqueRequest(torq)));
					}*/
				}
				else
					key_down = false;
			}
		}
	};
}

int run(int argc, char* argv[])
{
	GASS::CESim ce_sim;
	ce_sim.Init();
	ce_sim.Run();

	/*GASS::SimEngine* m_Engine = new GASS::SimEngine();
	GASS::GUIPtr gui(new GASS::GUI());
	gui->Init();

	m_Engine->Init(GASS::FilePath("GASS.xml"));
	GASS::GraphicsSystemPtr gfx_sys = m_Engine->GetSimSystemManager()->GetFirstSystemByClass<GASS::IGraphicsSystem>();
	GASS::RenderWindowPtr win = gfx_sys->CreateRenderWindow("MainWindow",800,600);
	win->CreateViewport("MainViewport", 0, 0, 1, 1);
	GASS::InputSystemPtr input_system = GASS::SimEngine::Get().GetSimSystemManager()->GetFirstSystemByClass<GASS::IInputSystem>();
	input_system->SetMainWindowHandle(win->GetHWND());*/
	/*GASS::ScenePtr scene(m_Engine->CreateScene("NewScene"));

	{
	GASS::SceneObjectTemplatePtr plane_template (new GASS::SceneObjectTemplate);
	plane_template->SetName("PlaneObject");
	plane_template->AddBaseSceneComponent("LocationComponent");
	plane_template->AddBaseSceneComponent("ManualMeshComponent");
	plane_template->AddBaseSceneComponent("PhysicsPlaneGeometryComponent");
	GASS::ComponentPtr plane_comp  = plane_template->AddBaseSceneComponent("PlaneGeometryComponent");
	plane_comp->SetPropertyByType("Size",GASS::Vec2(100,100));
	GASS::SimEngine::Get().GetSceneObjectTemplateManager()->AddTemplate(plane_template);
	}
	{
	GASS::SceneObjectTemplatePtr light_template (new GASS::SceneObjectTemplate);
	light_template->SetName("LightObject");
	light_template->AddBaseSceneComponent("LocationComponent");
	GASS::BaseSceneComponentPtr light_comp = light_template->AddBaseSceneComponent("LightComponent");
	light_comp->SetPropertyByString("DiffuseColor","0.5 0.5 0.5");
	light_comp->SetPropertyByString("AmbientColor","0.5 0.5 0.5");
	GASS::SimEngine::Get().GetSceneObjectTemplateManager()->AddTemplate(light_template);
	}

	{
	GASS::SceneObjectTemplatePtr box_template (new GASS::SceneObjectTemplate);
	box_template->SetName("BoxObject");
	box_template->AddBaseSceneComponent("LocationComponent");
	GASS::BaseSceneComponentPtr mmc  = box_template->AddBaseSceneComponent("ManualMeshComponent");
	mmc->SetPropertyByType("CastShadow",true);
	box_template->AddBaseSceneComponent("PhysicsBoxGeometryComponent");
	box_template->AddBaseSceneComponent("PhysicsBodyComponent");
	GASS::BaseSceneComponentPtr box_comp  = box_template->AddBaseSceneComponent("BoxGeometryComponent");
	box_comp->SetPropertyByType("Size",GASS::Vec3(1,1,1));
	box_comp->SetPropertyByType("Lines",false);

	GASS::SimEngine::Get().GetSceneObjectTemplateManager()->AddTemplate(box_template);
	}

	{
	GASS::SceneObjectTemplatePtr mesh_template (new GASS::SceneObjectTemplate);
	mesh_template->SetName("MeshObject");
	mesh_template->AddBaseSceneComponent("LocationComponent");
	mesh_template->AddBaseSceneComponent("MeshComponent");
	mesh_template->AddBaseSceneComponent("PhysicsBoxGeometryComponent");
	mesh_template->AddBaseSceneComponent("PhysicsBodyComponent");
	GASS::SimEngine::Get().GetSceneObjectTemplateManager()->AddTemplate(mesh_template);
	}

	GASS::SceneObjectPtr terrain_obj = scene->LoadObjectFromTemplate("PlaneObject",scene->GetRootSceneObject());
	GASS::SceneObjectPtr light_obj = scene->LoadObjectFromTemplate("LightObject",scene->GetRootSceneObject());
	light_obj->SendImmediateRequest(GASS::RotationRequestPtr(new GASS::RotationRequest(GASS::Vec3(40,32,0))));

	GASS::SceneObjectPtr vehicle = scene->LoadObjectFromTemplate("Liebherr",scene->GetRootSceneObject());
	vehicle->SendImmediateRequest(GASS::PositionRequestPtr(new GASS::PositionRequest(GASS::Vec3(0,4,-13))));

	//create free camera and set start pos
	GASS::SceneObjectPtr free_obj = scene->LoadObjectFromTemplate("FreeCameraObject",scene->GetRootSceneObject());
	if(free_obj)
	{
	free_obj->SendImmediateRequest(GASS::PositionRequestPtr(new GASS::PositionRequest(GASS::Vec3(0,2,0))));
	GASS::SystemMessagePtr camera_msg(new GASS::ChangeCameraRequest(free_obj->GetFirstComponentByClass<GASS::ICameraComponent>()));
	m_Engine->GetSimSystemManager()->PostMessage(camera_msg);
	}



	while(true)
	{
	m_Engine->Update();
	static bool key_down=false;
	if(GetAsyncKeyState(VK_SPACE))
	{
	if(!key_down)
	{


	key_down = true;
	GASS::Vec3 pos = free_obj->GetFirstComponentByClass<GASS::ILocationComponent>()->GetPosition();
	GASS::Quaternion rot = free_obj->GetFirstComponentByClass<GASS::ILocationComponent>()->GetRotation();
	GASS::Mat4 rot_mat;
	rot_mat.Identity();
	rot.ToRotationMatrix(rot_mat);
	GASS::Vec3 vel = rot_mat.GetViewDirVector()*-2500;
	GASS::Vec3 torq(0,0,2000);
	torq = rot_mat * torq;
	GASS::SceneObjectPtr box_obj = scene->LoadObjectFromTemplate("BoxObject",scene->GetRootSceneObject());
	box_obj->SendImmediateRequest(GASS::PositionRequestPtr(new GASS::PositionRequest(pos)));
	box_obj->SendImmediateRequest(GASS::RotationRequestPtr(new GASS::RotationRequest(rot)));
	box_obj->SendImmediateRequest(GASS::PhysicsBodyAddForceRequestPtr(new GASS::PhysicsBodyAddForceRequest(vel)));
	box_obj->SendImmediateRequest(GASS::PhysicsBodyAddTorqueRequestPtr(new GASS::PhysicsBodyAddTorqueRequest(torq)));
	}
	}
	else
	key_down = false;
	*/
return 0;
}



int main(int argc, char* argv[])
{
	int ret =0;
	try
	{
		ret = run(argc,argv);
	}
	catch(std::exception& e) 
	{
		std::cout << "Exception:" << e.what() << std::endl;
		getch();
	}
	return ret;
}