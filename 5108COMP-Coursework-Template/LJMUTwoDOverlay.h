#pragma once

#include "Task.h"
#include "SpriteFontLoaderDX11.h"
#include "SpriteRendererDX11.h"

namespace LJMUDX
{
	//class Entity3D;
	using namespace Glyph3;

	/////////////////////////
	// Simple Structure to
	// represent a Sprite Entry
	// and its properties.
	/////////////////////////
	struct LJMUSpriteEntry
	{
		ResourcePtr	    Sprite;
		Matrix4f		xform;
		Vector4f		colour;

		LJMUSpriteEntry() {};
		LJMUSpriteEntry(ResourcePtr ptexture, Matrix4f& pxform, Vector4f& pcolour)
		{
			this->Sprite	= ptexture;
			this->xform		= pxform;
			this->colour	= pcolour;
		};
	};

	/////////////////////////
	// Simple Structure to
	// represent a Text Entry
	// and its properties.
	/////////////////////////
	struct LJMUTextEntry
	{
		SpriteFontPtr   font;
		std::wstring	text;
		Matrix4f		xform;
		Vector4f		colour;

		LJMUTextEntry() {};
		LJMUTextEntry(SpriteFontPtr pfont, std::wstring& ptext, Matrix4f& pxform, Vector4f& pcolour)
		{
			this->font = pfont;
			this->text = ptext;
			this->xform = pxform;
			this->colour = pcolour;
		};
	};
	
	//-------------------------------------------------------------------------------------------------------------

	/////////////////////////////////////////
	// LJMUTWODOVERLAY.H
	// Class to simplify drawing of Sprite elements
	// and Text Elements using Sprite Fonts and 
	// Sprite Batches.
	//
	// AUTHORS:  DR CHRIS CARTER
	//			 MR MIKE BASKETT
	/////////////////////////////////////////
	class LJMUTwoDOverlay : public Task
	{
	public:
		//--------CONSTRUCTORS/DESTRUCTORS----------------------------------------------------
		LJMUTwoDOverlay(RendererDX11& prenderer, ResourcePtr prtt, ResourcePtr pdefsprite, SpriteFontPtr pdeffont);
		virtual ~LJMUTwoDOverlay();

		//-------TASK INTERFACE IMPLEMENTATION-----------------------------------------------
		virtual void Update(float ptime);
		virtual void QueuePreTasks(RendererDX11* pRenderer);
		virtual void ExecuteTask(PipelineManagerDX11* pPipelineManager, IParameterManager* pParamManager);
		virtual void Resize(UINT pwidth, UINT pheight);
		virtual std::wstring GetName();
		virtual void SetRenderParams(IParameterManager* pParamManager);
		virtual void SetUsageParams(IParameterManager* pParamManager);

		//--------PUBLIC METHODS-------------------------------------------------------------
		void drawSprite(Matrix4f& pxform, Vector4f& pcolour,ResourcePtr ptexture = nullptr);
		void drawSprite(LJMUSpriteEntry pentry);
		void drawText(std::wstring& ptext, Matrix4f& pxform, Vector4f& pcolour,SpriteFontPtr pfont = nullptr);
		void drawText(LJMUTextEntry pentry);

		//--------CLASS MEMBERS--------------------------------------------------------------
	protected:
		Vector4f						_colour;
		int								_obj_viewport;
		ResourcePtr						_obj_rendertarget;		
		SpriteRendererDX11*				_obj_spritebatch;

		SpriteFontPtr					_obj_ptr_def_font;
		ResourcePtr						_obj_ptr_def_sprite;

		std::vector<LJMUSpriteEntry>	_list_sprite;
		std::vector<LJMUTextEntry>      _list_text; 
	};
};