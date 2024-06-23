#include "stdafx.h"

#include "Level.h"
#include "SpaceLevel.h"
#include "SpacePlayer.h"
#include "Enemy.h"
#include "tinyxml.h"
#include "Projectile.h"
#include "Explosions.h"
#include "GlobalVariables.h"
#include "Attributes.h"
#include "MusicManager.h"

namespace capstone {

	// Static variables.
	Ogre::String SpaceLevel::skybox = "SpaceBox"; // Lightgrid for testing, SpaceBox for BETA and final.

	/**
	 * SpaceLevel constructor.
	 *
	 * @param wave		wave number to load
	 *
	 */
	SpaceLevel::SpaceLevel(int wave) 
		: fileName("data/levels.xml")
		, waveNum(wave)
		, mCamera(NULL)
		, mLight(NULL)
		, sceneMgr(NULL)
		, mPlayer(NULL)
		, enemies(NULL)
		, mTime(0)
	{
		srand((unsigned)time(NULL));
		init();
		createPlayer(sceneMgr);
		createTerrain(sceneMgr);
		createEnemies();
	}

	/**
	 * SpaceLevel destructor.
	 */
	SpaceLevel::~SpaceLevel() {

		// Delete dynamically allocated objects.
		delete mPlayer;

		// Delete all enemies.
		for (unsigned i = 0; i < enemies.size(); i++) 
			delete enemies[i];

		// Delete all projectiles
		for (unsigned i = 0; i < projectiles.size(); i++)
            delete projectiles[i];

		// Delete resources.
		for (unsigned i = 0; i < resources.size(); i++)
			delete resources[i];

		// Destroy the scene manager, which automatically destroys everything.
        if (sceneMgr)
            Ogre::Root::getSingleton().destroySceneManager(sceneMgr);

	}


// =====================================================================================
//
// #Initialization
//
// =====================================================================================

