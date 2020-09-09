#include "SharedGui.h"

SharedGuiRef SharedGui::mInstance = nullptr;
SharedGuiRef SharedGui::mHiddenInstance = nullptr;
ofColor	SharedGui::guiColor = ofColor(255);

SharedGui::SharedGui() {


}

void SharedGui::setup() {

	guiFilename = "settings.xml";
	hiddenGuiFilename = "hiddenSettings.xml";

	mainGui.setup("settings", guiFilename);
	hiddenGui.setup("hidden_settings", hiddenGuiFilename);


	mainGui.add(bCollapseAll.setup("Collapse all", false));
	bCollapseAll.addListener(this, &SharedGui::collapse);

	bFirstLoad = true;
	

}

void SharedGui::collapse() {
	mainGui.minimizeAll();
}

std::shared_ptr<ofParameterGroup> SharedGui::getGroup(std::string name){

	//returns access to data group
	//creates one if it doesn't exist
	if (paramGroups.count(name) == 0) {

		groupKeys.push_back(name);

		paramLabels.insert( make_pair(name, ofxLabel()));
		//paramLabels[name].setup( "____" + name, "");

		paramGroups.insert( make_pair(name, make_shared<ofParameterGroup>()));
		paramGroups[name]->setName(name);

		//ofLogNotice("Adding parameter group: " + name);
	}
	
	return paramGroups[name];

}

std::shared_ptr<ofParameterGroup> SharedGui::getHiddenGroup() {

	//returns access to data group
	//creates one if it doesn't exist
	if (paramGroups.count(hiddenGroupName) == 0) {

		groupKeys.push_back(hiddenGroupName);

		paramGroups.insert(make_pair(hiddenGroupName, make_shared<ofParameterGroup>()));
		paramGroups[hiddenGroupName]->setName(hiddenGroupName);

		//ofLogNotice("Adding parameter group: " + name);
	}

	return paramGroups[hiddenGroupName];

}


void SharedGui::finishSetup() {
	

	//formatting
	mainGui.setHeaderBackgroundColor(255);

	mainGui.setDefaultFillColor(128);
	mainGui.setDefaultBackgroundColor(ofColor(40));

	mainGui.setDefaultBorderColor(128);
	mainGui.setDefaultHeaderBackgroundColor(ofColor(128));

	//color applies to gui buttons only
	mainGui.setTextColor(ofColor(255, 0, 0));

	for (auto key : groupKeys) {

		if (key != hiddenGroupName) {

			paramLabels[key].setDefaultTextColor(ofColor(255));	//weirdly sets ALL text color to white
			paramLabels[key].setBackgroundColor(guiColor);

			mainGui.add(paramLabels[key].setup("____" + key, ""));
			mainGui.add( *paramGroups[key] );

		} else {
			hiddenGui.add(*paramGroups[key]);
		}

	}

	// ---------- STYLING ----------
	mainGui.setUseTTF(true);
	mainGui.loadFont("fonts/futura_light.ttf", 10);

	

	load();


}


void SharedGui::draw() {
	mainGui.draw();
}

void SharedGui::save() {
	mainGui.saveToFile(guiFilename);
	hiddenGui.saveToFile(hiddenGuiFilename);
	ofNotifyEvent(saveEvent, this);
	ofLog() << "SETTING SAVED";
}

void SharedGui::load() {
	mainGui.loadFromFile(guiFilename);
	hiddenGui.loadFromFile(hiddenGuiFilename);
	ofLog() << "SETTINGS LOADED";

	if (bFirstLoad) {
		ofNotifyEvent(firstLoadEvent, this);
		bFirstLoad = false;
	}

}