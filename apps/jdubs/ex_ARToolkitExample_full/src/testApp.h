#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "ofxOpenCv.h"

//#define _USE_LIVE_VIDEO		// uncomment this to use a live camera
								// otherwise, we'll use a movie file


class testApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);

		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);


		ofVideoGrabber grabber;
		// ofVideoPlayer grabber;
		ofxCvColorImage convert;
		ofxCvGrayscaleImage gray;

		int width, height, bpp;
		size_t        numPixels;
		size_t        numBytesRead;
		unsigned char *cameraBuffer;

		bool bDraw;
		bool useBCH;
		ofTrueTypeFont idNumb;
};

#endif
