#include "script.h"
#include "keyboard.h"
#include <math.h>
#include <string>
#include <ctime>
#include <iostream>
using namespace PLAYER;
using namespace ENTITY;
using namespace PED;

const int arraySize = 256;
Vehicle blankArray[arraySize];
int action = 0;
bool modEnabled = false;
bool prevState;
Vehicle worldCars[arraySize];
Vehicle myCar;
Entity target;
Vector3 getCoordsFromCam(float distance)//i did not write this function, props to whoever did
{
	Vector3 camRot = CAM::GET_GAMEPLAY_CAM_ROT(2);
	Vector3 camCoords = CAM::GET_GAMEPLAY_CAM_COORD();
	float tZ = camRot.z * 0.0174532924F;
	float tX = camRot.x * 0.0174532924F;
	float absX = abs(cos(tX));
	return{ camCoords.x + (-sin(tZ) * absX) * distance, 0,
		camCoords.y + (cos(tZ) * absX) * distance, 0,
		camCoords.z + (sin(tX)) * distance, 0 };
	/*A Vector3 actually has six parts to it, not three: x, _paddingx, y, _paddingy, z, and _paddingz.
	So you can declare a Vector3 inside curly braces like this if you know to separate the real XYZ values from the padding values (I just use 0 for them)*/
}
BOOL coolgraphics = false;
void main()
{
	while (true)
	{
		if (IsKeyJustUp(VK_F6)) {
			modEnabled = !modEnabled;
			SET_PLAYER_INVINCIBLE(PLAYER_ID(), modEnabled);
			if (modEnabled == false) {
				for (int i = 0; i < arraySize; i++) {
					worldCars[i] = blankArray[i];
				}
			}
		}
		if (modEnabled && PLAYER::IS_PLAYER_PLAYING(PLAYER::PLAYER_ID())) {

			myCar = PLAYER::GET_PLAYERS_LAST_VEHICLE();
			worldCars[arraySize];
			int numCarsInWorld = worldGetAllVehicles(worldCars, arraySize);
			for (int i = 0; i < arraySize; i++) {
				if (ENTITY::IS_ENTITY_TOUCHING_ENTITY(myCar, worldCars[i]) && worldCars[i] != myCar) {
					VEHICLE::EXPLODE_VEHICLE(worldCars[i], true, false);
					WAIT(1000);
				}
			}
			if (IsKeyJustUp(VK_F2)) {
				Vector3 camCoords = CAM::GET_GAMEPLAY_CAM_COORD();
				Vector3 farCoords = getCoordsFromCam(10000);
				Vector3 endCoords, surfaceNormal;
				BOOL hit;
				Entity entityHit = 0;

				int ray = WORLDPROBE::_CAST_RAY_POINT_TO_POINT(camCoords.x, camCoords.y, camCoords.z, farCoords.x, farCoords.y, farCoords.z, -1, PLAYER_PED_ID(), 7);
				WORLDPROBE::_GET_RAYCAST_RESULT(ray, &hit, &endCoords, &surfaceNormal, &entityHit);
				int what = ENTITY::GET_ENTITY_TYPE(entityHit);
				if (what == 2) {
					Vehicle toBlow = entityHit;
					VEHICLE::EXPLODE_VEHICLE(toBlow, true, false);
					WAIT(1000);
				}
			}
		}
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	main();
}