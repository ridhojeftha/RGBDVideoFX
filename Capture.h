
#ifndef CAPTURE_H
#define	CAPTURE_H

#include "libfreenect.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <pthread.h>
#include <stdio.h>

//#include <GL/glut.h>
//#include <GL/gl.h>
//#include <GL/glu.h>

#include <GL\freeglut.h>

	using namespace std;

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
	class Capture : public Freenect::FreenectDevice {
		public:
		Capture(freenect_context *_ctx, int _index) : Freenect::FreenectDevice(_ctx, _index), m_buffer_depth(freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_RGB).bytes),m_buffer_video(freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_RGB).bytes), m_gamma(2048), m_new_rgb_frame(false), m_new_depth_frame(false) {

				for( unsigned int i = 0 ; i < 2048 ; i++) {
					float v = i/2048.0;
					v = pow(v, 3)* 6;
					m_gamma[i] = v*6*256;
				}
			}

			//required callback method for RGB data from the Kinect
			void VideoCallback(void* _rgb, uint32_t timestamp) {
				m_rgb_mutex.lock();
				uint8_t* rgb = static_cast<uint8_t*>(_rgb);
				//printf("receiving rgb frame\n");
				copy(rgb, rgb+getVideoBufferSize(), m_buffer_video.begin());
				m_new_rgb_frame = true;
				m_rgb_mutex.unlock();
			};

			//required callback method for the Depth data from the Kinect
			void DepthCallback(void* _depth, uint32_t timestamp) {
				m_depth_mutex.lock();

				//The depth buffer contains a 640x480 array of 16bit (2 byte) depth values ranging from 0-2047
				//with all values >=1024 indicating "no data" (shadows).
				uint16_t* depth = static_cast<uint16_t*>(_depth);

                for( unsigned int i = 0 ; i < 640*480 ; i++) {


					int pval = m_gamma[depth[i]]; //the physical depth value
                    int d = 0;

                    if (pval<m_gamma[m_gamma.size()])
                       d = 255 - (255 * pval / (m_gamma[m_gamma.size()-1])-m_gamma[0]);

                            m_buffer_depth[3*i+0] = d;
							m_buffer_depth[3*i+1] = d;
							m_buffer_depth[3*i+2] = d;
                }



				//printf("receiving depth data\n");

				//printBuffer (depth);
				/*
				This snippet takes the raw depth value and maps it into a color.  The
				values are mapped into a somewhat smooth spectrum (from near to far):
				white, red, yellow, green, cyan, blue, black.  depth_mid[3*i + 0] is
				the red byte, depth_mid[3*i + 1] is the green byte, and depth_mid[3*i
				+ 2] is the blue byte. This gets swapped to the depth buffer in the GL Method.
				*/

				/*
					TODO: investigate using HSL (Hue, Saturation, Luminance) to directly map depth to colour and then transform to RGB.
					Might be computationally faster?
				*/

				/*
					if I need to produce the physical distance from raw Kinect values, refer to glpclview.c
					As per nicolas page, this is way to calculate physical distance. But investigate more if needed:
					1.0 / (double(depthValue) * -0.0030711016 + 3.3309495161)
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
				m_new_depth_frame = true;
				m_depth_mutex.unlock();
			}

			//boolean for control when a frame of RGB was received
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

			//boolean for control when a frame of depth data was received
			bool getDepth(vector<uint8_t> &buffer) {
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

			/*void printBuffer(uint16_t* &buffer){

				ofstream output;
				output.open ("depth.txt");
				output << "Depth Data to be printed out\n";
				int i = 0;
				for( unsigned int row = 0 ; row < 480 ; row++){
					for (unsigned int col = 0; col < 640; col++, i++){
						output << "|" << +buffer[i];
						cout << "printing out [" << row << "," << col << "]\n";
					}
					output << "~\n";
				}
				output.close();
			}*/
		private:
			vector<uint8_t> m_buffer_depth;
			vector<uint8_t> m_buffer_video;
			vector<uint16_t> m_gamma;
			Mutex m_rgb_mutex;
			Mutex m_depth_mutex;
			bool m_new_rgb_frame;
			bool m_new_depth_frame;
	};
#endif
