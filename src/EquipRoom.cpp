#include "stdafx.h"

#include "Level.h"
#include "Room.h"
#include "EquipRoom.h"
#include "SpacePlayer.h"
#include "Part.h"
#include "CompletePart.h"
#include "GlobalVariables.h"
#include "RoomGameplayState.h"
#include "Skill.h"
#include "tinyxml.h"

namespace capstone {

	/**
	 * EquipRoom constructor.
	 */
	EquipRoom::EquipRoom() 
		: Room()
		, fileName("data/parts.xml")
		, mCamera(NULL)
		, mLight(NULL)
		, sceneMgr(NULL)
		, mPlayer(NULL)
	{
		srand((unsigned)time(NULL));
		init();
		createPlayer(sceneMgr);
		createTerrain(sceneMgr);
		//placeParts(sceneMgr);
		createFlares();
	}

	/**
	 * EquipRoom destructor.
	 */
	EquipRoom::~EquipRoom() {

		// Delete dynamically allocated objects.
		delete mPlayer;

		// Destroy the scene manager, which automatically destroys everything.
        if (sceneMgr)
            Ogre::Root::getSingleton().destroySceneManager(sceneMgr);

		for (unsigned i = 0; i < flareList.size(); i++) 
			delete flareList[i];

		deleteAllParts();

	}


// =====================================================================================
//
// #Initialization
//
// =====================================================================================

	/**
	 * This function will initialize the level's properties.
	 */
	void EquipRoom::init() {

		Level::shouldFinish = false;

		// Get the Ogre root.
		Ogre::Root& ogreRoot = Ogre::Root::getSingleton();

        // Instantiate the scene manager.
        sceneMgr = ogreRoot.createSceneManager(Ogre::ST_GENERIC);

        // Set ambient light.
        sceneMgr->setAmbientLight(Ogre::ColourValue(0.25f, 0.25f, 0.25f));

        // Create a directional light.
        mLight = sceneMgr->createLight();
        mLight->setType(Ogre::Light::LT_DIRECTIONAL);
        mLight->setDirection(Ogre::Vector3(-1, -2, -1).normalisedCopy());

		// Instantiate the camera.
        mCamera = sceneMgr->createCamera("PlayerCam");
        mCamera->setNearClipDistance(1);
        mCamera->setFarClipDistance(5000);
        mCamera->setAutoAspectRatio(true);
        mCamera->setPosition(0, 2, 0);

	}

	/**
	 * This function creates the player on the planet.
	 *
	 * @param sceneMgr			pointer to the scene manager to create entities
	 *
	 */
	void EquipRoom::createPlayer(Ogre::SceneManager* sceneMgr) {
		
		// Create new player object.
		mPlayer = new SpacePlayer(mCamera, this);

	}

	/**
	 * This function creates the terrain and skybox inside the room.
	 *
	 * @param sceneMgr			pointer to the scene manager to create entities
	 *
	 */
	void EquipRoom::createTerrain(Ogre::SceneManager* sceneMgr) {

		// Create the skybox.
		sceneMgr->setSkyBox(true, "TronBox", 10, true);

		// Create terrain.
		Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
		Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			plane, 5000, 5000, 50, 50, true, 1, 20, 20, Ogre::Vector3::UNIT_Z);

