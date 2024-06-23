#include "stdafx.h"

#include "Game.h"
#include "Levels.h"
#include "GlobalVariables.h"

namespace capstone {

	/**
	 * PlanetLevel constructor.
	 * 
	 * @param game				pointer to the game application
	 *
	 */
	PlanetLevel::PlanetLevel(Game* game) 
		: Level(game)
		, sceneMgr(NULL)
		, sphereNode(NULL)
		, mController(NULL)
	{
		// Get the scene manager from the parent class.
		sceneMgr = Level::getSceneManager();

		// Setup the planet level.
		createWorld(sceneMgr);
		createTerrain(sceneMgr);
		setupController();
	}

	/**
	 * PlanetLevel destructor.
	 */
	PlanetLevel::~PlanetLevel() {

		// Delete buildings.
		for (unsigned i = 0; i < buildingList.size(); i++)
			delete buildingList[i];

		// Destroy scene nodes.
		sceneMgr->destroySceneNode(sphereNode);

		// Destroy the scene manager, which automatically destroys everything.
        if (sceneMgr)
            Ogre::Root::getSingleton().destroySceneManager(sceneMgr);

	}


//=====================================================================================
//
// #Initialization
//
//=====================================================================================

	/**
	 * This function will create the sphere for the world.
	 *
	 * @param sceneMgr			pointer to the scene manager to create objects
	 *
	 */
	void PlanetLevel::createWorld(Ogre::SceneManager* sceneMgr) {

		// Create sphere.
		createSphere("SphereMesh", mRadius, 200, 200);

		// Create a scene node from the scene manager.
		sphereNode = sceneMgr->getRootSceneNode()->createChildSceneNode();

		// Create object and attach it to the scene node.
		Ogre::Entity* sphereEntity = sceneMgr->createEntity("SphereEntity", "SphereMesh");
		sphereEntity->setMaterialName("PlanetGround");
		sphereNode->attachObject(sphereEntity);
		sphereNode->setPosition(0,0,0);

		// Buildings for the planet.
		InteractiveBuilding* b1 = new LaunchBuilding(this);
		buildingList.push_back(b1);
		InteractiveBuilding* b2 = new FactoryBuilding(this);
		buildingList.push_back(b2);
		InteractiveBuilding* b3 = new EquipmentBuilding(this);
		buildingList.push_back(b3);

	}

	/**
	 * This function creates the terrain and skybox on the planet.
	 *
	 * @param sceneMgr			pointer to the scene manager to create objects
	 *
	 */
	void PlanetLevel::createTerrain(Ogre::SceneManager* sceneMgr) {

		// Create the skybox.
		sceneMgr->setSkyBox(true, "SpaceBox", 10, true);

	}

	/**
	 * This function will create the sphere for the scene.
	 *
	 * @param strName			name for this sphere entity mesh
	 * @param r					the radius of the sphere
	 * @param nRings			number of rings to be created
	 * @param nSegments			number of segments for the sphere to be made up of
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
	 * This function will setup the controller for the input handling.
	 */
	void PlanetLevel::setupController() {

		// Create the editor controller.
		mController = new EditorController(this, mCamera, sceneMgr);

	}


//=====================================================================================
//
// #Update/Render
//
//=====================================================================================

	/**
	 * This function will update the planet level.
	 *
	 * @param delatT			time since last frame
	 *
	 */
	void PlanetLevel::update(Ogre::Real deltaT) {
		mController->update(deltaT);
	}


//=====================================================================================
//
// #I/O Event Handlers
//
//=====================================================================================

	/**
	 * This function will return the player's keyboard listener.
	 */
	OIS::KeyListener* PlanetLevel::getKeyListener() {
		return mController;
	}

	/**
	 * This function will return the player's mouse listener.
	 */
	OIS::MouseListener* PlanetLevel::getMouseListener() {
		return mController;
	}

}