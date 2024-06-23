#include "stdafx.h"

#include "Level.h"
#include "Room.h"
#include "FactoryRoom.h"
#include "SpacePlayer.h"
#include "Part.h"
#include "Blueprint.h"
#include "GlobalVariables.h"
#include "RoomGameplayState.h"
#include "Skill.h"
#include "tinyxml.h"

namespace capstone {

	/**
	 * FactoryRoom constructor.
	 */
	FactoryRoom::FactoryRoom() 
		: Room()
		, blueprintsFile("data/blueprints.xml")
		, partsFile("data/parts.xml")
		, mCamera(NULL)
		, mLight(NULL)
		, sceneMgr(NULL)
		, mPlayer(NULL)
	{
		srand((unsigned)time(NULL));
		init();
		createPlayer(sceneMgr);
		createTerrain(sceneMgr);
		placeParts(sceneMgr);
		createFlares();
	}

	/**
	 * FactoryRoom destructor.
	 */
	FactoryRoom::~FactoryRoom() {

		// Delete dynamically allocated objects.
		delete mPlayer;

		// Destroy the scene manager, which automatically destroys everything.
        if (sceneMgr)
            Ogre::Root::getSingleton().destroySceneManager(sceneMgr);

		for (unsigned i = 0; i < flareList.size(); i++) 
			delete flareList[i];

		for (unsigned i = 0; i < parts.size(); i++) 
			delete parts[i];
		
	}


// =====================================================================================
//
// #Initialization
//
// =====================================================================================

	/**
	 * This function will initialize the level's properties.
	 */
	void FactoryRoom::init() {

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
		mLight->setAttenuation(500, 1, 0, 0);

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
	void FactoryRoom::createPlayer(Ogre::SceneManager* sceneMgr) {
		
		// Create new player object.
		mPlayer = new SpacePlayer(mCamera, this);

	}

	/**
	 * This function creates the terrain and skybox inside the room.
	 *
	 * @param sceneMgr			pointer to the scene manager to create entities
	 *
	 */
	void FactoryRoom::createTerrain(Ogre::SceneManager* sceneMgr) {

		// Create the skybox.
		sceneMgr->setSkyBox(true, "Lightgrid", 10, true);

		// Create terrain.
		Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
		Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			plane, 5000, 5000, 50, 50, true, 1, 20, 20, Ogre::Vector3::UNIT_Z);

		Ogre::Entity* entGround = sceneMgr->createEntity("Lightgrid_Floor", "ground");
		sceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround);

