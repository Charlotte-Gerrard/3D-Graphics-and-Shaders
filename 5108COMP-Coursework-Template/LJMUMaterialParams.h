#pragma once

#include "MaterialDX11.h"

namespace LJMUDX
{

	//////////////////////////////
	// Class to represent the parameters
	// which can be passed to our shader
	// in addition to the one's that are
	// handled by the renderer.  
	//////////////////////////////
	class LJMUMaterialParams
	{
	protected:
		//--------CLASS MEMBERS-------------------------------------
		Glyph3::MaterialPtr _mat;

	public:
		//--------CONSTRUCTORS/DESTRUCTORS--------------------------
		LJMUMaterialParams(Glyph3::MaterialPtr pmtlptr) :_mat(pmtlptr){};
		virtual ~LJMUMaterialParams(){ _mat = nullptr; };

		//--------INTERFACE METHODS---------------------------------
		virtual void setStaticParameters(){};
		virtual void setDynamicParameters(){};
	};

}