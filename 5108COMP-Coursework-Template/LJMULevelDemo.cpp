//------------Include the Application Header File----------------------------
#include "LJMULevelDemo.h"

//------------DX TK AND STD/STL Includes-------------------------------------
#include <sstream>

//------------Include Hieroglyph Engine Files--------------------------------

//Include the Logging System
#include "Log.h"

//Include the Event System
#include "EventManager.h"
#include "EvtFrameStart.h"
#include "EvtChar.h"
#include "EvtKeyUp.h"
#include "EvtKeyDown.h"
#include "ScriptManager.h"

//Include the DirectX Rendering Components
#include "PipelineManagerDX11.h"
#include "BlendStateConfigDX11.h"
#include "BufferConfigDX11.h"
#include "DepthStencilStateConfigDX11.h"
#include "RasterizerStateConfigDX11.h"
#include "SwapChainConfigDX11.h"
#include "Texture2dConfigDX11.h"
#include "MaterialGeneratorDX11.h"

#include "FirstPersonCamera.h"
//
#include <FileSystem.h>
#include "LJMUMeshObj.h"
#include "SamplerStateConfigDX11.h"
#include <cmath>
//Add a Using Directive to avoid typing Glyph3 for basic constructs
using namespace Glyph3;
//Include our own application Namespace
using namespace LJMUDX;

LJMULevelDemo AppInstance; 

//---------CONSTRUCTORS-------------------------------------------------------

///////////////////////////////////////
//
///////////////////////////////////////
LJMULevelDemo::LJMULevelDemo():
//_render_text(nullptr),
_render_view(nullptr),
_obj_camera(nullptr),
_obj_renderer11(nullptr),
_obj_window(nullptr),
_swap_index(0),
_tgt_depth(nullptr),
_tgt_render(nullptr)
{
	
}

//---------METHODS------------------------------------------------------------

//////////////////////////////////////
// Get the Window Name of this Application
//////////////////////////////////////
std::wstring LJMULevelDemo::GetName()
{
	return(std::wstring(L"5108COMP: Coursework Template"));
}

