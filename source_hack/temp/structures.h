#ifndef STRUCTURES_H
#define STRUCTURES_H

class CClient;
class CInput;
class CPrediction;
class ClientModeShared;
class CEngineTrace;
class VPanelWrapper;
class CMatSystemSurface;

struct Structures
{
	CClient *client;
	CInput *input;
	CPrediction *prediction;
	ClientModeShared *client_mode;
	CEngineTrace *trace;
	VPanelWrapper *panel;
	CMatSystemSurface *surface;

	// todo: add
};

extern Structures structs;

#endif