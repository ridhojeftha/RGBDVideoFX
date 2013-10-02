#ifndef DEPTHPROCESSOR_H
#define	DEPTHPROCESSOR_H

#include "libfreenect.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <pthread.h>
#include <stdio.h>
#include <GL\freeglut.h>
#include "NormalMapGenerator.h"
#include "Vector3.h"
#include "PixelFilter.h"

	using namespace std;

	static vector<float> depthMap(640*480);
    static vector<int> rgbMap(640*480*3);

	//define a FreenectDevice and a Mutex class
	class Mutex {
		public:
			Mutex() {
				pthread_mutex_init( &m_mutex, NULL );
			}
			void lock() {
				pthread_mutex_lock( &m_mutex );
			}
			void unlock() {
				pthread_mutex_unlock( &m_mutex );
			}
		private:
			pthread_mutex_t m_mutex;
	};

	//FreenectDevice class to handle capturing from a Kinect
	class DepthProcessor : public Freenect::FreenectDevice {
		public:
		DepthProcessor(freenect_context *_ctx, int _index) : Freenect::FreenectDevice(_ctx, _index), m_buffer_depth(freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_RGB).bytes),m_buffer_video(freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_RGB).bytes), m_gamma(2048), m_new_rgb_frame(false), m_new_depth_frame(false) {

				for( unsigned int i = 0 ; i < 2048 ; i++) {
					float v = i/2048.0;
					v = pow(v, 3)* 6;
					m_gamma[i] = v*6*256;

				}

				for (int i=0; i<640*480*3; ++i){
                    vertexMap.push_back(0.0f);
				}
			}

			//required callback method for RGB data from the Kinect
			void VideoCallback(void* _rgb, uint32_t timestamp) {
				m_rgb_mutex.lock();
				uint8_t* rgb = static_cast<uint8_t*>(_rgb);

				copy(rgb, rgb+getVideoBufferSize(), rgbMap.begin());

				m_new_rgb_frame = true;
				m_rgb_mutex.unlock();
			};


			//required callback method for the Depth data from the Kinect
			void DepthCallback(void* _depth, uint32_t timestamp) {
				m_depth_mutex.lock();

				//The depth buffer contains a 640x480 array of 16bit (2 byte) depth values ranging from 0-2047
				depth = static_cast<uint16_t*>(_depth);

				//vertex map constants
                static const double fx_d = 1.0 / 5.9421434211923247e+02;
                static const double fy_d = 1.0 / 5.9104053696870778e+02;
                static const double cx_d = 3.3930780975300314e+02;
                static const double cy_d = 2.4273913761751615e+02;


               #pragma omp parallel for
                for( unsigned int i = 0 ; i < 640*480 ; i++) {
                    //fill holes in the depth values
                    //uint16_t filteredDepth = depth[i];
                    if (depth[i]>=2047){
                        //filter this pixel
                        depth[i] = filterPixel(i, depth, 0);
                        //depth[i] = filteredDepth;
                        //std::cout << "changed from 2047 to " << filteredDepth << std::endl;
                    }


					int pval = m_gamma[depth[i]]; //the physical depth value
					float d = 0.0;

                    if (pval<m_gamma[m_gamma.size()])
                       d = 1.0 - (1.0 * pval / (m_gamma[m_gamma.size()-1])-m_gamma[0]); //move this to a lookup table

                    depthMap[i] = d;

                    //calculate the vertex map at the same time
					float depth_mm = 0.0f;
                    if (depth[i] < 2047){
                        //convert to meters using Kinect's internal callibration - given by ROS
                        depth_mm = (1.0 / (double (depth[i]) * -0.0030711016 + 3.3309495161));
                    }

                        vertexMap[3*i+0] = float((i%640 - cx_d) * depth_mm * fx_d);
                        vertexMap[3*i+1] = float((i/640 - cy_d) * depth_mm * fy_d);
                        vertexMap[3*i+2] = float(depth_mm);
                }
                /*
				This snippet takes the raw depth value and maps it into a color.  The
				values are mapped into a somewhat smooth spectrum (from near to far):
				white, red, yellow, green, cyan, blue, black.  depth_mid[3*i + 0] is
				the red byte, depth_mid[3*i + 1] is the green byte, and depth_mid[3*i
				+ 2] is the blue byte. This gets swapped to the depth buffer in the GL Method.
				*/

                    /*for( unsigned int i = 0 ; i < 640*480 ; i++) {
					int pval = m_gamma[depth[i]]; //the physical depth value
					int lb = pval & 0xff;
					switch (pval>>8) {
						case 0:
							m_buffer_depth[3*i+0] = 255;
							m_buffer_depth[3*i+1] = 255-lb;
							m_buffer_depth[3*i+2] = 255-lb;
							break;
						case 1:
							m_buffer_depth[3*i+0] = 255;
							m_buffer_depth[3*i+1] = lb;
							m_buffer_depth[3*i+2] = 0;
							break;
						case 2:
							m_buffer_depth[3*i+0] = 255-lb;
							m_buffer_depth[3*i+1] = 255;
							m_buffer_depth[3*i+2] = 0;
							break;
						case 3:
							m_buffer_depth[3*i+0] = 0;
							m_buffer_depth[3*i+1] = 255;
							m_buffer_depth[3*i+2] = lb;
							break;
						case 4:
							m_buffer_depth[3*i+0] = 0;
							m_buffer_depth[3*i+1] = 255-lb;
							m_buffer_depth[3*i+2] = 255;
							break;
						case 5:
							m_buffer_depth[3*i+0] = 0;
							m_buffer_depth[3*i+1] = 0;
							m_buffer_depth[3*i+2] = 255-lb;
							break;
						default:
							m_buffer_depth[3*i+0] = 0;
							m_buffer_depth[3*i+1] = 0;
							m_buffer_depth[3*i+2] = 0;
							break;
					}
				}*/

				//call NormalMapGenerator to generate normals from here
				#pragma omp parallel for
				for( unsigned int i = 0 ; i < 640*480 ; i++) {

                        //calculate indices for right and up vertex
                        int rightIndex, upIndex;
                        if (i%640==639){rightIndex=i;}
                        else {rightIndex = i+1;} //vertex to the right

                        if (i/640==480){upIndex=i;}
                        else {upIndex = i+640;} //vertex above

                        //pass the three vectors that need to be used
                        Vector3 centreVert(vertexMap[3*i+0], vertexMap[3*i+1], vertexMap[3*i+2]);
                        Vector3 rightVert(vertexMap[3*(rightIndex)+0], vertexMap[3*(rightIndex)+1], vertexMap[3*(rightIndex)+2]); //vertex at x+1
                        Vector3 upVert(vertexMap[3*(upIndex)+0], vertexMap[3*(upIndex)+1], vertexMap[3*(upIndex)+2]); //vertex at y+1

                        generateNormalMap(i, centreVert, rightVert, upVert);
				}

				m_new_depth_frame = true;
				m_depth_mutex.unlock();
			}

			//method to be called to get RGB value
			bool getRGB(vector<uint8_t> &buffer) {
				m_rgb_mutex.lock();

				//TODO: cmaybe change m_new_depth_frame and m_new_rgb_frame to one variable as we want synchronized frames
				if(m_new_rgb_frame) {
					buffer.swap(m_buffer_video);
					m_new_rgb_frame = false;
					m_rgb_mutex.unlock();
					return true;
				} else {
					m_rgb_mutex.unlock();
					return false;
				}
			}

			/*
			 * Method to get Depth in vector<uint8_t> - required by openKinect
			 */
			bool getDepth(vector<uint8_t> &buffer){
				m_depth_mutex.lock();

				//TODO: cmaybe change m_new_depth_frame and m_new_rgb_frame to one variable as we want synchronized frames
				if(m_new_depth_frame) {
					buffer.swap(m_buffer_depth);
					m_new_depth_frame = false;
					m_depth_mutex.unlock();
					return true;
				} else {
					m_depth_mutex.unlock();
					return false;
				}
			}


		private:
			vector<uint8_t> m_buffer_depth;
			vector<uint8_t> m_buffer_video;
			vector<uint16_t> m_gamma;
			vector<float> vertexMap;
			uint16_t* depth;
			Mutex m_rgb_mutex;
			Mutex m_depth_mutex;
			bool m_new_rgb_frame;
			bool m_new_depth_frame;
	};

	//DepthMap
	    //Method to be called to get Depth Map
    //In the range of [0;1]
    vector<float> getDepthMap(){
    return depthMap;
	}

	//ColourMap
	    //Method to be called to get Normal Map
    //In the range of [0.0 - 1.0]
    vector<int> getRGBMap(){
    return rgbMap;
	}
#endif
