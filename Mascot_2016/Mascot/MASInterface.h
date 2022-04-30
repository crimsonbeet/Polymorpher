/* Created by Alex Nikol Bourakov on 23/11/2011  */


#ifndef MASInterface_H
#define MASInterface_H


typedef enum { 
	MAS_OverlapElim, 
	MAS_AndNot,
	MAS_And,
	MAS_Xor,
	MAS_Or,
	MAS_Not,
	MAS_Area,
	MAS_Size, 
	MAS_ActivateLayer

} MAS_Function; 



bool MASPreload_StaticLayers(long N, const char *Names[]); // N is the number of entries in Names; first entry in Names[] is empty and means nothing, so N is always > 1 if Names is not empty.

bool MASInitialize(long blocks_count_x, long blocks_count_y); 
bool MASGrid(long blocks_count_x, long blocks_count_y); 

bool MASLayerCreateDBStorage(const char* inpLayer_name); 

bool MASLayerAddPolygon2DBStorage(const char* inpLayer_name, long* points_x, long* points_y, long points_count); 

bool MASEvaluate1(MAS_Function action, const char* inpLayer_name, const char* outLayer_name = 0); /*if outLayer_name==0 then input layer becomes output*/

bool MASEvaluate2(MAS_Function action, const char** inpLayers, long inpLayers_count, const char* outLayer_name); 

bool MASSize(const char* inpLayer_name, const char* outLayer_name, int width); /*if outLayer_name==0 then input layer becomes output*/

void MASReleaseRTStorage();

long MASLayerCountRTPolygons(const char* outLayer_name, size_t* first_polygon_number = 0); 

long MASLayerGetRTPolygonPointsCount(const char* outLayer_name, long polygon_number); 

long MASLayerGetRTPolygon(const char* outLayer_name, long polygon_number, long* points_x, long* points_y, long points_count); 

long MASLayerRTPolygonLocate(const char* outLayer_name, long x, long y); /*returns polygon number or -1*/

bool MASLogLayer(const char* layer_Name);





#if defined(__cplusplus)
#include ".\MemoryAllocator.h"

bool MASLayerAddPolygon2DBStorage(const char* inpLayer_name, _sAlong& points_x, _sAlong& points_y); 

long MASLayerGetRTPolygon(const char* outLayer_name, long polygon_number, _sAlong& points_x, _sAlong& points_y); 

#endif


#endif /*MInterface_H*/