
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <sstream>
#include <filesystem>

using namespace cv;
using namespace std;

void help(char** argv ) {
	std::cout << "\n"
	<< "Play video from disk and serialize frames. \n"
	<< argv[0] <<" <video> <sec>\n"
	<< "For example:\n"
	<< argv[0] << " ../sample_video.mp4 2\n"
	<< std::endl;
}
	

int main( int argc, char** argv ) {
	
	if (argc != 3) {
		help(argv);
		return 0;
	}

  cv::namedWindow( "Frame", cv::WINDOW_AUTOSIZE );

  cv::VideoCapture cap;

  cap.open( string(argv[1]) );
      cout <<"Opened file: " <<argv[1] <<endl;

  // 
  int interval = atoi(argv[2]);

  // 動画ファイルのプロパティを取得
  int frameCount = cap.get(CAP_PROP_FRAME_COUNT);
  double fps = cap.get(CAP_PROP_FPS);
  int width = cap.get(CAP_PROP_FRAME_WIDTH);
  int height = cap.get(CAP_PROP_FRAME_HEIGHT);

  // Show meta data
  cout << "Frame count: " << frameCount << endl;
  cout << "FPS: " << fps << endl;
  cout << "Resolution: " << width << "x" << height << endl;

  // Create output folder  
  std::string outputFolder(argv[1]);
  // Remove extension from video file name
  outputFolder.replace(outputFolder.end()-4, outputFolder.end(),"");
  std::filesystem::create_directories(outputFolder);
  assert(!std::filesystem::create_directory(outputFolder));

  int head = interval * fps;
  int tail = frameCount - (interval * fps);

  frameCount = 0;
  cv::Mat frame;

  cout << "head: " << head << endl;
  cout << "tail: " << tail << endl;

  while (cap.read(frame)) {

    // Exit loop when out of frame
    if( frame.empty() ) break;

    cv::imshow( "Frame", frame );

    if((head < frameCount ) && (frameCount < tail )){

      ostringstream filename;
      filename << outputFolder << "/frame_" << frameCount << ".jpg";
      imwrite(filename.str(), frame);

    }

    frameCount++;

    if( (char)cv::waitKey(33) >= 0 ) break;

  }

  cap.release();

  return 0;

}
