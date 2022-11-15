#include <iostream>
#include "encoder.h"
#include <opencv2/opencv.hpp>
#include <chrono>

#include "transport.h"

int main()
{
	std::cout<<"------------ X264 TEST ------------"<<std::endl;

	// udp init
	transport::UDPClient udp;
	udp.init();
	udp.setDstAddr( "172.16.1.13", 2333 );

	//x264 encode
    	x264Encoder m_x264Encoder;
    	m_x264Encoder.Create(640, 640, 3, 25);

	// init video capture
	cv::Mat frame;
	cv::VideoCapture video("/home/firefly/x264_test/test_data/1.mp4");
	for (int i = 0; i < 10; ++i){
                video >> frame;
        }

	while(1){
		video >> frame;
                if( frame.empty() )
                        break;
		cv::imshow("video", frame);
		cv::resize( frame, frame, cv::Size( 640, 640 ) );

		// encode
		auto start = std::chrono::system_clock::now();
		int size = m_x264Encoder.EncodeOneFrame(frame);
		uchar* data = nullptr;
		data = m_x264Encoder.GetEncodedFrame();
		std::cout<<"size = "<<size<<std::endl;
		udp.sendOneFrame( data, size );
		auto end = std::chrono::system_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>( end - start );
  		std::cout<<"encode time: "<<double(duration.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den<<std::endl;
		cv::waitKey(1);
	}
		
	

	return 0;
}
