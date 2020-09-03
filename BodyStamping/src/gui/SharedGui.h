#pragma once

#include "ofMain.h"
#include "ofxGui.h"

typedef std::shared_ptr<class SharedGui> SharedGuiRef;

class SharedGui {

public:

	SharedGui();
	~SharedGui(){};

	void setup();

	//Params need to all be added to groups 
	//BEFORE groups are added to GUI
	void finishSetup();

	void draw();
	void save();
	void load();

	void setPosition(int x, int y) { mainGui.setPosition(x, y); }

	static SharedGuiRef get() {
		if (!mInstance) {
			mInstance = std::make_shared<SharedGui>();
		}
		return mInstance;
	}

	std::shared_ptr<ofParameterGroup> getGroup(std::string name);

	// Allows users to add elements for saving/loading 
	// without needing an actual graphical interface
	std::shared_ptr<ofParameterGroup> getHiddenGroup();
	
	ofEvent<void> & getSettingsLoadedEvent() { return firstLoadEvent; }
	ofEvent<void> & getSettingsSavedEvent() { return saveEvent; }

	void setColor( ofColor c ) { guiColor = c; }
	static ofColor getColor() { return guiColor; }

private:

	ofEvent<void> firstLoadEvent;
	ofEvent<void> saveEvent;
	bool bFirstLoad;

	static SharedGuiRef	mInstance;
	static SharedGuiRef	mHiddenInstance;

	void collapse();
	ofxButton bCollapseAll;

	ofxPanel mainGui;

	//identical to main gui, but not set up for drawing
	ofxPanel hiddenGui;

	std::vector<std::string> groupKeys;
	std::map<std::string, ofxLabel> paramLabels;
	std::map<std::string, std::shared_ptr<ofParameterGroup>> paramGroups;

	static ofColor guiColor;
	string guiFilename;
	string hiddenGuiFilename;
	ofxLabel camLabel;

	std::string hiddenGroupName = "HIDDEN";

};