	/**
	 * This function will initialize the level's properties.
	 */
	void SpaceLevel::init() {

		Level::shouldFinish = false;

		// Get the Ogre root.
		Ogre::Root& ogreRoot = Ogre::Root::getSingleton();

        // Instantiate the scene manager.
        sceneMgr = ogreRoot.createSceneManager(Ogre::ST_GENERIC);

        // Set ambient light.
        sceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));

        // Create a directional light.
        mLight = sceneMgr->createLight();
        mLight->setType(Ogre::Light::LT_DIRECTIONAL);
        mLight->setDirection(Ogre::Vector3(0, 0, 5).normalisedCopy());
		mLight->setAttenuation(10000000000, 1, 0, 0);

		mTopLight = sceneMgr->createLight();
        mTopLight->setType(Ogre::Light::LT_DIRECTIONAL);
        mTopLight->setDirection(Ogre::Vector3(0, -1, 0).normalisedCopy());
		mTopLight->setAttenuation(10000000000, 1, 0, 0);

		// Instantiate the camera.
        mCamera = sceneMgr->createCamera("PlayerCam");
        mCamera->setNearClipDistance(1);
        mCamera->setFarClipDistance(5000);
        mCamera->setAutoAspectRatio(true);
        mCamera->setPosition(0, 2, 0);
	}

	/**
	 * This function creates the player in space.
	 *
	 * @param sceneMgr		pointer to the scene manager to create entities
	 *
	 */
	void SpaceLevel::createPlayer(Ogre::SceneManager* sceneMgr) {
		
		// Create new player object.
		mPlayer = new SpacePlayer(mCamera, this);

	}

	/**
	 * This function creates the terrain and skybox for the space level.
	 *
	 * @param sceneMgr		pointer to the scene manager to create entities
	 *
	 */
	void SpaceLevel::createTerrain(Ogre::SceneManager* sceneMgr) {

		// Create the skybox.
		sceneMgr->setSkyBox(true, skybox, 10, true);

	}

	/**
	 * This function will parse through the XML file and create the enemies.
	 */
	void SpaceLevel::createEnemies() {

		TiXmlDocument doc;

		// Determine if the XML file exists.
		if(!doc.LoadFile(fileName)) {
			std::exception("XML Error: levels.xml is missing.");
		}

		// Determine if the document has a root element.
		TiXmlElement* root = doc.FirstChildElement();
		if(root == NULL) {
			std::exception("XML Error: No root element in levels.xml");
			doc.Clear();
		}

		// Begin loop.
		for (TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {

			// Grab the element name and attribute.
			std::string elemName = elem->Value();

			if(elemName == "wave") {

				// Grab the wave's properites/attributes.
				const char* attr = elem->Attribute("id");

				if (attr != NULL) {

					// Convert attribute values from string to int.
					int waveID = charToInt(attr);

					// If the wave number matches with the current wave.
					if (waveID == waveNum) {

						// Found the wave, now start generating enemies.
						for (TiXmlElement* e = elem->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {

							// Get the element name.
							std::string enemy = e->Value();
							
							// For every enemy, draw them onto the game.
							if (enemy == "enemy") {
								
								// Get attributes.
								const char* meshName = e->Attribute("mesh");
								const char* chance = e->Attribute("chance");
								const char* hp = e->Attribute("hp");

								// Convert attribute values from string to int.
								int probablity = charToInt(chance);
								int health = charToInt(hp);

								// Create enemies.
								for (int i = 0; i < probablity; i++) {

									// Random coordinates for the enemy's position.
									float x = (rand() % 500) * i;
									float z = rand() % 1500;
									//AC
									// New enemy object that will be pushed into the list.
									Enemy* e = new Enemy(this, meshName, Ogre::Vector3(x,0,z), 
										65.0f, Ogre::Degree(135.0), 450.0f, Ogre::Degree(180.0), health);
									enemies.push_back(e);

								}

							}
						}

					}

				}

			}

		} // End of loop.

		doc.Clear();
	}


// =====================================================================================
//
// #Update/Render
//
// =====================================================================================

	/**
	 * This function will update the space level.
	 *
	 * @param delatT		time since last frame
	 *
	 */
	void SpaceLevel::update(Ogre::Real deltaT) {

		// keep track of time
        mTime += deltaT;

		mPlayer->update(deltaT);
		updateEnemies(deltaT);
		updateProjectiles(deltaT);
		updatePowerups(deltaT);
		checkCollisions();
	}

	/**
	 * This function will update all the enemies on the level.
	 *
	 * @param delatT		time since last frame
	 *
	 */
	void SpaceLevel::updateEnemies(Ogre::Real deltaT) {
		
		for (unsigned i = 0; i < enemies.size(); /* noop */) {
			
			// Update enemies as long as they are alive.
			if (!enemies[i]->isAlive()) {

				Ogre::Vector3 pos = enemies[i]->getEnemyPosition();
				Ogre::Vector3 matterPos(pos.x - 10, pos.y, pos.z);
				Ogre::Vector3 metalPos(pos.x + 10, pos.y, pos.z);
				Ogre::Vector3 carbonPos(pos.x, pos.y + 10, pos.z);

				//Explosions* e = new Explosions(this,70.0f, enemies[i]->getEnemyPosition());
				// The Animation
				enemies[i]->Die();

				// The stuff the enemy will drop when he is dead.
				Resource* dm = new DarkMatter(this, matterPos);
				resources.push_back(dm);
				Resource* ms = new MetalScrap(this, metalPos);
				resources.push_back(ms);
				Resource* c = new Carbon(this, carbonPos);
				resources.push_back(c);

				delete enemies[i];
				enemies[i] = enemies.back();
                enemies.pop_back();
			}

			else {
				enemies[i]->mState->ExecuteState(enemies[i], deltaT);
				enemies[i]->update(deltaT);
				i++;
			}
			
		}
	}

	/**
	 * This function will update all the projectiles on the level.
	 *
	 * @param delatT		time since last frame
	 *
	 */
	void SpaceLevel::updateProjectiles(Ogre::Real deltaT) {

		// Loop throught all projectiles.
		for (unsigned i = 0; i < projectiles.size(); /* noop */) {

			// Update the projectile.
			Projectile* projectile = projectiles[i];
			projectile->update(deltaT);

			// If the projectile should be destroyed.
			if (projectile->getDestroy()) {

				// Delete the projectile.
                delete projectile;

                // Remove it from the list of projectiles.
                projectiles[i] = projectiles.back();
                projectiles.pop_back();
			} 
			
			// Move on to next projectile.
			else 
                i++;
            
		}

	}

	/**
	 * This function will update all the powerups on the level.
	 *
	 * @param delatT		time since last frame
	 *
	 */
	void SpaceLevel::updatePowerups(Ogre::Real deltaT) {

		// Loop through all power ups, including the resources.
		for (unsigned i = 0; i < resources.size(); /* noop */) {

			// Delete the power up if nessecary.
			if (resources[i]->isResourceDisposable()) {

				// Play sound.
				MusicManager::getInstance()->playJumpSound();

				// Apply powerup bonuses.
				if (darkMatter == resources[i]->getResourceId()) {
					std::cout << resources[i]->getAmount() << " dark matter added" << std::endl;
					Attributes::getInstance()->incrementMatterGained(resources[i]->getAmount());
					Attributes::getInstance()->incrementDarkMatter(resources[i]->getAmount());
				}
				
				else if (metalScrap == resources[i]->getResourceId()) {
					std::cout << resources[i]->getAmount() << " metal added" << std::endl;
					Attributes::getInstance()->incrementMetalGained(resources[i]->getAmount());
					Attributes::getInstance()->incrementMetal(resources[i]->getAmount());
				}
				
				else if (carbon == resources[i]->getResourceId()) {
					std::cout << resources[i]->getAmount() << " carbon added" << std::endl;
					Attributes::getInstance()->incrementCarbonGained(resources[i]->getAmount());
					Attributes::getInstance()->incrementCarbon(resources[i]->getAmount());
				}

				delete resources[i];
                resources[i] = resources.back();
                resources.pop_back();

			}

			// Otherwise, update the powerup.
			else {
				// Get the positions of both the player can the powerup box.
				Ogre::Vector3 playerPos = mPlayer->getSceneNode()->getPosition();
				Ogre::Vector3 powerupPos = resources[i]->getPosition();

				// Get direction and magnitude to the player.
				Ogre::Vector3 toPlayer = playerPos - powerupPos;
				Ogre::Real dist = toPlayer.length();

				// Set flag if the player is in range to pick up the power up.
				if (dist <= mPlayer->getRadius()) 
					resources[i]->setDisposable();

				// Otherwise, keep updating.
				else
					resources[i]->update(deltaT);

				i++;
			}

		} // End of for loop.

	}

	/**
	 * This function check for collisions with all projectiles.
	 */
	void SpaceLevel::checkCollisions() {

		// If there is no projectiles, there are no collisions.
		if (projectiles.size() == 0) 
			return;
		
		// Otherwise, loop throught all projectiles.
		for (unsigned i = 0; i < projectiles.size(); i++) {

			// Get the position of the projectile.
			Projectile* p = projectiles[i];
			Ogre::Vector3 pPos = p->getProjectilePosition();

			// Get the position of the player.
			Ogre::Vector3 plPos = mPlayer->getSceneNode()->getPosition();
			Ogre::Vector3 toPlayer = plPos - pPos;
			Ogre::Real distanceToPlayer = toPlayer.length();

			// Determine if the enemy got hit (using sphere collision shape for now).
			if(distanceToPlayer < mPlayer->getRadius() && mPlayer != p->getShooter()){

				std::cout << "Player got hit by: " << p->getDamageAmount() << std::endl;

				// Play sound
				MusicManager::getInstance()->playProjectileHitSound(p->getProjectilePosition(), Ogre::Vector3::ZERO);

				// Set projectile flag.
				p->destroy();

				// Apply damage as long as the enemy is alive.
				if (mPlayer->isAlive()) {
					//addObject(new Explosions(this, 50.0f, pPos)); // ANIMATE EXPLOSION!!!!!!!!
					mPlayer->applyDamage(p->getDamageAmount());
				}
			}

			// Determine if the projectile has collided with any of the enemies in the list.
			for (unsigned j = 0; j < enemies.size(); j++) {

				// Get the position of the enemy,
				Enemy* e = enemies[j];
				Ogre::Vector3 ePos = e->getEnemyPosition();

				// Get direction and distance from the bullet to the enemy.
				Ogre::Vector3 toEnemy = ePos - pPos;
				Ogre::Real distanceToEnemy = toEnemy.length();

				bool shouldHit = false;

				// If the bullet isn't to high AKA NOT above the enemy's y cooridnates + their height.
				if (pPos.y < ePos.y + e->getHeight()) {
					shouldHit = true;
				}

				// If the bullet is to low AKA below the enemy's y coordinates - their height.
				else if (pPos.y > ePos.y - e->getHeight()) {
					shouldHit = false;
				}

				// Determine if the enemy got hit (using sphere collision shape for now).
				if (distanceToEnemy < e->getHitRadius() && e != p->getShooter() && shouldHit) {

					std::cout << "Enemy got hit by: " << p->getDamageAmount() << std::endl;

					// Play sound
					MusicManager::getInstance()->playProjectileHitSound(p->getProjectilePosition(), Ogre::Vector3::ZERO);

					// Set projectile flag.
					p->destroy();

					// Set stats.
					Attributes::getInstance()->incrementShotCount();
					Attributes::getInstance()->incrementHitCount();

					// Apply damage.
					if (e->isAlive()) {
						e->applyDamage(p->getDamageAmount());
					}
					

				}

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
	OIS::KeyListener* SpaceLevel::getKeyListener() {
		return mPlayer;
	}

	/**
	 * This function will return the player's mouse listener.
	 */
	OIS::MouseListener* SpaceLevel::getMouseListener() {
		return mPlayer;
	}

}