		Ogre::Entity* entGround = sceneMgr->createEntity("Tron", "ground");
		sceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround);

		entGround->setMaterialName("Tron");
		//entGround->setCastShadows(false);

	}

	/**
	 * This function will creates parts and places them on the grid.
	 *
	 * @param sceneMgr			pointer to the scene manager to create entities
	 *
	 */
	void EquipRoom::placeParts(Ogre::SceneManager* sceneMgr){

		parts.clear();

		TiXmlDocument doc;

		// Determine if the XML file exists.
		if(!doc.LoadFile(fileName)) {
			std::exception("XML Error: parts.xml is missing.");
		}
		
		// Determine if the document has a root element.
		TiXmlElement* root = doc.FirstChildElement();
		if(root == NULL) {
			std::exception("XML Error: No root element in parts.xml");
			doc.Clear();
		}

		// Begin loop.
		for (TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {
			
			// Grab the element name and attribute.
			std::string elemName = elem->Value();
			
			// Melee parts
			if (elemName == "melpart") {

				// Get attributes.
				const char* part = elem->Attribute("name");
				const char* mesh = elem->Attribute("mesh");
				const char* type = elem->Attribute("type");
				const char* stat = elem->Attribute("stat");
				const char* skill = elem->Attribute("skill");

				// Convert attribute values to string.
				Ogre::String partName = part;
				Ogre::String meshName = mesh;
				Ogre::String partSkill = skill;

				// Convert attribute values from string to int.
				int statIncrease = charToInt(stat);
				int partType = charToInt(type);

				Ogre::Entity* ent = sceneMgr->createEntity(meshName);
				Ogre::SceneNode* node = sceneMgr->getRootSceneNode()->createChildSceneNode();
				node->yaw(Ogre::Degree(90));
				Ogre::Real scale = 0.6f;
				node->setScale(scale,scale,scale);

				// New part object that will be pushed into the list.
				Part* p = new CompletePart(partName, ent, partType, node, statIncrease, partSkill);
				parts.push_back(p);

			}

			// Projectile parts.
			else if(elemName == "projpart") {

				// Get attributes.
				const char* part = elem->Attribute("name");
				const char* mesh = elem->Attribute("mesh");
				const char* type = elem->Attribute("type");
				const char* stat = elem->Attribute("stat");
				const char* skill = elem->Attribute("skill");
				const char* size = elem->Attribute("size");
				const char* mat = elem->Attribute("material");
				const char* speed = elem->Attribute("speed");
				const char* accel = elem->Attribute("accel");
				const char* r = elem->Attribute("r");
				const char* g = elem->Attribute("g");
				const char* b = elem->Attribute("b");
				const char* life = elem->Attribute("life");

				// Convert attribute values to string.
				Ogre::String partName = part;
				Ogre::String meshName = mesh;
				Ogre::String partSkill = skill;
				Ogre::String material = mat;

				// Convert attribute values from string to int.
				int statIncrease = charToInt(stat);
				int partType = charToInt(type); 
				int projSize = charToInt(size);
				int projSpeed = charToInt(speed);
				int projAccel = charToInt(accel);
				int red = charToInt(r);
				int green = charToInt(g);
				int blue = charToInt(b);
				int projLife = charToInt(life);
				// end of int conversions

				Ogre::Entity* ent = sceneMgr->createEntity(meshName);
				Ogre::SceneNode* node = sceneMgr->getRootSceneNode()->createChildSceneNode();

				// New part object that will be pushed into the list.
				Part* p = new CompletePart(partName, ent, partType, node, statIncrease, partSkill, 
					projSize, material, projSpeed, projAccel, Ogre::ColourValue(red, green, blue), projLife);
				parts.push_back(p);

			}

			// Passive parts
			else if(elemName == "paspart"){

				// Get attributes.
				const char* part = elem->Attribute("name");
				const char* mesh = elem->Attribute("mesh");
				const char* type = elem->Attribute("type");
				const char* stat = elem->Attribute("stat");
				const char* skill = elem->Attribute("skill");

				// Convert attribute values to string.
				Ogre::String partName = part;
				Ogre::String meshName = mesh;
				Ogre::String partSkill = skill;

				// Convert attribute values from string to int.
				std::string s_statIncrease = stat;
				int statIncrease = atoi(s_statIncrease.c_str());
				std::string s_partType = type;
				int partType = atoi(s_partType.c_str());

				Ogre::Entity* ent = sceneMgr->createEntity(meshName);
				Ogre::SceneNode* node = sceneMgr->getRootSceneNode()->createChildSceneNode();
				node->yaw(Ogre::Degree(90));
				Ogre::Real scale = 0.6f;
				node->setScale(scale,scale,scale);

				// New part object that will be pushed into the list.
				Part* p = new CompletePart(partName, ent, partType, node, statIncrease, partSkill);
				parts.push_back(p);

			}
		}

		doc.Clear();


		// Place the parts.
		Ogre::Real set = 50;
		unsigned zMod = 0;
		unsigned xMod = 0;

		// Place objects onto the floor.
		for(unsigned i = 0; i < parts.size(); i++) {

			// Create a scene node for the part.
			Ogre::Entity* ent = parts[i]->getEntity();
			Ogre::SceneNode* sn = parts[i]->getSceneNode();
			sn->attachObject(ent);

			// Position part.
			sn->setPosition((220 - set * xMod),5,(-220 + set * zMod));

			zMod++;

			if(zMod >= 9) {
				xMod++;
				zMod = 0;
			}			
		}
	}

	/**
	 * This function will build a specfic part that the user has requested.
	 *
	 * @param part			pointer to the part that needs to be made
	 *
	 */
	void EquipRoom::buildPart(Part* part) {


	}

	void EquipRoom::deleteAllParts(){

		for (unsigned i = 0; i < parts.size(); i++){ 
			delete parts[i];
			parts[i] = NULL;
			parts.shrink_to_fit();
		}
	}

	/**
	 * This function will create the flares in the factory room.
	 */
	void EquipRoom::createFlares() {

		for (int i = 0; i < 4; i++) {
			RoomFlare* flare = new RoomFlare(this, Ogre::Vector3(0,20,0));
			flareList.push_back(flare);
		}

	}


// =====================================================================================
//
// #Update/Render
//
// =====================================================================================

	/**
	 * This function will update the planet level.
	 *
	 * delatT - time since last frame
	 *
	 */
	void EquipRoom::update(Ogre::Real deltaT) {

		// Update flares.
		for (unsigned i = 0; i < flareList.size(); i++) 
			flareList[i]->update(deltaT);

		// Update the player.
		mPlayer->update(deltaT);

		// Set minimum distance.
		Ogre::Real minDist = 5000;

		// Update all parts.
		for (unsigned i = 0; i < parts.size(); i++) {

			const Ogre::Vector3& partPos = parts[i]->getSceneNode()->getPosition();
			const Ogre::Vector3& playerPos = mPlayer->getSceneNode()->getPosition();

			// Get the distance between of the part and the player.
			Ogre::Vector3 toPlayer = playerPos - partPos;
			Ogre::Real dist = toPlayer.length();

			if (dist < minDist) {
				closestPart = parts[i];
				minDist = dist;
			}

			Ogre::Real dx = partPos.x - playerPos.x;
			Ogre::Real dz = partPos.z - playerPos.z;
			Ogre::Real distSq = dx*dx + dz*dz;
			Ogre::Real radSq = parts[i]->getRadius() * parts[i]->getRadius();

			if (distSq <= radSq)
				parts[i]->setPlayerClose(true);
			else
				parts[i]->setPlayerClose(false);

			// Constantly check if any of the part entities are attached to anything.
			// If not, attach it to its corresponding scene node on the plane.
			if (!parts[i]->getEntity()->isAttached()) {
				parts[i]->getSceneNode()->attachObject(parts[i]->getEntity());
				parts[i]->setIsEquipped(false);
			}

		}

	}


// =====================================================================================
//
// #Event Listeners
//
// =====================================================================================

	/**
	 * This function will return the player's keyboard listener.
	 */
	OIS::KeyListener* EquipRoom::getKeyListener() {
		return mPlayer;
	}

	/**
	 * This function will return the player's mouse listener.
	 */
	OIS::MouseListener* EquipRoom::getMouseListener() {
		return mPlayer;
	}

 }