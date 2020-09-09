#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetVerticalSync(true);
    ofEnableDepthTest();
    ofEnableAlphaBlending();
    
    //movie.load("images/trapped.mov");
    //movie.setLoopState(OF_LOOP_NORMAL);
    
    img.load("images/everest.png");
	img.setImageType(OF_IMAGE_GRAYSCALE);
    
    
    /*
     * Now we'll construct a mesh from scratch instead of getting it from a primitive
     * Since we'll add vertices in the order that makes sense for a OF_PRIMITIVE_TRIANGLES
     * style mesh (first 3 vertices are one triangle, the next 3 vertices are
     * the second triangle, etc.) we won't need to add any indices.
     *
     * Arrangement:
     * Each time we go through an X and a Y from the nested for loops, we'll create 1 unit cell.
     * Since there are no quads (i.e. rectangles) in OpenGl anymore, we need to do this with
     * two triangles:
     *
     *       A_______B
     *       |      /|
     *       |     / |
     *       |    /  |
     *       |   /   |
     *       |  /    |
     *       | /     |
     *       |/______|
     *       C       D
     *
     * The first triangle will be 0-1-2 while the second will be 1-3-2
     * (keep winding clockwise so the normals are all in the same direction)
     *
     * Since our for loops go through X and Y together, the XY version of the 
     * triangle above will be:
     *
     *  A: (x    , y    )
     *  B: (x + 1, y    )
     *  C: (x    , y + 1)
     *  D: (x + 1, y + 1)
     *
     */
    
    //set the mode. "OF_PRIMITIVE_TRIANGLES" expects triplets of
    //vertices, one triplet per triangle
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
    //Number of grid points in our mesh in each direction
    //These can be higher, but bigger triangles make it easier to see the movie texture
    int numX = 020;
    int numY = 160;
    
    //this will set the overall mesh dimensions to the same as
    //that of the movie file you give it
    int meshWidth = 1000;
    int meshHeight = meshWidth * (img.getHeight() / img.getWidth());
    
    //space between X and Y grid points
    int gridSpacingX = meshWidth/(numX-1);
    int gridSpacingY = meshHeight/(numY-1);
    
    //package them together for easy multiplication later
    ofVec3f gridSpacing(gridSpacingX, gridSpacingY, 1);

	// resolution of sample points within image
	int pixX = img.getWidth() / (float)numX;
	int pixY = img.getHeight() / (float)numY;
	ofVec2f imgRes = ofVec2f(pixX, pixY);

    for(int y = 0; y < numY - 1; y++){
        for(int x = 0; x < numX - 1; x++){
            
            //Create points for the first triangle (ABC)
            ofVec3f A(x    , y    , 0);
            ofVec3f B(x + 1, y    , 0);
            ofVec3f C(x    , y + 1, 0);
			ofVec3f D(x + 1, y + 1, 0);
            
			// get the grayscale value of the image and 
			float val1 = getValFromImg(A.x * pixX, A.y * pixY);
			float val2 = getValFromImg(B.x * pixX, B.y * pixY);
			float val3 = getValFromImg(C.x * pixX, C.y * pixY);
			float val4 = getValFromImg(D.x * pixX, D.y * pixY);

			A.z = getHeightForVal(val1);
			B.z = getHeightForVal(val2);
			C.z = getHeightForVal(val3);
			D.z = getHeightForVal(val4);

            //add the first triangle's vertices to the mesh
            mesh.addVertex(A * gridSpacing);
            mesh.addVertex(B * gridSpacing);
            mesh.addVertex(C * gridSpacing);
            
            //add the same vertices as texture coordinates. this will map the texture
            //linearly (no warping/stretching) but beware of situations where the size of the texture doesn't match the size of the mesh.
            mesh.addTexCoord(A * imgRes);
            mesh.addTexCoord(B * imgRes);
            mesh.addTexCoord(C * imgRes);

            //add second triangle (BDC)
            mesh.addVertex(B * gridSpacing);
            mesh.addVertex(D * gridSpacing);
            mesh.addVertex(C * gridSpacing);
            
            //add next texCoords
            mesh.addTexCoord(B * imgRes);
            mesh.addTexCoord(D * imgRes);
            mesh.addTexCoord(C * imgRes);
            
            
        }
    }


    
    //Since our mesh starts at (0,0) at the upper left corner, go through
    //all the mesh points and shift them over by half the width and height
    //so it is centered around the origin and draws nicer
    for(int i = 0; i < mesh.getNumVertices(); i++){
        mesh.setVertex(i, mesh.getVertex(i) - ofVec2f(meshWidth/2, meshHeight/2));
    }

    
    //get a copy of the mesh and store it so we can know the original
    //data after we've manipulated it
    originalMesh = mesh;


	// test plane primitive for num verts
	ofPlanePrimitive plane;
	plane.set(meshWidth, meshHeight, numX, numY);

	auto m = plane.getMesh();

	ofLog(OF_LOG_NOTICE, "Plane Primitive verts: " + ofToString(m.getNumVertices()));







	//set up the heads of the axis cones
	coneX.setRadius(15);
	coneX.setHeight(45);
	coneX.setResolution(8, 1, 0);
	coneX.setTopColor(ofColor(255, 0, 0));

	coneY.setRadius(15);
	coneY.setHeight(45);
	coneY.setResolution(8, 1, 0);
	coneY.setTopColor(ofColor(0, 255, 0));

	coneZ.setRadius(15);
	coneZ.setHeight(45);
	coneZ.setResolution(8, 1, 0);
	coneZ.setTopColor(ofColor(0, 0, 255));
    
}

