#include "stdafx.h"

#include "PlanetPlayer.h"
#include "Levels.h"
#include "Buildings.h"
#include "tinyxmls.h"
#include "GlobalVariables.h"

namespace capstone {

	// Initialize static variables.
	const char* PlanetLevel::planetFilename = "editor/planet.xml";

	/**
	 * PlanetLevel constructor.
	 *
	 * @param radius		the radius of the planet
	 *
	 */
	PlanetLevel::PlanetLevel(Ogre::Real radius) 
		: sceneMgr(NULL)
		, sphereNode(NULL)
		, mPlayer(NULL)
		, planetRadius(radius)
		, oIndex(0)
	{
		init();
		createWorld();
		populateWorld();
		createPlayer(sceneMgr);
		createTerrain(sceneMgr);
	}

	/**
	 * PlanetLevel destructor.
	 */
	PlanetLevel::~PlanetLevel() {

		// Delete dynamically allocated objects.
		delete mPlayer;

		// Destroy the scene manager, which automatically destroys everything.
		if (sceneMgr)
            Ogre::Root::getSingleton().destroySceneManager(sceneMgr);

		for (unsigned i = 0; i < buildingList.size(); i++)
			delete buildingList[i];

		for (unsigned i = 0; i < propList.size(); i++)
			delete propList[i];

	}


//=====================================================================================
//
// #Initialization
//
//=====================================================================================

