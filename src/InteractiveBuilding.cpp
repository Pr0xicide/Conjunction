#include "stdafx.h"

#include "InteractiveBuilding.h"
#include "PlanetLevel.h"
#include "tinyxmls.h"
#include "GlobalVariables.h"

namespace capstone {
	
	// Initialize static variables.
	const char* InteractiveBuilding::xmlFileName = "data/building.xml";

	/**
	 * InteractiveBuilding constructor.
	 * 
	 * @param level			pointer to the level to get variables
	 * @param position		position of the building
	 *
	 */
	InteractiveBuilding::InteractiveBuilding(PlanetLevel* level) 
		: level(level)
		, mEntity(NULL)
		, mSceneNode(NULL)
		, mRadius(30)
		, playerClose(false)
		, id(0)
		, name("")
	{

	}

	/**
	 * InteractiveBuilding destructor.
	 */
	InteractiveBuilding::~InteractiveBuilding() {

	}


// =====================================================================================
//
// #Initialization.
//
// =====================================================================================

	/**
	 * This function will get the description from the XML file.
	 */
	void InteractiveBuilding::readXML() {

		// TinyXML document object.
		TiXmlDocument doc;

		// Determine if the XML file exists.
		if(!doc.LoadFile(xmlFileName)) {
			std::exception("XML Error: buildings.xml is missing.");
		}

		// Determine if the document has a root element.
		TiXmlElement* root = doc.FirstChildElement();

		if (root == NULL) {
			std::exception("XML Error: No root element in levels.xml");
			doc.Clear();
		}

		// Begin loop.
		for (TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {

			std::string elemName = elem->Value();

			if (elemName == "building") {

				const char* c_id = elem->Attribute("id");
				int elemID = charToInt(c_id);

				if (elemID == id) {
					desc = elem->GetText();
				}

			}

		}

	}

}
