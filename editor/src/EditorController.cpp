#include "stdafx.h"

#include "Controllers.h"
#include "Levels.h"
#include "Game.h"
#include "tinyxml.h"

namespace capstone {

	bool EditorController::UAT = true;

	/**
	 * EditorController constructor.
	 *
	 * @param l					pointer to the level to place objects on
	 * @param c					camera pointer to re-adjust the position
	 * @param sm				pointer to the scene manager to create objects
	 *
	 */
	EditorController::EditorController(Level* l, Ogre::Camera* c, Ogre::SceneManager* sm) 
		: BaseController(l, c, sm)
		, wSpeed(0)
		, ySpeed(0)
		, mKeyboard(l->getGame()->getKeyboard())
		, scaleAmount(1)
		, fileName("entites.xml")
		, saveFileName("planet.xml")
	{

		setupMeshes();
		setupEditor();
		listSize = mModelList.size();

		if (UAT) load();

	}

	/**
	 * EditorController destructor.
	 */
	EditorController::~EditorController() {

		// Destroy scene nodes.
		for (unsigned i = 0; i < mNodes.size(); i++)
			delete mNodes[i];

		// Destroy all entites.
		for (unsigned i = 0; i < mModelList.size(); i++)
			delete mModelList[i];

		// Destroy all game objects.
		for (unsigned i = 0; i < mGameObjects.size(); i++)
			delete mGameObjects[i];

	}


//=====================================================================================
//
// #Initialization
//
//=====================================================================================

