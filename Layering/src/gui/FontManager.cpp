#include "FontManager.h"

FontManagerRef FontManager::mInstance = nullptr;
float FontManager::mBaseFontSize = 60.0f;

FontManager::FontManager() {


}

void FontManager::setup() {

}

void FontManager::addFont(string path, string name) {

	if (mDefaultFontKey == "") {
		mDefaultFontKey = name;
	}

	mFontMap.insert( make_pair( name, make_shared<ofTrueTypeFont>()) );
	mFontMap[name]->load(path, mBaseFontSize);

}

void FontManager::drawString(string s, int x, int y, float size, string fontName) {

	if( mFontMap.size() == 0  ) return;

	FontRef font;

	if (fontName != "" && mFontMap.count(fontName) != 0) {
		// Key exists
		font = mFontMap[fontName];
	} else {
		//use default
		font = mFontMap[mDefaultFontKey];
	}

	if (size == -1) {
		font->drawString(s, x, y);
	} else {
		float scale = size / mBaseFontSize;
		ofPushMatrix();
		ofTranslate(x, y);
		ofScale(scale, scale);
		font->drawString(s, 0, 0);
		ofPopMatrix();
	}

}

FontRef FontManager::getFont(string s) { 
	if(s == "")		return mFontMap[mDefaultFontKey];
	else			return mFontMap[s]; 
}