/////////////////////////////////////
// Assemble our Input Layouts for this
// Stage of the Pipeline.
/////////////////////////////////////
void LJMULevelDemo::inputAssemblyStage()
{		
	//srand(time(0));
	float floorwidth = 256.0f;
	float floorheight = 256.0f;
	//-----SETUP OUR GEOMETRY FOR THIS SCENE-----------------------------------------
	this->_texture = RendererDX11::Get()->LoadTexture(L"racetrack2.png");
	Vector4f floor_base_colour = Vector4f(1, 1, 1, 1);
	BasicMeshPtr floor_geometry = this->generateOBJMesh(L"cube.obj", floor_base_colour);
	Vector4f car_base_colour = Vector4f(1, 1, 1, 1);
	BasicMeshPtr car_geometry = this->generateOBJMesh(L"Lamborghini_Aventador_triangles.obj", car_base_colour);

	BasicMeshPtr tpond = this->generateOBJMesh(L"pond.obj", Vector4f(0, 0, 1, 1));
	BasicMeshPtr tduck = this->generateOBJMesh(L"duck.obj", Vector4f(0.5, 0.5, 0, 1));
	BasicMeshPtr tflag = this->generateOBJMesh(L"flag.obj", Vector4f(0.5, 0.5, 0.5, 1));

	setupLightSources();
	float floor_ambient_constant = 0.02f;
	float floor_diffuse_constant = 2.0f;
	float floor_specular_constant = 0.2f;
	float floor_shininess_constant = 100.0f;
	Vector4f floor_emisivity = Vector4f(0.4f, 0.4f, 0.4f, 1.0f);

	floor_material = CreateLitTexturedMaterial(wstring(L"racetrack.png"));
	floor_material->Parameters.SetVectorParameter(L"SurfaceConstants", Vector4f(floor_ambient_constant, floor_diffuse_constant, 
												 floor_specular_constant, floor_shininess_constant));
	floor_material->Parameters.SetVectorParameter(L"SurfaceEmissiveColour", floor_emisivity);
	setLights2Material(floor_material);

	GeometryActor* box = new GeometryActor();
	box->SetColor(Vector4f(1.0f, 1.0f, 1.0f, 1.0f));
	box->DrawBox(Vector3f(0.0f, 0.0f, 0.0f), Vector3f(floorwidth *-1, 0.1f, floorheight *-1));
	box->GetBody()->SetMaterial(floor_material);
	box->GetNode()->Position() = Vector3f(0.0f, 0.0f, 0.0f);

	this->_objects.push_back(box);
	this->m_pScene->AddActor(box);


	//car mesh

	float car_ambient_constant = 0.01f;
	float car_diffuse_constant = 2.0f;
	float car_specular_constant = 0.01f;
	float car_shininess_constant = 200.0f;
	Vector4f car_emmisivity = Vector4f(1.0f,1.0f, 1.0f, 1.0f);

	car_material = CreateLitTexturedMaterial(wstring(L"Lamborginhi_Aventador_diffuse.png"));
	car_material->Parameters.SetVectorParameter(L"SurfaceConstants", Vector4f(car_ambient_constant, car_diffuse_constant, car_specular_constant, car_shininess_constant));
	car_material->Parameters.SetVectorParameter(L"SurfaceEmissiveColour", car_emmisivity);
	setLights2Material(car_material);
	 car_material2 = CreateLitTexturedMaterial(wstring(L"Lamborginhi_Aventador_diffuse2.png"));
	car_material2->Parameters.SetVectorParameter(L"SurfaceConstants", Vector4f(car_ambient_constant, car_diffuse_constant, car_specular_constant, car_shininess_constant));
	car_material2->Parameters.SetVectorParameter(L"SurfaceEmissiveColour", car_emmisivity);
	setLights2Material(car_material2);
	
		_carActor = new Actor();
		_carActor->GetBody()->SetGeometry(car_geometry);
		_carActor->GetBody()->SetMaterial(car_material);
		_carActor->GetBody()->Position() = Vector3f(0.0f, 0.0f, 0.0f);
		_carActor->GetBody()->Scale() = Vector3f(0.05f, 0.05f, 0.05f);
		this->_list_models.push_back(_carActor);

		Actor* tmeshactor2 = new Actor();
		MaterialPtr tmtl2 = LJMUShaderMaker::MakeActorPhong(*this->_obj_renderer11);
		tmtl2->Parameters.SetShaderResourceParameter(L"ColorTexture", this->_local_content.getImage(1));
		tmeshactor2->GetBody()->SetGeometry(tpond);
		tmeshactor2->GetBody()->SetMaterial(tmtl2);
		tmeshactor2->GetBody()->Position() = Vector3f(90.0f, 0, -180.0f);
		tmeshactor2->GetBody()->Scale() = Vector3f(1, 0.5, 1);
		this->_list_models.push_back(tmeshactor2);

		this->m_pScene->AddActor(_carActor);
		this->m_pScene->AddActor(tmeshactor2);


		Actor* duck = new Actor();
		MaterialPtr tmtl3 = LJMUShaderMaker::MakeActorPhong(*this->_obj_renderer11);
		tmtl3->Parameters.SetShaderResourceParameter(L"ColorTexture", this->_local_content.getImage(1));
		duck->GetBody()->SetGeometry(tduck);
		duck->GetBody()->SetMaterial(tmtl3);
		duck->GetBody()->Position() = Vector3f(0, 0, 0);
		duck->GetBody()->Scale() = Vector3f(0.5, 0.5, 0.5);
		this->_list_models.push_back(duck);
		this->m_pScene->AddActor(duck);


		Actor* flag = new Actor();
		MaterialPtr tmtl4 = LJMUShaderMaker::MakeActorPhong(*this->_obj_renderer11);
		tmtl3->Parameters.SetShaderResourceParameter(L"ColorTexture", this->_local_content.getImage(1));
		flag->GetBody()->SetGeometry(tflag);
		flag->GetBody()->SetMaterial(tmtl4);
		flag->GetBody()->Scale() = Vector3f(1.5, 1, 1);
		Matrix3f trotation;
		trotation.RotationY(45);
		flag->GetNode()->Rotation() *= trotation;
		flag->GetBody()->Position() = Vector3f(110, 0, -110);
		this->_list_models.push_back(flag);
		this->m_pScene->AddActor(flag);

		Actor* flag2 = new Actor();
		flag2->GetBody()->SetGeometry(tflag);
		flag2->GetBody()->SetMaterial(tmtl4);
		flag2->GetBody()->Scale() = Vector3f(1.25, 1, 0.75);
		flag2->GetBody()->Position() = Vector3f(-210, 0, -140);
		this->_list_models.push_back(flag2);
		this->m_pScene->AddActor(flag2);

	//tree
	GeometryActor* bark = new GeometryActor();
	GeometryActor* leaves = new GeometryActor();
	bark->SetColor(Vector4f(0.8, 0.5, 0.1, 1.0f));
	bark->DrawBox(Vector3f(50, 5.0f, 50), Vector3f(5.5f, 25.5f, 5.5f));
	bark->UseSolidMaterial();
	leaves->SetColor(Vector4f(0.0f, 1.0f, 0.0f, 1.0f));

	leaves->DrawBox(Vector3f(0, 0.0f, 0), Vector3f(10, 10.5f, 10));
	leaves->UseSolidMaterial();
	this->_objects.push_back(bark);
	this->m_pScene->AddActor(bark);
	this->_objects.push_back(leaves);
	this->m_pScene->AddActor(leaves);

	//the sun!
	GeometryActor* sun = new GeometryActor();
	sun->SetColor(Vector4f(0.8, 0.8, 0.0, 1.0f));
	sun->DrawSphere(Vector3f(0.0f, 300.0f, 0.0f), 20.0f, 20, 20);
	sun->UseSolidMaterial();
	this->_objects.push_back(sun);
	this->m_pScene->AddActor(sun);

	this->createPlatform();



	//car route
		checkpoints.push_back(Vector3f(-180, 0, -10));
		checkpoints.push_back(Vector3f(-205, 0, -170));
		checkpoints.push_back(Vector3f(-184, 0, -200));
		checkpoints.push_back(Vector3f(-140, 0, -215));
		checkpoints.push_back(Vector3f(-120, 0, -170));
		checkpoints.push_back(Vector3f(-155, 0, -100));
		checkpoints.push_back(Vector3f(-135, 0, -10));
		checkpoints.push_back(Vector3f(90, 0, 150));
		checkpoints.push_back(Vector3f(160, 0, 130));
		checkpoints.push_back(Vector3f(185, 0, 70));
		checkpoints.push_back(Vector3f(120, 0, 10));
		checkpoints.push_back(Vector3f(0, 0, 25));
		checkpoints.push_back(Vector3f(-85, 0, -10));
		checkpoints.push_back(Vector3f(-120, 0, -75));
		checkpoints.push_back(Vector3f(-85, 0, -135));
		checkpoints.push_back(Vector3f(-20, 0, -140));
		checkpoints.push_back(Vector3f(200, 0, 20));
		checkpoints.push_back(Vector3f(225, 0, 100));
		checkpoints.push_back(Vector3f(215, 0, 150));
		checkpoints.push_back(Vector3f(150, 0, 200));
		checkpoints.push_back(Vector3f(100, 0, 200));
		checkpoints.push_back(Vector3f(-50, 0, 120));
	

		// shows route of the car. Uncomment to view.
	/*for (int x = 0; x < checkpoints.size(); x++)
	{
		if (x < checkpoints.size() - 1)
		{

			GeometryActor* cyl = new GeometryActor();
			Vector3f curr = checkpoints[x];
			Vector3f next = checkpoints[x + 1];
			cyl->SetColor(Vector4f(1.0f, 1.0f, 1.0f, 1.0f));
			cyl->DrawCylinder(curr, next, 2, 2, 2, 20);
			cyl->UseSolidMaterial();
			this->_objects.push_back(cyl);
			this->m_pScene->AddActor(cyl);
		}
		else
		{
			GeometryActor* cyl = new GeometryActor();
			Vector3f curr = checkpoints[x];
			Vector3f next = checkpoints[0];
			cyl->SetColor(Vector4f(1.0f, 1.0f, 1.0f, 1.0f));
			cyl->DrawCylinder(curr, next, 2, 2, 2, 20);
			cyl->UseSolidMaterial();
			this->_objects.push_back(cyl);
			this->m_pScene->AddActor(cyl);
		}
	}*/

	//tree points
	treepoints.push_back(Vector3f(-50, 0, -220));
	treepoints.push_back(Vector3f(150, 0, -150));
	treepoints.push_back(Vector3f(200, 0, -50));

	for (auto& ttree : treepoints)
	{
		MakeTrees(ttree);
	}

		duckRoute.push_back(Vector3f(90, 0, -180));
		duckRoute.push_back(Vector3f(60, 0, -180));
		duckRoute.push_back(Vector3f(60, 0, -210));
		duckRoute.push_back(Vector3f(90, 0, -210));
		duckRoute.push_back(Vector3f(110, 0, -190));

	
	createCrowd();
	
	//move car
	idx = 0;
	nextidx = idx + 1;
	state = 0;
	Matrix3f tstartRotation;

	car_refdir = Vector3f(0, 0,1);
	car_dir = car_refdir;
	car_targetdir = checkpoints[nextidx] - checkpoints[idx];
	prev_dist = car_targetdir.Magnitude();
	car_targetdir.Normalize();

	float angle = acos(car_refdir.Dot(car_targetdir));
	Vector3f axis = car_refdir.Cross(car_targetdir);
	axis.Normalize();

	tstartRotation.RotationEuler(axis, angle);
	_carActor->GetNode()->Rotation() = tstartRotation;
	_carActor->GetNode()->Position() = checkpoints[idx];

	linear_speed = 80.0f; // speed car travels at
	angular_speed = 1.5f; //speed angle changes


	this->_skybox = new SkyboxActor(this->_local_content.getImage(4), 10000.0f);
	this->m_pScene->AddActor(this->_skybox);


	//HUD
	this->inputAssembly2D();
	//
	PointLight* tlight = new PointLight();
	tlight->GetSpecular() = Vector4f(0.2f, 0.2f, 0.2f, 0.2f);
	tlight->GetDiffuse() = Vector4f(0.2f, 0.2f, 0.2f,0.2f);
	tlight->GetAmbient() = Vector4f(0.2f, 0.2f, 0.2f, 0.2f);
	tlight->GetNode()->Position() = Vector3f(-128.0f, 228.0f, -128.0f);
	m_pScene->AddLight(tlight);
	//setupLightSources();

}
void LJMULevelDemo::inputAssembly2D()
{
	Vector4f tcolour(1, 1, 1, 1);
	Matrix4f tmatrix;
	tmatrix.MakeIdentity();

	LJMUSpriteEntry tsprite;
	tsprite.xform = tmatrix;
	tsprite.colour = tcolour;
	tsprite.Sprite = this->_local_content.getImage(3);
	this->_sprites.push_back(tsprite);
	this->_sprites[0].xform = Matrix4f::ScaleMatrix(0.25f);
	Vector3f tplypos(20, 20, 0);
	this->_sprites[0].xform.SetTranslation(tplypos);

LJMUTextEntry ttext;
	ttext.xform = tmatrix;
	ttext.colour = tcolour;
	ttext.font = this->_local_content.getFont(2);
	ttext.text = L"FPS: ";
	Vector3f ttextpos = Vector3f(this->_obj_window->GetWidth() - 150, 10, 0);
	ttext.xform.SetTranslation(ttextpos);
	this->_text.push_back(ttext);

	LJMUTextEntry ttext2;
	ttext2.xform = tmatrix;
	ttext2.colour = tcolour;
	ttext2.font = this->_local_content.getFont(2);
	ttext2.text = L"Laps: ";
	Vector3f ttextpos2 = Vector3f(this->_obj_window->GetWidth() - 150, 40, 0);
	ttext2.xform.SetTranslation(ttextpos2);
	this->_text.push_back(ttext2);

	LJMUTextEntry ttext3;
	ttext3.xform = tmatrix;
	ttext3.colour = tcolour;
	ttext3.font = this->_local_content.getFont(2);
	ttext3.text = L"CAR POSITION: ";
	Vector3f ttextpos3 = Vector3f(this->_obj_window->GetWidth() - 550, 10, 0);
	ttext3.xform.SetTranslation(ttextpos3);
	this->_text.push_back(ttext3);

	LJMUTextEntry ttext4;
	ttext4.xform = tmatrix;
	ttext4.colour = tcolour;
	ttext4.font = this->_local_content.getFont(2);
	ttext4.text = L"CAR SPEED: ";
	Vector3f ttextpos4 = Vector3f(this->_obj_window->GetWidth() - 550, 40, 0);
	ttext4.xform.SetTranslation(ttextpos4);
	this->_text.push_back(ttext4);

	LJMUTextEntry ttext5;
	ttext5.xform = tmatrix;
	ttext5.colour = tcolour;
	ttext5.font = this->_local_content.getFont(2);
	ttext5.text = L"CAR DIRECTION ";
	Vector3f ttextpos5 = Vector3f(this->_obj_window->GetWidth() - 850, 10, 0);
	ttext5.xform.SetTranslation(ttextpos5);
	this->_text.push_back(ttext5);

//GUI
	Vector3f tpos = Vector3f(20, 100, 0);
	tmatrix.SetTranslation(tpos);

	LJMUSpriteEntry tpanel;
	tpanel.xform = tmatrix;
	tpanel.colour = Vector4f(1, 1, 1, 1);
	tpanel.Sprite = this->_local_content.getImage(5);
	this->_sprites.push_back(tpanel);

	LJMUSpriteEntry thl;
	thl.xform = tmatrix;
	thl.colour = Vector4f(1, 1, 1, 1);
	thl.Sprite = this->_local_content.getImage(6);
	this->_sprites.push_back(thl);

	for (int i = 0; i < this->_menuitems.size(); i++)
	{
		tmatrix.SetTranslation(tpos);
		LJMUTextEntry pi;
		pi.xform = tmatrix;
		pi.font = this->_local_content.getFont(7);
		pi.colour = Vector4f(1, 1, 1, 1);
		pi.text = this->_menuitems[i];
		this->_menutext.push_back(pi);
		tpos.y += 32.0f;
	}
		
		
}
void LJMULevelDemo::Update2D(float pptpf, float ptotal)
{
	std::wstringstream oss;
	oss << "FPS: " << 1 / pptpf;
	this->_text[0].text = oss.str();

	std::wstringstream lap;
	lap << "LAPS: " << this->_lapNo;
	this->_text[1].text = lap.str();

	Vector2f carPos;
	carPos.x = this->_carActor->GetNode()->Position().x;
	int roundValueX = round(carPos.x);
	carPos.y = this->_carActor->GetNode()->Position().z;
	int roundValueY = round(carPos.y);

	std::wstringstream carPo;
	carPo << "CAR POSITION: (" << roundValueX <<", "<<roundValueY<< ")";
	this->_text[2].text = carPo.str();

	std::wstringstream carSpe;
	carSpe << "CAR SPEED: " << this->linear_speed;
	this->_text[3].text = carSpe.str();


	float xround = round(car_dir.x * 100) / 100;
	float zround = round(car_dir.z * 100) / 100;

	float carDir = atan2(zround, xround) * 180 / PI;
	float carDir2 = round(carDir);
	std::wstringstream carDi;
	carDi << "CAR DIRECTION: " << carDir2;
	this->_text[4].text = carDi.str();


	float ttpf = this->m_pTimer->Elapsed();
	float ttotal = this->m_pTimer->Runtime();
	this->_sprites[2].xform = this->_menutext[this->_menu_sys.activeindex].xform; 
	Vector4f tsel = Vector4f(0, 0, 0, 1);
	Vector4f tnorm = Vector4f(1, 1, 1, 1);
	for (int i = 0; i < this->_menutext.size(); ++i)
	{
		this->_menutext[i].colour = i == this->_menu_sys.activeindex ? tsel : tnorm;
		this->_menutext[i].text = this->_menuitems[i];
	}
if (displayCar == 3)
	{
		this->_sprites[0].Sprite = this->_local_content.getImage(3);

	}
	else
		this->_sprites[0].Sprite = this->_local_content.getImage(9);


	
//
}
void LJMULevelDemo::updateTrigger(TriggerMode pmode)
{
	int ti = this->_menu_sys.activeindex;
	if (ti == CAR_SKIN)
	{
		if (skinVal == 1)
		{
			skinVal = 8;
			displayCar = 9;
		}
		else
		{
			skinVal = 1;
			displayCar = 3;
		}
	}
	if (ti == CAR_SPEED && pmode == TRIGGER_INC)
		linear_speed += 5.0f;
	if (ti == CAR_SPEED && pmode == TRIGGER_DEC)
		linear_speed -= 5.0f;

	if (ti == DAY_NIGHT)
	{
		this->_light_on = !this->_light_on;
	}
	if (ti == DISCO)
		this->disco_mode = !this->disco_mode;
}
void LJMULevelDemo::updateUI()
{
	srand(static_cast <unsigned> (time(0)));
	std::wstringstream ss;

	if (this->_light_on)
	{
		m_pScene->GetLight(0)->SetAmbient(Vector4f(0.5f, 0.5f, 0.5f, 0.5f));
		m_pScene->GetLight(0)->SetSpecular(Vector4f(0.5f, 0.5f, 0.5f, 0.5f));
		m_pScene->GetLight(0)->SetDiffuse(Vector4f(0.5f, 0.5f, 0.5f, 0.5f));

		
	}
	else if(this->_light_on == false)
	{
		m_pScene->GetLight(0)->SetAmbient(Vector4f(0.1f, 0.1f, 0.1f, 0.1f));
		m_pScene->GetLight(0)->SetSpecular(Vector4f(0.1f, 0.1f, 0.1f, 0.1f));
		m_pScene->GetLight(0)->SetDiffuse(Vector4f(0.1f, 0.1f, 0.1f, 0.1f));
	
	}
	if (disco_mode == true)
	{
		
		float col1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) +0.2;
		float col2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float col3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		m_pScene->GetLight(0)->SetAmbient(Vector4f(col1, col2, col3, 1.0f));
		m_pScene->GetLight(0)->SetSpecular(Vector4f(col1, col2,col3, 1.0f));
		m_pScene->GetLight(0)->SetDiffuse(Vector4f(col1, col2, col3, 1.0f));
	}
}
MaterialPtr LJMULevelDemo::createSimpleTexturedMaterial(wstring diffuse_map)
{
	MaterialPtr material = MaterialPtr(new MaterialDX11());
	RenderEffectDX11* pEffect = new RenderEffectDX11();

	pEffect->SetVertexShader(_obj_renderer11->LoadShader(VERTEX_SHADER, std::wstring(L"LJMUTextureMap.hlsl"), std::wstring(L"VSMAIN"), std::wstring(L"vs_4_0")));
	pEffect->SetPixelShader(_obj_renderer11->LoadShader(PIXEL_SHADER, std::wstring(L"LJMUTextureMap.hlsl"), std::wstring(L"PSMAIN"), std::wstring(L"ps_4_0")));

	ResourcePtr texture = RendererDX11::Get()->LoadTexture(diffuse_map);
	material->Parameters.SetShaderResourceParameter(L"ColorTexture", texture);
	SamplerStateConfigDX11 SamplerConfig;
	SamplerConfig.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerConfig.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerConfig.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerConfig.MaxAnisotropy = 16;

	int TextureSampler = RendererDX11::Get()->CreateSamplerState(&SamplerConfig);
	material->Parameters.SetSamplerParameter(L"TextureSampler", TextureSampler);
	material->Params[VT_PERSPECTIVE].bRender = true;
	material->Params[VT_PERSPECTIVE].pEffect = pEffect;

	return material;
}
BasicMeshPtr LJMULevelDemo::generateOBJMesh(std::wstring pmeshname, Vector4f pmeshcolour)
{
	LJMUDX::LJMUMeshOBJ* tmesh = new LJMUDX::LJMUMeshOBJ(this->_fs.GetModelsFolder() + pmeshname);
	int tvertcount = tmesh->positions.size();
	auto tia = std::make_shared < DrawExecutorDX11 < BasicVertexDX11::Vertex>>();
	tia->SetLayoutElements(BasicVertexDX11::GetElementCount(), BasicVertexDX11::Elements);
	tia->SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	tia->SetMaxVertexCount(tvertcount);

	BasicVertexDX11::Vertex tv;
	tv.color = pmeshcolour;
	for (auto& tobject : tmesh->objects)
	{
		for (auto& tface : tobject.faces)
		{
			for (size_t i = 0; i < 3; ++i)
			{
				tv.position = tmesh->positions[tface.PositionIndices[i]];
				tv.normal = tmesh->normals[tface.NormalIndices[i]];
				tv.texcoords = tmesh->coords[tface.CoordIndices[i]];
				tia->AddVertex(tv);
			}
		}
	}
	return tia;
}

