#pragma once

#include "Application.h"

#include "Win32RenderWindow.h"
#include "RendererDX11.h"

#include "ViewPerspective.h"
//Hieroglyph Includes
#include "Camera.h"
#include "Scene.h"
#include "GeometryActor.h"
#include "PointLight.h"

//STL Includes
#include <vector>
//
#include <stdio.h>
#include<stdlib.h>
#include <time.h>
//LJMU Framework Includes

#include "LJMUTwoDOverlay.h"
#include "LJMUMeshObj.h"
#include "LJMUShaderMaker.h"
#include "LJMUMaterialParams.h"
#include "Filesystem.h"
#include "LJMUH3Util.h"
#include "SkyBoxActor.h"
using namespace Glyph3;
using namespace std;

typedef std::shared_ptr<Glyph3::DrawExecutorDX11<Glyph3::BasicVertexDX11::Vertex>> BasicMeshPtr;
namespace LJMUDX
{
	struct LJMUDefaultResources
	{
		ResourcePtr		DefaultTexture;
		SpriteFontPtr	DefaultFont;
	};
	struct LJMUMenuSystem
	{
		int activeindex;
		int itemcount;
		LJMUMenuSystem()
		{
			activeindex = 0;
			itemcount = 0;
		}

		void increment()
		{
			activeindex++;
			if (activeindex >= itemcount)
				activeindex = 0;

		}
		void decrement()
		{
			activeindex--;
			if (activeindex < 0)
			{
				activeindex = itemcount - 1;
			}
		}
	};
	enum MenuItems
	{
		CAR_SPEED,
		CAR_SKIN,
		DAY_NIGHT,
		DISCO
	};
	enum TriggerMode
	{
		TRIGGER_INC,
		TRIGGER_DEC,
		TRIGGER_SELECT
	};
	//////////////////////////////////////
	//LJMULevelDemo.H
	//Class Application for a DirectX 11
	//Driven Application using the DirectX Toolkit
	//Hieroglyph 3 Rendering Engine and LUA.
	//
	//
	//AUTHORS:  DR Po Yang
	//			DR CHRIS CARTER
	//////////////////////////////////////
	class LJMULevelDemo : public Application //Inherit from the Hieroglyph Base Class
	{

	public:
		//------------CONSTRUCTORS------------------------------------------------
		LJMULevelDemo();	//Standard Empty Constructor which builds the object

	public:
		//------------INHERITED METHODS-------------------------------------------
		virtual void Initialize();					//Initialise the DirectX11 Scene
		virtual void Update();						//Update the DirectX Scene
		virtual void Shutdown();					//Shutdown the DirectX11 Scene

		virtual bool ConfigureEngineComponents();	//Initialise Hieroglyph and DirectX TK Modules
		virtual void ShutdownEngineComponents();	//Destroy Hieroglyph and DirectX TK Modules

		virtual void TakeScreenShot();				//Allow a screenshot to be generated

		virtual bool HandleEvent(EventPtr pEvent);	//Handle an I/O Event
		virtual std::wstring GetName();				//Get the Name of this App Instance



		//------------CUSTOM METHODS-----------------------------------------------
		void inputAssemblyStage();					//Stage to setup our VB and IB Info
		
		//std::wstring outputFPSInfo();				//Convert the timer's Frames Per Second to a formatted string
		void inputAssembly2D();

		void Update2D(float pptpf, float ptotal);


		//void setDayNight();
		void MakeTrees(Vector3f coords);
		void updateTrees();
		void updateLaps();
		void updateCrowd();
		void createPlatform();
		void createCrowd();
		void createPerson();

		void loadResources();

		void updateTrigger(TriggerMode pmode);
		void updateUI();

		MaterialPtr CreateLitTexturedMaterial(wstring diffuse_map);
		void setupLightSources();
		void setLights2Material(MaterialPtr material);

		void updateLighting();

	    protected:
			BasicMeshPtr generateOBJMesh(std::wstring pmesh, Vector4f pcolour = Vector4f(0, 0, 0, 1));
		//-------------CLASS MEMBERS-----------------------------------------------
		RendererDX11*			_obj_renderer11;	//Pointer to our DirectX 11 Device
		Win32RenderWindow*		_obj_window;		//Pointer to our Windows-Based Window

		int						_swap_index;		//Index of our Swap Chain 
		ResourcePtr				_tgt_render;		//Pointer to the GPU Render Target for Colour
		ResourcePtr				_tgt_depth;			//Pointer to the GPU Render Target for Depth
		//---
		FileSystem              _fs;
		//--------------HIEROGLYPH OBJECTS-----------------------------------------
		ViewPerspective*			_render_view;	//3D Output View - DirectX 11 Accelerated
		//LJMUTextOverlay*            _render_text;	//2D Output View - DirectX 11 Accelerated
		Camera*						_obj_camera;	//Camera Object
		//car mesh 
	
		std::vector<Actor*> _list_models;
		std::vector<BasicMeshPtr> _list_meshes;
		std::vector<LJMUMaterialParams*> _list_params;
	

		std::vector<GeometryActor*> _objects;
		std::vector<GeometryActor*> _trees;
		std::vector<GeometryActor*> _people;
		std::vector<GeometryActor*> _cylinds;
		
		//skybox
		SkyboxActor*						_skybox;

		//pathing variables
		std::vector<Vector3f> checkpoints;
		std::vector<Vector3f> duckRoute;
		std::vector<Vector3f> treepoints;
		int idx, nextidx;
		int indx, nextindx;

		//car variables
		Vector3f car_dir, car_refdir, car_targetdir;
		float linear_speed, angular_speed;
		float prev_dist;
		int state;
		//duck variables
		Vector3f duck_dir, duck_refdir, duck_targetdir;
		float swim_speed, turn_speed;
		float prev_dist_duck;
		int duckState;


		const float PI = 3.14159265f; 
		const float DEG_TO_RAD = PI / 180.0f;

		//crowd variables
		int targetHeight = 15;
		int waveIterator = 0;

		//
		ResourcePtr					_sprite_tex;
		SpriteFontPtr				_sprite_font;
		LJMUTwoDOverlay*			_obj_twod_rdr;
		//textures
		ResourcePtr _texture;
		MaterialPtr _texture2;
		//
		LJMUResourceHelper				_local_content;
		LJMUResourceHelper				_local_content2;
		//hud
		std::vector<LJMUSpriteEntry>	_sprites;
		std::vector<LJMUTextEntry>      _text;
		std::vector<LJMUTextEntry>      _menutext;
		std::vector<std::wstring> _menuitems;
		LJMUMenuSystem _menu_sys;
		bool _light_on;
		bool disco_mode;
		int skinVal = 1;
		int displayCar = 3;
		

		int _lapNo = 0;
		bool _atStart = false;


//		BasicMeshPtr generateOBJMesh(std::wstring pmeshname, Vector4f pmeshcolour);
		MaterialPtr createSimpleTexturedMaterial(wstring diffuse_map);

		//lighting
		Vector4f DirectionalLightColour;
		Vector4f DirectionalLightDirection;

		Vector4f SpotLightDirection;
		Vector4f SpotLightColour;
		Vector4f SpotLightPosition;
		Vector4f SpotLightRange;
		Vector4f SpotLightFocus;

		Vector4f PointLightPosition;
		Vector4f PointLightColour;
		Vector4f PointLightRange;


		Actor* _carActor;
		MaterialPtr floor_material;
		MaterialPtr car_material2;
		MaterialPtr car_material;
	};

}
