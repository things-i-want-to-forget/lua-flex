#ifndef SPROPS_H
#define SPROPS_H

struct surfacephysicsparams_t
{
	// vphysics physical properties
	float			friction;
	float			elasticity;				// collision elasticity - used to compute coefficient of restitution
	float			density;				// physical density (in kg / m^3)
	float			thickness;				// material thickness if not solid (sheet materials) in inches
	float			dampening;
};

struct surfaceaudioparams_t
{
	// sounds / audio data
	float			reflectivity;		// like elasticity, but how much sound should be reflected by this surface
	float			hardnessFactor;	// like elasticity, but only affects impact sound choices
	float			roughnessFactor;	// like friction, but only affects scrape sound choices

	// audio thresholds
	float			roughThreshold;	// surface roughness > this causes "rough" scrapes, < this causes "smooth" scrapes
	float			hardThreshold;	// surface hardness > this causes "hard" impacts, < this causes "soft" impacts
	float			hardVelocityThreshold;	// collision velocity > this causes "hard" impacts, < this causes "soft" impacts
	// NOTE: Hard impacts must meet both hardnessFactor AND velocity thresholds
};

struct surfacesoundnames_t
{
	unsigned short	stepleft;
	unsigned short	stepright;

	unsigned short	impactSoft;
	unsigned short	impactHard;

	unsigned short	scrapeSmooth;
	unsigned short	scrapeRough;

	unsigned short	bulletImpact;
	unsigned short	rolling;

	unsigned short	breakSound;
	unsigned short	strainSound;
};

struct surfacegameprops_t
{
	// game movement data
	float			maxSpeedFactor;			// Modulates player max speed when walking on this surface
	float			jumpFactor;				// Indicates how much higher the player should jump when on the surface
	float unk1, unk2, unk3;
	// Game-specific data
	unsigned short	material;
	// Indicates whether or not the player is on a ladder.
	unsigned char	climbable;
	unsigned char	pad;
};

//-----------------------------------------------------------------------------
// Purpose: Each different material has an entry like this
//-----------------------------------------------------------------------------
struct surfacedata_t
{
	surfacephysicsparams_t	physics;	// physics parameters
	surfaceaudioparams_t	audio;		// audio parameters
	surfacesoundnames_t		sounds;		// names of linked sounds
	surfacegameprops_t		game;		// Game data / properties

	float &penetration(void)
	{
		return *(float*)(0x4C + (char *)this);
	}
	unsigned short &gamematerial(void)
	{
		return *(unsigned short *)(0x54 + (char *)this);
	}
};

class CPhysicsSurfaceProps
{
public:

	virtual ~CPhysicsSurfaceProps(void) {}

	// parses a text file containing surface prop keys
	virtual int		ParseSurfaceData(const char *pFilename, const char *pTextfile) = 0;
	// current number of entries in the database
	virtual int		SurfacePropCount(void) = 0;

	virtual int		GetSurfaceIndex(const char *pSurfacePropName) = 0;
	virtual void	GetPhysicsProperties(int surfaceDataIndex, float *density, float *thickness, float *friction, float *elasticity) = 0;

	virtual surfacedata_t	*GetSurfaceData(int surfaceDataIndex) = 0;
};

#endif