void LJMULevelDemo::loadResources()
{
	this->_local_content.loadImage(1, std::wstring(L"Lamborginhi_Aventador_diffuse.png"));
	this->_local_content.loadFont(2, std::wstring(L"Calibri"), 32);
	this->_local_content.loadImage(3, std::wstring(L"Lambo_icon.png"));
	this->_local_content.loadImage(4, std::wstring(L"skycube.dds"));
	this->_local_content.loadImage(5, std::wstring(L"ui_panel_bg.png"));
	this->_local_content.loadImage(6, std::wstring(L"ui_highlight_bg.png"));
	this->_local_content.loadFont(7, std::wstring(L"Calibri"), 26);
	this->_local_content.loadImage(8, std::wstring(L"Lamborginhi_Aventador_diffuse2.png"));
	this->_local_content.loadImage(9, std::wstring(L"Lambo_icon2.png"));

	this->_menuitems.push_back(L"SPEED");
	this->_menuitems.push_back(L"SKIN");
	this->_menuitems.push_back(L"DAY/NIGHT");
	this->_menuitems.push_back(L"DISCO MODE");
	_menu_sys.itemcount = this->_menuitems.size();
}
////////////////////////////////////
// Initialise our DirectX 3D Scene
////////////////////////////////////
void LJMULevelDemo::Initialize()
{
	this->loadResources();
	//Call the Input Assembly Stage to setup the layout of our Engine Objects
	this->inputAssemblyStage();

	//this->_obj_camera = new Camera();

	//Vector3f tcamerapos(0.0f, 20.0f, -50.0f);
	//this->_obj_camera->Spatial().SetTranslation(tcamerapos);
	this->_obj_camera = new FirstPersonCamera();
	this->_obj_camera->SetEventManager(&this->EvtManager);

	Vector3f tcamerapos(0, 150, -350);
	this->_obj_camera->Spatial().SetTranslation(tcamerapos);
	this->_obj_camera->Spatial().RotateXBy(20 * DEG_TO_RAD);

	this->_render_view = new ViewPerspective(*this->_obj_renderer11,
		                                     this->_tgt_render, this->_tgt_depth);
	this->_render_view->SetBackColor(Vector4f(0.0f, 0.0f, 0.0f, 1.0f));
	this->_obj_camera->SetCameraView(this->_render_view);

	//this->_render_text = new LJMUTextOverlay(*this->_obj_renderer11, 
	//	                                      this->_tgt_render, 
	//										  std::wstring(L"Cambria"), 
	//										  25);	

	//this->_obj_camera->SetOverlayView(this->_render_text);
	
	float twidth = 1024.0f;
	float theight = 768.0f;
	this->_obj_camera->SetProjectionParams(0.1f, 1000.0f, twidth/ theight, 
		                                   static_cast<float>(GLYPH_PI) / 2.0f);
	
	this->_obj_twod_rdr = new LJMUTwoDOverlay(*this->_obj_renderer11, this->_tgt_render, this->_sprite_tex, this->_sprite_font);
		//this->_obj_twod_cam->SetOverlayView(this->_obj_twod_rdr);
	this->_obj_camera->SetOverlayView(this->_obj_twod_rdr);
	this->m_pScene->AddCamera(this->_obj_camera);	
	this->_light_on = true;
	
}