	/**
	 * This function will load the game objects onto the planet level.
	 */
	void EditorController::load() {

		// XML class object from TinyXML.
		TiXmlDocument doc;

		// Determine if the XML file exists.
		if(!doc.LoadFile(saveFileName)) 
			std::exception("XML Error: planet.xml is missing.");
		
		// Determine if the document has a root element.
		TiXmlElement* root = doc.FirstChildElement();
		if(root == NULL) {
			std::exception("XML Error: No root element in planet.xml");
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

				// New game object.
				GameObject* o = new GameObject(this,meshName,material,scale,pitch,yaw,roll,mSceneMgr);
				mGameObjects.push_back(o);

			}

		}

	}

	/**
	 * This function will setup all the meshes used for the editor.
	 */
	void EditorController::setupMeshes() {

		// XML class object from TinyXML.
		TiXmlDocument doc;

		// Determine if the XML file exists.
		if(!doc.LoadFile(fileName)) 
			std::exception("XML Error: entites.xml is missing.");
		
		// Determine if the document has a root element.
		TiXmlElement* root = doc.FirstChildElement();
		if(root == NULL) {
			std::exception("XML Error: No root element in entites.xml");
			doc.Clear();
		}

		// Begin loop.
		for (TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {

			// Grab the element name and attribute.
			std::string elemName = elem->Value();

			if(elemName == "model") {

				// Grab the object's properites/attributes.
				const char* meshName = elem->Attribute("mesh");
				const char* materialName = elem->Attribute("material");

				// Make a new entity and push it into the vector.
				Ogre::Entity* e = mSceneMgr->createEntity(meshName);
				e->setMaterialName(materialName);
				mModelList.push_back(e);
				mMeshNames.push_back(meshName);
				mMaterials.push_back(materialName);

			}

		}


	}

	/**
	 * This function will setup the level editor.
	 */
	void EditorController::setupEditor() {

		if (mLevel->getPlanetRadius() != NULL) {

			// Set the object on top the planet.
			mParentNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
			mParentNode->setPosition(0,0,0);
			mSceneNode = mParentNode->createChildSceneNode();
			mSceneNode->attachObject(mModelList[0]);
			mSceneNode->setPosition(0, mLevel->getPlanetRadius(), 0);
			currentIndex = 0;

			// Make another scene node that is a child of the parent.
			mCameraPivotNode = mSceneNode->createChildSceneNode();
			mCameraPivotNode->setPosition(0, 150, 0);
			mCameraNode = mCameraPivotNode->createChildSceneNode();
			mCameraNode->setPosition(0, 0, -150);
			mCameraNode->attachObject(mCamera);
			mCamera->lookAt(mSceneNode->getPosition());

			// Push scene nodes into the vector.
			mNodes.push_back(mParentNode);
			mNodes.push_back(mSceneNode);
			mNodes.push_back(mCameraPivotNode);
			mNodes.push_back(mCameraNode);

		}

		else {
			return;
		}

	}


//=====================================================================================
//
// #Update/Rendering
//
//=====================================================================================

	/**
	 * This function will update this controller.
	 *
	 * @param deltaT			time since last frame	
	 *
	 */
	void EditorController::update(Ogre::Real deltaT) {

		// Call the update helper methods.
		updateModel(deltaT);
		updateObjects(deltaT);

	}

	/**
	 * This function will update the controlled 3D model on the scene.
	 */
	void EditorController::updateModel(Ogre::Real deltaT)  {

		// Scaling.
		//if (mKeyboard->isKeyDown(OIS::KC_UP))	scaleModel(true);
		//if (mKeyboard->isKeyDown(OIS::KC_DOWN))	scaleModel(false);

		// Reset to zero every frame.
        wSpeed = 0;
		ySpeed = 0;

		// Get direction based off of keyboard input.
		if (mKeyboard->isKeyDown(OIS::KC_W))	wSpeed += 5000 / mLevel->getPlanetRadius();
		if (mKeyboard->isKeyDown(OIS::KC_S))	wSpeed -= 5000 / mLevel->getPlanetRadius();
        if (mKeyboard->isKeyDown(OIS::KC_A))	ySpeed += 100;
		if (mKeyboard->isKeyDown(OIS::KC_D))	ySpeed -= 100;

		// Get rotation speed.
		walkSpeed = wSpeed;
		yawSpeed = ySpeed;

		// Apply rotations.
		mParentNode->pitch(walkSpeed * deltaT);
		mParentNode->yaw(yawSpeed * deltaT);

	}

	/**
	 * This function will update all of the game objects on the scene.
	 */
	void EditorController::updateObjects(Ogre::Real deltaT)  {
		
		// Go through all game objects.
		for (unsigned i = 0; i < mGameObjects.size(); i++) {

			// Remove object.
			if (mGameObjects[i]->isObjectDisposable()) {
				delete mGameObjects[i];
				mGameObjects[i] = mGameObjects.back();
				mGameObjects.pop_back();
			}

			// Otherwise, update it.
			else {
				mGameObjects[i]->update(deltaT);
			}

		}

	}


//======================================================================================
//
// #Editor Controls
//
//======================================================================================

	/**
	 * This function will save all of the entites in the world into a XML file.
	 */
	void EditorController::saveSettings() {

		// Instance variables.
		TiXmlDocument doc;  
		TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");  
		doc.LinkEndChild(decl);  
 
		// Root element.
		TiXmlElement* rootElement = new TiXmlElement("root");  
		doc.LinkEndChild(rootElement);  
 
		// Loop through all game entites and write a new element for each of them.
		for (unsigned i = 0; i < mGameObjects.size(); i++) {

			// New element.
			TiXmlElement* element = new TiXmlElement("object");

			// Set element attributes.
			element->SetAttribute("mesh", mGameObjects[i]->getMeshName());
			element->SetAttribute("material", mGameObjects[i]->getMaterialName());

			// Object scale.
			element->SetDoubleAttribute("scale", mGameObjects[i]->getObjectScale());

			// Rotations.
			element->SetDoubleAttribute("pitch", mGameObjects[i]->getXRotation());
			element->SetDoubleAttribute("yaw", mGameObjects[i]->getYRotation());
			element->SetDoubleAttribute("roll", mGameObjects[i]->getZRotation());

			// Set the element to be a child of the root element.
			rootElement->LinkEndChild(element);

		}
 
		// Save file.
		doc.SaveFile(saveFileName);  

	}

	/**
	 * This function will cycle through the list to swap the current model
	 * with a new one.
	 *
	 * @param cycleForward			boolean to go forward or backwards
	 *
	 */
	void EditorController::switchModel(bool cycleForward) {

		// Variable to the next index in the list.
		int next;

		// Move forward in the list.
		if (cycleForward)  { next = (currentIndex + 1) % listSize; }
		else			   { next = (currentIndex - 1) % listSize; }

		// Set the current mesh index.
		currentIndex = (unsigned)next;
		
		// Switch mesh from the scene node.
		mSceneNode->detachAllObjects();
		mSceneNode->attachObject(mModelList[next]);

	}

	/**
	 * This function will scale the model.
	 *
	 * @param increase				boolean determining to scale up or down
	 *
	 */
	void EditorController::scaleModel(bool increase) {

		Ogre::Real scaleFactor = 1;

		// Scale up or down according the the parameter.
		if (increase) { scaleFactor += 0.01f; scaleAmount += 0.01f; }
		else		  { scaleFactor -= 0.01f; scaleAmount -= 0.01f; }

		// Apply scale.
		mSceneNode->scale(scaleFactor,scaleFactor,scaleFactor);

	}

	/**
	 * This function will place the object on the planet when the user hits the key.
	 */
	void EditorController::placeObject() {

		// New game object and push it to the list.
		GameObject* obj = new GameObject(this, mMeshNames[currentIndex], mMaterials[currentIndex], mSceneNode, mSceneMgr);
		mGameObjects.push_back(obj);

	}


//======================================================================================
//
// #I/O Event Listeners
//
//======================================================================================

	/**
	 * This function will handle all keyboard events, when the user strikes any key.
	 */
	bool EditorController::keyPressed(const OIS::KeyEvent& arg) {

		// Save objects into the XML file.
		if (arg.key == OIS::KC_RETURN) {
			saveSettings();
		}

		// Remove the last placed object.
		if (arg.key == OIS::KC_BACK) { 

			if (!mGameObjects.empty()) 
				mGameObjects.back()->setDisposableFlag(); 

		}

		// Swap models if 'Q' or 'E' is hit.
		if (arg.key == OIS::KC_Q)		{ switchModel(false); }
		if (arg.key == OIS::KC_E)		{ switchModel(true); }

		return true;
	}

	/**
	 * This function will handle all keyboard events, when the user releases from any key.
	 */
	bool EditorController::keyReleased(const OIS::KeyEvent& arg) {
		return true;
	}

	/**
	 * This function will handle for when the mouse moves.
	 */
	bool EditorController::mouseMoved(const OIS::MouseEvent& arg) {
		return true;
	}

	/**
	 * This function will handle the event for when the mouse is clicked.
	 */
	bool EditorController::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
		
		// If the left mouse button is clicked.
		if (id == OIS::MB_Left) {
			placeObject();
		}
		
		return true;
	}

	/**
	 * This function will handle the event for when the mouse is clicked is over.
	 */
	bool EditorController::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
		return true;
	}

}