#pragma once

#include "DonutAPI.h"

#include "Globals.h"

using namespace Screen;

class Textures 
{
public:
	struct TextureSet
	{
		Texture tex[MAX_TEX] = {};
	};
	TextureSet texSet = {};

public:

	void createTextures();
	void destroyTextures();

	// Textures
	
	// Building block textures
	void brickTile(Texture& tex);
	void blackBrickTile(Texture& tex);
	void stoneTile(Texture& tex);
	void blackStoneTile(Texture& tex);
	void cobbleStoneTile(Texture& tex);
	void woodTile(Texture& tex);
	void woodenBeamTile(Texture& tex);
	void glassTile(Texture& tex);
	void rGlassTile(Texture& tex);
	void gGlassTile(Texture& tex);
	void bGlassTile(Texture& tex);
	
	// Dec textures
	void bannerTile(Texture& tex);

	// Floor textures
	void grassTile(Texture& tex);
	void gravelTile(Texture& tex);
	void carpetTile(Texture& tex);
	void rCarpetTile(Texture& tex);
	void bCarpetTile(Texture& tex);
	
	// Slope/stair textures
	void wSlopeTile(Texture& tex);
	void bSlopeTile(Texture& tex);
	void dgSlopeTile(Texture& tex);
	void gSlopeTile(Texture& tex);

	// Terrain textures
	void treeTile(Texture& tex);
	void dgTreeTile(Texture& tex);
	void sTreeTile(Texture& tex);
	void bushTile(Texture& tex);
	void sBushTile(Texture& tex);

	// Player textures
	void rightArmTile(Texture& tex);
	void leftArmTile(Texture& tex);

	// Mob textures 
	void horseTile(Texture& tex);

	// People
	void salesmanTile(Texture& tex);
	void blacksmithTile(Texture& tex);
	void butcherTile(Texture& tex);
	void maleCitizen(Texture& tex); // Adult
	void femaleCitizen(Texture& tex);
	void boyCitizen(Texture& tex); // Kid
	void girlCitizen(Texture& tex);

	// Hostile mob textures
	void skeletonTile(Texture& tex);
	void zombieTile(Texture& tex);
	void witchTile(Texture& tex);
	void bearTile(Texture& tex);
	void vampireTile(Texture& tex);
	void lichTile(Texture& tex);
	void mageTile(Texture& tex);

private:
	// Look away
	Texture brickTex = CreateTexture(16, 16); 
	Texture blackBrickTex = CreateTexture(16, 16);
	Texture stoneTex = CreateTexture(16, 16);
	Texture blackStoneTex = CreateTexture(16, 16);
	Texture cobbleTex = CreateTexture(16, 16);
	Texture woodTex = CreateTexture(16, 16);
	Texture beamTex = CreateTexture(16, 16);
	Texture glassTex = CreateTexture(16, 16);
	Texture rGlassTex = CreateTexture(16, 16);
	Texture gGlassTex = CreateTexture(16, 16);
	Texture bGlassTex = CreateTexture(16, 16);

	Texture bannerTex = CreateTexture(16, 16);

	Texture grassTex = CreateTexture(16, 16);
	Texture gravelTex = CreateTexture(16, 16);
	Texture carpetTex = CreateTexture(16, 16);
	Texture rCarpetTex = CreateTexture(16, 16);
	Texture bCarpetTex = CreateTexture(16, 16);

	Texture wSlopeTex = CreateTexture(16, 16);
	Texture bSlopeTex = CreateTexture(16, 16);
	Texture dgSlopeTex = CreateTexture(16, 16);
	Texture gSlopeTex = CreateTexture(16, 16);

	Texture treeTex = CreateTexture(16, 16);
	Texture dgTreeTex = CreateTexture(16, 16);
	Texture sTreeTex = CreateTexture(16, 16);
	Texture bushTex = CreateTexture(16, 16);
	Texture sBushTex = CreateTexture(16, 16);

	Texture rightArmTex = CreateTexture(16, 16);
	Texture leftArmTex = CreateTexture(16, 16);

	Texture horseTex = CreateTexture(16, 16);

	Texture salesmanTex = CreateTexture(16, 16);
	Texture blacksmithTex = CreateTexture(16, 16);
	Texture butcherTex = CreateTexture(16, 16);
	Texture maleTex = CreateTexture(16, 16);
	Texture femaleTex = CreateTexture(16, 16);
	Texture boyTex = CreateTexture(16, 16);
	Texture girlTex = CreateTexture(16, 16);

	Texture skeletonTex = CreateTexture(16, 16);
	Texture zombieTex = CreateTexture(16, 16);
	Texture witchTex = CreateTexture(16, 16);
	Texture bearTex = CreateTexture(16, 16);
	Texture vampireTex = CreateTexture(16, 16);
	Texture lichTex = CreateTexture(16, 16);
	Texture mageTex = CreateTexture(16, 16);
};