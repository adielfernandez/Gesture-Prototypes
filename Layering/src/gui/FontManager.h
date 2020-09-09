#pragma once

#include "ofMain.h"
#include "ofxGui.h"

typedef std::shared_ptr<class ofTrueTypeFont> FontRef;
typedef std::shared_ptr<class FontManager> FontManagerRef;

class FontManager {

public:

	FontManager();
	~FontManager(){};

	void setup();
	
	
	// loads font at 500 px size, then uses push/pop matrix
	// to shrink font down 
	void addFont(string path, string name);


	// Draws a scaled version of the larger base font
	void drawString(string s, int x, int y, float size = -1, string fontName = "");


	// direct font access, only draws according to base font size
	FontRef getFont(string s);

	static FontManagerRef get() {
		if (!mInstance) {
			mInstance = std::make_shared<FontManager>();
		}
		return mInstance;
	}


private:

	static FontManagerRef	mInstance;

	static float			mBaseFontSize;

	std::map<std::string, FontRef> mFontMap;
	std::string				mDefaultFontKey = "";

};