float ofApp::getValFromImg(int x, int y) {
	float val = img.getColor(x, y).r;
	
	return getHeightForVal(val);
}

float ofApp::getHeightForVal(float val) {
	return ofMap(val, 0, 255, 0, 150);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //update the movie so our texture has the next frame
    //movie.update();

}

//--------------------------------------------------------------
void ofApp::draw(){

    //play the movie
    //if(movie.isLoaded()){
    //    movie.play();
    //}
    
    
    ofBackgroundGradient(ofColor(80), ofColor(0));
    
    ofSetColor(255);
    ofDrawBitmapString("Framerate: " + ofToString(ofGetFrameRate()), 15, 15);
    ofDrawBitmapString("Num Vertices: " + ofToString(mesh.getNumVertices()), 15, 30);
    
    
    ofDrawBitmapString("Press SPACEBAR to randomize the triangles in the mesh", 15, 60);
    ofDrawBitmapString("Press 'r' to restore the vertex positions from the original mesh", 15, 75);
    ofDrawBitmapString("Press 'w' to toggle wireframe drawing", 15, 90);
    
    //start the camera so we can move our mesh around
    cam.begin();
    
    
    //bind the texture, but only if the movie has actually loaded
    //if(movie.isLoaded()) movie.getTexture().bind();
	
	img.getTexture().bind();
    
    if(bWire){
        mesh.drawWireframe();
    } else {
        mesh.draw();
    }

	img.getTexture().unbind();

	drawAxes();

    //finish camera manipulation
    cam.end();
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if(key == 'w'){
        bWire = !bWire;
    }
    
    //scatter all the triangles
    if(key == ' '){
        
        
        //go through all the points by 3's (3 verts in each triangle)
        //and give them a random distance
        for(int i = 0; i < mesh.getNumVertices(); i += 3){
            
            //create a random direction then add that same ofVec3f to all three
            //vertices. Doing it this way moves the three vertices in each triangle
            //the same amount keeping the triangle together. Try giving each
            //vertex a different random amount and see what happens
            
            ofVec3f scatter;
            float dist = 100;
            scatter.set(ofRandom(-dist, dist), ofRandom(-dist, dist), ofRandom(-dist, dist));

            mesh.setVertex(i + 0, mesh.getVertex(i + 0) + scatter);
            mesh.setVertex(i + 1, mesh.getVertex(i + 1) + scatter);
            mesh.setVertex(i + 2, mesh.getVertex(i + 2) + scatter);
            
        }

        
        
        
    }
    

    //Since we kept a copy of the mesh when it was in its original state we can
    //restore it very easily
    if(key == 'r'){
        mesh = originalMesh;
    }
    
    
}

void ofApp::drawAxes() {


	ofVec3f axisLengths = ofVec3f(100, 100, 100);


	ofSetLineWidth(2);

	//X-Axis (Red)
	ofSetColor(255, 0, 0);
	ofDrawLine(0, 0, 0, axisLengths.x, 0, 0);


	ofPushMatrix();
	ofTranslate(axisLengths.x, 0);
	ofRotate(90, 0, 0, 1);

	coneX.draw();

	ofPopMatrix();

	//Y-Axis (Green)
	ofSetColor(0, 255, 0);
	ofDrawLine(0, 0, 0, 0, axisLengths.y, 0);


	ofPushMatrix();
	ofTranslate(0, axisLengths.y);
	ofRotate(180, 0, 0, 1);

	coneY.draw();

	ofPopMatrix();

	//Z-Axis (Blue)
	ofSetColor(0, 128, 255);
	ofDrawLine(0, 0, 0, 0, 0, axisLengths.z);


	ofPushMatrix();
	ofTranslate(0, 0, axisLengths.z);
	ofRotate(-90, 1, 0, 0);

	coneZ.draw();

	ofPopMatrix();





}


//--------------------------------------------------------------
//Billboarding code adapted from Zach Lieberman's Sample code:
//https://forum.openframeworks.cc/t/billboard-using-ofnode/14142

float ofApp::getBillboardAngle(ofVec3f globalPos) {


	ofNode posNode;
	posNode.setGlobalPosition(globalPos);
	posNode.lookAt(cam.getGlobalPosition(), cam.getUpDir());
	ofQuaternion posQuat = posNode.getGlobalOrientation();

	float ang = 0;
	ofPoint vec;
	posQuat.getRotate(ang, vec);

	return ang;

}

//--------------------------------------------------------------
ofVec3f ofApp::getBillboardVec(ofVec3f globalPos) {

	ofNode posNode;
	posNode.setGlobalPosition(globalPos);
	posNode.lookAt(cam.getGlobalPosition(), cam.getUpDir());
	ofQuaternion posQuat = posNode.getGlobalOrientation();

	float ang = 0;
	ofPoint vec;
	posQuat.getRotate(ang, vec);

	return vec;

}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