///////////////////////////////////
// Update the State of our Game and 
// Output the Results to Screen (Render)
/////////////////////////////////// 
void LJMULevelDemo::Update()
{
	this->m_pTimer->Update();
	EvtManager.ProcessEvent(EvtFrameStartPtr( new EvtFrameStart(this->m_pTimer->Elapsed())));

	float tpf = m_pTimer->Elapsed();
	if (tpf > 10.0f / 60.0f)
	{
		tpf = 1 / 60.0f;
	}
	//make the sun go round.
	Matrix3f trotation;
	trotation.RotationZ(m_pTimer->Elapsed()/-5);
	this->_objects[3]->GetNode()->Rotation() *= trotation ;


	this->_objects[2]->GetNode()->Position() = Vector3f(50, 40.0f, 50);
	Matrix3f tleafRot;
	tleafRot.RotationY(m_pTimer->Elapsed());
	this->_objects[2]->GetNode()->Rotation() *= tleafRot;



	this->_list_models[2]->GetNode()->Position() = Vector3f(100, 0, -180);
	Matrix3f duckSwim;
	duckSwim.RotationY(m_pTimer->Elapsed());
	this->_list_models[2]->GetNode()->Rotation() *= duckSwim;
	

	updateTrees();
	updateCrowd();

	//car move
	if (tpf > 10.0f / 60.0f)
	{
		tpf = 1 / 60.0f;
	}
	if (state == 0)
	{
		car_targetdir = checkpoints[nextidx] - _carActor->GetNode()->Position(); //sets the target direction to the poisiton held in the next index
		float cur_dist = car_targetdir.Magnitude(); 
		car_targetdir.Normalize();
		car_dir = car_targetdir; 
		if (cur_dist > prev_dist) //if the current distance is greater than the previous distance
		{
			idx++;
			nextidx++;
			idx = idx % checkpoints.size();
			nextidx = nextidx % checkpoints.size();
			_carActor->GetNode()->Position() = checkpoints[idx];

			car_targetdir = checkpoints[nextidx] - _carActor->GetNode()->Position();
			cur_dist = car_targetdir.Magnitude();
			car_targetdir.Normalize();

			float angle = acos(car_dir.Dot(car_targetdir));
			if (angle > angular_speed * tpf)
			{
				state = 1;
			}

		}
		else
		{
			_carActor->GetNode()->Position() = _carActor->GetNode()->Position() + car_dir * linear_speed * tpf; //moves it along
			prev_dist = cur_dist;
		}
	}
	else
	{
		float angle = acos(car_dir.Dot(car_targetdir));
		if (angle < angular_speed * tpf)
		{
			
			car_dir = car_targetdir;
			float angle = acos(car_refdir.Dot(car_targetdir));
			Vector3f axis = car_refdir.Cross(car_dir);
			axis.Normalize();
			Matrix3f tstartRotation;
			tstartRotation.RotationEuler(axis, angle);
			_carActor->GetNode()->Rotation() = tstartRotation;

			car_targetdir = checkpoints[nextidx] - _carActor->GetNode()->Position();
			prev_dist = car_targetdir.Magnitude();
			state = 0;
		}
		else
		{
			angle = angle + angular_speed * tpf;
			Vector3f axis = car_dir.Cross(car_targetdir);
			axis.Normalize();
			Matrix3f tstartRotation;
			tstartRotation.RotationEuler(axis, angular_speed * tpf);
			_carActor->GetNode()->Rotation() *= tstartRotation;
			car_dir = _carActor->GetNode()->Rotation() * car_refdir;
			car_dir.Normalize();
		}
	}


	float ttpf = this->m_pTimer->Elapsed();
	float ttotal = this->m_pTimer->Runtime();
	this->Update2D(ttpf, ttotal);

	for (auto it = this->_sprites.begin(); it != this->_sprites.end(); ++it)
	{
		this->_obj_twod_rdr->drawSprite(*it);
	}
		for (auto it = this->_text.begin(); it != this->_text.end(); ++it)
	{
		this->_obj_twod_rdr->drawText(*it);
	}
			for (auto it = this->_menutext.begin(); it != this->_menutext.end(); ++it)
	{
		this->_obj_twod_rdr->drawText(*it);
	}
		this->updateLaps();
		this->updateUI();

		if (skinVal == 8)
		{
			this->_carActor->GetBody()->SetMaterial(car_material2);
		}
		else
		{
			this->_carActor->GetBody()->SetMaterial(car_material);
		}

		if (_light_on ==true)
		{
			PointLightRange = Vector4f(400.0f, 50.0f, 400.0f, 1.0f);
			PointLightColour = Vector4f(1.0f, 1.0f, 1.0f, 0.1f);
			SpotLightRange = Vector4f(200.0f, 500.0f, 200.0f, 1.0f);
			SpotLightFocus = Vector4f(10.0f, 10.0f, 10.0f, 1.0f);
			SpotLightColour = Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
			DirectionalLightColour = Vector4f(0.5f, 0.5f, 0.5f, 0.5f);
			Vector3f directionalLightDir_v3 = Vector3f(0.0f, 0.0f, 1.0f);
			directionalLightDir_v3.Normalize();
			DirectionalLightDirection = Vector4f(directionalLightDir_v3, 1.0f);
		}
		else
		{
			PointLightRange = Vector4f(10.0f, 10.0f, 10.0f, 0.0f);
			PointLightColour = Vector4f(0.0f, 0.0f, 0.2f, 0.2f);
			SpotLightRange = Vector4f(100.0f, 100.0f, 100.0f, 1.0f);
			SpotLightFocus = Vector4f(50.0f, 50.0f, 50.0f, 1.0f);
			SpotLightColour = Vector4f(1.0f, 0.0f, 0.0f, 1.0f);
			DirectionalLightColour = Vector4f(0.0f, 0.0f, 0.1f, 0.1f);
			Vector3f directionalLightDir_v3 = Vector3f(-200.0f, -200.0f, -200.0f);
			directionalLightDir_v3.Normalize();
			DirectionalLightDirection = Vector4f(directionalLightDir_v3, 1.0f);
		}
		updateLighting();
		//updateLighting();
	//----------START RENDERING--------------------------------------------------------------

	this->m_pScene->Update(m_pTimer->Elapsed());
	this->m_pScene->Render(this->_obj_renderer11);

	//--------END RENDERING-------------------------------------------------------------
	this->_obj_renderer11->Present(this->_obj_window->GetHandle(), this->_obj_window->GetSwapChain());
}