	/**
	 * This function will initialize the level's properties.
	 */
	void PlanetLevel::init() {

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
	 * This function will create the sphere for the world.
	 */
	void PlanetLevel::createWorld() {

		// Create sphere.
		createSphere("SphereMesh", planetRadius, 200, 200);

		// Create a scene node from the scene manager.
		sphereNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
		//sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

		// Create object and attach it to the scene node.
		Ogre::Entity* sphereEntity = sceneMgr->createEntity("SphereEntity", "SphereMesh");
		sphereEntity->setMaterialName("PlanetGround");
		//sphereEntity->setCastShadows(true);
		sphereNode->attachObject(sphereEntity);

	}

	/**
	 * This function will create the sphere from scratch.
	 *
	 * @param strName			name of the sphere mesh
	 * @param r					radius of the planet itself
	 * @param nRings			number of rings for the sphere
	 * @pararm nSegments		number of segments that the planet will be broken into
	 *
	 */
	void PlanetLevel::createSphere(const std::string& strName, const float r, const int nRings, const int nSegments) {

		Ogre::MeshPtr pSphere = Ogre::MeshManager::getSingleton().createManual(strName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		Ogre::SubMesh *pSphereVertex = pSphere->createSubMesh();
 
		pSphere->sharedVertexData = new Ogre::VertexData();
		Ogre::VertexData* vertexData = pSphere->sharedVertexData;
 
		// define the vertex format
		Ogre::VertexDeclaration* vertexDecl = vertexData->vertexDeclaration;
		size_t currOffset = 0;
		
		// positions
		vertexDecl->addElement(0, currOffset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
		currOffset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
		
		// normals
		vertexDecl->addElement(0, currOffset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
		currOffset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
		
		// two dimensional texture coordinates
		vertexDecl->addElement(0, currOffset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 0);
		currOffset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);
 
		// allocate the vertex buffer
		vertexData->vertexCount = (nRings + 1) * (nSegments+1);
		Ogre::HardwareVertexBufferSharedPtr vBuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(vertexDecl->getVertexSize(0), vertexData->vertexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
		Ogre::VertexBufferBinding* binding = vertexData->vertexBufferBinding;
		binding->setBinding(0, vBuf);
		float* pVertex = static_cast<float*>(vBuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));
 
		// allocate index buffer
		pSphereVertex->indexData->indexCount = 6 * nRings * (nSegments + 1);
		pSphereVertex->indexData->indexBuffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(Ogre::HardwareIndexBuffer::IT_16BIT, pSphereVertex->indexData->indexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
		Ogre::HardwareIndexBufferSharedPtr iBuf = pSphereVertex->indexData->indexBuffer;
		unsigned short* pIndices = static_cast<unsigned short*>(iBuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));
 
		float fDeltaRingAngle = (Ogre::Math::PI / nRings);
		float fDeltaSegAngle = (2 * Ogre::Math::PI / nSegments);
		unsigned short wVerticeIndex = 0 ;
 
		// Generate the group of rings for the sphere
		for( int ring = 0; ring <= nRings; ring++ ) {
			float r0 = r * sinf (ring * fDeltaRingAngle);
			float y0 = r * cosf (ring * fDeltaRingAngle);
 
			// Generate the group of segments for the current ring
			for(int seg = 0; seg <= nSegments; seg++) {
				float x0 = r0 * sinf(seg * fDeltaSegAngle);
				float z0 = r0 * cosf(seg * fDeltaSegAngle);
 
				// Add one vertex to the strip which makes up the sphere
				*pVertex++ = x0;
				*pVertex++ = y0;
				*pVertex++ = z0;
			
				Ogre::Vector3 vNormal = Ogre::Vector3(x0, y0, z0).normalisedCopy();
				*pVertex++ = vNormal.x;
				*pVertex++ = vNormal.y;
				*pVertex++ = vNormal.z;
 
				*pVertex++ = (float) seg / (float) nSegments;
				*pVertex++ = (float) ring / (float) nRings;
 
				if (ring != nRings) {
					// each vertex (except the last) has six indices pointing to it
					*pIndices++ = wVerticeIndex + nSegments + 1;
					*pIndices++ = wVerticeIndex;               
					*pIndices++ = wVerticeIndex + nSegments;
					*pIndices++ = wVerticeIndex + nSegments + 1;
					*pIndices++ = wVerticeIndex + 1;
					*pIndices++ = wVerticeIndex;
					wVerticeIndex ++;
				}
			}; // end for seg
		} // end for ring
 
		// Unlock
		vBuf->unlock();
		iBuf->unlock();
		 
		// Generate face list
		pSphereVertex->useSharedVertices = true;
 
		// the original code was missing this line:
		pSphere->_setBounds( Ogre::AxisAlignedBox( Ogre::Vector3(-r, -r, -r), Ogre::Vector3(r, r, r) ), false );
		pSphere->_setBoundingSphereRadius(r);
		
		// this line makes clear the mesh is loaded (avoids memory leaks)
		pSphere->load();
	  }

	/**
	 * This function will populate the planet with game objects from an XML file.
	 */
	void PlanetLevel::populateWorld() {

		// New room buildings.
		InteractiveBuilding* b1 = new EquipmentBuilding(this, Ogre::Vector3(planetRadius-3,0,0));
		buildingList.push_back(b1);
		InteractiveBuilding* b2 = new FactoryBuilding(this, Ogre::Vector3(-planetRadius+3,0,0));
		buildingList.push_back(b2);
		InteractiveBuilding* b3 = new LaunchBuilding(this, Ogre::Vector3(0,planetRadius,0));
		buildingList.push_back(b3);

		// TinyXML document object.
		TiXmlDocument doc;

		// Determine if the XML file exists.
		if(!doc.LoadFile(planetFilename)) {
			std::exception("XML Error: levels.xml is missing.");
		}

		// Determine if the document has a root element.
		TiXmlElement* root = doc.FirstChildElement();

		if (root == NULL) {
			std::exception("XML Error: No root element in levels.xml");
			doc.Clear();
		}

		// Begin loop.
		for (TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {

			// Grab the element name.
			std::string elemName = elem->Value();
			
			// For each object load it on the planet.
			if (elemName == "object") {

				// Grab the object's properites/attributes.
				std::string meshName = elem->Attribute("mesh");
				std::string material = elem->Attribute("material");

				float scale = atof(elem->Attribute("scale"));
				Ogre::Degree pitch(atof(elem->Attribute("pitch")));
				Ogre::Degree yaw(atof(elem->Attribute("yaw")));
				Ogre::Degree roll(atof(elem->Attribute("roll")));

				// Set data structure.
				PlanetObjectStruct* objStruct = new PlanetObjectStruct;
				objStruct->meshName = meshName;
				objStruct->materialName = material;
				objStruct->scale = scale;
				objStruct->pitch = pitch;
				objStruct->yaw = yaw;
				objStruct->roll = roll;

				// New object.
				PlanetProp* p = new PlanetProp(this, objStruct);
				propList.push_back(p);

			}

		}

	}

	/**
	 * This function creates the player on the planet.
	 * 
	 * @param sceneMgr			pointer to the scene manager to create entities
	 *
	 */
	void PlanetLevel::createPlayer(Ogre::SceneManager* sceneMgr) {
		
		// Create new player object.
		mPlayer = new PlanetPlayer(mCamera, this);

	}

	/**
	 * This function creates the terrain and skybox on the planet.
	 * 
	 * @param sceneMgr			pointer to the scene manager to create entities
	 *
	 */
	void PlanetLevel::createTerrain(Ogre::SceneManager* sceneMgr) {
		// Create the skybox.
		sceneMgr->setSkyBox(true, "SpaceBox", 10, true);
	}


// =====================================================================================
//
// #Update/Render
//
// =====================================================================================

	/**
	 * This function will update the planet level.
	 * 
	 * @param delatT			time since last frame
	 *
	 */
	void PlanetLevel::update(Ogre::Real deltaT) {
		mPlayer->update(deltaT);
		closestInteractiveObject();
	}

	/**
	 * This function determines the closest interactive object to the player.
	 */
	void PlanetLevel::closestInteractiveObject() {

		// Set minimum distance.
		Ogre::Real minDist = planetRadius * planetRadius;

		// Loop through all buildings.
		for (unsigned i = 0; i < buildingList.size(); i++) {

			// Get vector positions.
			Ogre::Vector3 buildingPos = buildingList[i]->getSceneNode()->getPosition();
			Ogre::Vector3 playerPos = mPlayer->getSceneNode()->_getDerivedPosition();

			// Get the distance.
			Ogre::Vector3 toPlayer = playerPos - buildingPos;
			Ogre::Real distance = toPlayer.length();

			// Determine the lowest value.
			if (distance < minDist) {
				minDist = distance;
				oIndex = i;
			}

		}

		// Determine if the player is close enough to interact with the building.
		if (minDist <= buildingList[0]->getRadius()) 
			buildingList[oIndex]->setPlayerClose(true);
		
		else 
			buildingList[oIndex]->setPlayerClose(false);
		

	}

}