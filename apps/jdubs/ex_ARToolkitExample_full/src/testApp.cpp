#include "testApp.h"


// ----  AR TK+ STUFF - ripped from the multi marker demo app
// needs a home in a nice ofxARToolkit addon :)

#include "ARToolKitPlus/TrackerMultiMarkerImpl.h"


class MyLogger : public ARToolKitPlus::Logger
{
    void artLog(const char* nStr)
    {
        printf(nStr);
    }
};

static    MyLogger      logger;
ARToolKitPlus::TrackerMultiMarker *tracker;





//--------------------------------------------------------------
void testApp::setup(){	
	width = 640;
	height = 480;
	bpp = 1;
	
	grabber.initGrabber(width, height);
	convert.allocate(width, height);
	gray.allocate(width, height);

	numPixels = width*height*bpp;
	cameraBuffer = new unsigned char[numPixels];
	
	bDraw = false;
	useBCH = true;
	
	ofBackground(127,127,127);
	
	idNumb.loadFont("frabk.ttf", 24);	
	
	// ----------------------------------  AR TK+ STUFF - ripped from the single marker demo app
	
    // create a tracker that does:
    //  - 6x6 sized marker images
    //  - samples at a maximum of 6x6
    //  - works with luminance (gray) images
    //  - can load a maximum of 10 patterns
    //  - can detect a maximum of 10 patterns in one image
	tracker = new ARToolKitPlus::TrackerMultiMarkerImpl<6,6,6, 10, 10>(width,height);
	
	const char* description = tracker->getDescription();
	printf("ARToolKitPlus compile-time information:\n%s\n\n", description);
	
    // set a logger so we can output error messages
    tracker->setLogger(&logger);
	tracker->setPixelFormat(ARToolKitPlus::PIXEL_FORMAT_LUM);	
	
	// load std. ARToolKit camera file
    if( !tracker->init( "/Users/jamie/Desktop/ScratchML/of_preRelease_v0061_osxSL_FAT/apps/jdubs/ex_ARToolkitExample_2/bin/data/LogitechPro4000.dat", "/Users/jamie/Desktop/ScratchML/of_preRelease_v0061_osxSL_FAT/apps/jdubs/ex_ARToolkitExample_2/bin/data/markerboard_480-499.cfg", 1.0f, 1000.0f) )
	{
		printf("ERROR: init() failed\n");
		delete cameraBuffer;
		delete tracker;
		return;
	}
	
    // define size of the marker
    //tracker->setPatternWidth(80); // I'm not sure how to define the size with multimarkers since it doesnt seem to have this option.
	
	// the marker in the BCH test image has a thin border...
    tracker->setBorderWidth(useBCH ? 0.125f : 0.250f);
	
    // set a threshold. alternatively we could also activate automatic thresholding
    //tracker->setThreshold(20);
	tracker->activateAutoThreshold(true);
    // let's use lookup-table undistortion for high-speed
    // note: LUT only works with images up to 1024x1024
    tracker->setUndistortionMode(ARToolKitPlus::UNDIST_LUT);
	
    // RPP is more robust than ARToolKit's standard pose estimator
    tracker->setPoseEstimator(ARToolKitPlus::POSE_ESTIMATOR_RPP);
	
    // switch to simple ID based markers
    // use the tool in tools/IdPatGen to generate markers
    tracker->setMarkerMode(useBCH ? ARToolKitPlus::MARKER_ID_BCH : ARToolKitPlus::MARKER_ID_SIMPLE);
}

//--------------------------------------------------------------
void testApp::update(){
	
	
	grabber.grabFrame();
	if(grabber.isFrameNew())
	{
		
		//convert our camera frame to grayscale
		convert.setFromPixels(grabber.getPixels(), width, height);
		gray = convert;
		
		//find the marker and get back the confidence
		int markerId = tracker->calc(gray.getPixels());
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(0xffffff);
	grabber.draw(0, 0);
	
    //this is where we use the calculated matrix from ARToolkitPlus to put
	//in our graphics at the location and orientation of the marker.
	//- the matrix has the 0,0 point as the center of the marker. 
	
	int numDetected = tracker->getNumDetectedMarkers();
	// cout<<"numberofmarkers:"<<numDetected<<endl;
	
	
	glViewport(0, 0, 640, 480 );
	glMatrixMode( GL_PROJECTION );
	glLoadMatrixf(tracker->getProjectionMatrix());
	
	for(int num=0; num<numDetected; num++)
	{
		
		ARToolKitPlus::ARMarkerInfo marker = tracker->getDetectedMarker(num);
		
		float m34[ 3 ][ 4 ];
		float c[ 2 ] = { 0.0f, 0.0f };
		float w = 40.0f;
		
		
		tracker->rppGetTransMat( &marker, c, w, m34 );
		
		// this is some crazy matrix transformative stuff. I think initially it came out of one of the arToolkit functions.... 
		// but i got it from here: http://chihara.naist.jp/people/STAFF/imura/computer/OpenGL/artp/disp_content
		float m[ 16 ]; 
		for ( int i = 0; i < 3; i++ ) {
			for ( int j = 0; j < 4; j++ ) {
				m[ j * 4 + i ] = m34[ i ][ j ];
			}
		}
		for ( int j = 0; j < 3; j++ ) {
			m[ j * 4 + 3 ] = 0.0f;
		}
		m[ 3 * 4 + 3 ] = 1.0f;
		

		glMatrixMode( GL_MODELVIEW );
		glLoadMatrixf( m );
		
		// glPushMatrix();
		// glRotatef(-90, 0, 0, 1);
		// ofSetColor(255 , 255, 255 );
		// grabber.draw(-30,-20,60,40);
		// glPopMatrix();
		
		glPushMatrix();
		glRotatef(-90, 1, 0, 0);
		glRotatef(-90, 0, 1, 0);		
		ofSetColor(255 , 255, 255 );
		idNumb.drawString(ofToString(marker.id), -idNumb.stringWidth(ofToString(marker.id))/2,0);
		glPopMatrix();

	}
	
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key == 's'){ grabber.videoSettings(); }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}