///////////////////////////////////
// Configure the DirectX 11 Programmable
// Pipeline Stages and Create the Window
// Calls 
///////////////////////////////////
bool LJMULevelDemo::ConfigureEngineComponents()
{
	// The application currently supplies the 
	int twidth = 1024;
	int theight = 768;

	// Set the render window parameters and initialize the window
	this->_obj_window = new Win32RenderWindow();
	this->_obj_window->SetPosition(25, 25);
	this->_obj_window->SetSize(twidth, theight);
	this->_obj_window->SetCaption(this->GetName());
	this->_obj_window->Initialize(this);


	// Create the renderer and initialize it for the desired device
	// type and feature level.
	this->_obj_renderer11 = new RendererDX11();

	if (!this->_obj_renderer11->Initialize(D3D_DRIVER_TYPE_HARDWARE, D3D_FEATURE_LEVEL_11_0))
	{
		Log::Get().Write(L"Could not create hardware device, trying to create the reference device...");

		if (!this->_obj_renderer11->Initialize(D3D_DRIVER_TYPE_REFERENCE, D3D_FEATURE_LEVEL_10_0))
		{
			ShowWindow(this->_obj_window->GetHandle(), SW_HIDE);
			MessageBox(this->_obj_window->GetHandle(), L"Could not create a hardware or software Direct3D 11 device!", L"5108COMP Coursework Template", MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
			this->RequestTermination();
			return(false);
		}
		// If using the reference device, utilize a fixed time step for any animations.
		this->m_pTimer->SetFixedTimeStep(1.0f / 10.0f);
	}

	// Create a swap chain for the window that we started out with.  This
	// demonstrates using a configuration object for fast and concise object
	// creation.
	SwapChainConfigDX11 tconfig;
	tconfig.SetWidth(this->_obj_window->GetWidth());
	tconfig.SetHeight(this->_obj_window->GetHeight());
	tconfig.SetOutputWindow(this->_obj_window->GetHandle());
	this->_swap_index = this->_obj_renderer11->CreateSwapChain(&tconfig);
	this->_obj_window->SetSwapChain(this->_swap_index);
	
	//Create Colour and Depth Buffers
	this->_tgt_render = this->_obj_renderer11->GetSwapChainResource(this->_swap_index);

	Texture2dConfigDX11 tdepthconfig;
	tdepthconfig.SetDepthBuffer(twidth, theight);
	this->_tgt_depth = this->_obj_renderer11->CreateTexture2D(&tdepthconfig, 0);

	// Bind the swap chain render target and the depth buffer for use in rendering.  
	this->_obj_renderer11->pImmPipeline->ClearRenderTargets();
	this->_obj_renderer11->pImmPipeline->OutputMergerStage.DesiredState.RenderTargetViews.SetState(0, this->_tgt_render->m_iResourceRTV);
	this->_obj_renderer11->pImmPipeline->OutputMergerStage.DesiredState.DepthTargetViews.SetState(this->_tgt_depth->m_iResourceDSV);
	this->_obj_renderer11->pImmPipeline->ApplyRenderTargets();

	D3D11_VIEWPORT tviewport;
	tviewport.Width = static_cast< float >(twidth);
	tviewport.Height = static_cast< float >(theight);
	tviewport.MinDepth = 0.0f;
	tviewport.MaxDepth = 1.0f;
	tviewport.TopLeftX = 0;
	tviewport.TopLeftY = 0;

	int tvpindex = this->_obj_renderer11->CreateViewPort(tviewport);
	this->_obj_renderer11->pImmPipeline->RasterizerStage.DesiredState.ViewportCount.SetState(1);
	this->_obj_renderer11->pImmPipeline->RasterizerStage.DesiredState.Viewports.SetState(0, tvpindex);
	return(true);
}

//////////////////////////////////
//Handle Input Events in the Application
//////////////////////////////////
bool LJMULevelDemo::HandleEvent(EventPtr pevent)
{
	eEVENT e = pevent->GetEventType();

	if (e == SYSTEM_KEYBOARD_KEYDOWN)
	{
		EvtKeyDownPtr tkey_down = std::static_pointer_cast<EvtKeyDown>(pevent);
		unsigned int  tkeycode = tkey_down->GetCharacterCode();

		if (tkeycode == VK_UP)
		{
			this->_menu_sys.decrement();
		}
		if (tkeycode == VK_DOWN)
		{
			this->_menu_sys.increment();
		}
		if (tkeycode == VK_RIGHT)
		{
			this->updateTrigger(TRIGGER_INC);
		}
		if (tkeycode == VK_LEFT)
		{
			this->updateTrigger(TRIGGER_DEC);
		}
		if (tkeycode == VK_RETURN)
		{
			this->updateTrigger(TRIGGER_SELECT);
		}
	}
	else if (e == SYSTEM_KEYBOARD_KEYUP)
	{
		EvtKeyUpPtr tkey_up = std::static_pointer_cast<EvtKeyUp>(pevent);
		unsigned int tkeycode = tkey_up->GetCharacterCode();
	}

	return(Application::HandleEvent(pevent));
}

//////////////////////////////////
// Destroy Resources created by the engine
//////////////////////////////////
void LJMULevelDemo::ShutdownEngineComponents()
{
	if (this->_obj_renderer11)
	{
		this->_obj_renderer11->Shutdown();
		delete this->_obj_renderer11;
	}

	if (this->_obj_window)
	{
		this->_obj_window->Shutdown();
		delete this->_obj_window;
	}
}

//////////////////////////////////
// Shutdown the Application
//////////////////////////////////
void LJMULevelDemo::Shutdown()
{
	//NOTHING TO DO HERE
}

//////////////////////////////////
// Take a Screenshot of the Application
//////////////////////////////////
void LJMULevelDemo::TakeScreenShot()
{
	if (this->m_bSaveScreenshot)
	{
		this->m_bSaveScreenshot = false;
		this->_obj_renderer11->pImmPipeline->SaveTextureScreenShot(0, this->GetName());
	}
}

//////////////////////////////////////
// Output our Frame Rate
//////////////////////////////////////

void LJMULevelDemo::MakeTrees(Vector3f coords)
{	
	GeometryActor* bark = new GeometryActor();
	GeometryActor* leaves = new GeometryActor();
	bark->SetColor(Vector4f(0.8, 0.5, 0.1, 1.0f));
	bark->DrawBox(Vector3f(coords), Vector3f(5.5f, 25.5f, 5.5f));
	bark->UseSolidMaterial();
	leaves->SetColor(Vector4f(0.0f, 1.0f, 0.0f, 1.0f));

	leaves->DrawBox(Vector3f(0, 0.0f, 0), Vector3f(10, 10.5f, 10));
	leaves->UseSolidMaterial();
	this->_trees.push_back(bark);
	this->m_pScene->AddActor(bark);
	this->_trees.push_back(leaves);
	this->m_pScene->AddActor(leaves);
}
void LJMULevelDemo::updateTrees()
{
	for (int i = 0; i < _trees.size(); i++)
	{
		if (i % 2 == 0) 
			continue; //don't update bark
		else
		{

			Vector3f leafCords;
			int applicableCoord = i / 2; //there's twice as many objects in _trees as there are in treepoints, dividing by 2 gives the currect coordinate
			leafCords = treepoints[applicableCoord];
			leafCords.y = treepoints[applicableCoord].y + 30.0f; //the leaves should be 30 higher on y axis. 

			this->_trees[i]->GetNode()->Position() = leafCords; //translate
			Matrix3f tleafRot;
			tleafRot.RotationY(m_pTimer->Elapsed());
			this->_trees[i]->GetNode()->Rotation() *= tleafRot; //rotate
		}
	}
}

void LJMULevelDemo::updateLaps()
{

	Vector3f compair;
	compair.x = this->_carActor->GetNode()->Position().x;
	compair.y = this->_carActor->GetNode()->Position().y;
	compair.z = this->_carActor->GetNode()->Position().z;

	Vector3f compair2 = Vector3f(-180, 0, -10);
	if (compair == compair2)
	{
		if (_atStart == true)
		{
			return;
		}
		else
		{
			this->_lapNo = this->_lapNo +1;
			_atStart = true;
		}
	}
	else
		_atStart = false;
}

void LJMULevelDemo::createPlatform()
{
	GeometryActor* _platform = new GeometryActor();
	_platform->SetColor(Vector4f(0.2, 0.2, 0.2, 1.0f));
	//_platform->DrawBox(Vector3f(200, 5.0f, -200), Vector3f(25, 2.5f, 25));
	_platform->DrawBox(Vector3f(-100, 0, 170), Vector3f(50, 7.5f, 20));
	_platform->UseSolidMaterial();
	this->_objects.push_back(_platform);

	this->m_pScene->AddActor(_platform);
	GeometryActor* _platform2 = new GeometryActor();
	_platform2->SetColor(Vector4f(0.8, 0.2, 0.2, 1.0f));
	_platform2->DrawBox(Vector3f(-100, 0, 175), Vector3f(50, 15, 10));
	_platform2->UseSolidMaterial();
	this->_objects.push_back(_platform2);
	this->m_pScene->AddActor(_platform2);


}
void LJMULevelDemo::createCrowd()
{
	
	for (int i = 0; i < 6; i++)
	{
		Vector3f startpos = Vector3f(-140, 7, 160);
		startpos.x = startpos.x + (i * 15);
		GeometryActor* _body = new GeometryActor();
		_body->SetColor(Vector4f(0.8, 0.0, 0.0, 1.0f));
		_body->DrawBox(startpos, Vector3f(4, 5, 4));
		_body->UseSolidMaterial();
		this->_people.push_back(_body);
		this->m_pScene->AddActor(_body);

		startpos.y = startpos.y + 7;
		GeometryActor* _head = new GeometryActor();
		_head->SetColor(Vector4f(0.8, 0.8, 0.0, 1.0f));
		_head->DrawSphere(Vector3f(startpos), 5, 5, 5);
		_head->UseSolidMaterial();
		this->_people.push_back(_head);
		this->m_pScene->AddActor(_head);
	}
}

void LJMULevelDemo::updateCrowd()
{
	//bool jump = true;
	int headIterator = waveIterator + 1;
	switch (targetHeight)
	case 15:
	{
		if (this->_people[waveIterator]->GetNode()->Position().y < targetHeight)
		{
			this->_people[waveIterator]->GetNode()->Position().y += 0.25f;
			this->_people[headIterator]->GetNode()->Position().y += 0.25f;
		}
		else if (this->_people[waveIterator]->GetNode()->Position().y == targetHeight)
			targetHeight = 7;
		break;
	case 7:
		if (this->_people[waveIterator]->GetNode()->Position().y > targetHeight)
		{
			this->_people[waveIterator]->GetNode()->Position().y -= 0.25;
			this->_people[headIterator]->GetNode()->Position().y -= 0.25;
		}
		else if (this->_people[waveIterator]->GetNode()->Position().y == targetHeight)
		{
			targetHeight = 15;
			if (waveIterator == 10)
				waveIterator = 0;
			else
			waveIterator = waveIterator + 2;
		}
		break;
	}
	
}

void LJMULevelDemo::setupLightSources()
{
	Vector3f directionalLightDir_v3 = Vector3f(0.0f, 0.0f, 1.0f);
	directionalLightDir_v3.Normalize();
	DirectionalLightDirection = Vector4f(directionalLightDir_v3, 1.0f);
	DirectionalLightColour = Vector4f(0.5f, 0.5f, 0.5f, 0.5f);

	Vector3f spotLightDir_v3 = Vector3f(0.5f, -1.0f, 0.5f);
	spotLightDir_v3.Normalize();
	SpotLightDirection = Vector4f(spotLightDir_v3, 1.0f);
	SpotLightColour = Vector4f(1.0f, 1.0f, 0.0f, 1.0f);
	SpotLightPosition = Vector4f(0.0f, 50.0f, 0.0f, 1.0f);
	SpotLightRange = Vector4f(200.0f, 500.0f, 200.0f, 1.0f);
	SpotLightFocus = Vector4f(10.0f, 100.0f, 10.0f, 1.0f);

	PointLightPosition = Vector4f(1.0f,21.0f, 1.0f, 1.0f);
	PointLightColour = Vector4f(1.0f, 0.0f, 0.0f, 1.0f);
	PointLightRange = Vector4f(100.0f, 50.0f, 100.0f, 0.0f);
}

MaterialPtr LJMULevelDemo::CreateLitTexturedMaterial(wstring diffuse_map)
{
	MaterialPtr material = MaterialPtr(new MaterialDX11());
	RenderEffectDX11* pEffect = new RenderEffectDX11();
	pEffect->SetVertexShader(_obj_renderer11->LoadShader(VERTEX_SHADER, std::wstring(L"LJMULitTexture.hlsl"), 
														std::wstring(L"VSMAIN"), std::wstring(L"vs_4_0")));

	pEffect->SetPixelShader(_obj_renderer11->LoadShader(PIXEL_SHADER, std::wstring(L"LJMULitTexture.hlsl"),
														std::wstring(L"PSMAIN"), std::wstring(L"ps_4_0")));

	ResourcePtr texture = RendererDX11::Get()->LoadTexture(diffuse_map);
	material->Parameters.SetShaderResourceParameter(L"ColorTexture", texture);

	SamplerStateConfigDX11 SamplerConfig;
	SamplerConfig.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerConfig.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerConfig.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerConfig.MaxAnisotropy = 16;

	int TextureSampler = RendererDX11::Get()->CreateSamplerState(&SamplerConfig);
	material->Parameters.SetSamplerParameter(L"TextureSampler", TextureSampler);

	material->Params[VT_PERSPECTIVE].bRender = true;
	material->Params[VT_PERSPECTIVE].pEffect = pEffect;

	return material;
}

void LJMULevelDemo::setLights2Material(MaterialPtr material)
{
	material->Parameters.SetVectorParameter(L"DirectionalLightColour", DirectionalLightColour);
	material->Parameters.SetVectorParameter(L"DirectionalLightDirection", DirectionalLightDirection);

	material->Parameters.SetVectorParameter(L"SpotLightColour", SpotLightColour);
	material->Parameters.SetVectorParameter(L"SpotLightDirection", SpotLightDirection);
	material->Parameters.SetVectorParameter(L"SpotLightPosition", SpotLightPosition);
	material->Parameters.SetVectorParameter(L"SpotLightFocus", SpotLightFocus);

	material->Parameters.SetVectorParameter(L"PointLightColour", PointLightColour);
	material->Parameters.SetVectorParameter(L"PointLightPosition", PointLightPosition);
	material->Parameters.SetVectorParameter(L"PointLightRange", PointLightRange);
}

void LJMULevelDemo::updateLighting()
{
	setLights2Material(floor_material);

	setLights2Material(car_material);

	setLights2Material(car_material2);
}