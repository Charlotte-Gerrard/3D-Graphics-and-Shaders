#include "LJMUTwoDOverlay.h"
#include "Entity3D.h"
#include "Node3D.h"
#include "Texture2dConfigDX11.h"
#include "Log.h"
#include "IParameterManager.h"
#include "PipelineManagerDX11.h"
#include "Texture2dDX11.h"

using namespace LJMUDX;
using namespace Glyph3;

///////////////////////////
// Constructor for our Sprite
// Overlay Class. 
///////////////////////////
LJMUTwoDOverlay::LJMUTwoDOverlay(RendererDX11& prenderer, ResourcePtr prtt, ResourcePtr pdefsprite, SpriteFontPtr pdeffont)
{
	this->_obj_rendertarget = prtt;
	ResourceDX11* tptr_res = prenderer.GetResourceByIndex(this->_obj_rendertarget->m_iResource);

	if (tptr_res->GetType() == RT_TEXTURE2D)
	{
		Texture2dDX11* ttex = (Texture2dDX11*)tptr_res;
		D3D11_TEXTURE2D_DESC tdesc = ttex->GetActualDescription();
		D3D11_VIEWPORT tviewport;
		tviewport.Width = static_cast<float>(tdesc.Width);
		tviewport.Height = static_cast<float>(tdesc.Height);
		tviewport.MinDepth = 0.0f;
		tviewport.MaxDepth = 1.0f;
		tviewport.TopLeftX = 0;
		tviewport.TopLeftY = 0;
		this->_obj_viewport = prenderer.CreateViewPort(tviewport);
	}

	//Create the Sprite Font and the Sprite Renderer (SpriteBatch) for our Sprite Elements
	this->_obj_ptr_def_sprite	= pdefsprite;
	this->_obj_ptr_def_font		= pdeffont;

	this->_obj_spritebatch = new SpriteRendererDX11();
	this->_obj_spritebatch->Initialize();
}

///////////////////////////
// Destructor For our Sprite
// Overlay Class
///////////////////////////
LJMUTwoDOverlay::~LJMUTwoDOverlay()
{
	SAFE_DELETE(this->_obj_spritebatch);
}

///////////////////////////
// Queue the Render Tasks
///////////////////////////
void LJMUTwoDOverlay::QueuePreTasks(RendererDX11* prenderer)
{
	prenderer->QueueTask(this);
}

///////////////////////////
// Execute our Sprite Rendering
// Task
///////////////////////////
void LJMUTwoDOverlay::ExecuteTask(PipelineManagerDX11* ppipelinemanager, IParameterManager* pparamManager)
{
	//If we have any Sprite elements to draw
	if (this->_list_sprite.size() > 0 || this->_list_text.size() > 0)
	{
		// Set the parameters for rendering this view
		ppipelinemanager->ClearRenderTargets();
		ppipelinemanager->OutputMergerStage.DesiredState.RenderTargetViews.SetState(0,this->_obj_rendertarget->m_iResourceRTV);
		ppipelinemanager->ApplyRenderTargets();

		ppipelinemanager->RasterizerStage.DesiredState.ViewportCount.SetState(1);
		ppipelinemanager->RasterizerStage.DesiredState.Viewports.SetState(0,this->_obj_viewport);
		ppipelinemanager->RasterizerStage.DesiredState.RasterizerState.SetState(0);

		// Set default states for these stages
		ppipelinemanager->OutputMergerStage.DesiredState.DepthStencilState.SetState(0);
		ppipelinemanager->OutputMergerStage.DesiredState.BlendState.SetState(0);

		//------------DRAW OUR SPRITES AND TEXT ELEMENTS----------------------------------------------------------------

		//Loop over each Sprite element and use the spritebatch to draw them
		for (auto te : this->_list_sprite)
		{
			ResourcePtr ttexture = te.Sprite == nullptr ? this->_obj_ptr_def_sprite : te.Sprite;
			//Use the RenderSprite function of our Sprite Batch to draw the Sprite
			this->_obj_spritebatch->Render(ppipelinemanager, pparamManager,ttexture,te.xform, te.colour);
		}

		//Loop over each text element and use the spritebatch to draw them
		for (auto te : this->_list_text)
		{
			SpriteFontPtr tfont = te.font == nullptr ? this->_obj_ptr_def_font : te.font;
			//Use the RenderText function of our Sprite Batch to draw the text
			this->_obj_spritebatch->RenderText(ppipelinemanager, pparamManager,tfont,te.text.c_str(), te.xform, te.colour);
		}

		//--------------------------------------------------------------------------------------------------------------

		//Clear our list of Sprite entries each frame
		this->_list_sprite.clear();
		this->_list_text.clear();
	}
}

void LJMUTwoDOverlay::drawSprite(Matrix4f& pxform, Vector4f& pcolour, ResourcePtr ptexture)
{
	this->_list_sprite.push_back(LJMUSpriteEntry(ptexture, pxform, pcolour));
}

void LJMUTwoDOverlay::drawText(std::wstring& ptext, Matrix4f& pxform, Vector4f& pcolour,SpriteFontPtr pfont)
{
	this->_list_text.push_back(LJMUTextEntry(pfont,ptext, pxform, pcolour));
}

void  LJMUTwoDOverlay::drawSprite(LJMUSpriteEntry pentry)
{
	this->_list_sprite.push_back(pentry);
}

void  LJMUTwoDOverlay::drawText(LJMUTextEntry pentry)
{
	this->_list_text.push_back(pentry);
}


//------------INTERFACE METHODS---------------------------------------------------

///////////////////////////
// Get the Name of this Task
///////////////////////////
std::wstring LJMUTwoDOverlay::GetName()
{
	return(L"LJMUTwoDOverlay");
}

///////////////////////////
// Resize the Viewport
///////////////////////////
void LJMUTwoDOverlay::Resize(UINT pwidth, UINT pheight)
{
	RendererDX11::Get()->ResizeViewport(this->_obj_viewport, pwidth, pheight);
}

///////////////////////////
//
///////////////////////////
void LJMUTwoDOverlay::SetRenderParams(IParameterManager* pParamManager)
{
	//NO RENDER PARAMS NEEDED
}

///////////////////////////
//
///////////////////////////
void LJMUTwoDOverlay::SetUsageParams(IParameterManager* pParamManager)
{
	//NO USAGE PARAMS NEEDED
}

///////////////////////////
//
///////////////////////////
void LJMUTwoDOverlay::Update(float ptime)
{
	//NOTHING TO UPDATE
}