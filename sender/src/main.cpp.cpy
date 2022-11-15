#include <iostream>
#include "encoder.h"
#include <opencv2/opencv.hpp>

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
    	
	cv::Mat frame = cv::imread( "/home/firefly/x264_test/test_data/bus.jpg" );
	cv::imshow("src", frame);
	cv::waitKey(0);	

	int size = m_x264Encoder.EncodeOneFrame(frame);
        uchar* data = nullptr;
        data = m_x264Encoder.GetEncodedFrame();
	std::cout<<"size = "<<size<<std::endl;	
	udp.sendOneFrame( data, size );

	return 0;
}