		entGround->setMaterialName("GLightgrid");
		//entGround->setCastShadows(false);

	}

	/**
	 * This function will creates parts and places them on the grid.
	 *
	 * @param sceneMgr			pointer to the scene manager to create entities
	 *
	 */
	void FactoryRoom::placeParts(Ogre::SceneManager* sceneMgr){

		TiXmlDocument doc;

		// Determine if the XML file exists.
		if(!doc.LoadFile(blueprintsFile)) {
			std::exception("XML Error: blueprints.xml is missing.");
		}

		// Determine if the document has a root element.
		TiXmlElement* root = doc.FirstChildElement();
		if(root == NULL) {
			std::exception("XML Error: No root element in blueprints.xml");
			doc.Clear();
		}

		// Begin loop.
		for (TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {

			// Grab the element name and attribute.
			std::string elemName = elem->Value();

			// Melee part blueprint.
			if(elemName == "melbprint"){

				// Get attributes.
				const char* part = elem->Attribute("name");
				const char* mesh = elem->Attribute("mesh");
				const char* type = elem->Attribute("type");
				const char* stat = elem->Attribute("stat");
				const char* skill = elem->Attribute("skill");
				const char* metal = elem->Attribute("metal");
				const char* dm = elem->Attribute("darkmatter");
				const char* carbon = elem->Attribute("carbon");

				// Convert attribute values to string.
				Ogre::String partName = part;
				Ogre::String meshName = mesh;
				Ogre::String partSkill = skill;

				// Convert attribute values from string to int.
				int statIncrease = charToInt(stat);
				int partType = charToInt(type);
				int reqMetal = charToInt(metal);
				int reqDarkMatter = charToInt(dm);
				int reqCarbon = charToInt(carbon);

				Ogre::Entity* ent = sceneMgr->createEntity(meshName);
				Ogre::SceneNode* node = sceneMgr->getRootSceneNode()->createChildSceneNode();
				node->yaw(Ogre::Degree(90));
				Ogre::Real scale = 0.6f;
				node->setScale(scale,scale,scale);

				// New part object which represents a blueprint that will be pushed into the list.
				Part* p = new Blueprint(partName, mesh, ent, partType, node, statIncrease, partSkill, 
					reqMetal, reqDarkMatter, reqCarbon);
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
				const char* metal = elem->Attribute("metal");
				const char* dm = elem->Attribute("darkmatter");
				const char* carbon = elem->Attribute("carbon");

				// Convert attribute values to string.
				Ogre::String partName = part;
				Ogre::String meshName = mesh;
				Ogre::String partSkill = skill;
				Ogre::String material = mat;

				// Convert attribute values from string to int and floats.
				int statIncrease = charToInt(stat);
				int partType = charToInt(type);
				int projSize = charToInt(size);
				int projSpeed = charToInt(speed);
				int projAccel = charToInt(accel);
				int red = charToInt(r);
				int green = charToInt(g);
				int blue = charToInt(b);
				int projLife = charToInt(life);
				int reqMetal = charToInt(metal);
				int reqDarkMatter = charToInt(dm);
				int reqCarbon = charToInt(carbon);

				Ogre::Entity* ent = sceneMgr->createEntity(meshName);
				Ogre::SceneNode* node = sceneMgr->getRootSceneNode()->createChildSceneNode();

				// New part object that will be pushed into the list.
				Part* p = new Blueprint(partName, meshName, ent, 
					partType, node, statIncrease, partSkill, reqMetal, 
					reqDarkMatter, reqCarbon, projSize, 
					material, projSpeed, projAccel, 
					Ogre::ColourValue(red, green, blue), 
					projLife);

				parts.push_back(p);

			}

		}

		doc.Clear();

		// Place the parts.
		Ogre::Real set = 50;
		unsigned zMod = 0;
		unsigned xMod = 0;

		// Place the part onto the scene in a grid format.
		for (unsigned i=0; i < parts.size(); i++) {

			Ogre::Entity* ent = parts[i]->getEntity();
			Ogre::SceneNode* sn = parts[i]->getSceneNode();
			sn->attachObject(ent);

			sn->setPosition((220-set*xMod),5,(-220+set*zMod));

			zMod++;

			if (zMod >= 9) {
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
	void FactoryRoom::buildPart(Part* part) {

		// Getting the approprate element and skillType to be printed to the xml doc.
		switch(part->getSkillType()){
			case MELEE_SKILL:
				buildMeleePart(part);
				break;

			case PROJECTILE_SKILL:
				buildProjectilePart(part);
				break;

			case PASSIVE_SKILL:
				buildPassivePart(part);
				break;
		}
	}

	/**
	 * This function will create the flares in the factory room.
	 */
	void FactoryRoom::createFlares() {

		//for (int i = 0; i < 5; i++) {
		//	RoomFlare* flare = new RoomFlare(this, Ogre::Vector3(0,20,0));
		//	flareList.push_back(flare);
		//}

	}

	void FactoryRoom::buildMeleePart(Part* part){

		const char* element = "melpart";
		const char* partName = "";
		const char* meshName = "";
		const char* partType = "";
		const char* stat = "";
		const char* skillName = "";
		const char* skillType = "0";

		// Converting part name to a proper string.
		std::ostringstream pn;
		pn << part->getName();
		std::string pName = pn.str();
		partName = pName.c_str();

		// Converting part type to a string.
		std::ostringstream p;
		p << part->getPartType();
		std::string pType = p.str();
		partType = pType.c_str();

		// Converting mesh name to a proper string.
		std::ostringstream mn;
		mn << part->getMeshName();
		std::string mName = mn.str();
		meshName = mName.c_str();

		// Converting stat increase to a string.
		std::ostringstream s;
		s << part->getStatIncrease();
		std::string statIn = s.str();
		stat = statIn.c_str();

		// Converting mesh name to a proper string.
		std::ostringstream sn;
		sn << part->getSkill()->getName();
		std::string sName = sn.str();
		skillName = sName.c_str();

		TiXmlDocument doc;

		// Determine if the XML file exists.
		if(!doc.LoadFile(partsFile)) {
			std::exception("XML Error: parts.xml is missing.");
		}
		
		// Determine if the document has a root element.
		TiXmlElement* root = doc.FirstChildElement();
		if(root == NULL) {
			std::exception("XML Error: No root element in parts.xml");
			doc.Clear();
		}

		TiXmlElement* e = new TiXmlElement(element);
		e->SetAttribute("name", partName);
		e->SetAttribute("mesh", meshName);
		e->SetAttribute("type", partType);
		e->SetAttribute("stat", stat);
		e->SetAttribute("skill", skillName);
		e->SetAttribute("stype", skillType);
		root->LinkEndChild(e);
		
		doc.SaveFile(partsFile);
		doc.Clear();
	}

	void FactoryRoom::buildProjectilePart(Part* part){

		const char* element = "projpart";
		const char* partName = "";
		const char* meshName = "";
		const char* partType = "";
		const char* stat = "";
		const char* skillName = "";
		const char* skillType = "1";
		const char* size = "";
		const char* material = "";
		const char* speed = "";
		const char* accel = "";
		const char* r = "";
		const char* g = "";
		const char* b = "";
		const char* life = "";

		// Converting skill name to a proper string.
		std::ostringstream skName;
		skName << part->getSkill()->getName();
		std::string skiName = skName.str();
		skillName = skiName.c_str();

		// Converting part name to a proper string.
		std::ostringstream pn;
		pn << part->getName();
		std::string pName = pn.str();
		partName = pName.c_str();

		// Converting part type to a string.
		std::ostringstream p;
		p << part->getPartType();
		std::string pType = p.str();
		partType = pType.c_str();

		// Converting material name to a string.
		std::ostringstream mat;
		mat << part->getMaterial();
		std::string mMaterial = mat.str();
		material = mMaterial.c_str();

		// Converting mesh name to a proper string.
		std::ostringstream mn;
		mn << part->getMeshName();
		std::string mName = mn.str();
		meshName = mName.c_str();

		// Converting stat increase to a string.
		std::ostringstream st;
		st << part->getStatIncrease();
		std::string statIn = st.str();
		stat = statIn.c_str();

		// Converting size to a string.
		std::ostringstream si;
		si << part->getSize();
		std::string s_size = si.str();
		size = s_size.c_str();

		// Converting speed to a string.
		std::ostringstream sp;
		sp << part->getSpeed();
		std::string s_sp = sp.str();
		speed = s_sp.c_str();

		// Converting acceleration to a string.
		std::ostringstream ac;
		ac << part->getAccel();
		std::string s_ac = ac.str();
		accel = s_ac.c_str();

		// Converting red to a string.
		std::ostringstream red;
		red << part->getRed();
		std::string s_red = red.str();
		r = s_red.c_str();

		// Converting green to a string.
		std::ostringstream green;
		green << part->getGreen();
		std::string s_green = green.str();
		g = s_green.c_str();

		// Converting blue to a string.
		std::ostringstream blue;
		blue << part->getBlue();
		std::string s_blue = blue.str();
		b = s_blue.c_str();

		// Converting life to a string.
		std::ostringstream li;
		li << part->getLifeSpan();
		std::string s_li = li.str();
		life = s_li.c_str();

		TiXmlDocument doc;

		// Determine if the XML file exists.
		if(!doc.LoadFile(partsFile)) {
			std::exception("XML Error: parts.xml is missing.");
		}
		
		// Determine if the document has a root element.
		TiXmlElement* root = doc.FirstChildElement();
		if(root == NULL) {
			std::exception("XML Error: No root element in parts.xml");
			doc.Clear();
		}

		TiXmlElement* e = new TiXmlElement(element);
		e->SetAttribute("name", partName);
		e->SetAttribute("mesh", meshName);
		e->SetAttribute("type", partType);
		e->SetAttribute("stat", stat);
		e->SetAttribute("skill", skillName);
		e->SetAttribute("stype", skillType);
		e->SetAttribute("size", size);
		e->SetAttribute("material", material);
		e->SetAttribute("speed", speed);
		e->SetAttribute("accel", accel);
		e->SetAttribute("r", r);
		e->SetAttribute("g", g);
		e->SetAttribute("b", b);
		e->SetAttribute("life", life);
		root->LinkEndChild(e);
		
		doc.SaveFile(partsFile);
		doc.Clear();

	}

	void FactoryRoom::buildPassivePart(Part* part){

		const char* element = "paspart";
		const char* partName = part->getName().c_str();
		const char* meshName = part->getMeshName().c_str();
		const char* partType = "";
		const char* stat = "";
		const char* skillName = part->getSkill()->getName().c_str();
		const char* skillType = "2";

		// Converting part type to a string.
		std::ostringstream p;
		p << part->getPartType();
		std::string pType = p.str();
		partType = pType.c_str();

		// Converting stat increase to a string.
		std::ostringstream s;
		s << part->getStatIncrease();
		std::string statIn = s.str();
		stat = statIn.c_str();

		TiXmlDocument doc;

		// Determine if the XML file exists.
		if(!doc.LoadFile(partsFile)) {
			std::exception("XML Error: parts.xml is missing.");
		}
		
		// Determine if the document has a root element.
		TiXmlElement* root = doc.FirstChildElement();
		if(root == NULL) {
			std::exception("XML Error: No root element in parts.xml");
			doc.Clear();
		}

		TiXmlElement* e = new TiXmlElement(element);
		e->SetAttribute("name", partName);
		e->SetAttribute("mesh", meshName);
		e->SetAttribute("type", partType);
		e->SetAttribute("stat", stat);
		e->SetAttribute("skill", skillName);
		e->SetAttribute("stype", skillType);
		root->LinkEndChild(e);
		
		doc.SaveFile(partsFile);
		doc.Clear();

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
	void FactoryRoom::update(Ogre::Real deltaT) {

		// Update the player.
		mPlayer->update(deltaT);

		// Update flares.
		for (unsigned i = 0; i < flareList.size(); i++) 
			flareList[i]->update(deltaT);

		Ogre::Real minDist = 5000;

		// Update all parts.
		for (unsigned i = 0; i < parts.size(); i++) {

			const Ogre::Vector3& partPos = parts[i]->getSceneNode()->getPosition();
			const Ogre::Vector3& playerPos = mPlayer->getSceneNode()->getPosition();

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

			if(distSq <= radSq)
				parts[i]->setPlayerClose(true);
			else
				parts[i]->setPlayerClose(false);

			// Constantly check if any of the part entities are attached to anything.
			// If not, attach it to its corresponding scene node on the plane.
			if(!parts[i]->getEntity()->isAttached()) {
				parts[i]->getSceneNode()->attachObject(parts[i]->getEntity());
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
	OIS::KeyListener* FactoryRoom::getKeyListener() {
		return mPlayer;
	}

	/**
	 * This function will return the player's mouse listener.
	 */
	OIS::MouseListener* FactoryRoom::getMouseListener() {
		return mPlayer;
	}

 }